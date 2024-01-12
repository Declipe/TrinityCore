#pragma once
#include "Position.h"
#include "SharedDefines.h"
#include "ZoneScript.h"
#include <map>

class Battlefield;
class Player;
class Creature;

class TC_GAME_API BfGraveyard
{
public:
    BfGraveyard(Battlefield* bf);
    virtual ~BfGraveyard();

    // Method to changing who controls the graveyard
    void GiveControlTo(TeamId team);
    TeamId GetControlTeamId() const { return m_ControlTeam; }

    // Find the nearest graveyard to a player
    float GetDistance(Player* player);

    // Initialize the graveyard
    void Initialize(TeamId startcontrol, uint32 gy);

    // Set spirit service for the graveyard
    void SetSpirit(Creature* spirit, TeamId team);

    // Add a player to the graveyard
    void AddPlayer(ObjectGuid player_guid);

    // Remove a player from the graveyard
    void RemovePlayer(ObjectGuid player_guid);

    // Resurrect players
    void Resurrect();

    // Move players waiting to that graveyard on the nearest one
    void RelocateDeadPlayers();

    // Check if this graveyard has a spirit guide
    bool HasNpc(ObjectGuid guid);

    // Check if a player is in this graveyard's resurrect queue
    bool HasPlayer(ObjectGuid guid) { return m_ResurrectQueue.find(guid) != m_ResurrectQueue.end(); }

    // Get the graveyard's ID.
    uint32 GetGraveyardId() const { return m_GraveyardId; }

protected:
    Battlefield* m_Bf;
    uint32 m_GraveyardId;
    TeamId m_ControlTeam;
    ObjectGuid m_SpiritGuide[PVP_TEAMS_COUNT];
    GuidSet m_ResurrectQueue;
};
