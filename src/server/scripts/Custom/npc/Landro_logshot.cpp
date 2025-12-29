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

enum SummonerSpells
{
    SPELL_SUMMON_VISUAL = 61564
};

enum FastSummonerConfig
{
    NPC_MINION = 1234,

    SUMMON_DISTANCE = 20,
    SUMMON_RADIUS = 5,
    MAX_SUMMONS = 30,
};

enum SummonerConfig
{
    NPC_MINION_1 = 1234,
    NPC_MINION_2 = 1235,
    NPC_MINION_3 = 1236,
};

enum Eventssaq
{
    EVENT_CONTINUOUS_SUMMON = 1,
    EVENT_CHECK_PLAYERS = 2
};

    struct npc_continuous_summoner : public ScriptedAI
    {
        npc_continuous_summoner(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        SummonList summons;
        bool isActive;
        bool hasPlayersNearby;

        void Reset() override
        {
            events.Reset();
            summons.DespawnAll();
            isActive = false;
            hasPlayersNearby = false;

            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1s);
        }

        void CheckForPlayersNearby()
        {
            bool foundPlayer = false;

            std::list<Player*> playerList;
            me->GetPlayerListInGrid(playerList, SUMMON_DISTANCE);

            if (!playerList.empty())
            {
                foundPlayer = true;

                if (!hasPlayersNearby)
                {
                    hasPlayersNearby = true;
                    OnPlayersApproach();
                }
            }
            else
            {
                if (hasPlayersNearby)
                {
                    hasPlayersNearby = false;
                    OnPlayersLeave();
                }
            }
        }

        void OnPlayersApproach()
        {
            me->Yell("Someone is approaching! Calling for defenders!", LANG_UNIVERSAL);

            // Start a continuous call
            if (!events.HasEventScheduled(EVENT_CONTINUOUS_SUMMON))
                events.ScheduleEvent(EVENT_CONTINUOUS_SUMMON, 5ms);
        }

        void OnPlayersLeave()
        {
            me->Yell("The threat has passed...", LANG_UNIVERSAL);

            // cancel call
            events.CancelEvent(EVENT_CONTINUOUS_SUMMON);

            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);

            if (summon->IsInWorld())
            {
                summon->CastSpell(summon, SPELL_SUMMON_VISUAL, true);

                summon->GetMotionMaster()->MoveRandom(10.0f);
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        {
            summons.Despawn(summon);
        }

        void ContinuousSummon()
        {
            // limit
            if (summons.size() >= MAX_SUMMONS)
                return;

            if (roll_chance_i(40))
                DoCast(me, SPELL_SUMMON_VISUAL);

            // summon amount
            uint8 count = urand(1, 2);

            for (uint8 i = 0; i < count; ++i)
            {
                if (summons.size() >= MAX_SUMMONS)
                    break;

                float angle = frand(0, 2 * M_PI);
                float dist = frand(2.0f, SUMMON_RADIUS);

                float x = me->GetPositionX() + dist * cos(angle);
                float y = me->GetPositionY() + dist * sin(angle);
                float z = me->GetPositionZ();

                uint32 npcEntry = NPC_MINION_1 + urand(0, 2);

                me->SummonCreature(npcEntry, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_PLAYERS:
                    CheckForPlayersNearby();
                    events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1s);
                    break;

                case EVENT_CONTINUOUS_SUMMON:
                    if (hasPlayersNearby)
                    {
                        ContinuousSummon();
                        events.ScheduleEvent(EVENT_CONTINUOUS_SUMMON, 2s);
                    }
                    break;
                }
            }
        }
    };

enum LandrosTexts
{
    SAY_WRONG = 1,
    SAY_CORRECT = 2
};

enum LandroMenus
{
    WELCOME = 8855,
    PROMOTION_MENU_TEXT = 8856,
    PROMOTION = 9197
};

    struct landro_longshot : public ScriptedAI
    {
        landro_longshot(Creature* creature) : ScriptedAI(creature) { }

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
                return false;

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

void AddSC_landro_longshot()
{
    RegisterCreatureAI(landro_longshot);
    RegisterCreatureAI(npc_continuous_summoner);
}
