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

#include "ScriptMgr.h"
#include "BattlefieldMgr.h"
#include "Battlefield.h"
#include "Log.h"
#include "Player.h"
#include "ZoneScript.h"

BattlefieldMgr::BattlefieldMgr()
{
    m_updateTimer = 0;
}

void BattlefieldMgr::Die()
{
    for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
        delete* itr;

    m_BattlefieldSet.clear();
    m_BattlefieldScriptIds.fill(0);
    m_BattlefieldMap.clear();
    m_BattlefieldZoneMap.clear();
    m_BattleFieldInfoContainer.clear();
}

BattlefieldMgr* BattlefieldMgr::instance()
{
    static BattlefieldMgr instance;
    return &instance;
}

void BattlefieldMgr::InitBattleFields()
{
    uint32 oldMSTime = getMSTime();

    m_BattleFieldInfoContainer.clear();
    //                                                 0       1            2               3             4       5         6        7             8         9             10
    QueryResult result = WorldDatabase.Query("SELECT TypeId, ScriptName, cooldownTimer, durationTimer, enabled, active, minlevel, maxplayers, controlteam, remainingtime, comment FROM battlefields");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Battlefield definitions. DB table `battlefields` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 typeId = 0;

    // for setup declare
    uint32 cooldownTimer = 0;
    uint32 durationTimer = 0;
    uint32 minlevel = 0;
    uint32 maxplayers = 0;
    uint8 controlteam = 0;
    uint32 remainingtime = 0;
    bool active = false;
    std::string comment = "";
    bool enabledStatus = false;

    do
    {
        Field* fields = result->Fetch();

        typeId = fields[0].GetUInt8();

        if (typeId >= BATTLEFIELD_BATTLEID_MAX)
        {
            TC_LOG_ERROR("sql.sql", "Invalid Battlefield TypeId value %u in battlefields; skipped.", typeId);
            continue;
        }

        // helper for keeping full info about special events
        BattlefieldId realTypeId = BattlefieldId(typeId);
        m_BattlefieldScriptIds[realTypeId] = sObjectMgr->GetScriptId(fields[1].GetString());

        BattleFieldList sel;
        sel.scriptname = fields[1].GetString();
        sel.cooldownTimer = fields[2].GetUInt32();
        sel.durationTimer = fields[3].GetUInt32();
        sel.enabled = fields[4].GetUInt8() > 0;
        sel.active = fields[5].GetUInt8() > 0;
        sel.minlevel = fields[6].GetUInt32();
        sel.maxplayers = fields[7].GetUInt32();
        sel.controlteam = fields[8].GetUInt8();
        sel.remainingtime = fields[9].GetUInt32();
        sel.comment = fields[10].GetString();

        m_BattleFieldInfoContainer[typeId] = sel;

        ++count;
    } while (result->NextRow());

    Battlefield* bf;
    for (uint8 i = 1; i < BATTLEFIELD_BATTLEID_MAX; ++i)
    {
        if (!m_BattlefieldScriptIds[i])
        {
            TC_LOG_ERROR("sql.sql", "Could not initialize Battlefield for type ID %u; no entry in database.", i);
            continue;
        }

        bf = sScriptMgr->CreateBattlefield(m_BattlefieldScriptIds[i]);
        if (!bf)
        {
            TC_LOG_ERROR("server", "Could not initialize Battlefield for type ID %u; got NULL pointer from script.", i);
            continue;
        }

        for (BattleFieldInfoContainer::const_iterator itr = m_BattleFieldInfoContainer.begin(); itr != m_BattleFieldInfoContainer.end(); ++itr)
        {
            if (itr->first == i)
            {
                enabledStatus = itr->second.enabled;
                cooldownTimer = itr->second.cooldownTimer;
                durationTimer = itr->second.durationTimer;
                minlevel = itr->second.minlevel;
                maxplayers = itr->second.maxplayers;
                controlteam = itr->second.controlteam;
                remainingtime = itr->second.remainingtime;
                comment = itr->second.comment;
                active = itr->second.active;
            }
        }

        if (!bf->SetupBattlefield(active, enabledStatus, i, cooldownTimer, durationTimer, minlevel, maxplayers, controlteam, remainingtime))
        {
            TC_LOG_ERROR("server", "Could not initialize battlefield for type ID %u; SetupBattlefield failed.", i);
            delete bf;
            continue;

        }

        TC_LOG_INFO("server.loading", ">> Battlefield (id: %u) - %s successfully initialized", i, comment.c_str());
        m_BattlefieldSet.push_back(bf);
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Battlefield definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlefieldMgr::Update(uint32 diff)
{
    m_updateTimer += diff;
    if (m_updateTimer > BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL)
    {
        for (BattlefieldSet::const_iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
            (*itr)->Update(m_updateTimer);
        m_updateTimer = 0;
    }
}

void BattlefieldMgr::AddBattlefield(uint32 eventId, Battlefield* handle)
{
    m_BattlefieldMap[eventId] = handle;
}

void BattlefieldMgr::HandlePlayerEnterZone(Player* player, uint32 zoneId)
{
    BattlefieldZoneMap::const_iterator itr = m_BattlefieldZoneMap.find(zoneId);
    if (itr == m_BattlefieldZoneMap.end())
        return;

    if (itr->second->HasPlayer(player))
        return;

    itr->second->HandlePlayerEnterZone(player, zoneId);
    TC_LOG_DEBUG("battlefield", "BattlefieldMgr::HandlePlayerEnterZone: player %u entered battlefield id %u", player->GetGUID().GetCounter(), itr->second->GetBattleId());
}

void BattlefieldMgr::HandlePlayerLeaveZone(Player* player, uint32 zoneId)
{
    BattlefieldZoneMap::const_iterator itr = m_BattlefieldZoneMap.find(zoneId);
    if (itr == m_BattlefieldZoneMap.end())
        return;

    if (!itr->second->HasPlayer(player))
        return;

    itr->second->HandlePlayerLeaveZone(player, zoneId);
    TC_LOG_DEBUG("battlefield", "BattlefieldMgr::HandlePlayerLeaveZone: player %u left battlefield id %u", player->GetGUID().GetCounter(), itr->second->GetBattleId());
}

Battlefield* BattlefieldMgr::GetEnabledBattlefieldByZoneId(uint32 zoneId)
{
    BattlefieldZoneMap::const_iterator itr = m_BattlefieldZoneMap.find(zoneId);
    if (itr == m_BattlefieldZoneMap.end())
        return nullptr;

    if (!itr->second->IsEnabled())
        return nullptr;

    return itr->second;
}

Battlefield* BattlefieldMgr::GetEnabledBattlefield(BattlefieldId battleId)
{
    BattlefieldMap::const_iterator itr = m_BattlefieldMap.find(battleId);
    if (itr == m_BattlefieldMap.end())
        return nullptr;

    if (!itr->second->IsEnabled())
        return nullptr;

    return itr->second;
}

// Zone sector
void BattlefieldMgr::AddZone(uint32 zoneId, Battlefield* handle)
{
    m_BattlefieldZoneMap[zoneId] = handle;
}

ZoneScript* BattlefieldMgr::GetZoneScriptbyZoneId(uint32 zoneId) const
{
    BattlefieldZoneMap::const_iterator itr = m_BattlefieldZoneMap.find(zoneId);
    if (itr != m_BattlefieldZoneMap.end())
        return itr->second;
    else
        return nullptr;
}

ZoneScript* BattlefieldMgr::GetZoneScriptbyEventId(uint32 eventId) const
{
    BattlefieldMap::const_iterator itr = m_BattlefieldMap.find(eventId);
    if (itr != m_BattlefieldMap.end())
        return itr->second;
    else
        return nullptr;
}
