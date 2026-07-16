#include "BfCapturePoint.h"
#include "Battlefield.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ObjectAccessor.h"
#include <G3D/g3dmath.h>

// *******************************************************
// ******************* CapturePoint **********************
// *******************************************************

BfCapturePoint::BfCapturePoint(Battlefield* bf) : m_Bf(bf), m_capturePointGUID()
{
    m_maxValue = 0.0f;
    m_minValue = 0.0f;
    m_maxSpeed = 0;
    m_value = 0;
    m_team = TEAM_ALLIANCE;
    m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL;
    m_OldState = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL;
    m_neutralValuePct = 0;
    m_capturePointEntry = 0;
}

BfCapturePoint::~BfCapturePoint()
{
    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
        m_activePlayers[i].clear();
    m_maxValue = 0.0f;
    m_minValue = 0.0f;
    m_maxSpeed = 0;
    m_value = 0;
    m_team = TEAM_ALLIANCE;
    m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL;
    m_OldState = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL;
    m_neutralValuePct = 0;
    m_Bf = nullptr;
    m_capturePointEntry = 0;
}

void BfCapturePoint::HandlePlayerEnter(Player* player)
{
    if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
    {
        player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldState1, 1);
        player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate2, uint32(ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f)));
        player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate3, m_neutralValuePct);
    }
    m_activePlayers[player->GetTeamId()].insert(player->GetGUID());
}

GuidSet::iterator BfCapturePoint::HandlePlayerLeave(Player* player)
{
    if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
        player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldState1, 0);

    GuidSet::iterator current = m_activePlayers[player->GetTeamId()].find(player->GetGUID());

    if (current == m_activePlayers[player->GetTeamId()].end())
        return current; // return end()

    m_activePlayers[player->GetTeamId()].erase(current++);
    return current;
}

void BfCapturePoint::SendChangePhase()
{
    if (!m_capturePointGUID)
        return;

    if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
    {
        // send this too, sometimes the slider disappears, dunno why :(
        SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldState1, 1);
        // send these updates to only the ones in this objective
        SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate2, (uint32)std::ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f));
        // send this too, sometimes it resets :S
        SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate3, m_neutralValuePct);
    }
}

void BfCapturePoint::SetArtKit(GameObject* capturePoint)
{
    uint32 artkit = 0;
    switch (m_State)
    {
        case BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE: artkit = 1; break;
        case BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE: artkit = 2; break;
        default:
            break;
    }
    capturePoint->SetGoArtKit(artkit);
}

void BfCapturePoint::ChangeState()
{
    if (!m_capturePointGUID)
    {
        TC_LOG_ERROR("bg.battlefield", "BfCapturePoint::ChangeState m_capturePointGUID doesn't exist");
        return;
    }
    SetArtKit(m_Bf->GetGameObject(m_capturePointGUID));
    return;
}

void BfCapturePoint::InitState(TeamId team)
{
    m_team = team;
    if (m_team == TEAM_ALLIANCE)
    {
        m_value = m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE;
        m_OldState = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE;
    }
    else
    {
        m_value = -m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE;
        m_OldState = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE;
    }
    SendChangePhase();
    ChangeState();
}

bool BfCapturePoint::SetCapturePointData(GameObject* capturePoint)
{
    ASSERT(capturePoint);
    TC_LOG_DEBUG("bg.battlefield", "Creating capture point {}", capturePoint->GetEntry());
    m_capturePointGUID = capturePoint->GetGUID();

    // check info existence
    GameObjectTemplate const* goinfo = capturePoint->GetGOInfo();
    if (goinfo->type != GAMEOBJECT_TYPE_CAPTURE_POINT)
    {
        TC_LOG_ERROR("bg.battlefield", "OutdoorPvP: GO {} is not a capture point!", capturePoint->GetEntry());
        return false;
    }

    // get the needed values from goinfo
    m_maxValue = goinfo->capturePoint.maxTime;
    m_maxSpeed = m_maxValue / (goinfo->capturePoint.minTime ? goinfo->capturePoint.minTime : 60);
    m_neutralValuePct = goinfo->capturePoint.neutralPercent;
    m_minValue = m_maxValue * goinfo->capturePoint.neutralPercent / 100;
    m_capturePointEntry = capturePoint->GetEntry();
    return true;
}

