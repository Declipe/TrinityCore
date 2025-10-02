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

 /// @todo Implement proper support for vehicle+player teleportation
 /// @todo Use spell victory/defeat in wg instead of RewardMarkOfHonor() && RewardHonor
 /// @todo Add proper implement of achievement

#include "BattlefieldWG.h"
#include "BfWGGameObjectBuilding.h"
#include "AchievementMgr.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "Creature.h"
#include "CreatureTextMgr.h"
#include "GameTime.h"
#include "Log.h"
#include "MotionMaster.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "Random.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "TemporarySummon.h"
#include "World.h"
#include "WorldSession.h"
#include "WorldStatePackets.h"
#include "WintergraspCapturePoint.h"
#include "WintergraspGraveyard.h"
#include "WintergraspWorkshop.h"

uint32 const ClockWorldState[] = { 3781, 4354 };
Position const WintergraspStalkerPos = { 4948.985f, 2937.789f, 550.5172f,  1.815142f };
Position const WintergraspRelicPos = { 5440.379f, 2840.493f, 430.2816f, -1.832595f };
QuaternionData const WintergraspRelicRot = { 0.f, 0.f, -0.7933531f, 0.6087617f };

uint8 const WG_MAX_MAP_GUARDS = 48;
WintergraspGuardPositionData const WGMapGuards[WG_MAX_MAP_GUARDS] =
{
    // near gateway west
    //                  Position                         entry
    { { 5035.649f, 2899.071f, 386.954f, 0.238f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 5040.845f, 2899.765f, 387.720f, 3.235f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5050.898f, 2888.398f, 389.988f, 0.003f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5056.666f, 2887.741f, 390.597f, 2.936f }, BATTLEFIELD_WG_NPC_GUARD_H },

    { { 5056.902f, 3084.399f, 366.117f, 2.982f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 5053.120f, 3087.289f, 365.853f, 5.503f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5043.986f, 3091.536f, 366.640f, 3.068f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5038.528f, 3093.595f, 367.971f, 5.923f }, BATTLEFIELD_WG_NPC_GUARD_H },

    { { 5066.540f, 3208.132f, 356.751f, 3.896f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 5063.531f, 3201.505f, 356.656f, 1.555f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5044.723f, 3209.649f, 356.765f, 5.019f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5047.662f, 3205.615f, 356.631f, 2.466f }, BATTLEFIELD_WG_NPC_GUARD_H },
    // near gateway middle
    { { 5015.149f, 2864.815f, 389.698f, 4.519f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 5015.065f, 2860.534f, 389.670f, 1.46f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5011.854f, 2842.118f, 390.818f, 4.377f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5011.397f, 2836.878f, 390.787f, 1.46f }, BATTLEFIELD_WG_NPC_GUARD_H },

    { { 4907.736f, 2893.682f, 380.683f, 4.381f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4906.906f, 2887.569f, 380.322f, 1.432f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4903.887f, 2875.121f, 380.293f, 4.515f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4903.731f, 2869.803f, 380.489f, 1.511f }, BATTLEFIELD_WG_NPC_GUARD_H },

    { { 4694.722f, 2850.790f, 387.813f, 1.884f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4693.121f, 2856.010f, 388.157f, 5.042f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4691.131f, 2866.784f, 388.188f, 1.747f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4690.532f, 2873.707f, 387.915f, 4.881f }, BATTLEFIELD_WG_NPC_GUARD_H },
    // near gateway east
    { { 5036.830f, 2777.146f, 380.827f, 5.769f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 5042.109f, 2773.620f, 380.983f, 2.619f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5049.915f, 2771.682f, 381.529f, 6.000f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5056.865f, 2769.007f, 382.229f, 2.844f }, BATTLEFIELD_WG_NPC_GUARD_H },

    { { 5001.452f, 2703.385f, 370.740f, 0.001f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 5005.199f, 2702.609f, 370.079f, 2.966f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5012.662f, 2703.812f, 370.054f, 5.911f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 5018.141f, 2701.317f, 369.713f, 3.202f }, BATTLEFIELD_WG_NPC_GUARD_H },

    // bridge west
    { { 4753.241f, 3306.832f, 366.610f, 4.277f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4751.814f, 3302.814f, 366.611f, 1.104f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4747.562f, 3285.551f, 366.599f, 4.453f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4746.070f, 3280.985f, 366.599f, 1.221f }, BATTLEFIELD_WG_NPC_GUARD_H },

    { { 4607.844f, 3321.693f, 366.745f, 1.261f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4610.448f, 3325.968f, 366.577f, 4.112f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4614.288f, 3344.082f, 365.875f, 1.324f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4616.000f, 3351.321f, 365.793f, 4.575f }, BATTLEFIELD_WG_NPC_GUARD_A },

    // bridge east
    { { 4537.335f, 2315.821f, 370.297f, 2.023f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4534.507f, 2321.530f, 370.354f, 5.184f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4524.305f, 2340.637f, 370.228f, 1.956f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4520.690f, 2348.646f, 370.933f, 5.058f }, BATTLEFIELD_WG_NPC_GUARD_A },

    { { 4685.365f, 2389.966f, 370.037f, 1.854f }, BATTLEFIELD_WG_NPC_GUARD_H },
    { { 4686.326f, 2395.769f, 369.573f, 4.524f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4681.975f, 2404.197f, 369.582f, 1.807f }, BATTLEFIELD_WG_NPC_GUARD_A },
    { { 4679.878f, 2409.111f, 369.896f, 5.148f }, BATTLEFIELD_WG_NPC_GUARD_H }
};

