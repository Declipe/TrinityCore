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

#ifndef BATTLEFIELD_H_
#define BATTLEFIELD_H_

#include "Position.h"
#include "SharedDefines.h"
#include "ZoneScript.h"
#include "BfCapturePoint.h"
#include "BfGraveyard.h"
#include <map>

enum BattlefieldTypes
{
    BATTLEFIELD_WG  = 1, // Wintergrasp
    BATTLEFIELD_MAX
};

enum BattlefieldIDs
{
    BATTLEFIELD_BATTLEID_WG                      = 1        // Wintergrasp battle
};

enum BattlefieldSounds
{
    BF_SOUND_HORDE_WINS                          = 8454,
    BF_SOUND_ALLIANCE_WINS                       = 8455,
    BF_SOUND_START                               = 3439
};

enum BattlefieldTimers
{
    BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL        = 1000
};

// some class predefs
class Creature;
class GameObject;
class Group;
class Map;
class Player;
class Unit;
class WorldPacket;

struct QuaternionData;
struct WorldSafeLocsEntry;

/* Containers : players, capturePoints, graveyards */
typedef std::vector<BfGraveyard*> GraveyardVect;
struct PlayerHolder
{
    bool inZone;
    bool inQueue;
    bool inWar;
    bool isWaitingQueue;
    bool isWaitingWar;
    bool isWaitingKick;
    TeamId team;
    time_t time;
};
typedef std::unordered_map<ObjectGuid, PlayerHolder> PlayerHolderContainer;

class TC_GAME_API Battlefield : public ZoneScript
{
    friend class BattlefieldMgr;

    public:
        /// Constructor
        Battlefield();
        ~Battlefield();

        /// typedef of map witch store capturepoint and the associate gameobject entry
        typedef std::map<ObjectGuid::LowType /*lowguid */, BfCapturePoint*> BfCapturePointMap;

        /// Call this to init the Battlefield
        virtual bool SetupBattlefield() { return true; }

        virtual void prepareDelete() { }

        /**
         * \brief Called every time for update bf data and time
         * - Update timer for start/end battle
         * - Invite player in zone to queue m_StartGroupingTimer minutes before start
         * - Kick Afk players
         * \param diff : time ellapsed since last call (in ms)
         */
        virtual void Update(uint32 diff);
        void InitStalker(uint32 entry, Position const& pos);
        void SendWarning(uint8 id, WorldObject const* target = nullptr);
        void StartBattle();
        void EndBattle(bool endByTimer);
        void RegisterZone(uint32 zoneid);
        void HideNpc(Creature* creature);
        void ShowNpc(Creature* creature, bool aggressive);

        /** Player section  BattlefieldPlayers.cpp **/
        PlayerHolderContainer const& GetPlayerMap() const { return m_PlayerMap; }
        uint32 GetMapId() const { return m_MapId; }
        void AddPlayer(Player* plr, bool InZone, bool IsWaitingQueue, bool IsWaitingWar, bool IsWaitingKick, time_t time = 0);
        bool HasPlayer(Player* player) const;
        bool NeedToRemove(Player* player) const;
        void RemovePlayer(Player* plr);
        uint32 TeamSize(TeamId team);
        
        void KickPlayerFromBattlefield(ObjectGuid guid);
        void UpdateZoneStatusInPlayerMap(Player* plr, bool inZone);
        // Invite all players in zone to join the queue, called x minutes before battle start in Update()
        void InvitePlayersInZoneToQueue();
        // Invite all players in queue to join battle on battle start
        void InvitePlayersInQueueToWar();
        // Invite all players not in queue to join battle on battle start
        void TryInvitePlayersNotInQueueToWarOrKickThem();
        void InviteNewPlayerToQueue(Player* player, uint32 battleId, bool isInZone);
        void InviteNewPlayerToWar(Player* player, uint32 battleId, bool isInZone);

