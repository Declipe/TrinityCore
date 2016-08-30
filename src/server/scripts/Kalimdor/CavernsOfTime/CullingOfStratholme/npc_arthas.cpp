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

#include "culling_of_stratholme.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum PositionIndices
{
    RP1_UTHER_SPAWN = 0,
    RP1_JAINA_SPAWN,
    RP1_UTHER_WALK_TO,
    RP1_ARTHAS_WALK_TO_START,
    NUM_POSITIONS
};

enum Data
{
    DATA_UTHER_MOVED
};

enum RPEvents
{
    RP1_EVENT_ARTHAS1 = 1,
    RP1_EVENT_UTHER1,
    RP1_EVENT_ARTHAS2,
    RP1_EVENT_ARTHAS3,
    RP1_EVENT_UTHER2,
    RP1_EVENT_ARTHAS4,
    RP1_EVENT_UTHER3,
    RP1_EVENT_ARTHAS5,
    RP1_EVENT_UTHER4,
    RP1_EVENT_ARTHAS6,
    RP1_EVENT_UTHER5,
    RP1_EVENT_ARTHAS7,
    RP1_EVENT_JAINA1,
    RP1_EVENT_ARTHAS8,
    RP1_EVENT_ARTHAS8_2,
    RP1_EVENT_UTHER6,
    RP1_EVENT_ARTHAS9,
    RP1_EVENT_JAINA2,
    RP1_EVENT_ARTHAS10
};

enum RPEventLines1
{
    RP1_LINE_ARTHAS1  =  0, // Glad you could make it, Uther.
    RP1_LINE_UTHER1   =  0, // Watch your tone with me, boy. You may be the prince, but I'm still your superior as a paladin!
    RP1_LINE_ARTHAS2  =  1, // As if I could forget. Listen, Uther, there's something about the plague you should know...
    RP1_LINE_ARTHAS3  =  2, // Oh, no. We're too late. These people have all been infected! They may look fine now, but it's just a matter of time before they turn into the undead!
    RP1_LINE_UTHER2   =  1, // What?
    RP1_LINE_ARTHAS4  =  3, // This entire city must be purged.
    RP1_LINE_UTHER3   =  2, // How can you even consider that? There's got to be some other way.
    RP1_LINE_ARTHAS5  =  4, // Damn it, Uther! As your future king, I order you to purge this city!
    RP1_LINE_UTHER4   =  3, // You are not my king yet, boy! Nor would I obey that command even if you were!
    RP1_LINE_ARTHAS6  =  5, // Then I must consider this an act of treason.
    RP1_LINE_UTHER5   =  4, // Treason? Have you lost your mind, Arthas?
    RP1_LINE_ARTHAS7  =  6, // Have I? Lord Uther, by my right of succession and the sovereignty of my crown, I hereby relieve you of your command and suspend your paladins from service.
    RP1_LINE_JAINA1   =  0, // Arthas! You can't just--
    RP1_LINE_ARTHAS8  =  7, // It's done! Those of you who have the will to save this land, follow me! The rest of you... get out of my sight!
    RP1_LINE_UTHER6   =  5, // You've just crossed a terrible threshold, Arthas.
    RP1_LINE_ARTHAS9  =  8, // Jaina?
    RP1_LINE_JAINA2   =  1, // I'm sorry, Arthas. I can't watch you do this.
    RP1_LINE_ARTHAS10 =  9  // Take position here, and I will lead a small force inside Stratholme to begin the culling. We must contain and purge the infected for the sake of all of Lordaeron!
};

enum MovementIDs
{
    // Arthas

    // Uther
    MOVEMENT_UTHER_RP1 = 0,

    // Jaina
};

enum Entries
{
    NPC_UTHER = 26528,
    NPC_JAINA = 26497
};

class npc_arthas_stratholme : public CreatureScript
{
    public:
    npc_arthas_stratholme() : CreatureScript("npc_arthas_stratholme") { }