BattlefieldWG::~BattlefieldWG()
{
    m_isRelicInteractible = false;

    for (WintergraspGraveyard* extendedGrave : m_WG)
        delete extendedGrave;
    m_WG.clear();
    m_capturePoints.clear();

    for (WintergraspCapturePoint* extendedCP : m_WCP)
        delete extendedCP;
    m_WCP.clear();
    m_GraveyardList.clear();

    for (WintergraspWorkshop* workshop : m_Workshops)
        delete workshop;
    m_Workshops.clear();

    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
        delete building;
    m_BuildingsInZone.clear();

    m_tenacityTeam = TEAM_ALLIANCE;
    m_tenacityStack = 0;
    m_saveTimer = 0;
    m_titansRelicGUID.Clear();
}

bool BattlefieldWG::SetupBattlefield()
{
    m_TypeId = BATTLEFIELD_WG;                              // See enum BattlefieldTypes
    m_BattleId = BATTLEFIELD_BATTLEID_WG;
    m_ZoneId = AREA_WINTERGRASP;
    m_MapId = BATTLEFIELD_WG_MAPID;
    m_Map = sMapMgr->FindMap(m_MapId, 0);
    if (!m_Map)
        m_Map = sMapMgr->CreateBaseMap(m_MapId);

    InitStalker(BATTLEFIELD_WG_NPC_STALKER, WintergraspStalkerPos);

    m_MaxPlayer = sWorld->getIntConfig(CONFIG_WINTERGRASP_PLR_MAX);
    m_IsEnabled = sWorld->getBoolConfig(CONFIG_WINTERGRASP_ENABLE);
    m_MinPlayer = sWorld->getIntConfig(CONFIG_WINTERGRASP_PLR_MIN);
    m_MinLevel = sWorld->getIntConfig(CONFIG_WINTERGRASP_PLR_MIN_LVL);
    m_BattleTime = sWorld->getIntConfig(CONFIG_WINTERGRASP_BATTLETIME) * MINUTE * IN_MILLISECONDS;
    m_NoWarBattleTime = sWorld->getIntConfig(CONFIG_WINTERGRASP_NOBATTLETIME) * MINUTE * IN_MILLISECONDS;

    for (uint8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_freeslots[i] = m_MaxPlayer;

    m_TimeForAcceptInvite = 20;
    m_StartGroupingTimer = 15 * MINUTE * IN_MILLISECONDS;
    m_StartGrouping = false;

    m_tenacityTeam = TEAM_NEUTRAL;
    m_tenacityStack = 0;

    KickPosition.Relocate(5728.117f, 2714.346f, 697.733f, 0);
    KickPosition.m_mapId = m_MapId;

    RegisterZone(m_ZoneId);
    m_Data32.resize(BATTLEFIELD_WG_DATA_MAX);
    m_saveTimer = 60000;

    // Load from db
    m_isActive = sWorld->getWorldState(WS_BATTLEFIELD_WG_ACTIVE) != 0;
    m_DefenderTeam = TeamId(sWorld->getWorldState(WS_BATTLEFIELD_WG_DEFENDER));
    m_Timer = sWorld->getWorldState(ClockWorldState[0]);

    FillBuildings();
    FillGraveyards();
    FillWorkshops();

    SpawnPortalAndDefenders();

    SpawnMapGuards();

    UpdateCounterVehicle(true);
    return true;
}

void BattlefieldWG::SpawnMapGuards()
{
    for (uint8 i = 0; i < WG_MAX_MAP_GUARDS; i++)
    {
        WintergraspGuardPositionData const& mapGuardData = WGMapGuards[i];
        if (Creature* creature = SpawnCreature(mapGuardData.Entry, mapGuardData.Pos))
        {
            MapGuards.push_back(creature->GetGUID());
            if (isCreatureGuard(creature))
                creature->GetMotionMaster()->MoveRandom(25.f);
        }
    }
}

void BattlefieldWG::prepareDelete()
{
    for (WintergraspWorkshop* workshop : m_Workshops)
        workshop->prepareDelete();
    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
        building->prepareDelete();

    for (WintergraspGraveyard* extendedGrave : m_WG)
        extendedGrave->prepareDelete();
    for (WintergraspCapturePoint* extendedCapturePoint : m_WCP)
        extendedCapturePoint->prepareDelete();

    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
    {
        for (ObjectGuid guid : DefenderPortalList[i])
            if (GameObject* go = GetGameObject(guid))
            {
                go->SetRespawnTime(0);                  // not save respawn time
                go->Delete();
                go = nullptr;
            }
        DefenderPortalList[i].clear();
    }

    for (ObjectGuid guid : MapGuards)
        if (Creature* creature = GetCreature(guid))
            creature->ClearZoneScript();
    MapGuards.clear();

    for (uint8 team = 0; team < PVP_TEAMS_COUNT; ++team)
    {
        for (auto itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
            if (Creature* creature = GetCreature(*itr))
                if (creature->IsVehicle())
                    creature->DespawnOrUnsummon();

        m_vehicles[team].clear();
    }
}

void BattlefieldWG::switchSide()
{
    SetDefenderTeam(GetOtherTeam(m_DefenderTeam));

    // Remove relic
    if (!m_titansRelicGUID.IsEmpty())
        if (GameObject* relic = GetGameObject(m_titansRelicGUID))
            relic->RemoveFromWorld();
    m_titansRelicGUID.Clear();

    if (IsWarTime())
    {
        // Spawn titan relic
        if (GameObject* relic = SpawnGameObject(GO_WINTERGRASP_TITAN_S_RELIC, WintergraspRelicPos, WintergraspRelicRot))
        {
            // Update faction of relic, only attacker can click on
            relic->SetFaction(WintergraspFaction[GetAttackerTeam()]);
            // Set in use (not allow to click on before last door is broken)
            relic->SetFlag(GO_FLAG_IN_USE | GO_FLAG_NOT_SELECTABLE);
            m_titansRelicGUID = relic->GetGUID();
        }
        else
            TC_LOG_ERROR("bg.battlefield", "WG: Failed to spawn titan relic.");
    }

    SetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT, 0);
    SetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF, 0);
    SetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, 0);
    SetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, 0);

    UpdateAttackers();
    UpdateDefenders();
    UpdateWorkshopsAndGraves();

    SendInitWorldStatesToAll();
}

