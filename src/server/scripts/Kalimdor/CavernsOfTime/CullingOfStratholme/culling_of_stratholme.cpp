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
#include "AreaBoundary.h"

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
                {
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "[GM] Teleport all players to Arthas", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_GM_INITIAL);
                    for (uint32 state = 1; state <= COMPLETE; state = state << 1)
                        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, Trinity::StringFormat("[GM] Set instance progress 0x%X", state).c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OFFSET_GM_INITIAL + state);
                }

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
                case GOSSIP_OFFSET_GM_INITIAL:
                    CloseGossipMenuFor(player);
                    if (!player->CanBeGameMaster())
                        break;
                    if (InstanceScript* instance = creature->GetInstanceScript())
                        instance->SetGuidData(DATA_GM_RECALL, player->GetGUID());
                    break;
                default: // handle GM instance state switch
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

// Generic data for crate fluff events
enum CrateGenericMisc
{
    MOVEID_EVENT1 = 4200,
    MOVEID_EVENT2,
    MOVEID_EVENT3,
    MOVEID_EVENT4,
    MOVEID_EVENT5,

    ACTION_START_FLUFF = 9001
};

// Crate fluff event #1
struct npc_jena_anderson : public CreatureScript
{
    npc_jena_anderson() : CreatureScript("npc_jena_anderson") { }

    static Creature* Find(Creature* /*helper*/) { return nullptr; }
    struct npc_jena_andersonAI : public NullCreatureAI
    {
        npc_jena_andersonAI(Creature* creature) : NullCreatureAI(creature) { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetInstanceAI<npc_jena_andersonAI>(creature);
    }
};
struct npc_martha_goslin : public CreatureScript
{
    npc_martha_goslin() : CreatureScript("npc_martha_goslin") { }
    struct npc_martha_goslinAI : public NullCreatureAI
    {
        npc_martha_goslinAI(Creature* creature) : NullCreatureAI(creature) { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetInstanceAI<npc_martha_goslinAI>(creature);
    }
};

// Crate fluff event #2
enum CrateEvent2Misc
{
    NPC_BARTLEBY    = 27907,

    EVENT_BARTLEBY_IDLE = 1,
    EVENT_BARTLEBY1,
    EVENT_BARTLEBY2,
    EVENT_BARTLEBY2_2,
    EVENT_BARTLEBY3,