GameObject* BfCapturePoint::GetCapturePointGo()
{
    return m_Bf->GetGameObject(m_capturePointGUID);
}

void BfCapturePoint::Update(uint32 diff)
{
    if (!m_Bf->IsWarTime())
        return;

    GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID);
    if (!capturePoint)
        return;

    float radius = (float)capturePoint->GetGOInfo()->capturePoint.radius / 2;

    // make vector of players near GO
    std::vector<Player*> _players;
    Trinity::AnyPlayerInObjectRangeCheck checker(capturePoint, radius);
    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(capturePoint, _players, checker);
    Cell::VisitWorldObjects(capturePoint, searcher, radius);

    // Part 1
    for (uint32 team = 0; team < 2; ++team)
    {
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end();)
        {
            ObjectGuid playerGuid = *itr;
            ++itr;

            if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                if (!capturePoint->IsWithinDistInMap(player, radius) || !player->IsOutdoorPvPActive())
                    HandlePlayerLeave(player);
        }
    }

    // Part 2
    for (auto const& pointer : _players)
    {
        for (uint32 team = 0; team < 2; ++team)
        {
            // Enter new plrs
            if (pointer->IsOutdoorPvPActive() && !IsInsideObjective(pointer))
                HandlePlayerEnter(pointer);
        }
    }
    _players.clear();

    // get the difference of numbers
    float fact_diff = ((float)m_activePlayers[0].size() - (float)m_activePlayers[1].size()) * float(diff) / float(BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL);
    if (!fact_diff)
        return;

    uint32 Challenger = 0;
    float maxDiff = m_maxSpeed * diff;

    if (fact_diff < 0)
    {
        // horde is in majority, but it's already horde-controlled -> no change
        if (m_State == BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE && m_value <= -m_maxValue)
            return;

        if (fact_diff < -maxDiff)
            fact_diff = -maxDiff;

        Challenger = HORDE;
    }
    else
    {
        // ally is in majority, but it's already ally-controlled -> no change
        if (m_State == BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE && m_value >= m_maxValue)
            return;

        if (fact_diff > maxDiff)
            fact_diff = maxDiff;

        Challenger = ALLIANCE;
    }

    float oldValue = m_value;
    //TeamId oldTeam = m_team;

    m_OldState = m_State;

    m_value += fact_diff;

    if (m_value < -m_minValue) // red
    {
        if (m_value < -m_maxValue)
            m_value = -m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE;
        m_team = TEAM_HORDE;
    }
    else if (m_value > m_minValue) // blue
    {
        if (m_value > m_maxValue)
            m_value = m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE;
        m_team = TEAM_ALLIANCE;
    }
    else if (oldValue * m_value <= 0) // grey, go through mid point
    {
        // if challenger is ally, then n->a challenge
        if (Challenger == ALLIANCE)
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE;
        // if challenger is horde, then n->h challenge
        else if (Challenger == HORDE)
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }
    else // grey, did not go through mid point
    {
        // old phase and current are on the same side, so one team challenges the other
        if (Challenger == ALLIANCE && (m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE || m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE))
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE;
        else if (Challenger == HORDE && (m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE || m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE))
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }

    if (m_value != oldValue)
        SendChangePhase();

    if (m_OldState != m_State)
    {
        //FMT_LOG_ERROR("bg.battlefield", "{}->{}", m_OldState, m_State);
        ChangeState();
        ChangeTeam(m_team);
        return;
    }

    return;
}

void BfCapturePoint::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (uint8 team = 0; team < PVP_TEAMS_COUNT; ++team)
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)  // send to all players present in the area
            if (Player* player = ObjectAccessor::FindPlayer(*itr))
                player->SendUpdateWorldState(field, value);
}

void BfCapturePoint::SendObjectiveComplete(uint32 id, ObjectGuid guid)
{
    uint8 team;
    switch (m_State)
    {
        case BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE:
            team = TEAM_ALLIANCE;
            break;
        case BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE:
            team = TEAM_HORDE;
            break;
        default:
            return;
    }

    // send to all players present in the area
    for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(*itr))
            player->KilledMonsterCredit(id, guid);
}

bool BfCapturePoint::IsInsideObjective(Player* player) const
{
    return m_activePlayers[player->GetTeamId()].find(player->GetGUID()) != m_activePlayers[player->GetTeamId()].end();
}
