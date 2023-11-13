/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
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

#ifndef BATTLEFIELD_MGR_H_
#define BATTLEFIELD_MGR_H_

#include "Battlefield.h"
#include <unordered_map>

struct BattleFieldList
{
    std::string scriptname;
    uint32 cooldownTimer;
    uint32 durationTimer;    
    bool enabled;
    uint32 minlevel;
    uint32 maxplayers;
    uint8 controlteam;
    uint32 remainingtime;
    bool active;
    std::string comment;
};
typedef std::unordered_map<uint32, BattleFieldList> BattleFieldInfoContainer;

class Player;
class BattleField;
class ZoneScript;
enum BattlefieldId;

class TC_GAME_API BattlefieldMgr
{
    private:
        BattlefieldMgr();
        ~BattlefieldMgr() { };

    public:
        static BattlefieldMgr* instance();

        // cleanup
        void Die();

        // create SpecialEvents
        void InitBattleFields();

        void Update(uint32 diff);

        // event sector
        void AddBattlefield(uint32 eventId, Battlefield* handle);

        Battlefield* GetEnabledBattlefieldByZoneId(uint32 zoneId);
        Battlefield* GetEnabledBattlefield(BattlefieldId battleId);

        // ZoneScript
        void AddZone(uint32 zoneId, Battlefield* handle);
        ZoneScript* GetZoneScriptbyZoneId(uint32 zoneId) const;
        ZoneScript* GetZoneScriptbyEventId(uint32 eventId) const;

        // called when a player enters an battlefield area
        void HandlePlayerEnterZone(Player* player, uint32 zoneId);
        // called when player leaves an battlefield area
        void HandlePlayerLeaveZone(Player* player, uint32 zoneId);

    private:
        typedef std::vector<Battlefield*> BattlefieldSet;
        typedef std::unordered_map<uint32 /*eventid*/, Battlefield*> BattlefieldMap;
        typedef std::unordered_map<uint32 /*zoneid*/, Battlefield*> BattlefieldZoneMap;
        typedef std::array<uint32, BATTLEFIELD_BATTLEID_MAX> BattlefieldScriptIds;        

        // contains all initiated outdoor pvp events
        // used when initing / cleaning up
        BattlefieldSet m_BattlefieldSet;

        // maps the event ids to an outdoor pvp event
        // used in player event handling
        BattlefieldMap m_BattlefieldMap;

        // maps the zone ids to an outdoor pvp event
        // used in player event handling
        BattlefieldZoneMap m_BattlefieldZoneMap;

        // Holds the outdoor PvP templates
        BattlefieldScriptIds m_BattlefieldScriptIds;

        BattleFieldInfoContainer m_BattleFieldInfoContainer;
        // update interval
        uint32 m_updateTimer;
};

#define sBattlefieldMgr BattlefieldMgr::instance()

#endif // BATTLEFIELD_MGR_H_
