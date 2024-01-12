#include "BattlefieldWG.h"

namespace WorldPackets
{
    namespace WorldState
    {
        class InitWorldStates;
    }
}

// *********************************************************
// *****************WorkShop Data & Element*****************
// *********************************************************

uint8 const WG_MAX_WORKSHOP = 6;

struct StaticWintergraspWorkshopInfo
{
    uint32 entry;
    uint8 WorkshopId;
    uint32 WorldStateId;
    Position Pos;
    QuaternionData Rot;

    struct
    {
        uint8 AllianceCapture;
        uint8 AllianceAttack;
        uint8 HordeCapture;
        uint8 HordeAttack;
    } TextIds;
};

StaticWintergraspWorkshopInfo const WorkshopData[WG_MAX_WORKSHOP] =
{
    //entry              WorkshopId                    WorldStateId                       Position                                QuaternionData                     AllianceCapture                                                AllianceAttack                                 HordeCapture                                        HordeAttack
    { 194959, BATTLEFIELD_WG_WORKSHOP_SE, WS_BATTLEFIELD_WG_WORKSHOP_SE, { 4398.08f, 2356.5f, 376.19f, 0.525406f }, { 0.f, 0.f, 0.259692f, 0.965692f }, { BATTLEFIELD_WG_TEXT_EASTSPARK_CAPTURE_ALLIANCE,     BATTLEFIELD_WG_TEXT_EASTSPARK_ATTACK_ALLIANCE,     BATTLEFIELD_WG_TEXT_EASTSPARK_CAPTURE_HORDE,     BATTLEFIELD_WG_TEXT_EASTSPARK_ATTACK_HORDE     } },
    { 194962, BATTLEFIELD_WG_WORKSHOP_SW, WS_BATTLEFIELD_WG_WORKSHOP_SW, { 4390.78f, 3304.09f, 372.429f, 6.09702f }, { 0.f, 0.f, 0.0929482f, -0.995671f }, { BATTLEFIELD_WG_TEXT_WESTSPARK_CAPTURE_ALLIANCE,     BATTLEFIELD_WG_TEXT_WESTSPARK_ATTACK_ALLIANCE,     BATTLEFIELD_WG_TEXT_WESTSPARK_CAPTURE_HORDE,     BATTLEFIELD_WG_TEXT_WESTSPARK_ATTACK_HORDE     } },
    { 190475, BATTLEFIELD_WG_WORKSHOP_NE, WS_BATTLEFIELD_WG_WORKSHOP_NE, { 4949.34f, 2432.59f, 320.177f, 1.38621f }, { 0.f, 0.f, 0.638929f, 0.769266f }, { BATTLEFIELD_WG_TEXT_SUNKEN_RING_CAPTURE_ALLIANCE,   BATTLEFIELD_WG_TEXT_SUNKEN_RING_ATTACK_ALLIANCE,   BATTLEFIELD_WG_TEXT_SUNKEN_RING_CAPTURE_HORDE,   BATTLEFIELD_WG_TEXT_SUNKEN_RING_ATTACK_HORDE   } },
    { 190487, BATTLEFIELD_WG_WORKSHOP_NW, WS_BATTLEFIELD_WG_WORKSHOP_NW, { 4948.52f, 3342.34f, 376.875f, 4.40057f }, { 0.f, 0.f, 0.808329f, -0.588731f }, { BATTLEFIELD_WG_TEXT_BROKEN_TEMPLE_CAPTURE_ALLIANCE, BATTLEFIELD_WG_TEXT_BROKEN_TEMPLE_ATTACK_ALLIANCE, BATTLEFIELD_WG_TEXT_BROKEN_TEMPLE_CAPTURE_HORDE, BATTLEFIELD_WG_TEXT_BROKEN_TEMPLE_ATTACK_HORDE } },

    // KEEP WORKSHOPS - It can't be taken, so it doesn't have a textids
    { 0, BATTLEFIELD_WG_WORKSHOP_KEEP_WEST, WS_BATTLEFIELD_WG_WORKSHOP_K_W, { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.0130891f, 0.999914f }, { 0, 0, 0, 0 } },
    { 0, BATTLEFIELD_WG_WORKSHOP_KEEP_EAST, WS_BATTLEFIELD_WG_WORKSHOP_K_E, { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.0130891f, 0.999914f }, { 0, 0, 0, 0 } }
};

class WintergraspWorkshop
{
private:
    BattlefieldWG* _wg;                             // Pointer to wintergrasp    
    WintergraspGameObjectState _state;              // For worldstate
    TeamId _teamControl;                            // Team witch control the workshop
    WintergraspCapturePoint* _capturePoint;         // Pointer to CapturePoint

    StaticWintergraspWorkshopInfo const* _staticInfo;
    GuidVector m_gameObjects[PVP_TEAMS_COUNT];
    GuidVector m_creatures[PVP_TEAMS_COUNT];

public:
    WintergraspWorkshop(BattlefieldWG* wg, uint8 type);
    ~WintergraspWorkshop();

    uint8 GetId() const;
    TeamId GetTeamControl() const { return _teamControl; }

    // Called on change faction in CapturePoint class
    void GiveControlTo(TeamId teamId, bool init = false);

    void UpdateGraveyardAndWorkshop();

    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet);

    void Save();

    void LinkCapturePoint(WintergraspCapturePoint* pointer);

    void FillRelatedGoAndNpc();

    void UpdateCreatureAndGo();

    void prepareDelete();

    ObjectGuid m_workshopGoGUID;
};