    LINE_BARTLEBY_IDLE  = 0,
    LINE_BARTLEBY1      = 1, // Well, guess I should load everything back into the cart.
    LINE_BARTLEBY2      = 2, // Oh, come on! My cart broke, my horse lost a shoe, and now the cargo goes bad!
    LINE_BARTLEBY3      = 3  // I guess I'll go find the authorities. If I'm lucky they'll tell me it's the plague and that we're all going to die.
};
static const Movement::PointsArray bartlebyPath1 = {
    { 1671.586f, 872.1134f, 120.4314f },
    { 1672.917f, 871.3535f, 120.3277f }
};
static const Movement::PointsArray bartlebyPath2 = {
    { 1669.969f, 870.7180f, 120.1384f },
    { 1667.148f, 870.1765f, 120.0045f },
    { 1664.689f, 868.1503f, 119.9065f },
    { 1662.898f, 866.5289f, 120.0467f },
    { 1659.009f, 861.1918f, 119.3174f },
    { 1658.206f, 859.9042f, 119.4396f },
    { 1655.873f, 856.0323f, 119.1204f },
    { 1654.021f, 852.6975f, 119.3411f },
    { 1650.059f, 845.4074f, 119.0619f },
    { 1648.144f, 841.8294f, 119.5816f },
    { 1644.644f, 832.8294f, 119.8316f },
    { 1642.712f, 828.8032f, 119.8451f },
    { 1641.215f, 825.3640f, 120.1445f },
    { 1638.808f, 817.3542f, 119.9748f },
    { 1638.145f, 815.3091f, 120.1469f },
    { 1635.010f, 805.8387f, 119.8493f },
    { 1633.980f, 802.8589f, 119.8655f },
    { 1630.980f, 791.6089f, 119.3655f },
    { 1628.730f, 783.8589f, 119.1155f },
    { 1628.106f, 782.6379f, 118.5758f },
    { 1627.364f, 780.3354f, 118.5013f },
    { 1626.114f, 774.8354f, 118.0013f },
    { 1625.746f, 773.9089f, 117.7027f },
    { 1625.181f, 771.5463f, 117.5337f },
    { 1623.431f, 765.7963f, 117.0337f },
    { 1621.931f, 760.2963f, 116.7837f },
    { 1621.453f, 759.4308f, 116.1794f },
    { 1620.671f, 756.5874f, 116.0332f },
    { 1619.671f, 752.8374f, 115.5332f },
    { 1618.921f, 749.0874f, 115.0332f },
    { 1617.759f, 744.8360f, 114.2508f },
    { 1617.185f, 742.1613f, 114.0811f },
    { 1616.935f, 741.1613f, 114.0811f },
    { 1615.685f, 736.6613f, 113.3311f },
    { 1614.935f, 732.6613f, 113.0811f },
    { 1614.185f, 729.9113f, 112.5811f },
    { 1614.082f, 730.3239f, 112.2263f },
    { 1613.991f, 729.7391f, 112.3380f },
    { 1613.491f, 728.4891f, 112.0880f },
    { 1612.741f, 725.7391f, 111.5880f },
    { 1611.741f, 722.7391f, 110.8380f },
    { 1610.491f, 718.9891f, 110.0880f },
    { 1609.491f, 715.4891f, 109.5880f },
    { 1608.241f, 711.7391f, 109.0880f },
    { 1607.669f, 710.4952f, 108.4556f },
    { 1606.452f, 706.7616f, 108.1364f },
    { 1604.952f, 702.7616f, 107.3864f },
    { 1603.702f, 698.5116f, 106.8864f },
    { 1602.202f, 694.7616f, 106.3864f },
    { 1601.766f, 693.5259f, 105.9085f },
    { 1601.327f, 692.0245f, 105.8733f },
    { 1600.077f, 687.5245f, 105.3733f },
    { 1593.179f, 667.8270f, 103.3361f },
    { 1592.436f, 665.4673f, 103.2942f },
    { 1588.686f, 658.7173f, 102.5442f },
    { 1587.781f, 657.0647f, 102.1362f },
    { 1586.359f, 654.4169f, 101.7926f },
    { 1586.304f, 654.0751f, 101.8038f },
    { 1583.304f, 652.0751f, 101.8038f },
    { 1581.581f, 652.3631f, 101.6499f },
    { 1577.755f, 654.0015f, 101.9234f },
    { 1574.082f, 657.1602f, 102.0366f },
    { 1571.396f, 659.9266f, 102.3597f },
    { 1564.711f, 666.6931f, 102.1827f }
};
struct npc_bartleby_battson : public CreatureScript
{
    npc_bartleby_battson() : CreatureScript("npc_bartleby_battson") { }

    static Creature* Find(Creature* helper) { return helper->FindNearestCreature(NPC_BARTLEBY, 5.0f, true); }
    struct npc_bartleby_battsonAI : public NullCreatureAI
    {
        npc_bartleby_battsonAI(Creature* creature) : NullCreatureAI(creature), started(false) { }

        void InitializeAI() override
        {
            events.ScheduleEvent(EVENT_BARTLEBY_IDLE, Minutes(1), Minutes(2));
        }