void BattlefieldWG::Update(uint32 diff)
{
    Battlefield::Update(diff);
    if (m_saveTimer <= diff)
    {
        sWorld->setWorldState(WS_BATTLEFIELD_WG_ACTIVE, m_isActive);
        sWorld->setWorldState(WS_BATTLEFIELD_WG_DEFENDER, m_DefenderTeam);
        sWorld->setWorldState(ClockWorldState[0], m_Timer);
        sWorld->setWorldState(WS_BATTLEFIELD_WG_ATTACKED_A, GetData(BATTLEFIELD_WG_DATA_WON_A));
        sWorld->setWorldState(WS_BATTLEFIELD_WG_DEFENDED_A, GetData(BATTLEFIELD_WG_DATA_DEF_A));
        sWorld->setWorldState(WS_BATTLEFIELD_WG_ATTACKED_H, GetData(BATTLEFIELD_WG_DATA_WON_H));
        sWorld->setWorldState(WS_BATTLEFIELD_WG_DEFENDED_H, GetData(BATTLEFIELD_WG_DATA_DEF_H));
        m_saveTimer = 60 * IN_MILLISECONDS;
    }
    else
        m_saveTimer -= diff;
}

void BattlefieldWG::OnBattleStart()
{
    // Spawn titan relic
    if (GameObject* relic = SpawnGameObject(GO_WINTERGRASP_TITAN_S_RELIC, WintergraspRelicPos, WintergraspRelicRot))
    {
        // Update faction of relic, only attacker can click on
        relic->SetFaction(WintergraspFaction[GetAttackerTeam()]);
        // Set in use (not allow to click on before last door is broken)
        relic->SetFlag(GO_FLAG_IN_USE | GO_FLAG_NOT_SELECTABLE);
        m_titansRelicGUID = relic->GetGUID();
    }
    else
        TC_LOG_ERROR("bg.battlefield", "WG: Failed to spawn titan relic.");

    SetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT, 0);
    SetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF, 0);
    SetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, 0);
    SetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, 0);

    UpdateAttackers();
    UpdateDefenders();
    UpdateWorkshopsAndGraves();

    // TODO check need removeplayer?
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
    {
        if (itr->second.inZone)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                float x, y, z;
                player->GetPosition(x, y, z);
                if (5500 > x && x > 5392 && y < 2880 && y > 2800 && z < 480)
                    player->TeleportTo(571, 5349.8686f, 2838.481f, 409.240f, 0.046328f);
                SendInitWorldStatesTo(player);
            }
    }

    // Initialize vehicle counter
    UpdateCounterVehicle(true);
    // Send start warning to all players
    SendWarning(BATTLEFIELD_WG_TEXT_START_BATTLE);
}

