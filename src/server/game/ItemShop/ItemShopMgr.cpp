/*
 * Copyright (C) 2016-2020 AtieshCore <https://gitlab.com/healthstone/>
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

#include "DatabaseEnv.h"
#include "ItemShopMgr.h"
#include "Timer.h"
#include "Log.h"

ItemShopMgr* ItemShopMgr::instance()
{
    static ItemShopMgr instance;
    return &instance;
}

ItemShopMgr::~ItemShopMgr()
{
    _itemShopStore.clear();
}

void ItemShopMgr::Initialize()
{
    _LoadItemsFromShop();
}

void ItemShopMgr::Reload()
{
    _LoadItemsFromShop();
}

void ItemShopMgr::_LoadItemsFromShop()
{
    uint32 oldMSTime = getMSTime();

    _itemShopStore.clear();                                  // for reload case

    //                                                0     1        2         3            4        5
    QueryResult result = ZynDatabase.Query("SELECT id, entryID, itemName, groupName, coinCost, gossipAction FROM item_shop");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 item from shop. DB table `item_shop` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();

        ItemFromShop is;
        is.entryID = fields[1].GetUInt32();
        is.itemName = fields[2].GetString();
        is.groupName = fields[3].GetString();
        is.coinCost = fields[4].GetUInt32();
        is.gossipAction = fields[5].GetUInt32();

        _itemShopStore[id] = is;

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} Items from Shop in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ItemShopMgr::AddItemFromShop(ItemFromShop& data)
{
    // find max id
    uint32 new_id = 0;
    for (ItemShopContainer::const_iterator itr = _itemShopStore.begin(); itr != _itemShopStore.end(); ++itr)
        if (itr->first > new_id)
            new_id = itr->first;

    // use next
    ++new_id;

    _itemShopStore[new_id] = data;

    ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZYN_INS_ITEM_FROM_SHOP);
    stmt->setUInt32(0, new_id);
    stmt->setUInt32(1, data.entryID);
    stmt->setString(2, data.itemName);
    stmt->setString(3, data.groupName);
    stmt->setUInt32(4, data.coinCost);
    stmt->setUInt32(5, data.gossipAction);
    ZynDatabase.Execute(stmt);

    return true;
}

bool ItemShopMgr::DeleteItemFromShop(uint32 const& entryID, std::string const& groupName)
{
    std::vector<uint32>entryIDs;
    for (ItemShopContainer::iterator itr = _itemShopStore.begin(); itr != _itemShopStore.end(); ++itr)
    {
        if (itr->second.entryID == entryID && itr->second.groupName == groupName)
            entryIDs.push_back(itr->first);
    }

    if (entryIDs.empty())
    {
        entryIDs.clear();
        return false;
    }

    for (const uint32& value : entryIDs) {
        _itemShopStore.erase(value);
    }

    entryIDs.clear();

    ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZYN_DEL_ITEM_FROM_SHOP);
    stmt->setUInt32(0, entryID);
    stmt->setString(1, groupName);
    ZynDatabase.Execute(stmt);

    return true;
}
