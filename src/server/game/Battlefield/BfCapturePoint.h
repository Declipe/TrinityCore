#pragma once
#include "SharedDefines.h"
#include "ZoneScript.h"

class Battlefield;
class Player;

namespace WorldPackets
{
    namespace WorldState
    {
        class InitWorldStates;
    }
}

enum BattlefieldObjectiveStates
{
    BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL = 0,
    BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE
};

class TC_GAME_API BfCapturePoint
{
public:
    BfCapturePoint(Battlefield* bf);
    virtual ~BfCapturePoint();

    virtual void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& /*packet*/) { }

    // Send world state update to all players present
    void SendUpdateWorldState(uint32 field, uint32 value);

    // Send kill notify to players in the controlling faction
    void SendObjectiveComplete(uint32 id, ObjectGuid guid);

    // Used when player is activated/inactivated in the area
    virtual void HandlePlayerEnter(Player* player);
    virtual GuidSet::iterator HandlePlayerLeave(Player* player);
    //virtual void HandlePlayerActivityChanged(Player* player);

    // Checks if player is in range of a capture credit marker
    bool IsInsideObjective(Player* player) const;

    // Returns true if the state of the objective has changed, in this case, the OutdoorPvP must send a world state ui update.
    virtual void Update(uint32 diff);
    virtual void ChangeTeam(TeamId /*team*/) { }
    virtual void SendChangePhase();
    virtual void ChangeState();
    virtual void InitState(TeamId team);
    virtual void SetArtKit(GameObject* capturePoint);

    bool SetCapturePointData(GameObject* capturePoint);
    GameObject* GetCapturePointGo();
    uint32 GetCapturePointEntry() const { return m_capturePointEntry; }

    TeamId GetTeamId() const { return m_team; }

protected:
    // active Players in the area of the objective, 0 - alliance, 1 - horde
    GuidSet m_activePlayers[PVP_TEAMS_COUNT];

    // Total shift needed to capture the objective
    float m_maxValue;
    float m_minValue;

    // Maximum speed of capture
    float m_maxSpeed;

    // The status of the objective
    float m_value;
    TeamId m_team;

    // Objective states
    BattlefieldObjectiveStates m_OldState;
    BattlefieldObjectiveStates m_State;

    // Neutral value on capture bar
    uint32 m_neutralValuePct;

    // Pointer to the Battlefield this objective belongs to
    Battlefield* m_Bf;

    // Capture point entry
    uint32 m_capturePointEntry;

    // Gameobject related to that capture point
    ObjectGuid m_capturePointGUID;
};