        /// Called when player (player) enter in zone
        void HandlePlayerEnterZone(Player* player, uint32 zone);
        /// Called when player (player) leave the zone
        void HandlePlayerLeaveZone(Player* player, uint32 zone);
        // Called in WorldSession::HandleBfEntryInviteResponse
        void PlayerAcceptInviteToQueue(Player* player);
        // Called in WorldSession::HandleBfEntryInviteResponse
        void PlayerAcceptInviteToWar(Player* player);
        // Called in WorldSession::HandleBfEntryInviteResponse
        void PlayerDeclineInviteToWar(Player* player);
        // Called in WorldSession::HandleBfQueueExitRequest
        void AskToLeaveQueue(Player* player);
        // Called in WorldSession::HandleHearthAndResurrect
        void PlayerAskToLeave(Player* player);
        void PlayerTryToReQueueByLevelUp(Player* player);

        void DoPlaySoundToAll(uint32 soundID);

        // use for send a packet for all player list
        void BroadcastPacketToZone(WorldPacket const* data) const;
        void BroadcastPacketToQueue(WorldPacket const* data) const;
        void BroadcastPacketToWar(WorldPacket const* data) const;

        void TeamCastSpell(TeamId team, int32 spellId);
        void SendInitWorldStatesTo(Player* player);
        /// Update data of a worldstate to all players present in zone
        void SendUpdateWorldState(uint32 variable, uint32 value);
        void SendAreaSpiritHealerQueryOpcode(Player* player, ObjectGuid guid);

        /// Called when a Unit is kill in battlefield zone
        virtual void HandleKill(Player* /*killer*/, Unit* /*killed*/) { };

        uint32 GetTypeId() const { return m_TypeId; }
        uint32 GetZoneId() const { return m_ZoneId; }

        void TeamApplyBuff(TeamId team, uint32 spellId, uint32 spellId2 = 0);

        /// Return true if battle is start, false if battle is not started
        bool IsWarTime() const { return m_isActive; }

        /// Enable or Disable battlefield
        void ToggleBattlefield(bool enable) { m_IsEnabled = enable; }
        /// Return if battlefield is enable
        bool IsEnabled() const { return m_IsEnabled; }

        // All-purpose data storage 64 bit
        virtual uint64 GetData64(uint32 dataId) const override { return m_Data64[dataId]; }
        virtual void SetData64(uint32 dataId, uint64 value) override { m_Data64[dataId] = value; }

        // All-purpose data storage 32 bit
        virtual uint32 GetData(uint32 dataId) const override { return m_Data32[dataId]; }
        virtual void SetData(uint32 dataId, uint32 value) override { m_Data32[dataId] = value; }
        virtual void UpdateData(uint32 index, int32 pad) { m_Data32[index] += pad; }

        // Battlefield - generic methods
        TeamId GetDefenderTeam() const { return m_DefenderTeam; }
        TeamId GetAttackerTeam() const { return TeamId(1 - m_DefenderTeam); }
        TeamId GetOtherTeam(TeamId team) const { return (team == TEAM_HORDE ? TEAM_ALLIANCE : TEAM_HORDE); }
        void SetDefenderTeam(TeamId team) { m_DefenderTeam = team; }

        // Group methods
        /**
         * \brief Find a not full battlefield group, if there is no, create one
         * \param TeamId : Id of player team for who we search a group (player->GetTeamId())
         */
        Group* GetFreeBfRaid(TeamId TeamId);
        /// Return battlefield group where player is.
        Group* GetGroupPlayer(ObjectGuid guid, TeamId TeamId);
        /// Force player to join a battlefield group
        bool AddOrSetPlayerToCorrectBfGroup(Player* player);

        // Graveyard methods
        // Find which graveyard the player must be teleported to to be resurrected by spiritguide
        WorldSafeLocsEntry const* GetClosestGraveyard(Player* player);

        virtual void AddPlayerToResurrectQueue(ObjectGuid npc_guid, ObjectGuid player_guid);
        void RemovePlayerFromResurrectQueue(ObjectGuid player_guid);
        void SetGraveyardNumber(uint32 number) { m_GraveyardList.resize(number); }
        BfGraveyard* GetGraveyardById(uint32 id) const;

        // Misc methods
        Creature* SpawnCreature(uint32 entry, Position const& pos);
        GameObject* SpawnGameObject(uint32 entry, Position const& pos, QuaternionData const& rot);

        Creature* GetCreature(ObjectGuid guid);
        GameObject* GetGameObject(ObjectGuid guid);

