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
#include "SmartAI.h"
#include "Player.h"
#include "SpellInfo.h"

enum InnEventEntries
{
    NPC_FORREST = 30551,
    NPC_BELFAST = 30571,
    NPC_JAMES   = 30553,
    NPC_FRAS    = 30552,
    NPC_MAL     = 31017,
    NPC_GRYAN   = 30561
};
enum InnEventEvents
{
    EVENT_FORREST_1 = 1,    // This whole situation seems a bit paranoid, don't you think?
    EVENT_JAMES_1,          // Orders are orders. If the Prince says jump...
    EVENT_FRAS_1,           // It's a strange order, you can't deny. Suspicious food? Under that definition, you should arrest Belfast!
    EVENT_BELFAST_MOVE = 100,
    EVENT_BELFAST_1,        // I HEARD THAT! No more ale for you! Not a drop!
    EVENT_MAL_1,            // Enough, Michael. Business is hurting enough with this scare as it is. We can use every copper.
    EVENT_GRYAN_1,          // The soldiers are doing important work. The safety of the people is more important, Mal, if you're interested in your customers living to spend another day.
    EVENT_MAL_2,            // Mal Corricks grudgingly nods.
    EVENT_MAL_3,            // I can't argue with that.
    EVENT_JAMES_2,          // Don't worry too much. By the time I went off duty, we hadn't found a scrap of befouled grain here.
    EVENT_FORREST_2,        // Thank the Light for that.
    EVENT_FRAS_2            // Fras Siabi nods.
};
enum InnEventLines
{
    LINE_JAMES_1    = 0,
    LINE_JAMES_2    = 1,

    LINE_FRAS_1     = 0,
    LINE_FRAS_2     = 1,

    LINE_MAL_1      = 0,
    LINE_MAL_2      = 1,
    LINE_MAL_3      = 2,

    LINE_FORREST_1  = 0,
    LINE_FORREST_2  = 1,

    LINE_BELFAST_0  = 0,
    LINE_BELFAST_1  = 1,
    LINE_GRYAN_1    = 0,
};
enum InnEventMisc
{
    DATA_REQUEST_FACING = 0,
    DATA_REACHED_WP     = 1
};

class npc_hearthsinger_forresten_cot : public CreatureScript
{
    public:
        npc_hearthsinger_forresten_cot() : CreatureScript("npc_hearthsinger_forresten_cot") { }

        struct npc_hearthsinger_forresten_cotAI : public NullCreatureAI
        {
            npc_hearthsinger_forresten_cotAI(Creature* creature) : NullCreatureAI(creature), instance(creature->GetInstanceScript()), _hadBelfast(false), _hadTalk(false) { }

            void UpdateAI(uint32 diff)
            {
                events.Update(diff);
                while (uint32 eventId = events.ExecuteEvent())
                {
                    uint32 talkerEntry, line;
                    switch (eventId)
                    {
                        case EVENT_FORREST_1:
                            talkerEntry = 0, line = LINE_FORREST_1;
                            break;
                        case EVENT_JAMES_1:
                            talkerEntry = NPC_JAMES, line = LINE_JAMES_1;
                            break;
                        case EVENT_FRAS_1:
                            talkerEntry = NPC_FRAS, line = LINE_FRAS_1;
                            break;
                        case EVENT_BELFAST_MOVE:
                            if (Creature* belfast = me->FindNearestCreature(NPC_BELFAST, 80.0f, true))
                                belfast->AI()->DoAction(EVENT_BELFAST_MOVE);
                            return;
                        case EVENT_BELFAST_1:
                            talkerEntry = NPC_BELFAST, line = LINE_BELFAST_1;
                            break;
                        case EVENT_MAL_1:
                            talkerEntry = NPC_MAL, line = LINE_MAL_1;
                            break;
                        case EVENT_GRYAN_1:
                            talkerEntry = NPC_GRYAN, line = LINE_GRYAN_1;
                            break;
                        case EVENT_MAL_2:
                            talkerEntry = NPC_MAL, line = LINE_MAL_2;
                            break;
                        case EVENT_MAL_3:
                            talkerEntry = NPC_MAL, line = LINE_MAL_3;
                            break;
                        case EVENT_JAMES_2:
                            talkerEntry = NPC_JAMES, line = LINE_JAMES_2;
                            break;
                        case EVENT_FORREST_2:
                            talkerEntry = 0, line = LINE_FORREST_2;
                            break;
                        case EVENT_FRAS_2:
                            talkerEntry = NPC_FRAS, line = LINE_FRAS_2;
                            break;
                        default:
                            talkerEntry = 0, line = 0;
                            break;
                    }
                    Creature* talker = me;
                    if (talkerEntry)
                        talker = me->FindNearestCreature(talkerEntry, 80.0f, true);
                    if (talker)
                        talker->AI()->Talk(line, ObjectAccessor::GetPlayer(*talker, _triggeringPlayer));
                }
            }