void BattlefieldWG::UpdateCounterVehicle(bool init)
{
    if (init)
    {
        SetData(BATTLEFIELD_WG_DATA_VEHICLE_H, 0);
        SetData(BATTLEFIELD_WG_DATA_VEHICLE_A, 0);
    }
    SetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H, 0);
    SetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A, 0);

    for (WintergraspWorkshop* workshop : m_Workshops)
    {
        if (workshop->GetTeamControl() == TEAM_ALLIANCE)
            UpdateData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A, 4);
        else if (workshop->GetTeamControl() == TEAM_HORDE)
            UpdateData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H, 4);
    }

    UpdateVehicleCountWG();
}

void BattlefieldWG::OnBattleEnd(bool endByTimer)
{
    // Remove relic
    if (!m_titansRelicGUID.IsEmpty())
        if (GameObject* relic = GetGameObject(m_titansRelicGUID))
            relic->RemoveFromWorld();
    m_titansRelicGUID.Clear();

    // change collision wall state closed
    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
    {
        building->RebuildGate();
        building->UpdateCreatureAndGo();
        building->UpdateTurretAttack(true);
    }

    // successful defense
    if (endByTimer)
        UpdateData(GetDefenderTeam() == TEAM_HORDE ? BATTLEFIELD_WG_DATA_DEF_H : BATTLEFIELD_WG_DATA_DEF_A, 1);
    // successful attack (note that teams have already been swapped, so defender team is the one who won)
    else
        UpdateData(GetDefenderTeam() == TEAM_HORDE ? BATTLEFIELD_WG_DATA_WON_H : BATTLEFIELD_WG_DATA_WON_A, 1);

    // Update all graveyard, control is to defender when no wartime
    for (uint8 i = 0; i < BATTLEFIELD_WG_GY_HORDE; i++)
        if (BfGraveyard* graveyard = GetGraveyardById(i))
            graveyard->GiveControlTo(GetDefenderTeam());

    UpdateDefenders();

    // Saving data
    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
        building->Save();

    for (WintergraspWorkshop* workshop : m_Workshops)
        workshop->Save();

    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
    {
        if (itr->second.inZone)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                RemoveAurasFromPlayer(player);

                if (itr->second.team == GetDefenderTeam())
                {
                    player->CastSpell(player, SPELL_ESSENCE_OF_WINTERGRASP, true);
                    player->CastSpell(player, SPELL_VICTORY_REWARD, true);
                    // Complete victory quests
                    player->AreaExploredOrEventHappens(QUEST_VICTORY_WINTERGRASP_A);
                    player->AreaExploredOrEventHappens(QUEST_VICTORY_WINTERGRASP_H);
                    // Send Wintergrasp victory achievement
                    DoCompleteOrIncrementAchievement(ACHIEVEMENTS_WIN_WG, player);
                    // Award achievement for succeeding in Wintergrasp in 10 minutes or less
                    if (!endByTimer && GetTimer() <= 10000)
                        DoCompleteOrIncrementAchievement(ACHIEVEMENTS_WIN_WG_TIMER_10, player);
                }
                else
                {
                    player->CastSpell(player, SPELL_DEFEAT_REWARD, true);
                }

                if (!endByTimer)
                {
                    player->RemoveAurasDueToSpell(m_DefenderTeam == TEAM_ALLIANCE ? SPELL_HORDE_CONTROL_PHASE_SHIFT : SPELL_ALLIANCE_CONTROL_PHASE_SHIFT, player->GetGUID());
                    player->AddAura(m_DefenderTeam == TEAM_HORDE ? SPELL_HORDE_CONTROL_PHASE_SHIFT : SPELL_ALLIANCE_CONTROL_PHASE_SHIFT, player);
                }
            }
    }

    for (uint8 team = 0; team < PVP_TEAMS_COUNT; ++team)
    {
        for (auto itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
            if (Creature* creature = GetCreature(*itr))
                if (creature->IsVehicle())
                    creature->DespawnOrUnsummon();

        m_vehicles[team].clear();
    }

    if (!endByTimer) // win alli/horde
        SendWarning(GetDefenderTeam() == TEAM_ALLIANCE ? BATTLEFIELD_WG_TEXT_FORTRESS_CAPTURE_ALLIANCE : BATTLEFIELD_WG_TEXT_FORTRESS_CAPTURE_HORDE);
    else // defend alli/horde
        SendWarning(GetDefenderTeam() == TEAM_ALLIANCE ? BATTLEFIELD_WG_TEXT_FORTRESS_DEFEND_ALLIANCE : BATTLEFIELD_WG_TEXT_FORTRESS_DEFEND_HORDE);
}