        void DoAction(int32 action) override
        {
            if (started || action != ACTION_START_FLUFF)
                return;
            started = true;
            events.CancelEvent(EVENT_BARTLEBY_IDLE);
            events.ScheduleEvent(EVENT_BARTLEBY1, Seconds(15), Seconds(30));
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == EFFECT_MOTION_TYPE)
                switch (id)
                {
                    case MOVEID_EVENT1:
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        events.ScheduleEvent(EVENT_BARTLEBY2, Seconds(4));
                        events.ScheduleEvent(EVENT_BARTLEBY2_2, Seconds(6));
                        events.ScheduleEvent(EVENT_BARTLEBY3, Seconds(12));
                        break;
                    case MOVEID_EVENT2:
                        me->DespawnOrUnsummon(Seconds(1));
                        break;
                }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_BARTLEBY_IDLE:
                        Talk(LINE_BARTLEBY_IDLE);
                        events.Repeat(Minutes(2), Minutes(4));
                        break;
                    case EVENT_BARTLEBY1:
                        Talk(LINE_BARTLEBY1);
                        me->GetMotionMaster()->MoveSmoothPath(MOVEID_EVENT1, bartlebyPath1, true);
                        break;
                    case EVENT_BARTLEBY2:
                        Talk(LINE_BARTLEBY2);
                        break;
                    case EVENT_BARTLEBY2_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        break;
                    case EVENT_BARTLEBY3:
                        Talk(LINE_BARTLEBY3);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        me->GetMotionMaster()->MoveSmoothPath(MOVEID_EVENT2, bartlebyPath2, true);
                        break;
                }
        }

        bool started;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetInstanceAI<npc_bartleby_battsonAI>(creature);
    }
};

// Crate fluff event #3
enum CrateEvent3Misc
{
    NPC_MALCOLM     = 27891,
    NPC_SCRUFFY     = 27892,

    EVENT_SCRUFFY1  =     1,
    EVENT_MALCOLM2,
    EVENT_SCRUFFY_MOVE,
    EVENT_MALCOLM_MOVE,
    EVENT_MALCOLM_FACE3,
    EVENT_SCRUFFY_EMOTE,
    EVENT_MALCOLM3,
    EVENT_MALCOLM4,
    EVENT_MALCOLM_MOVE2,

