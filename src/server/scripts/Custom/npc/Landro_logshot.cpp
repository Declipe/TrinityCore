/*
* Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
* Copyright (C) 2016-2017 AtieshCore <https://gitlab.com/healthstone/>
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

#include "GossipDef.h"
#include "PromotionCodeMgr.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "Language.h"

#include <sstream>
#include <string>
#include <vector>

enum LandrosTexts
{
    SAY_WRONG = 1,
    SAY_CORRECT = 2
};

enum LandroMenus
{
    WELCOME             = 8855,
    PROMOTION_MENU_TEXT = 8856,
    PROMOTION           = 9197
};

class landro_longshot : public CreatureScript
{
public:

    landro_longshot() : CreatureScript("landro_longshot") { }

    struct landro_longshotAI : public ScriptedAI
    {
        landro_longshotAI(Creature* creature) : ScriptedAI(creature) { }

        bool OnGossipHello(Player* player) override
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Do you desire something special?", GOSSIP_SENDER_MAIN, PROMOTION);
            player->PlayerTalkClass->SendGossipMenu(WELCOME, me->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 MenuID = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            player->PlayerTalkClass->ClearMenus();

            uint32 ActionMenuID = 0;
            std::string OptionText = "";
            /// Find items for given menu id.
            uint8 index = 0;
            GossipMenuItemsMapBounds bounds = sObjectMgr->GetGossipMenuItemsMapBounds(MenuID);
            /// Return if there are none.
            if (bounds.first != bounds.second)
            {
                /// Iterate over each of them.
                for (GossipMenuItemsContainer::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
                {
                    /// Find the one with the given menu item id.
                    ActionMenuID = itr->second.ActionMenuID;
                    OptionText = itr->second.OptionText;
                    ++index;
                    if (ActionMenuID != 0)
                        AddGossipItemFor(player, GOSSIP_ICON_CHAT, OptionText, GOSSIP_SENDER_MAIN, ActionMenuID);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_CHAT, OptionText, GOSSIP_SENDER_MAIN, MenuID + index, "", ActionMenuID, true);
                }
            }

            player->PlayerTalkClass->SendGossipMenu(PROMOTION_MENU_TEXT, me->GetGUID());
            return true;
        }

        bool OnGossipSelectCode(Player* player, uint32 /*menu_id*/, uint32 gossipListId, char const* code) override
        {
            if (!player)
                return false;;

            uint32 MenuID = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);

            player->PlayerTalkClass->ClearMenus();
            if (!code)
                return false;

            if (!sPromotionCodeMgr->CheckedEnteredCodeByPlayer(code, player, MenuID))
                me->AI()->Talk(SAY_WRONG);
            else
                me->AI()->Talk(SAY_CORRECT);

            player->PlayerTalkClass->SendCloseGossip();
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new landro_longshotAI(creature);
    }
};

void AddSC_landro_longshot()
{
    new landro_longshot();
}