    struct npc_arthas_stratholmeAI : public ScriptedAI
    {
        npc_arthas_stratholmeAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

        static const float _snapbackDistanceThreshold; // how far we can be from where we're supposed at start of phase to be before we snap back
        static const std::map<ProgressStates, Position> _snapbackPositions; // positions we should be at when starting a given phase

        static const std::array<Position, NUM_POSITIONS> _positions;

        void AdvanceToPosition(ProgressStates newState)
        {
            std::map<ProgressStates, Position>::const_iterator it = _snapbackPositions.find(newState);
            if (it != _snapbackPositions.end())
            {
                Position const& target = it->second;
                std::cout << "arthas: have snapback, " << target.GetExactDist(me) << "yd away from me" << std::endl;
                if (target.GetExactDist(me) > _snapbackDistanceThreshold)
                    me->NearTeleportTo(target);
            }
            else
                std::cout << "arthas: no snapback for state " << uint32(newState) << std::endl;
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case -ACTION_PROGRESS_UPDATE:
                    AdvanceToPosition(ProgressStates(instance->GetData(DATA_INSTANCE_PROGRESS)));
                    break;
            }
        }

        void SetGUID(ObjectGuid guid, int32 type) override
        {
            _eventStarterGuid = guid;
            switch (type)
            {
                case -ACTION_START_RP_EVENT:
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, _positions[RP1_UTHER_SPAWN], TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->setActive(true);
                        uther->GetMotionMaster()->MovePoint(MOVEMENT_UTHER_RP1, _positions[RP1_UTHER_WALK_TO]);
                    }
                    me->SummonCreature(NPC_JAINA, _positions[RP1_JAINA_SPAWN], TEMPSUMMON_MANUAL_DESPAWN);
                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(0, _positions[RP1_ARTHAS_WALK_TO_START]);
                    break;
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case DATA_UTHER_MOVED:
                    switch (data)
                    {
                        case MOVEMENT_UTHER_RP1:
                            if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            {
                                uther->SetFacingToObject(me);
                                me->SetFacingToObject(uther);

                                events.ScheduleEvent(RP1_EVENT_ARTHAS1, Seconds(1));
                                events.ScheduleEvent(RP1_EVENT_UTHER1, Seconds(5));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS2, Seconds(12));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS3, Seconds(22));
                                events.ScheduleEvent(RP1_EVENT_UTHER2, Seconds(33));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS4, Seconds(35));
                                events.ScheduleEvent(RP1_EVENT_UTHER3, Seconds(39));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS5, Seconds(45));
                                events.ScheduleEvent(RP1_EVENT_UTHER4, Seconds(51));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS6, Seconds(57));
                                events.ScheduleEvent(RP1_EVENT_UTHER5, Seconds(60));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS7, Seconds(65));
                                events.ScheduleEvent(RP1_EVENT_JAINA1, Seconds(78));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS8, Seconds(80));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS8_2, Seconds(86));
                                events.ScheduleEvent(RP1_EVENT_UTHER6, Seconds(92));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS9, Seconds(97));
                                events.ScheduleEvent(RP1_EVENT_JAINA2, Seconds(99));
                                events.ScheduleEvent(RP1_EVENT_ARTHAS10, Seconds(124));
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);
            while (uint32 event = events.ExecuteEvent())
            {
                uint32 talkerEntry = UINT_MAX, talkerLine = 0;
                switch (event)
                {
                    case RP1_EVENT_ARTHAS1:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS1;
                        break;
                    case RP1_EVENT_UTHER1:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER1;
                        break;
                    case RP1_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS2;
                        break;
                    case RP1_EVENT_ARTHAS3:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS3;
                        break;
                    case RP1_EVENT_UTHER2:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER2;
                        break;
                    case RP1_EVENT_ARTHAS4:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS4;
                        break;
                    case RP1_EVENT_UTHER3:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER3;
                        break;
                    case RP1_EVENT_ARTHAS5:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS5;
                        break;
                    case RP1_EVENT_UTHER4:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER4;
                        break;
                    case RP1_EVENT_ARTHAS6:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS6;
                        break;
                    case RP1_EVENT_UTHER5:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER5;
                        break;
                    case RP1_EVENT_ARTHAS7:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS7;
                        break;
                    case RP1_EVENT_JAINA1:
                        talkerEntry = NPC_JAINA, talkerLine = RP1_LINE_JAINA1;
                        break;
                    case RP1_EVENT_ARTHAS8:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS8;
                        break;
                    case RP1_EVENT_ARTHAS8_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        break;
                    case RP1_EVENT_UTHER6:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER6;
                        break;
                    case RP1_EVENT_ARTHAS9:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS9;
                        break;
                    case RP1_EVENT_JAINA2:
                        talkerEntry = NPC_JAINA, talkerLine = RP1_LINE_JAINA2;
                        break;
                    case RP1_EVENT_ARTHAS10:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS10;
                        instance->SetData(DATA_UTHER_FINISHED, 1);
                        break;
                    default:
                        break;
                }
                
                if (talkerEntry != UINT_MAX)
                {
                    Creature* talker;
                    if (talkerEntry)
                        talker = me->FindNearestCreature(talkerEntry, 100.0f, true);
                    else
                        talker = me;
                    if (talker)
                        talker->AI()->Talk(talkerLine, ObjectAccessor::GetPlayer(*talker, _eventStarterGuid));
                }
            }
        }

        private:
            InstanceScript* const instance;
            EventMap events;
            ObjectGuid _eventStarterGuid;
    };

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        std::cout << (player ? player->GetName() : "nullptr") << " select " << action << " on " << (creature ? creature->GetName() : "nullptr") << std::endl;
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        std::cout << (player ? player->GetName() : "nullptr") << " hello " << (creature ? creature->GetName() : "nullptr") << std::endl;
        return false;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetInstanceAI<npc_arthas_stratholmeAI>(creature);
    }
};
const float npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackDistanceThreshold = 10.0f;
const std::map<ProgressStates, Position> npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackPositions = {
    { COMPLETE, { 1813.298f, 1283.578f, 142.3258f, 3.878161f } }
};