    LINE_MALCOLM1   =     0, // Looks like a storm's coming in, Scruffy...
    LINE_SCRUFFY1   =     0, // %s begins to growl...
    LINE_MALCOLM2   =     1, // What's wrong, pal?
    LINE_MALCOLM3   =     2, // What did you find, boy?
    LINE_MALCOLM4   =     3, // This is no good, Scruffy. Stay here and guard the house, I need to go find a soldier.
};
static const Position malcolmSpawn = { 1605.2420f, 805.4160f, 122.9956f, 5.284148f };
static const Position scruffySpawn = { 1601.1030f, 805.3391f, 123.7677f, 5.471561f };
static const Movement::PointsArray malcolmPath = {
    { 1604.245f, 806.4799f, 123.0291f },
    { 1604.988f, 805.8108f, 123.0291f },
    { 1608.523f, 800.3172f, 122.5636f },
    { 1609.076f, 799.8616f, 122.6980f },
    { 1609.576f, 799.1116f, 122.6980f },
    { 1612.576f, 796.8616f, 122.1980f },
    { 1613.629f, 795.9061f, 121.8324f }
};
static const Movement::PointsArray scruffyPath = {
    { 1600.004f, 806.3074f, 123.8376f },
    { 1600.781f, 805.6780f, 123.8376f },
    { 1604.224f, 802.0498f, 123.0886f },
    { 1604.549f, 801.7065f, 123.0492f },
    { 1606.523f, 799.9467f, 122.9261f },
    { 1606.655f, 799.5586f, 122.7780f },
    { 1609.755f, 797.2820f, 122.3657f },
    { 1611.604f, 795.5046f, 122.0837f }
};
static const Movement::PointsArray malcolmPath2 = {
    { 1619.153f, 798.0073f, 121.4171f },
    { 1621.158f, 798.6797f, 120.8393f },
    { 1622.953f, 799.5732f, 120.9390f },
    { 1623.703f, 799.8232f, 120.9390f },
    { 1624.453f, 802.0732f, 120.9390f },
    { 1624.479f, 802.8199f, 120.4636f },
    { 1624.002f, 804.5468f, 121.3668f },
    { 1624.752f, 805.2968f, 121.1168f },
    { 1623.752f, 805.5468f, 121.3668f },
    { 1623.526f, 808.2736f, 121.2700f }
};
static const Movement::PointsArray scruffyPath2 = {
    { 1613.088f, 794.5215f, 122.1889f },
    { 1614.179f, 794.0447f, 121.7464f },
    { 1616.998f, 793.0639f, 121.3057f },
    { 1621.170f, 794.7802f, 120.8156f },
    { 1622.230f, 795.6549f, 120.5766f },
    { 1624.277f, 797.8823f, 120.3798f },
    { 1624.999f, 798.7412f, 120.5467f },
    { 1626.999f, 801.2412f, 120.2967f },
    { 1628.722f, 805.1000f, 120.2136f }
};
static const float scruffyFacing2 = 5.734883f;
static const Movement::PointsArray malcolmPath3 = {
    { 1624.479f, 808.1288f, 120.9776f },
    { 1626.838f, 808.1813f, 120.6276f },
    { 1627.280f, 807.3809f, 120.2768f },
    { 1628.257f, 807.1616f, 120.3377f },
    { 1630.257f, 808.9116f, 120.3377f },
    { 1630.235f, 810.9423f, 120.3985f }
};
static const float malcolmFacing3 = 2.303835f;
static const Position scruffyPos3 = { 1629.004f, 810.138f, 120.4927f };
static const Movement::PointsArray malcolmPath4 = {
    { 1633.221f, 810.4380f, 120.2148f },
    { 1637.633f, 810.0463f, 120.0794f },
    { 1638.513f, 810.9830f, 119.9439f },
    { 1641.174f, 815.0283f, 120.1939f },
    { 1642.674f, 818.9854f, 119.9439f },
    { 1644.072f, 822.2241f, 120.1545f },
    { 1644.710f, 824.6933f, 119.9116f },
    { 1646.290f, 828.7598f, 120.0492f },
    { 1649.444f, 836.2549f, 119.2863f },
    { 1650.179f, 837.8422f, 119.4052f },
    { 1650.679f, 838.5922f, 119.4052f },
    { 1654.576f, 847.1667f, 119.0511f },
    { 1655.585f, 849.0662f, 119.1610f },
    { 1659.596f, 858.2399f, 119.2232f },
    { 1660.843f, 860.6360f, 119.5880f },
    { 1666.319f, 871.9752f, 119.7821f },
    { 1667.038f, 873.1796f, 119.9924f },
    { 1667.788f, 874.6796f, 120.2424f },
    { 1671.561f, 882.8347f, 119.7776f },
    { 1673.602f, 886.5914f, 119.8583f },
    { 1678.839f, 899.4649f, 119.9093f },
    { 1679.969f, 901.5616f, 120.2496f },
    { 1682.465f, 910.9940f, 120.4706f },
    { 1683.396f, 913.5186f, 120.7802f },
    { 1684.195f, 916.7935f, 120.5898f },
    { 1685.654f, 920.6906f, 120.8133f },
    { 1687.636f, 925.1472f, 120.2918f },
    { 1689.939f, 929.3484f, 120.2842f },
    { 1691.542f, 933.3212f, 119.8984f },
    { 1693.478f, 937.1957f, 119.9730f },
    { 1696.199f, 944.5081f, 119.9687f },
    { 1697.549f, 947.8503f, 120.3882f },
    { 1700.049f, 954.1003f, 120.8882f },
    { 1700.771f, 955.6768f, 120.7328f },
    { 1701.678f, 957.3701f, 121.1093f },
    { 1702.178f, 962.1201f, 121.6093f },
    { 1702.531f, 964.4929f, 121.6341f },
    { 1703.102f, 968.4348f, 122.2155f },
    { 1699.959f, 975.9057f, 122.2345f },
    { 1699.201f, 978.1485f, 122.5168f },
    { 1693.720f, 989.7047f, 122.2989f },
    { 1692.886f, 991.5643f, 122.7157f },
    { 1688.386f, 1000.314f, 122.9657f },
    { 1688.126f, 1000.414f, 122.9252f },
    { 1686.979f, 1002.745f, 123.5141f },
    { 1685.729f, 1008.245f, 124.0141f },
    { 1685.289f, 1008.901f, 123.9621f },
    { 1684.719f, 1010.909f, 124.5019f },
    { 1683.969f, 1017.409f, 125.0019f },
    { 1683.598f, 1017.778f, 124.8063f },
    { 1683.211f, 1020.651f, 125.4366f },
    { 1682.461f, 1025.651f, 125.9366f },
    { 1681.661f, 1032.742f, 125.9537f },
    { 1681.260f, 1035.870f, 126.1730f },
    { 1680.544f, 1050.879f, 125.9257f },
    { 1680.535f, 1054.749f, 125.9715f },
    { 1680.476f, 1062.975f, 125.9798f },
    { 1680.652f, 1066.777f, 126.1676f },
    { 1681.482f, 1075.029f, 126.2654f },
    { 1682.196f, 1078.414f, 126.6603f },
    { 1683.277f, 1086.956f, 126.8661f },
    { 1683.617f, 1088.229f, 127.3948f },
    { 1684.117f, 1090.979f, 127.3948f },
    { 1687.117f, 1094.729f, 127.8948f },
    { 1687.148f, 1094.779f, 127.8260f },
    { 1689.088f, 1097.265f, 128.7533f },
    { 1692.588f, 1099.265f, 129.2533f },
    { 1694.088f, 1100.265f, 129.7533f },
    { 1696.527f, 1101.835f, 130.2339f },
    { 1697.750f, 1102.589f, 130.8327f },
    { 1699.750f, 1104.089f, 131.0827f },
    { 1702.500f, 1105.589f, 132.0827f },
    { 1705.500f, 1107.339f, 132.5827f },
    { 1707.250f, 1108.339f, 133.0827f },
    { 1709.266f, 1109.362f, 133.3899f },
    { 1710.148f, 1109.964f, 133.9583f },
    { 1711.148f, 1110.714f, 134.4583f },
    { 1713.148f, 1111.464f, 134.9583f },
    { 1714.898f, 1112.214f, 135.4583f },
    { 1717.398f, 1113.464f, 135.9583f },
    { 1719.148f, 1114.214f, 136.7083f },
    { 1719.710f, 1114.490f, 136.6331f },
    { 1721.044f, 1115.318f, 137.4891f },
    { 1723.294f, 1116.318f, 137.9891f },
    { 1725.794f, 1117.818f, 138.7391f },
    { 1727.294f, 1118.568f, 138.9891f },
    { 1730.044f, 1120.068f, 139.7391f },
    { 1731.544f, 1121.068f, 140.2391f }
};
static const float scruffyFacing4 = 5.445427f;
struct npc_malcolm_moore : public CreatureScript
{
    npc_malcolm_moore() : CreatureScript("npc_malcolm_moore") { }