// *******************************************************
// ******************* Reward System *********************
// *******************************************************
void BattlefieldWG::DoCompleteOrIncrementAchievement(uint32 achievement, Player* player, uint8 /*incrementNumber*/)
{
    AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievement);

    if (!achievementEntry)
        return;

    switch (achievement)
    {
    case ACHIEVEMENTS_WIN_WG_100:
    {
        // player->UpdateAchievementCriteria();
        break;
    }
    default:
    {
        if (player)
            player->CompletedAchievement(achievementEntry);
        break;
    }
    }

}

void BattlefieldWG::OnStartGrouping()
{
    SendWarning(BATTLEFIELD_WG_TEXT_START_GROUPING);
}

uint8 BattlefieldWG::GetSpiritGraveyardId(uint32 areaId) const
{
    switch (areaId)
    {
    case AREA_WINTERGRASP_FORTRESS:
        return BATTLEFIELD_WG_GY_KEEP;
    case AREA_THE_SUNKEN_RING:
        return BATTLEFIELD_WG_GY_WORKSHOP_NE;
    case AREA_THE_BROKEN_TEMPLATE:
        return BATTLEFIELD_WG_GY_WORKSHOP_NW;
    case AREA_WESTPARK_WORKSHOP:
        return BATTLEFIELD_WG_GY_WORKSHOP_SW;
    case AREA_EASTPARK_WORKSHOP:
        return BATTLEFIELD_WG_GY_WORKSHOP_SE;
    case AREA_WINTERGRASP:
        return BATTLEFIELD_WG_GY_ALLIANCE;
    case AREA_THE_CHILLED_QUAGMIRE:
        return BATTLEFIELD_WG_GY_HORDE;
    default:
        TC_LOG_ERROR("bg.battlefield", "BattlefieldWG::GetSpiritGraveyardId: Unexpected Area Id {}", areaId);
        break;
    }

    return 0;
}

// Called when player kill a unit in wg zone
void BattlefieldWG::HandleKill(Player* killer, Unit* victim)
{
    if (killer == victim)
        return;

    if (victim->GetTypeId() == TYPEID_PLAYER)
    {
        HandlePromotion(killer, victim);

        // Allow to Skin non-released corpse
        victim->SetUnitFlag(UNIT_FLAG_SKINNABLE);
    }

    /// @todoRecent PvP activity worldstate
}

bool BattlefieldWG::FindAndRemoveVehicleFromList(Unit* vehicle)
{
    for (uint32 team = 0; team < PVP_TEAMS_COUNT; ++team)
    {
        auto itr = m_vehicles[team].find(vehicle->GetGUID());
        if (itr != m_vehicles[team].end())
        {
            m_vehicles[team].erase(itr);

            if (team == TEAM_HORDE)
                UpdateData(BATTLEFIELD_WG_DATA_VEHICLE_H, -1);
            else
                UpdateData(BATTLEFIELD_WG_DATA_VEHICLE_A, -1);
            return true;
        }
    }

    return false;
}

void BattlefieldWG::OnUnitDeath(Unit* unit)
{
    if (IsWarTime())
        if (unit->IsVehicle())
            if (FindAndRemoveVehicleFromList(unit))
                UpdateVehicleCountWG();
}

void BattlefieldWG::HandlePromotion(Player* playerKiller, Unit* unitKilled)
{
    uint32 teamId = playerKiller->GetTeamId();
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
    {
        if (itr->second.inWar)
        {
            if (unsigned(itr->second.team) == teamId)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    if (player->GetDistance2d(unitKilled) < 40.0f)
                        PromotePlayer(player);
        }
    }
}

