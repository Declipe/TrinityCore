#include "BfGraveyard.h"
#include "Battleground.h"
#include "Battlefield.h"
#include "DBCStores.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Log.h"
#include "ObjectAccessor.h"

// ----------------------
// - BfGraveyard Method -
// ----------------------
BfGraveyard::BfGraveyard(Battlefield* bf)
{
    m_Bf = bf;
    m_GraveyardId = 0;
    m_ControlTeam = TEAM_NEUTRAL;
}

BfGraveyard::~BfGraveyard()
{
    m_Bf = nullptr;
    m_GraveyardId = 0;
    m_ControlTeam = TEAM_NEUTRAL;
    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_SpiritGuide[i].Clear();
    m_ResurrectQueue.clear();
}

void BfGraveyard::Initialize(TeamId startControl, uint32 graveyardId)
{
    m_ControlTeam = startControl;
    m_GraveyardId = graveyardId;
}

void BfGraveyard::SetSpirit(Creature* spirit, TeamId team)
{
    if (!spirit)
    {
        TC_LOG_ERROR("bg.battlefield", "BfGraveyard::SetSpirit: Invalid Spirit.");
        return;
    }

    m_SpiritGuide[team] = spirit->GetGUID();
    spirit->SetReactState(REACT_PASSIVE);
}

float BfGraveyard::GetDistance(Player* player)
{
    WorldSafeLocsEntry const* safeLoc = sDBCMgr->GetWorldSafeLocsEntry(m_GraveyardId);
    return player->GetDistance2d(safeLoc->Loc.X, safeLoc->Loc.Y);
}

void BfGraveyard::AddPlayer(ObjectGuid playerGuid)
{
    if (!m_ResurrectQueue.count(playerGuid))
    {
        m_ResurrectQueue.insert(playerGuid);

        if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
            player->CastSpell(player, SPELL_WAITING_FOR_RESURRECT, true);
    }
}

void BfGraveyard::RemovePlayer(ObjectGuid playerGuid)
{
    m_ResurrectQueue.erase(m_ResurrectQueue.find(playerGuid));

    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
        player->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);
}

void BfGraveyard::Resurrect()
{
    if (m_ResurrectQueue.empty())
        return;

    for (GuidSet::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
    {
        // Get player object from his guid
        Player* player = ObjectAccessor::FindPlayer(*itr);
        if (!player)
            continue;

        // Check if the player is in world and on the good graveyard
        if (player->IsInWorld())
            if (Creature* spirit = m_Bf->GetCreature(m_SpiritGuide[m_ControlTeam]))
                spirit->CastSpell(spirit, SPELL_SPIRIT_HEAL, true);

        // Resurrect player
        player->CastSpell(player, SPELL_RESURRECTION_VISUAL, true);
        player->ResurrectPlayer(1.0f);
        player->CastSpell(player, 6962, true);
        player->CastSpell(player, SPELL_SPIRIT_HEAL_MANA, true);

        player->SpawnCorpseBones(false);
    }

    m_ResurrectQueue.clear();
}

// For changing graveyard control
void BfGraveyard::GiveControlTo(TeamId team)
{
    m_ControlTeam = team;

    // Guide switching
    // Note: Visiblity changes are made by phasing
    if (Creature* oldSpirit = m_Bf->GetCreature(m_SpiritGuide[1 - team]))
        oldSpirit->SetVisible(false);
    else
        TC_LOG_ERROR("bg.battlefield", "BfGraveyard::GiveControlTo: oldSpirit - Invalid Spirit.");

    if (Creature* newSpirit = m_Bf->GetCreature(m_SpiritGuide[team]))
        newSpirit->SetVisible(true);
    else
        TC_LOG_ERROR("bg.battlefield", "BfGraveyard::GiveControlTo: newSpirit - Invalid Spirit.");
    
    // Teleport to other graveyard, player witch were on this graveyard
    RelocateDeadPlayers();
}

void BfGraveyard::RelocateDeadPlayers()
{
    WorldSafeLocsEntry const* closestGrave = nullptr;
    for (GuidSet::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindPlayer(*itr);
        if (!player)
            continue;

        if (closestGrave)
            player->TeleportTo(player->GetMapId(), closestGrave->Loc.X, closestGrave->Loc.Y, closestGrave->Loc.Z, player->GetOrientation());
        else
        {
            closestGrave = m_Bf->GetClosestGraveyard(player);
            if (closestGrave)
                player->TeleportTo(player->GetMapId(), closestGrave->Loc.X, closestGrave->Loc.Y, closestGrave->Loc.Z, player->GetOrientation());
        }
    }
}

bool BfGraveyard::HasNpc(ObjectGuid guid)
{
    if (!m_SpiritGuide[TEAM_ALLIANCE] || !m_SpiritGuide[TEAM_HORDE])
        return false;

    if (!m_Bf->GetCreature(m_SpiritGuide[TEAM_ALLIANCE]) ||
        !m_Bf->GetCreature(m_SpiritGuide[TEAM_HORDE]))
        return false;

    return (m_SpiritGuide[TEAM_ALLIANCE] == guid || m_SpiritGuide[TEAM_HORDE] == guid);
}