    static void Spawn(Map* map) { map->SummonCreature(NPC_MALCOLM, malcolmSpawn); }
    struct npc_malcolm_mooreAI : public NullCreatureAI
    {
        npc_malcolm_mooreAI(Creature* creature) : NullCreatureAI(creature) { }

        void InitializeAI() override
        {
            me->GetMotionMaster()->MoveSmoothPath(MOVEID_EVENT1, malcolmPath, true);
            if (Creature* scruffy = me->SummonCreature(NPC_SCRUFFY, scruffySpawn))
                scruffy->GetMotionMaster()->MoveSmoothPath(0, scruffyPath, true);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == EFFECT_MOTION_TYPE || type == POINT_MOTION_TYPE)
                switch (id)
                {
                    case MOVEID_EVENT1:
                        Talk(LINE_MALCOLM1);
                        me->GetMotionMaster()->MoveSmoothPath(0, malcolmPath2, true);
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                        {
                            scruffy->StopMoving();
                            scruffy->GetMotionMaster()->Clear();
                            scruffy->GetMotionMaster()->MoveSmoothPath(MOVEID_EVENT2, scruffyPath2, false);
                        }
                        break;
                    case MOVEID_EVENT2:
                        events.ScheduleEvent(EVENT_SCRUFFY1, Seconds(0));
                        events.ScheduleEvent(EVENT_MALCOLM2, Seconds(1));
                        events.ScheduleEvent(EVENT_SCRUFFY_MOVE, Seconds(4));
                        events.ScheduleEvent(EVENT_MALCOLM_MOVE, Seconds(8));
                        break;
                    case MOVEID_EVENT3:
                        events.ScheduleEvent(EVENT_MALCOLM_FACE3, Seconds(0));
                        events.ScheduleEvent(EVENT_MALCOLM3, Seconds(1));
                        events.ScheduleEvent(EVENT_MALCOLM4, Seconds(6));
                        events.ScheduleEvent(EVENT_MALCOLM_MOVE2, Seconds(12));
                        break;
                    case MOVEID_EVENT4:
                        me->DespawnOrUnsummon();
                        break;
                    case MOVEID_EVENT5:
                        events.ScheduleEvent(EVENT_SCRUFFY_EMOTE, Seconds(0));
                        break;
                }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_SCRUFFY1:
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                        {
                            scruffy->SetFacingTo(scruffyFacing2);
                            scruffy->AI()->Talk(LINE_SCRUFFY1);
                        }
                        break;
                    case EVENT_MALCOLM2:
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                        {
                            Talk(LINE_MALCOLM2, scruffy);
                            me->SetFacingToObject(scruffy);
                        }
                        break;
                    case EVENT_SCRUFFY_MOVE:
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                            scruffy->GetMotionMaster()->MovePoint(MOVEID_EVENT5, scruffyPos3, false);
                        break;
                    case EVENT_MALCOLM_MOVE:
                        me->GetMotionMaster()->MoveSmoothPath(MOVEID_EVENT3, malcolmPath3, true);
                        break;
                    case EVENT_SCRUFFY_EMOTE:
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                            scruffy->HandleEmoteCommand(EMOTE_ONESHOT_CREATURE_SPECIAL);
                        break;
                    case EVENT_MALCOLM_FACE3:
                        me->SetFacingTo(malcolmFacing3);
                        break;
                    case EVENT_MALCOLM3:
                        Talk(LINE_MALCOLM3, me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true));
                        break;
                    case EVENT_MALCOLM4:
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                        {
                            me->SetFacingToObject(scruffy);
                            Talk(LINE_MALCOLM4, scruffy);
                        }
                        break;
                    case EVENT_MALCOLM_MOVE2:
                        me->GetMotionMaster()->MoveSmoothPath(MOVEID_EVENT4, malcolmPath4, true);
                        if (Creature* scruffy = me->FindNearestCreature(NPC_SCRUFFY, 100.0f, true))
                            scruffy->SetFacingTo(scruffyFacing4);
                        break;
                }
        }

        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetInstanceAI<npc_malcolm_mooreAI>(creature);
    }
};