// Update rank for player
void BattlefieldWG::PromotePlayer(Player* killer)
{
    if (!m_isActive)
        return;
    // Updating rank of player
    if (Aura* auraRecruit = killer->GetAura(SPELL_RECRUIT))
    {
        if (auraRecruit->GetStackAmount() >= 5)
        {
            killer->RemoveAura(SPELL_RECRUIT);
            killer->CastSpell(killer, SPELL_CORPORAL, true);
            if (Creature* stalker = GetCreature(StalkerGuid))
                sCreatureTextMgr->SendChat(stalker, BATTLEFIELD_WG_TEXT_RANK_CORPORAL, killer, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_NORMAL, 0, TEAM_OTHER, false, killer);
        }
        else
            killer->CastSpell(killer, SPELL_RECRUIT, true);
    }
    else if (Aura* auraCorporal = killer->GetAura(SPELL_CORPORAL))
    {
        if (auraCorporal->GetStackAmount() >= 5)
        {
            killer->RemoveAura(SPELL_CORPORAL);
            killer->CastSpell(killer, SPELL_LIEUTENANT, true);
            if (Creature* stalker = GetCreature(StalkerGuid))
                sCreatureTextMgr->SendChat(stalker, BATTLEFIELD_WG_TEXT_RANK_FIRST_LIEUTENANT, killer, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_NORMAL, 0, TEAM_OTHER, false, killer);
        }
        else
            killer->CastSpell(killer, SPELL_CORPORAL, true);
    }
}

void BattlefieldWG::RemoveAurasFromPlayer(Player* player)
{
    player->RemoveAurasDueToSpell(SPELL_RECRUIT);
    player->RemoveAurasDueToSpell(SPELL_CORPORAL);
    player->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
    player->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
    player->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
    player->RemoveAurasDueToSpell(SPELL_TENACITY);
    player->RemoveAurasDueToSpell(SPELL_ESSENCE_OF_WINTERGRASP);
    player->RemoveAurasDueToSpell(SPELL_WINTERGRASP_RESTRICTED_FLIGHT_AREA);
}

void BattlefieldWG::OnPlayerJoinWar(Player* player)
{
    RemoveAurasFromPlayer(player);

    player->CastSpell(player, SPELL_RECRUIT, true);

    if (player->GetZoneId() != m_ZoneId)
    {
        if (player->GetTeamId() == GetDefenderTeam())
            player->TeleportTo(571, 5345, 2842, 410, 3.14f);
        else
        {
            if (player->GetTeamId() == TEAM_HORDE)
                player->TeleportTo(571, 5025.857422f, 3674.628906f, 362.737122f, 4.135169f);
            else
                player->TeleportTo(571, 5101.284f, 2186.564f, 373.549f, 3.812f);
        }
    }

    UpdateTenacity();

    if (player->GetTeamId() == GetAttackerTeam())
    {
        if (GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT) < 3)
            player->SetAuraStack(SPELL_TOWER_CONTROL, player, 3 - GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT));
    }
    else
    {
        if (GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT) > 0)
            player->SetAuraStack(SPELL_TOWER_CONTROL, player, GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT));
    }
    SendInitWorldStatesTo(player);
}

void BattlefieldWG::OnPlayerLeaveWar(Player* player)
{
    // Remove all aura from WG /// @todo false we can go out of this zone on retail and keep Rank buff, remove on end of WG
    if (!player->GetSession()->PlayerLogout())
    {
        if (Creature* vehicle = player->GetVehicleCreatureBase())   // Remove vehicle of player if he go out.
            vehicle->DespawnOrUnsummon();

        RemoveAurasFromPlayer(player);
    }

    player->RemoveAurasDueToSpell(SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT);
    player->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT);
    player->RemoveAurasDueToSpell(SPELL_HORDE_CONTROL_PHASE_SHIFT);
    player->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROL_PHASE_SHIFT);
    UpdateTenacity();
}

void BattlefieldWG::OnPlayerLeaveZone(Player* player)
{
    if (!m_isActive)
        RemoveAurasFromPlayer(player);

    player->RemoveAurasDueToSpell(SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT);
    player->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT);
    player->RemoveAurasDueToSpell(SPELL_HORDE_CONTROL_PHASE_SHIFT);
    player->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROL_PHASE_SHIFT);
}

void BattlefieldWG::OnPlayerEnterZone(Player* player)
{
    if (!m_isActive)
        RemoveAurasFromPlayer(player);

    player->AddAura(m_DefenderTeam == TEAM_HORDE ? SPELL_HORDE_CONTROL_PHASE_SHIFT : SPELL_ALLIANCE_CONTROL_PHASE_SHIFT, player);
    // Send worldstate to player
    SendInitWorldStatesTo(player);
}

uint32 BattlefieldWG::GetData(uint32 data) const
{
    switch (data)
    {
        // Used to determine when the phasing spells must be cast
        // See: SpellArea::IsFitToRequirements
    case AREA_THE_SUNKEN_RING:
    case AREA_THE_BROKEN_TEMPLATE:
    case AREA_WESTPARK_WORKSHOP:
    case AREA_EASTPARK_WORKSHOP:
        // Graveyards and Workshops are controlled by the same team.
        if (BfGraveyard const* graveyard = GetGraveyardById(GetSpiritGraveyardId(data)))
            return graveyard->GetControlTeamId();
        break;
    default:
        break;
    }

    return Battlefield::GetData(data);
}

