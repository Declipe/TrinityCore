/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "culling_of_stratholme.h"
#include "ScriptedEscortAI.h"
#include "PassiveAI.h"
#include "Player.h"
#include "SpellInfo.h"

enum Chromie1Gossip
{
    // offsets from GOSSIP_ACTION_INFO_DEF
    GOSSIP_OFFSET_EXPLAIN = 0,
    GOSSIP_OFFSET_SKIP,
    GOSSIP_OFFSET_TELEPORT,
    GOSSIP_OFFSET_EXPLAIN_1,
    GOSSIP_OFFSET_EXPLAIN_2,
    GOSSIP_OFFSET_SKIP_1,
    GOSSIP_OFFSET_GM_INITIAL,

    GOSSIP_MENU_INITIAL         =  9586,
    GOSSIP_TEXT_INITIAL         = 12939,
    GOSSIP_OPTION_EXPLAIN       =     0,
    GOSSIP_OPTION_SKIP          =     2,

    GOSSIP_TEXT_TELEPORT        = 13470,
    GOSSIP_OPTION_TELEPORT      =     1,

    GOSSIP_MENU_EXPLAIN_1       =  9594,
    GOSSIP_TEXT_EXPLAIN_1       = 12949,
    GOSSIP_OPTION_EXPLAIN_1     =     0,

    GOSSIP_MENU_EXPLAIN_2       =  9595,
    GOSSIP_TEXT_EXPLAIN_2       = 12950,
    GOSSIP_OPTION_EXPLAIN_2     =     0,

    GOSSIP_MENU_EXPLAIN_3       =  9596,
    GOSSIP_TEXT_EXPLAIN_3       = 12952,

    GOSSIP_MENU_SKIP_1          = 11277,
    GOSSIP_TEXT_SKIP_1          = 15704,
    GOSSIP_OPTION_SKIP_1        =     0
};

enum Chromie1Misc
{
    ITEM_ARCANE_DISRUPTOR       = 37888,
    QUEST_DISPELLING_ILLUSIONS  = 13149,
    SPELL_TELEPORT_PLAYER       = 53435
};
class npc_chromie_start : public CreatureScript
{
    public:
        npc_chromie_start() : CreatureScript("npc_chromie_start") { }

        void AdvanceDungeon(Creature* creature)
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS) == JUST_STARTED)
                    instance->SetData(DATA_CRATES_START, 1);
        }

        void AdvanceDungeonFar(Creature* creature)
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS) <= CRATES_DONE)
                    instance->SetData(DATA_SKIP_TO_PURGE, 1);
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                if (player->CanBeGameMaster()) // GM instance state override menu
                    for (uint32 state = 1; state <= COMPLETE; state = state << 1)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, Trinity::StringFormat("[GM] Set instance progress 0x%X", state), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_GM_INITIAL + state);

                uint32 state = instance->GetData(DATA_INSTANCE_PROGRESS);
                if (state <= CRATES_IN_PROGRESS)
                {
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_INITIAL, GOSSIP_OPTION_EXPLAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_EXPLAIN);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_INITIAL, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_INITIAL, GOSSIP_OPTION_TELEPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_TELEPORT);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_TELEPORT, creature->GetGUID());
                }
            }
            else // random fallback, this should really never happen
                player->SEND_GOSSIP_MENU(GOSSIP_TEXT_INITIAL, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action - GOSSIP_ACTION_INFO_DEF)
            {
                case GOSSIP_OFFSET_EXPLAIN:
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_EXPLAIN_1, GOSSIP_OPTION_EXPLAIN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_EXPLAIN_1);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_EXPLAIN_1, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_SKIP:
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_SKIP_1, GOSSIP_OPTION_SKIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_SKIP_1);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_SKIP_1, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_SKIP_1:
                    AdvanceDungeonFar(creature);
                    // intentional missing break
                case GOSSIP_OFFSET_TELEPORT:
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_TELEPORT_PLAYER);
                    break;
                case GOSSIP_OFFSET_EXPLAIN_1:
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_EXPLAIN_2, GOSSIP_OPTION_EXPLAIN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_EXPLAIN_2);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_EXPLAIN_2, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_EXPLAIN_2:
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_EXPLAIN_3, creature->GetGUID());
                    AdvanceDungeon(creature);
                    if (!player->HasItemCount(ITEM_ARCANE_DISRUPTOR))
                        player->AddItem(ITEM_ARCANE_DISRUPTOR, 1); // @todo figure out spell
                    break;
                default: // handle GM instance commands
                    player->CLOSE_GOSSIP_MENU();
                    if (!player->CanBeGameMaster())
                        break;
                    if (InstanceScript* instance = creature->GetInstanceScript())
                        instance->SetData(DATA_GM_OVERRIDE, action - GOSSIP_ACTION_INFO_DEF - GOSSIP_OFFSET_GM_INITIAL);
                    break;
            }
            return false;
        }

        bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest) override
        {
            if (quest->GetQuestId() == QUEST_DISPELLING_ILLUSIONS)
                AdvanceDungeon(creature);
            return true;
        }
};