            // Player has hit the Belfast stairs areatrigger, we are taking him over for a moment
            void SetGUID(ObjectGuid guid, int32 /*id*/) override
            {
                if (_hadBelfast)
                    return;
                _hadBelfast = true;
                if (Creature* belfast = me->FindNearestCreature(NPC_BELFAST, 100.0f, true))
                    if (Player* invoker = ObjectAccessor::GetPlayer(*belfast, guid))
                    {
                        belfast->StopMoving();
                        belfast->SetFacingToObject(invoker);
                        belfast->AI()->Talk(LINE_BELFAST_0);
                    }
            }

            // Belfast SmartAI telling us it's reached the WP
            void SetData(uint32 /*data*/, uint32 /*value*/) override
            {
                events.ScheduleEvent(EVENT_BELFAST_1, Seconds(0));
                events.ScheduleEvent(EVENT_MAL_1, Seconds(6));
                events.ScheduleEvent(EVENT_GRYAN_1, Seconds(12));
                events.ScheduleEvent(EVENT_MAL_2, Seconds(18));
                events.ScheduleEvent(EVENT_MAL_3, Seconds(20));
                events.ScheduleEvent(EVENT_JAMES_2, Seconds(26));
                events.ScheduleEvent(EVENT_FORREST_2, Seconds(32));
                events.ScheduleEvent(EVENT_FRAS_2, Seconds(38));
            }

            void MoveInLineOfSight(Unit* unit) override
            {
                if (!_hadTalk && unit->ToPlayer() && instance->GetData(DATA_INSTANCE_PROGRESS) <= CRATES_IN_PROGRESS && me->GetDistance2d(unit) <= 10.0f)
                {
                    _hadTalk = true;
                    _triggeringPlayer = unit->GetGUID();
                    Seconds offset = Seconds(urand(10,30));
                    events.ScheduleEvent(EVENT_FORREST_1, offset);
                    events.ScheduleEvent(EVENT_JAMES_1, offset+Seconds(6));
                    events.ScheduleEvent(EVENT_FRAS_1, offset+Seconds(12));
                    events.ScheduleEvent(EVENT_BELFAST_MOVE, offset+Seconds(12));
                }
            }


            private:
                InstanceScript const* const instance;
                EventMap events;
                bool _hadBelfast;
                bool _hadTalk;
                ObjectGuid _triggeringPlayer;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_hearthsinger_forresten_cotAI>(creature);
        }
};