        // Script-methods

        /// Called on start
        virtual void OnBattleStart() { }
        /// Called at the end of battle
        virtual void OnBattleEnd(bool /*endByTimer*/) { }
        /// Called x minutes before battle start when player in zone are invite to join queue
        virtual void OnStartGrouping() { }
        /// Called when a player accept to join the battle
        virtual void OnPlayerJoinWar(Player* /*player*/) { }
        /// Called when a player leave the battle
        virtual void OnPlayerLeaveWar(Player* /*player*/) { }
        /// Called when a player leave battlefield zone
        virtual void OnPlayerLeaveZone(Player* /*player*/) { }
        /// Called when a player enter in battlefield zone
        virtual void OnPlayerEnterZone(Player* /*player*/) { }
        virtual void switchSide() { }
        
        uint32 GetBattleId() const { return m_BattleId; }

        virtual void DoCompleteOrIncrementAchievement(uint32 /*achievement*/, Player* /*player*/, uint8 /*incrementNumber = 1*/) { }

        /// Send all worldstate data to all player in zone.
        virtual void SendInitWorldStatesToAll() = 0;
        virtual void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& /*packet*/) = 0;

        /// Return if we can use mount in battlefield
        bool CanFlyIn() { return !m_isActive; }

        GraveyardVect GetGraveyardVector() const { return m_GraveyardList; }

        uint32 GetTimer() const { return m_Timer; }
        void SetTimer(uint32 timer) { m_Timer = timer; }
        uint32 GetFreeslot(TeamId team) const { return m_freeslots[team]; }
        uint8 GetMinimalLevel() { return m_MinLevel; }

    protected:
        ObjectGuid StalkerGuid;
        uint32 m_Timer;                                         // Global timer for event
        bool m_IsEnabled;
        bool m_isActive;
        TeamId m_DefenderTeam;

        // Map of the objectives belonging to this OutdoorPvP
        BfCapturePointMap m_capturePoints;

        // Players info maps
        PlayerHolderContainer      m_PlayerMap;

        // Variables that must exist for each battlefield
        uint32 m_TypeId;                                        // See enum BattlefieldTypes
        uint32 m_BattleId;                                      // BattleID (for packet)
        uint32 m_ZoneId;                                        // ZoneID of Wintergrasp = 4197
        uint32 m_MapId;                                         // MapId where is Battlefield
        Map* m_Map;
        uint32 m_MaxPlayer;                                     // Maximum number of player that participated to Battlefield
        uint32 m_MinPlayer;                                     // Minimum number of player for Battlefield start
        uint32 m_freeslots[PVP_TEAMS_COUNT];                    // Count of free slots for new members
        uint32 m_TimeForAcceptInvite;
        uint32 m_MinLevel;                                      // Required level to participate at Battlefield
        uint32 m_BattleTime;                                    // Length of a battle
        uint32 m_NoWarBattleTime;                               // Time between two battles
        uint32 m_StartGroupingTimer;                            // Timer for invite players in area 15 minute before start battle
        uint32 m_LastResurrectTimer;                            // Timer for resurrect player every 30 sec
        WorldLocation KickPosition;                             // Position where players are teleported if they switch to afk during the battle or if they don't accept invitation

        // Graveyard variables
        GraveyardVect m_GraveyardList;                          // Vector witch contain the different GY of the battle

        bool m_StartGrouping;                                   // bool for know if all players in area has been invited

        GuidUnorderedSet m_Groups[PVP_TEAMS_COUNT];              // Contain different raid group

        std::vector<uint64> m_Data64;
        std::vector<uint32> m_Data32;

        // use for switch off all worldstate for client
        virtual void SendRemoveWorldStates(Player* /*player*/) { }

        // CapturePoint system
        void AddCapturePoint(BfCapturePoint* cp) { m_capturePoints[cp->GetCapturePointEntry()] = cp; }

        BfCapturePoint* GetCapturePoint(ObjectGuid::LowType lowguid) const
        {
            Battlefield::BfCapturePointMap::const_iterator itr = m_capturePoints.find(lowguid);
            if (itr != m_capturePoints.end())
                return itr->second;
            return nullptr;
        }
};

#endif
