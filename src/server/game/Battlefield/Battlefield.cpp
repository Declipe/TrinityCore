/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "CellImpl.h"
#include "CreatureTextMgr.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MiscPackets.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldStatePackets.h"
#include <G3D/g3dmath.h>

Battlefield::Battlefield()
{
    m_Timer = 0;
    m_IsEnabled = true;
    m_isActive = false;
    m_DefenderTeam = TEAM_NEUTRAL;

    m_TypeId = 0;
    m_BattleId = 0;
    m_ZoneId = 0;
    m_MapId = 0;
    m_Map = nullptr;
    m_MaxPlayer = 0;
    m_MinPlayer = 0;

    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_freeslots[i] = 0;

    m_MinLevel = 0;
    m_BattleTime = 0;
    m_NoWarBattleTime = 0;
    m_TimeForAcceptInvite = 20;

    m_LastResurrectTimer = 30 * IN_MILLISECONDS;
    m_StartGroupingTimer = 0;
    m_StartGrouping = false;
}

Battlefield::~Battlefield()
{
    StalkerGuid.Clear();
    m_Timer = 0;
    m_IsEnabled = false;
    m_isActive = false;
    m_DefenderTeam = TEAM_ALLIANCE;

    m_PlayerMap.clear();

    m_TypeId                = 0;
    m_BattleId              = 0;
    m_ZoneId                = 0;
    m_MapId                 = 0;
    m_Map = nullptr;
    m_MaxPlayer             = 0;
    m_MinPlayer             = 0;
    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_freeslots[i]      = 0;
    m_TimeForAcceptInvite   = 0;
    m_MinLevel              = 0;
    m_BattleTime            = 0;
    m_NoWarBattleTime       = 0;
    m_StartGroupingTimer    = 0;
    m_LastResurrectTimer    = 0;

    KickPosition.Relocate(0, 0, 0, 0);
    KickPosition.m_mapId = 0;

    m_StartGrouping = false;
    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_Groups[i].clear();

    m_Data64.clear();
    m_Data32.clear();
}