void BattlefieldWG::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_DEFENDED_A, GetData(BATTLEFIELD_WG_DATA_DEF_A));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_DEFENDED_H, GetData(BATTLEFIELD_WG_DATA_DEF_H));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_ATTACKED_A, GetData(BATTLEFIELD_WG_DATA_WON_A));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_ATTACKED_H, GetData(BATTLEFIELD_WG_DATA_WON_H));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_ATTACKER, GetAttackerTeam());
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_DEFENDER, GetDefenderTeam());

    // Note: cleanup these two, their names look awkward
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_ACTIVE, IsWarTime() ? 0 : 1);
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_SHOW_WORLDSTATE, IsWarTime() ? 1 : 0);

    for (uint32 itr = 0; itr < 2; ++itr)
        packet.Worldstates.emplace_back(ClockWorldState[itr], int32(GameTime::GetGameTime()) + int32(m_Timer) / int32(1000));

    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_VEHICLE_H, GetData(BATTLEFIELD_WG_DATA_VEHICLE_H));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_MAX_VEHICLE_H, GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_VEHICLE_A, GetData(BATTLEFIELD_WG_DATA_VEHICLE_A));
    packet.Worldstates.emplace_back(WS_BATTLEFIELD_WG_MAX_VEHICLE_A, GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A));

    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
        building->FillInitialWorldStates(packet);

    for (WintergraspWorkshop* workshop : m_Workshops)
        workshop->FillInitialWorldStates(packet);
}

void BattlefieldWG::SendInitWorldStatesToAll()
{
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            SendInitWorldStatesTo(player);
}

void BattlefieldWG::BrokenWallOrTower(TeamId team, BfWGGameObjectBuilding* building)
{
    if (team == GetDefenderTeam())
    {
        for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
            if (itr->second.inWar && itr->second.team == GetAttackerTeam())
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    if (player->GetDistance2d(ASSERT_NOTNULL(GetGameObject(building->GetGUID()))) < 50.0f)
                        player->KilledMonsterCredit(QUEST_CREDIT_DEFEND_SIEGE);
    }
}

// Called when a tower is broke
void BattlefieldWG::UpdatedDestroyedTowerCount(TeamId team)
{
    // Southern tower
    if (team == GetAttackerTeam())
    {
        // Update counter
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, -1);
        UpdateData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT, 1);

        // Remove buff stack on attackers
        for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
            if (itr->second.inWar && itr->second.team == GetAttackerTeam())
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    player->RemoveAuraFromStack(SPELL_TOWER_CONTROL);

        // Add buff stack to defenders and give achievement/quest credit
        for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
            if (itr->second.inWar && itr->second.team == GetDefenderTeam())
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    player->CastSpell(player, SPELL_TOWER_CONTROL, true);
                    player->KilledMonsterCredit(QUEST_CREDIT_TOWERS_DESTROYED);
                    DoCompleteOrIncrementAchievement(ACHIEVEMENTS_WG_TOWER_DESTROY, player);
                }

        // If all three south towers are destroyed (ie. all attack towers), remove ten minutes from battle time
        if (GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT) == 3)
        {
            if (int32(m_Timer - 600000) < 0)
                m_Timer = 0;
            else
                m_Timer -= 600000;
            SendInitWorldStatesToAll();
        }
    }
    else // Keep tower
    {
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, -1);
        UpdateData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF, 1);
    }
}

void BattlefieldWG::ProcessEvent(WorldObject* obj, uint32 eventId)
{
    if (!obj || !IsWarTime())
        return;

    // We handle only gameobjects here
    GameObject* go = obj->ToGameObject();
    if (!go)
        return;

    // On click on titan relic
    if (go->GetEntry() == GO_WINTERGRASP_TITAN_S_RELIC)
    {
        if (CanInteractWithRelic())
            EndBattle(false);
        else if (GameObject* relic = GetRelic())
            relic->SetRespawnTime(RESPAWN_IMMEDIATELY);
    }

    // if destroy or damage event, search the wall/tower and update worldstate/send warning message
    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
    {
        if (go->GetGUID() == building->GetGUID())
        {
            if (GameObject* buildingGo = GetGameObject(building->GetGUID()))
            {
                if (buildingGo->GetGOInfo()->building.damagedEvent == eventId)
                    building->Damaged();
                else if (buildingGo->GetGOInfo()->building.destroyedEvent == eventId)
                    building->Destroyed();
                break;
            }
        }
    }
}

