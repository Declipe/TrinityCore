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

#ifndef ITEMSHOP_MGR_H
#define ITEMSHOP_MGR_H

#include "Common.h"
#include <unordered_map>

struct ItemFromShop
{
    uint32 entryID;
    std::string itemName;
    std::string groupName;
    uint32 coinCost;
    uint32 gossipAction;
};
typedef std::unordered_map<uint32, ItemFromShop> ItemShopContainer;

class Player;

class TC_GAME_API ItemShopMgr
{
    private:
        ItemShopMgr() { }
        ~ItemShopMgr();

    public:
        static ItemShopMgr* instance();

        void Initialize();
        void Reload();

        //Promo codes start
        ItemFromShop const* GetItembyGUID(uint32 id) const
        {
            ItemShopContainer::const_iterator itr = _itemShopStore.find(id);
            if (itr == _itemShopStore.end()) return nullptr;
            return &itr->second;
        }

        ItemShopContainer const& GetItemShopContainer() const { return _itemShopStore; }

        bool AddItemFromShop(ItemFromShop& data);
        bool DeleteItemFromShop(uint32 const& entryID, std::string const& groupName);

    protected:
        void _LoadItemsFromShop();

    private:
        ItemShopContainer _itemShopStore;
};

#define sItemShopMgr ItemShopMgr::instance()

#endif
