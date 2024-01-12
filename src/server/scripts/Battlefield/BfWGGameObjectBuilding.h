#include "BattlefieldWG.h"

// ********************************************************************
// *         Structs using for Building, Graveyard, Workshop          *
// ********************************************************************
// Structure for different buildings that can be destroyed during battle

class BfWGGameObjectBuilding
{
private:
    // WG object
    BattlefieldWG* _wg;
    // Linked gameobject
    ObjectGuid _buildGUID;
    // the team that controls this point
    TeamId _teamControl;
    WintergraspGameObjectBuildingType _type;
    uint32 _worldState;
    WintergraspGameObjectState _state;
    StaticWintergraspTowerInfo const* _staticTowerInfo;

    // attackers container
    GuidVector m_attackerGameObjects[PVP_TEAMS_COUNT];
    GuidVector m_attackerCreatures[PVP_TEAMS_COUNT];

    GuidVector m_TowerCannonBottomList;
    GuidVector m_TurretTopList;

public:
    BfWGGameObjectBuilding(BattlefieldWG* wg, WintergraspGameObjectBuildingType type, uint32 worldState);
    ~BfWGGameObjectBuilding();
    void Init(GameObject* go);
    ObjectGuid const& GetGUID() const { return _buildGUID; }

    void Rebuild();
    void RebuildGate();

    // Called when associated gameobject is damaged
    void Damaged();
    // Called when associated gameobject is destroyed
    void Destroyed();
    void UpdateCreatureAndGo();
    void UpdateTurretAttack(bool disable);
    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet);
    void Save();
    void prepareDelete();
};