// Called when a tower is damaged, used for honor reward calcul
void BattlefieldWG::UpdateDamagedTowerCount(TeamId team)
{
    if (team == GetAttackerTeam())
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, 1);
    else
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, 1);
}

// Update vehicle count WorldState to player
void BattlefieldWG::UpdateVehicleCountWG()
{
    SendUpdateWorldState(WS_BATTLEFIELD_WG_VEHICLE_H, GetData(BATTLEFIELD_WG_DATA_VEHICLE_H));
    SendUpdateWorldState(WS_BATTLEFIELD_WG_MAX_VEHICLE_H, GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H));
    SendUpdateWorldState(WS_BATTLEFIELD_WG_VEHICLE_A, GetData(BATTLEFIELD_WG_DATA_VEHICLE_A));
    SendUpdateWorldState(WS_BATTLEFIELD_WG_MAX_VEHICLE_A, GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A));
}

void BattlefieldWG::UpdateTenacity()
{
    uint32 alliancePlayers = TeamSize(TEAM_ALLIANCE);
    uint32 hordePlayers = TeamSize(TEAM_HORDE);
    int32 newStack = 0;

    if (alliancePlayers && hordePlayers)
    {
        if (alliancePlayers < hordePlayers)
            newStack = int32((float(hordePlayers / alliancePlayers) - 1) * 4);  // positive, should cast on alliance
        else if (alliancePlayers > hordePlayers)
            newStack = int32((1 - float(alliancePlayers / hordePlayers)) * 4);  // negative, should cast on horde
    }

    if (newStack == int32(m_tenacityStack))
        return;

    m_tenacityStack = newStack;
    // Remove old buff
    if (m_tenacityTeam != TEAM_NEUTRAL)
    {
        for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
            if (itr->second.team == m_tenacityTeam)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    if (player->GetLevel() >= m_MinLevel)
                        player->RemoveAurasDueToSpell(SPELL_TENACITY);

        for (auto itr = m_vehicles[m_tenacityTeam].begin(); itr != m_vehicles[m_tenacityTeam].end(); ++itr)
            if (Creature* creature = GetCreature(*itr))
                creature->RemoveAurasDueToSpell(SPELL_TENACITY_VEHICLE);
    }

    // Apply new buff
    if (newStack)
    {
        m_tenacityTeam = newStack > 0 ? TEAM_ALLIANCE : TEAM_HORDE;

        if (newStack < 0)
            newStack = -newStack;
        if (newStack > 20)
            newStack = 20;

        uint32 buff_honor = SPELL_GREATEST_HONOR;
        if (newStack < 15)
            buff_honor = SPELL_GREATER_HONOR;
        if (newStack < 10)
            buff_honor = SPELL_GREAT_HONOR;
        if (newStack < 5)
            buff_honor = 0;

        for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
            if (itr->second.inWar && itr->second.team == m_tenacityTeam)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    player->SetAuraStack(SPELL_TENACITY, player, newStack);
                    if (buff_honor != 0)
                        player->CastSpell(player, buff_honor, true);
                }

        for (auto itr = m_vehicles[m_tenacityTeam].begin(); itr != m_vehicles[m_tenacityTeam].end(); ++itr)
            if (Creature* creature = GetCreature(*itr))
            {
                creature->SetAuraStack(SPELL_TENACITY_VEHICLE, creature, newStack);
                if (buff_honor != 0)
                    creature->CastSpell(creature, buff_honor, true);
            }
    }
    else
        m_tenacityTeam = TEAM_NEUTRAL;
}

class Battlefield_wintergrasp : public BattlefieldScript
{
public:
    Battlefield_wintergrasp() : BattlefieldScript("battlefield_wg") { }

    Battlefield* GetBattlefield() const override
    {
        return new BattlefieldWG();
    }
};

class npc_wg_give_promotion_credit : public CreatureScript
{
public:
    npc_wg_give_promotion_credit() : CreatureScript("npc_wg_give_promotion_credit") { }

    struct npc_wg_give_promotion_creditAI : public ScriptedAI
    {
        npc_wg_give_promotion_creditAI(Creature* creature) : ScriptedAI(creature) { }

        void JustDied(Unit* killer) override
        {
            if (!killer || killer->GetTypeId() != TYPEID_PLAYER)
                return;

            BattlefieldWG* wintergrasp = static_cast<BattlefieldWG*>(sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG));
            if (!wintergrasp)
                return;

            wintergrasp->HandlePromotion(killer->ToPlayer(), me);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_wg_give_promotion_creditAI(creature);
    }
};

void AddSC_BF_wintergrasp() {
    new Battlefield_wintergrasp();
    new npc_wg_give_promotion_credit();
}