enum Chromie2Gossip
{
    // offsets from GOSSIP_ACTION_INFO_DEF
    GOSSIP_OFFSET_STEP1 = 0,
    GOSSIP_OFFSET_STEP2,
    GOSSIP_OFFSET_STEP3,

    GOSSIP_MENU_STEP1   =  9610,
    GOSSIP_TEXT_STEP1   = 12992,
    GOSSIP_OPTION_STEP1 =     0,

    GOSSIP_MENU_STEP2   =  9611,
    GOSSIP_TEXT_STEP2   = 12993,
    GOSSIP_OPTION_STEP2 =     0,

    GOSSIP_MENU_STEP3   =  9612,
    GOSSIP_TEXT_STEP3   = 12994,
    GOSSIP_OPTION_STEP3 =     0,
    
    GOSSIP_MENU_STEP4   =  9613,
    GOSSIP_TEXT_STEP4   = 12995
};
enum Chromie2Misc
{
    WHISPER_CRATES_DONE = 0
};
class npc_chromie_middle : public CreatureScript
{
    public:
        npc_chromie_middle() : CreatureScript("npc_chromie_middle") { }

        void AdvanceDungeon(Creature* creature)
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS) == CRATES_DONE)
                    instance->SetData(DATA_UTHER_START, 1);
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS))
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_STEP1, GOSSIP_OPTION_STEP1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_STEP1);
            player->SEND_GOSSIP_MENU(GOSSIP_TEXT_STEP1, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action - GOSSIP_ACTION_INFO_DEF)
            {
                case GOSSIP_OFFSET_STEP1:
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_STEP2, GOSSIP_OPTION_STEP2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_STEP2);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_STEP2, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_STEP2:
                    player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_STEP3, GOSSIP_OPTION_STEP3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_STEP3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_STEP3, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_STEP3:
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXT_STEP4, creature->GetGUID());
                    AdvanceDungeon(creature);
                    break;

            }
            return false;
        }

        bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest) override
        {
            return true;
        }

        struct npc_chromie_middleAI : public StratholmeNPCAIWrapper<NullCreatureAI>
        {
            npc_chromie_middleAI(Creature* creature) : StratholmeNPCAIWrapper<NullCreatureAI>(creature, ProgressStates(ALL & ~(JUST_STARTED | CRATES_IN_PROGRESS))) { }

            void JustRespawned() override
            {
                if (instance->GetData(DATA_INSTANCE_PROGRESS) == CRATES_DONE)
                    Talk(WHISPER_CRATES_DONE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_chromie_middleAI>(creature);
        }
};

enum CrateMisc
{
    GO_SUSPICIOUS_CRATE     = 190094,
    GO_PLAGUED_CRATE        = 190095,
    SPELL_ARCANE_DISRUPTION =  49590,
    SPELL_CRATES_CREDIT     =  58109
};
class npc_crate_helper : public CreatureScript
{
    public:
        npc_crate_helper() : CreatureScript("npc_crate_helper_cot") { }

        struct npc_crate_helperAI : public StratholmeNPCAIWrapper<NullCreatureAI>
        {
            npc_crate_helperAI(Creature* creature) : StratholmeNPCAIWrapper<NullCreatureAI>(creature, ProgressStates(CRATES_IN_PROGRESS | CRATES_DONE)), _crateRevealed(false) { }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
            {
                if (!_crateRevealed && spell->Id == SPELL_ARCANE_DISRUPTION)
                {
                    _crateRevealed = true;
                    if (InstanceScript* instance = me->GetInstanceScript())
                    {
                        instance->SetData(DATA_CRATE_REVEALED, 1);
                        if (GameObject* crate = me->FindNearestGameObject(GO_SUSPICIOUS_CRATE, 5.0f))
                        {
                            crate->SummonGameObject(GO_PLAGUED_CRATE, *crate, crate->GetWorldRotation(), DAY);
                            crate->Delete();
                        }
                    }
                }
            }

            uint32 GetData(uint32 data) const override
            {
                if (data == DATA_CRATE_REVEALED)
                    return _crateRevealed ? 1 : 0;
                return 0;
            }

        private:
            bool _crateRevealed;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_crate_helperAI>(creature);
        }
};

void AddSC_culling_of_stratholme()
{
    new npc_chromie_start();
    new npc_chromie_middle();
    new npc_crate_helper();
}