void Battlefield::Update(uint32 diff)
{
    if (m_Timer <= diff)
    {
        // Battlefield ends on time
        if (IsWarTime())
            EndBattle(true);
        else // Time to start a new battle!
            StartBattle();
    }
    else
        m_Timer -= diff;

    // Invite players a few minutes before the battle's beginning
    if (!IsWarTime() && !m_StartGrouping && m_Timer <= m_StartGroupingTimer)
    {
        m_StartGrouping = true;
        InvitePlayersInZoneToQueue();
        OnStartGrouping();
    }

    time_t now = GameTime::GetGameTime();
    std::set<ObjectGuid>_playerkick;
    for (PlayerHolderContainer::iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
    {
        // check estimation time for awaiting war and waiting to kick
        if (itr->second.isWaitingWar || itr->second.isWaitingKick)
        {
            if (itr->second.time <= now)
                _playerkick.insert(itr->first);
        }
    }

    for (auto itr = _playerkick.begin(); itr != _playerkick.end(); ++itr)
        KickPlayerFromBattlefield(*itr);
    _playerkick.clear();

    if (m_LastResurrectTimer <= diff)
    {
        for (uint8 i = 0; i < m_GraveyardList.size(); i++)
            if (GetGraveyardById(i))
                m_GraveyardList[i]->Resurrect();
        m_LastResurrectTimer = RESURRECTION_INTERVAL;
    }
    else
        m_LastResurrectTimer -= diff;

    if (IsWarTime())
    {
        for (BfCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            itr->second->Update(diff);
    }
}

void Battlefield::InitStalker(uint32 entry, Position const& pos)
{
    if (Creature* creature = SpawnCreature(entry, pos))
        StalkerGuid = creature->GetGUID();
    else
        TC_LOG_ERROR("bg.battlefield", "Battlefield::InitStalker: Could not spawn Stalker (Creature entry {}), zone messages will be unavailable!", entry);
}

void Battlefield::SendWarning(uint8 id, WorldObject const* target /*= nullptr*/)
{
    if (Creature* stalker = GetCreature(StalkerGuid))
        sCreatureTextMgr->SendChat(stalker, id, target);
}

void Battlefield::StartBattle()
{
    if (m_isActive)
        return;

    for (uint8 team = 0; team < PVP_TEAMS_COUNT; ++team)
        m_Groups[team].clear();

    m_Timer = m_BattleTime;
    m_isActive = true;

    InvitePlayersInQueueToWar();
    TryInvitePlayersNotInQueueToWarOrKickThem();

    OnBattleStart();
}

void Battlefield::EndBattle(bool endByTimer)
{
    if (!m_isActive)
        return;

    m_isActive = false;

    m_StartGrouping = false;

    if (!endByTimer)
        SetDefenderTeam(GetAttackerTeam());

    for (PlayerHolderContainer::iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
    {
        itr->second.inQueue = false;
        itr->second.inWar = false;
        itr->second.isWaitingQueue = false;
        itr->second.isWaitingWar = false;
        itr->second.isWaitingKick = false;
        itr->second.time = 0;
    }

    for (uint8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_freeslots[i] = m_MaxPlayer;

    OnBattleEnd(endByTimer);
    InvitePlayersInZoneToQueue();

    // Reset bat0tlefield timer
    m_Timer = m_NoWarBattleTime;
    SendInitWorldStatesToAll();
}

void Battlefield::RegisterZone(uint32 zoneId)
{
    sBattlefieldMgr->AddZone(zoneId, this);
}

void Battlefield::HideNpc(Creature* creature)
{
    creature->CombatStop();
    creature->SetReactState(REACT_PASSIVE);
    creature->SetUnitFlag(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_UNINTERACTIBLE);
    creature->SetRespawnDelay(RESPAWN_ONE_DAY);
    creature->DisappearAndDie();
    creature->SetVisible(false);
}

void Battlefield::ShowNpc(Creature* creature, bool aggressive)
{
    creature->SetVisible(true);
    creature->RemoveUnitFlag(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_UNINTERACTIBLE);
    if (!creature->IsAlive())
        creature->Respawn(true);
    if (aggressive)
        creature->SetReactState(REACT_AGGRESSIVE);
    else
    {
        creature->SetUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
        creature->SetReactState(REACT_PASSIVE);
    }
}

// ****************************************************
// ******************* Group System *******************
// ****************************************************
Group* Battlefield::GetFreeBfRaid(TeamId TeamId)
{
    for (auto itr = m_Groups[TeamId].begin(); itr != m_Groups[TeamId].end(); ++itr)
        if (Group* group = sGroupMgr->GetGroupByGUID(*itr))
            if (!group->IsFull())
                return group;

    return nullptr;
}

Group* Battlefield::GetGroupPlayer(ObjectGuid guid, TeamId TeamId)
{
    for (auto itr = m_Groups[TeamId].begin(); itr != m_Groups[TeamId].end(); ++itr)
        if (Group* group = sGroupMgr->GetGroupByGUID(*itr))
            if (group->IsMember(guid))
                return group;

    return nullptr;
}

bool Battlefield::AddOrSetPlayerToCorrectBfGroup(Player* player)
{
    if (!player->IsInWorld())
        return false;

    if (Group* group = player->GetGroup())
        group->RemoveMember(player->GetGUID());

    Group* group = GetFreeBfRaid(player->GetTeamId());
    if (!group)
    {
        group = new Group;
        group->SetBattlefieldGroup(this);
        group->Create(player);
        sGroupMgr->AddGroup(group);
        m_Groups[player->GetTeamId()].insert(group->GetGUID());
    }
    else if (group->IsMember(player->GetGUID()))
    {
        uint8 subgroup = group->GetMemberGroup(player->GetGUID());
        player->SetBattlegroundOrBattlefieldRaid(group, subgroup);
    }
    else
        group->AddMember(player);

    return true;
}

//***************End of Group System*******************

//*****************************************************
//***************Spirit Guide System*******************
//*****************************************************

//--------------------
//-Battlefield Method-
//--------------------
BfGraveyard* Battlefield::GetGraveyardById(uint32 id) const
{
    if (id < m_GraveyardList.size())
    {
        if (BfGraveyard* graveyard = m_GraveyardList.at(id))
            return graveyard;
        else
            TC_LOG_ERROR("bg.battlefield", "Battlefield::GetGraveyardById Id:{} does not exist.", id);
    }
    else
        TC_LOG_ERROR("bg.battlefield", "Battlefield::GetGraveyardById Id:{} could not be found.", id);

    return nullptr;
}

WorldSafeLocsEntry const* Battlefield::GetClosestGraveyard(Player* player)
{
    BfGraveyard* closestGY = nullptr;
    float maxdist = -1;
    for (uint8 i = 0; i < m_GraveyardList.size(); i++)
    {
        if (m_GraveyardList[i])
        {
            if (m_GraveyardList[i]->GetControlTeamId() != player->GetTeamId())
                continue;

            float dist = m_GraveyardList[i]->GetDistance(player);
            if (dist < maxdist || maxdist < 0)
            {
                closestGY = m_GraveyardList[i];
                maxdist = dist;
            }
        }
    }

    if (closestGY)
        return sDBCMgr->GetWorldSafeLocsEntry(closestGY->GetGraveyardId());

    return nullptr;
}

void Battlefield::AddPlayerToResurrectQueue(ObjectGuid npcGuid, ObjectGuid playerGuid)
{
    for (uint8 i = 0; i < m_GraveyardList.size(); i++)
    {
        if (!m_GraveyardList[i])
            continue;

        if (m_GraveyardList[i]->HasNpc(npcGuid))
        {
            m_GraveyardList[i]->AddPlayer(playerGuid);
            break;
        }
    }
}

void Battlefield::RemovePlayerFromResurrectQueue(ObjectGuid playerGuid)
{
    for (uint8 i = 0; i < m_GraveyardList.size(); i++)
    {
        if (!m_GraveyardList[i])
            continue;

        if (m_GraveyardList[i]->HasPlayer(playerGuid))
        {
            m_GraveyardList[i]->RemovePlayer(playerGuid);
            break;
        }
    }
}

// *******************************************************
// *************** End Spirit Guide system ***************
// *******************************************************
// ********************** Misc ***************************
// *******************************************************

Creature* Battlefield::SpawnCreature(uint32 entry, Position const& pos)
{
    //Get map object
    if (!m_Map)
    {
        TC_LOG_ERROR("bg.battlefield", "Battlefield::SpawnCreature: Can't create creature entry: {}, map not found.", entry);
        return nullptr;
    }

    Creature* creature = new Creature();
    if (!creature->Create(m_Map->GenerateLowGuid<HighGuid::Unit>(), m_Map, PHASEMASK_NORMAL, entry, pos))
    {
        TC_LOG_ERROR("bg.battlefield", "Battlefield::SpawnCreature: Can't create creature entry: {}", entry);
        delete creature;
        return nullptr;
    }

    creature->SetHomePosition(pos);

    // Set creature in world
    m_Map->AddToMap(creature);
    creature->setActive(true);
    creature->SetFarVisible(true);

    return creature;
}

// Method for spawning gameobject on map
GameObject* Battlefield::SpawnGameObject(uint32 entry, Position const& pos, QuaternionData const& rot)
{
    //Get map object
    if (!m_Map)
    {
        TC_LOG_ERROR("bg.battlefield", "Battlefield::SpawnGameObject: Can't create gameobject entry: {}, map not found.", entry);
        return nullptr;
    }

    // Create gameobject
    GameObject* go = new GameObject;
    if (!go->Create(m_Map->GenerateLowGuid<HighGuid::GameObject>(), entry, m_Map, PHASEMASK_NORMAL, pos, rot, 255, GO_STATE_READY))
    {
        TC_LOG_ERROR("bg.battlefield", "Battlefield::SpawnGameObject: Gameobject template {} could not be found in the database! Battlefield has not been created!", entry);
        TC_LOG_ERROR("bg.battlefield", "Battlefield::SpawnGameObject: Could not create gameobject template {}! Battlefield has not been created!", entry);
        delete go;
        return nullptr;
    }

    // Add to world
    m_Map->AddToMap(go);
    go->setActive(true);
    go->SetFarVisible(true);

    return go;
}

Creature* Battlefield::GetCreature(ObjectGuid guid)
{
    if (!m_Map)
        return nullptr;
    return m_Map->GetCreature(guid);
}

GameObject* Battlefield::GetGameObject(ObjectGuid guid)
{
    if (!m_Map)
        return nullptr;
    return m_Map->GetGameObject(guid);
}