class at_stratholme_inn_stairs_cot : public AreaTriggerScript
{
    public:
        at_stratholme_inn_stairs_cot() : AreaTriggerScript("at_stratholme_inn_stairs_cot") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS) <= CRATES_IN_PROGRESS)
                    // Forrest's script will handle Belfast for this, since SmartAI lacks the features to do it (we can't pass a custom target)
                    if (Creature* forrest = player->FindNearestCreature(NPC_FORREST, 200.0f, true))
                        forrest->AI()->SetGUID(player->GetGUID());
            return true;
        }
};

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
    SPELL_TELEPORT_PLAYER       = 53435,
    ACHIEVEMENT_NORMAL          =   479,
    ACHIEVEMENT_HEROIC          =   500
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
                        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, Trinity::StringFormat("[GM] Set instance progress 0x%X", state).c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_GM_INITIAL + state);

                uint32 state = instance->GetData(DATA_INSTANCE_PROGRESS);
                if (state < PURGE_STARTING)
                {
                    AddGossipItemFor(player, GOSSIP_MENU_INITIAL, GOSSIP_OPTION_EXPLAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_EXPLAIN);
                    {
                        bool shouldAddSkipGossip = true;
                        Map::PlayerList const& players = instance->instance->GetPlayers();
                        for (Map::PlayerList::const_iterator it = players.begin(); it != players.end(); ++it)
                        {
                            if (Player* player = it->GetSource())
                            {
                                if (player->IsGameMaster())
                                    continue;
                                if (!player->HasAchieved(instance->instance->GetSpawnMode() == DUNGEON_DIFFICULTY_HEROIC ? ACHIEVEMENT_HEROIC : ACHIEVEMENT_NORMAL))
                                {
                                    shouldAddSkipGossip = false;
                                    break;
                                }
                            }
                        }
                        if (shouldAddSkipGossip)
                            AddGossipItemFor(player, GOSSIP_MENU_INITIAL, GOSSIP_OPTION_SKIP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_SKIP);
                    }
                    SendGossipMenuFor(player, GOSSIP_TEXT_INITIAL, creature->GetGUID());
                }
                else
                {
                    AddGossipItemFor(player, GOSSIP_MENU_INITIAL, GOSSIP_OPTION_TELEPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_TELEPORT);
                    SendGossipMenuFor(player, GOSSIP_TEXT_TELEPORT, creature->GetGUID());
                }
            }
            else // random fallback, this should really never happen
                SendGossipMenuFor(player, GOSSIP_TEXT_INITIAL, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
        {
            ClearGossipMenuFor(player);
            switch (action - GOSSIP_ACTION_INFO_DEF)
            {
                case GOSSIP_OFFSET_EXPLAIN:
                    AddGossipItemFor(player, GOSSIP_MENU_EXPLAIN_1, GOSSIP_OPTION_EXPLAIN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_EXPLAIN_1);
                    SendGossipMenuFor(player, GOSSIP_TEXT_EXPLAIN_1, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_SKIP:
                    AddGossipItemFor(player, GOSSIP_MENU_SKIP_1, GOSSIP_OPTION_SKIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_SKIP_1);
                    SendGossipMenuFor(player, GOSSIP_TEXT_SKIP_1, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_SKIP_1:
                    AdvanceDungeonFar(creature);
                    // intentional missing break
                case GOSSIP_OFFSET_TELEPORT:
                    CloseGossipMenuFor(player);
                    creature->CastSpell(player, SPELL_TELEPORT_PLAYER);
                    break;
                case GOSSIP_OFFSET_EXPLAIN_1:
                    AddGossipItemFor(player, GOSSIP_MENU_EXPLAIN_2, GOSSIP_OPTION_EXPLAIN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_EXPLAIN_2);
                    SendGossipMenuFor(player, GOSSIP_TEXT_EXPLAIN_2, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_EXPLAIN_2:
                    SendGossipMenuFor(player, GOSSIP_TEXT_EXPLAIN_3, creature->GetGUID());
                    AdvanceDungeon(creature);
                    if (!player->HasItemCount(ITEM_ARCANE_DISRUPTOR))
                        player->AddItem(ITEM_ARCANE_DISRUPTOR, 1); // @todo figure out spell
                    break;
                default: // handle GM instance commands
                    CloseGossipMenuFor(player);
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
    WHISPER_CRATES_DONE = 0,
    WHISPER_COME_TALK   = 1
};
class npc_chromie_middle : public StratholmeCreatureScript<NullCreatureAI>
{
    public:
        npc_chromie_middle() : StratholmeCreatureScript("npc_chromie_middle", ProgressStates(ALL & ~(JUST_STARTED | CRATES_IN_PROGRESS))) { }

        void AdvanceDungeon(Creature* creature, Player const* player)
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS) == CRATES_DONE)
                    instance->SetGuidData(DATA_UTHER_START, player->GetGUID());
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_INSTANCE_PROGRESS))
                    AddGossipItemFor(player, GOSSIP_MENU_STEP1, GOSSIP_OPTION_STEP1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_STEP1);
            SendGossipMenuFor(player, GOSSIP_TEXT_STEP1, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
        {
            ClearGossipMenuFor(player);
            switch (action - GOSSIP_ACTION_INFO_DEF)
            {
                case GOSSIP_OFFSET_STEP1:
                    AddGossipItemFor(player, GOSSIP_MENU_STEP2, GOSSIP_OPTION_STEP2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_STEP2);
                    SendGossipMenuFor(player, GOSSIP_TEXT_STEP2, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_STEP2:
                    AddGossipItemFor(player, GOSSIP_MENU_STEP3, GOSSIP_OPTION_STEP3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_STEP3);
                    SendGossipMenuFor(player, GOSSIP_TEXT_STEP3, creature->GetGUID());
                    break;
                case GOSSIP_OFFSET_STEP3:
                    SendGossipMenuFor(player, GOSSIP_TEXT_STEP4, creature->GetGUID());
                    AdvanceDungeon(creature, player);
                    break;

            }
            return false;
        }

        struct npc_chromie_middleAI : public StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper
        {
            npc_chromie_middleAI(Creature* creature, ProgressStates _respawnMask, ProgressStates _despawnMask) : StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper(creature, _respawnMask, _despawnMask), _whisperDelay(0) { }

            void JustRespawned() override
            {
                if (instance->GetData(DATA_INSTANCE_PROGRESS) == CRATES_DONE)
                    _whisperDelay = 18 * IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) override
            {
                if (!_whisperDelay)
                    return;
                if (_whisperDelay > diff)
                    _whisperDelay -= diff;
                else
                {
                    if (instance->GetData(DATA_INSTANCE_PROGRESS) == CRATES_DONE && _whispered.empty())
                        Talk(WHISPER_CRATES_DONE);
                    _whisperDelay = 0;
                }
            }

            void MoveInLineOfSight(Unit* unit) override
            {
                if (Player* player = unit->ToPlayer())
                    if (instance->GetData(DATA_INSTANCE_PROGRESS) == CRATES_DONE && player->GetQuestStatus(QUEST_DISPELLING_ILLUSIONS) == QUEST_STATUS_COMPLETE && me->GetDistance2d(player) < 40.0f)
                    {
                        time_t& whisperedTime = _whispered[player->GetGUID()];
                        time_t now = time(NULL);
                        if (!whisperedTime || (now - whisperedTime) > 15)
                        {
                            Talk(WHISPER_COME_TALK, player);
                            whisperedTime = now;
                        }
                    }
            }

            uint32 _whisperDelay;
            std::map<ObjectGuid, time_t> _whispered;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_chromie_middleAI>(creature, _respawnMask, _despawnMask);
        }
};

enum CrateMisc
{
    GO_SUSPICIOUS_CRATE     = 190094,
    GO_PLAGUED_CRATE        = 190095,
    SPELL_ARCANE_DISRUPTION =  49590,
    SPELL_CRATES_CREDIT     =  58109
};
class npc_crate_helper : public StratholmeCreatureScript<NullCreatureAI>
{
    public:
        npc_crate_helper() : StratholmeCreatureScript<NullCreatureAI>("npc_crate_helper_cot", ProgressStates(CRATES_IN_PROGRESS | CRATES_DONE)) { }

        struct npc_crate_helperAI : public StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper
        {
            npc_crate_helperAI(Creature* creature, ProgressStates _respawnMask, ProgressStates _despawnMask) : StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper(creature, _respawnMask, _despawnMask), _crateRevealed(false) { }

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
            return GetInstanceAI<npc_crate_helperAI>(creature, _respawnMask, _despawnMask);
        }
};

struct npc_stratholme_fluff_living : public StratholmeCreatureScript<NullCreatureAI>
{
    npc_stratholme_fluff_living() : StratholmeCreatureScript<NullCreatureAI>("npc_stratholme_fluff_living", ProgressStates(WAVES_IN_PROGRESS - 1)) { }
};
struct npc_stratholme_smart_living : public StratholmeCreatureScript<SmartAI>
{
    npc_stratholme_smart_living() : StratholmeCreatureScript<SmartAI>("npc_stratholme_smart_living", ProgressStates(WAVES_IN_PROGRESS - 1)) { }
};
struct npc_stratholme_fluff_undead : public StratholmeCreatureScript<AggressorAI>
{
    npc_stratholme_fluff_undead() : StratholmeCreatureScript<AggressorAI>("npc_stratholme_fluff_undead", WAVES_IN_PROGRESS, ProgressStates(ALL & ~(WAVES_IN_PROGRESS-1))) { }
};
struct npc_stratholme_smart_undead : public StratholmeCreatureScript<SmartAI>
{
    npc_stratholme_smart_undead() : StratholmeCreatureScript<SmartAI>("npc_stratholme_smart_undead", WAVES_IN_PROGRESS, ProgressStates(ALL & ~(WAVES_IN_PROGRESS - 1))) { }
};

void AddSC_culling_of_stratholme()
{
    new npc_hearthsinger_forresten_cot();
    new at_stratholme_inn_stairs_cot();

    new npc_chromie_start();
    new npc_chromie_middle();
    new npc_crate_helper();

    new npc_stratholme_fluff_living();
    new npc_stratholme_smart_living();
    new npc_stratholme_fluff_undead();
    new npc_stratholme_smart_undead();
}