// @todo sniff
const std::array<Position, NUM_POSITIONS> npc_arthas_stratholme::npc_arthas_stratholmeAI::_positions = {{
    { 1754.485f, 1254.018f, 137.6302f, 0.455525f }, // RP1_UTHER_SPAWN
    { 1891.232f, 1295.902f, 144.1589f, 5.701918f }, // RP1_JAINA_SPAWN
    { 1891.801f, 1286.831f, 143.6345f, 0.082460f }, // RP1_UTHER_WALK_TO
    { 1897.830f, 1295.786f, 143.7800f, 4.728092f }  // RP1_ARTHAS_WALK_TO_START
}};

// Arthas' AI is the one controlling everything, all this AI does is report any movementinforms back to Arthas AI using SetData
struct npc_uther_stratholme : public CreatureScript
{
    npc_uther_stratholme() : CreatureScript("npc_uther_stratholme") { }
    struct npc_uther_stratholmeAI : public StratholmeNPCAIWrapper<NullCreatureAI>
    {
        npc_uther_stratholmeAI(Creature* creature) : StratholmeNPCAIWrapper<NullCreatureAI>(creature, UTHER_TALK) { }
        void MovementInform(uint32 type, uint32 id) override {
            if (type == POINT_MOTION_TYPE) if (TempSummon* self = me->ToTempSummon()) self->GetSummonerCreatureBase()->AI()->SetData(DATA_UTHER_MOVED, id);
        }
    };
    CreatureAI* GetAI(Creature* creature) const override { return GetInstanceAI<npc_uther_stratholmeAI>(creature); }
};

void AddSC_npc_arthas_stratholme()
{
    new npc_arthas_stratholme();
    new npc_uther_stratholme();
}