enum CrateMisc
{
    GO_SUSPICIOUS_CRATE = 190094,
    GO_CRATE_HIGHLIGHT = 190117,
    GO_PLAGUED_CRATE = 190095,
    SPELL_ARCANE_DISRUPTION = 49590,
    SPELL_CRATES_CREDIT = 58109
};
class npc_crate_helper : public StratholmeCreatureScript<NullCreatureAI>
{
    public:
    npc_crate_helper() : StratholmeCreatureScript<NullCreatureAI>("npc_crate_helper_cot", ProgressStates(CRATES_IN_PROGRESS | CRATES_DONE)) { }

    struct npc_crate_helperAI : public StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper
    {
        npc_crate_helperAI(Creature* creature, ProgressStates _respawnMask, ProgressStates _despawnMask) : StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper(creature, _respawnMask, _despawnMask), _crateRevealed(false) { }

        void replaceIfCloser(Creature* candidate, Creature*& current, float& currentDist) const
        {
            if (!candidate)
                return;
            float newDist = me->GetExactDist2dSq(candidate);
            if (newDist >= currentDist)
                return;
            currentDist = newDist;
            current = candidate;
        }
        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
        {
            if (!_crateRevealed && spell->Id == SPELL_ARCANE_DISRUPTION)
            {
                _crateRevealed = true;
                if (InstanceScript* instance = me->GetInstanceScript())
                {
                    // Update world state
                    instance->SetData(DATA_CRATE_REVEALED, 1);

                    // Replace suspicious crate with plagued crate
                    if (GameObject* crate = me->FindNearestGameObject(GO_SUSPICIOUS_CRATE, 5.0f))
                    {
                        crate->SummonGameObject(GO_PLAGUED_CRATE, *crate, crate->GetWorldRotation(), DAY);
                        crate->Delete();
                    }
                    if (GameObject* highlight = me->FindNearestGameObject(GO_CRATE_HIGHLIGHT, 5.0f))
                        highlight->Delete();

                    // Find nearest fluff event and initiate it
                    Creature* closest = nullptr;
                    float closestDist = INFINITY;
                    replaceIfCloser(npc_jena_anderson::Find(me), closest, closestDist);
                    replaceIfCloser(npc_bartleby_battson::Find(me), closest, closestDist);
                    if (closest)
                        closest->AI()->DoAction(ACTION_START_FLUFF);
                    else
                        npc_malcolm_moore::Spawn(me->GetMap());
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

// Blanket spawn control AI
struct npc_stratholme_fluff_living : public StratholmeCreatureScript<NullCreatureAI>
{
    npc_stratholme_fluff_living() : StratholmeCreatureScript<NullCreatureAI>("npc_stratholme_fluff_living", ProgressStates(WAVES_IN_PROGRESS - 1)) { }
};
struct npc_stratholme_smart_living : public StratholmeCreatureScript<SmartAI>
{
    npc_stratholme_smart_living() : StratholmeCreatureScript<SmartAI>("npc_stratholme_smart_living", ProgressStates(WAVES_IN_PROGRESS - 1)) { }
};
static const std::unique_ptr<AreaBoundary const> waveArea (new RectangleBoundary(2028.0f, 2372.0f, 1115.0f, 1355.0f));
struct npc_stratholme_fluff_undead : public StratholmeCreatureScript<AggressorAI>
{
    npc_stratholme_fluff_undead() : StratholmeCreatureScript<AggressorAI>("npc_stratholme_fluff_undead", ProgressStates(ALL & ~(GAUNTLET_COMPLETE | MALGANIS_IN_PROGRESS | COMPLETE) & ~(WAVES_IN_PROGRESS-1))) { }
    bool CanSpawn(ObjectGuid::LowType spawnId, uint32 entry, CreatureTemplate const* baseTemplate, CreatureTemplate const* actTemplate, CreatureData const* cData, Map const* map) const override
    {
        if (InstanceMap const* instance = map->ToInstanceMap())
            if (InstanceScript const* script = instance->GetInstanceScript())
                if (waveArea->IsWithinBoundary(Position(cData->posX, cData->posY, cData->posZ)) && script->GetData(DATA_INSTANCE_PROGRESS) > WAVES_IN_PROGRESS)
                    return false;
        return StratholmeCreatureScript<AggressorAI>::CanSpawn(spawnId, entry, baseTemplate, actTemplate, cData, map);
    }
};
struct npc_stratholme_smart_undead : public StratholmeCreatureScript<SmartAI>
{
    npc_stratholme_smart_undead() : StratholmeCreatureScript<SmartAI>("npc_stratholme_smart_undead", ProgressStates(ALL & ~(WAVES_IN_PROGRESS - 1))) { }
};

void AddSC_culling_of_stratholme()
{
    new npc_hearthsinger_forresten_cot();
    new at_stratholme_inn_stairs_cot();

    new npc_chromie_start();
    new npc_chromie_middle();

    new npc_jena_anderson();
    new npc_martha_goslin();
    new npc_bartleby_battson();
    new npc_malcolm_moore();
    new npc_crate_helper();

    new npc_stratholme_fluff_living();
    new npc_stratholme_smart_living();
    new npc_stratholme_fluff_undead();
    new npc_stratholme_smart_undead();
}
