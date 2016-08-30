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

    ARTHAS_PURGE_PENDING_POS,
    RP2_ARTHAS_MOVE_1,
    RP2_ARTHAS_MOVE_2,
    RP2_ARTHAS_MOVE_3,
    RP2_ARTHAS_MOVE_4,
    RP2_ARTHAS_MOVE_5,
    RP2_PRIEST1_POS,
    RP2_PRIEST2_POS,
    RP2_FOOT1_POS,
    RP2_FOOT2_POS,
    RP2_FOOT3_POS,
    RP2_FOOT4_POS,
    RP2_MALGANIS_POS,
    NUM_POSITIONS
};

enum Actions
{
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
    RP1_EVENT_ARTHAS10,

    RP2_EVENT_ARTHAS_MOVE_1,
    RP2_EVENT_CITIZEN1,
    RP2_EVENT_ARTHAS2,
    RP2_EVENT_ARTHAS_MOVE_2,
    RP2_EVENT_RESIDENT1,
    RP2_EVENT_ARTHAS_MOVE_3,
    RP2_EVENT_REACT1,
    RP2_EVENT_REACT2,
    RP2_EVENT_REACT3,
    RP2_EVENT_REACT4,
    RP2_EVENT_REACT5,
    RP2_EVENT_ARTHAS_MOVE_4,
    RP2_EVENT_ARTHAS3,
    RP2_EVENT_MALGANIS1,
    RP2_EVENT_TROOPS_FACE,
    RP2_EVENT_ARTHAS_FACE,
    RP2_EVENT_MALGANIS2,
    RP2_EVENT_MALGANIS_LEAVE1,
    RP2_EVENT_MALGANIS_LEAVE2,
    RP2_EVENT_ARTHAS4,
    RP2_EVENT_ARTHAS4_2,
    RP2_EVENT_ARTHAS_MOVE_5,
    RP2_EVENT_ARTHAS5,
    RP2_EVENT_ARTHAS5_2,
    RP2_EVENT_ARTHAS5_3,
    RP2_EVENT_WAVE_START
};

enum RPEventLines1
{
    RP1_LINE_ARTHAS1    =  0, // Glad you could make it, Uther.
    RP1_LINE_UTHER1     =  0, // Watch your tone with me, boy. You may be the prince, but I'm still your superior as a paladin!
    RP1_LINE_ARTHAS2    =  1, // As if I could forget. Listen, Uther, there's something about the plague you should know...
    RP1_LINE_ARTHAS3    =  2, // Oh, no. We're too late. These people have all been infected! They may look fine now, but it's just a matter of time before they turn into the undead!
    RP1_LINE_UTHER2     =  1, // What?
    RP1_LINE_ARTHAS4    =  3, // This entire city must be purged.
    RP1_LINE_UTHER3     =  2, // How can you even consider that? There's got to be some other way.
    RP1_LINE_ARTHAS5    =  4, // Damn it, Uther! As your future king, I order you to purge this city!
    RP1_LINE_UTHER4     =  3, // You are not my king yet, boy! Nor would I obey that command even if you were!
    RP1_LINE_ARTHAS6    =  5, // Then I must consider this an act of treason.
    RP1_LINE_UTHER5     =  4, // Treason? Have you lost your mind, Arthas?
    RP1_LINE_ARTHAS7    =  6, // Have I? Lord Uther, by my right of succession and the sovereignty of my crown, I hereby relieve you of your command and suspend your paladins from service.
    RP1_LINE_JAINA1     =  0, // Arthas! You can't just--
    RP1_LINE_ARTHAS8    =  7, // It's done! Those of you who have the will to save this land, follow me! The rest of you... get out of my sight!
    RP1_LINE_UTHER6     =  5, // You've just crossed a terrible threshold, Arthas.
    RP1_LINE_ARTHAS9    =  8, // Jaina?
    RP1_LINE_JAINA2     =  1, // I'm sorry, Arthas. I can't watch you do this.
    RP1_LINE_ARTHAS10   =  9  // Take position here, and I will lead a small force inside Stratholme to begin the culling. We must contain and purge the infected for the sake of all of Lordaeron!
};

enum RPEventLines2
{
    RP2_LINE_ARTHAS1    = 10, // Everyone looks ready. Remember, these people are all infected with the plague and will die soon. We must purge Stratholme to protect the remainder of Lordaeron from the Scourge. Let's go.
    RP2_LINE_CITIZEN1   =  0, // Prince Arthas, may the light be praised! Many people in the town have begun to fall seriously ill, can you help us?
    RP2_LINE_ARTHAS2    = 11, // I can only help you with a clean death.
    RP2_LINE_RESIDENT1  =  0, // Oh, no... / What? This can't be!
    RP2_LINE_ARTHAS3    = 12, // That was just the beginning.
    RP2_LINE_MALGANIS1  =  0, // Yes, this is the beginning. I've been waiting for you, young prince. I am Mal'Ganis.
    RP2_LINE_MALGANIS2  =  1, // As you can see, your people are now mine. I will now turn this city household by household, until the flame of life has been snuffed out... forever.
    RP2_LINE_ARTHAS4    = 13, // I won't allow it, Mal'Ganis! Better that these people die by my hand than serve as your slaves in death!
    RP2_LINE_ARTHAS5    = 14  // Mal'Ganis will send out some of his Scourge minions to interfere with us. Those of you with the strongest steel and magic shall go forth and destroy them. I will lead the rest of my forces in purging Stratholme of the infected.
};

enum Entries
{
    NPC_MALGANIS_BUNNY          = 20562,
    NPC_UTHER                   = 26528,
    NPC_JAINA                   = 26497,
    NPC_MALGANIS                = 26533,
    NPC_CITIZEN                 = 28167,
    NPC_RESIDENT                = 28169,
    NPC_FOOTMAN                 = 27745,
    NPC_PRIEST                  = 27747,

    SPELL_CRUSADER_STRIKE       = 50773,
    SPELL_SHADOWSTEP_VISUAL     = 51908
};

class npc_arthas_stratholme : public CreatureScript
{
    public:
    npc_arthas_stratholme() : CreatureScript("npc_arthas_stratholme") { }

    struct npc_arthas_stratholmeAI : public ScriptedAI
    {
        npc_arthas_stratholmeAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

        static const std::array<Position, NUM_POSITIONS> _positions; // all kinds of positions we'll need for RP events
        static const float _snapbackDistanceThreshold; // how far we can be from where we're supposed at start of phase to be before we snap back
        typedef std::pair<bool, Position const*> SnapbackInfo;
        static const std::map<ProgressStates, SnapbackInfo> _snapbackPositions; // positions we should be at when starting a given phase

        void AdvanceToPosition(ProgressStates newState)
        {
            events.Reset();
            std::map<ProgressStates, SnapbackInfo>::const_iterator it = _snapbackPositions.find(newState);
            if (it != _snapbackPositions.end())
            {
                SnapbackInfo const& target = it->second;
                if (target.first)
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                else
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                if (target.second)
                    std::cout << "arthas: have snapback, " << target.second->GetExactDist(me) << "yd away from me" << std::endl;
                else
                    std::cout << "no snapback for state " << uint32(newState) << ", only gossip data" << std::endl;
                if (target.second && target.second->GetExactDist(me) > _snapbackDistanceThreshold)
                    me->NearTeleportTo(*target.second);
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
                case -ACTION_START_RP_EVENT1:
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, _positions[RP1_UTHER_SPAWN], TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->setActive(true);
                        uther->GetMotionMaster()->MovePoint(RP1_UTHER_WALK_TO, _positions[RP1_UTHER_WALK_TO]);
                    }
                    me->SummonCreature(NPC_JAINA, _positions[RP1_JAINA_SPAWN], TEMPSUMMON_MANUAL_DESPAWN);
                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(0, _positions[RP1_ARTHAS_WALK_TO_START]);
                    break;
                case -ACTION_START_RP_EVENT2:
                    Talk(RP2_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_1, Seconds(9));
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            switch (id)
            {
                case RP1_UTHER_WALK_TO:
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
                case RP2_ARTHAS_MOVE_1:
                    if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                        citizen->SetFacingToObject(me);
                    if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT, 100.0f, true))
                        resident->SetFacingToObject(me);
                    events.ScheduleEvent(RP2_EVENT_CITIZEN1, Seconds(2));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS2, Seconds(10));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_2, Seconds(11));
                    break;
                case RP2_ARTHAS_MOVE_2:
                    if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                    {
                        DoCast(citizen, SPELL_CRUSADER_STRIKE);
                        me->Kill(citizen);
                    }
                    events.ScheduleEvent(RP2_EVENT_RESIDENT1, Seconds(0));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_3, Seconds(0));
                    break;
                case RP2_ARTHAS_MOVE_3:
                    if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT, 100.0f, true))
                    {
                        DoCast(resident, SPELL_CRUSADER_STRIKE);
                        me->Kill(resident);
                    }
                    events.ScheduleEvent(RP2_EVENT_REACT1, Seconds(1));
                    events.ScheduleEvent(RP2_EVENT_REACT2, Seconds(2));
                    events.ScheduleEvent(RP2_EVENT_REACT3, Seconds(3));
                    events.ScheduleEvent(RP2_EVENT_REACT4, Seconds(5));
                    events.ScheduleEvent(RP2_EVENT_REACT5, Seconds(5));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_4, Seconds(3));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS3, Seconds(5));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS1, Seconds(9));
                    events.ScheduleEvent(RP2_EVENT_TROOPS_FACE, Seconds(10));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_FACE, Seconds(12));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS2, Seconds(21));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS_LEAVE1, Seconds(33));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS_LEAVE2, Seconds(34));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS4, Seconds(34));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS4_2, Seconds(38));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_5, Seconds(44));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5, Seconds(44));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5_2, Seconds(50));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5_3, Seconds(56));
                    events.ScheduleEvent(RP2_EVENT_WAVE_START, Seconds(63));
                default:
                    break;
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case DATA_UTHER_MOVED:
                    // we use the _positions array indices for movement IDs, so we're allowed to do this
                    // movement IDs are unique across ALL npcs involved, not just single ones, so we can forward everything to arthas' MovementInform
                    MovementInform(POINT_MOTION_TYPE, data);
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
                    case RP2_EVENT_ARTHAS_MOVE_1:
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_1, _positions[RP2_ARTHAS_MOVE_1]);
                        break;
                    case RP2_EVENT_CITIZEN1:
                        talkerEntry = NPC_CITIZEN, talkerLine = RP2_LINE_CITIZEN1;
                        break;
                    case RP2_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS2;
                    case RP2_EVENT_ARTHAS_MOVE_2:
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_2, _positions[RP2_ARTHAS_MOVE_2]);
                        break;
                    case RP2_EVENT_RESIDENT1:
                        talkerEntry = NPC_RESIDENT, talkerLine = RP2_LINE_RESIDENT1;
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_3:
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_3, _positions[RP2_ARTHAS_MOVE_3]);
                        break;
                    case RP2_EVENT_REACT1:
                    case RP2_EVENT_REACT2:
                    case RP2_EVENT_REACT3:
                    case RP2_EVENT_REACT4:
                    case RP2_EVENT_REACT5:
                    {
                        std::list<Creature*> nearbyVictims;
                        me->GetCreatureListWithEntryInGrid(nearbyVictims, urand(0, 1) ? NPC_CITIZEN : NPC_RESIDENT, 60.0f);
                        if (!nearbyVictims.empty())
                        {
                            std::list<Creature*>::iterator it = nearbyVictims.begin();
                            std::advance(it, urand(0, nearbyVictims.size()-1));
                            Emote emote;
                            switch (urand(0, 3))
                            {
                                case 0:
                                    emote = EMOTE_ONESHOT_TALK;
                                    break;
                                case 1:
                                    emote = EMOTE_ONESHOT_EXCLAMATION;
                                    break;
                                case 2:
                                    emote = EMOTE_ONESHOT_RUDE;
                                    break;
                                case 3:
                                    emote = EMOTE_ONESHOT_ROAR;
                                    break;
                            }
                            (*it)->HandleEmoteCommand(emote);
                        }
                    }
                    case RP2_EVENT_ARTHAS_MOVE_4:
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_4, _positions[RP2_ARTHAS_MOVE_4]);
                        break;
                    case RP2_EVENT_ARTHAS3:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS3;
                        break;
                    case RP2_EVENT_MALGANIS1:
                        if (Creature* bunny = me->FindNearestCreature(NPC_MALGANIS_BUNNY, 80.0f, true))
                            bunny->CastSpell(bunny, SPELL_SHADOWSTEP_VISUAL);
                        if (Creature* malganis = instance->instance->SummonCreature(NPC_MALGANIS, _positions[RP2_MALGANIS_POS]))
                        {
                            malganis->CastSpell(malganis, SPELL_SHADOWSTEP_VISUAL);
                            malganis->AI()->Talk(RP2_LINE_MALGANIS1);
                        }
                        break;
                    case RP2_EVENT_TROOPS_FACE:
                    {
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                        {
                            std::list<Creature*> troops;
                            me->GetCreatureListWithEntryInGrid(troops, NPC_FOOTMAN, 50.0f);
                            me->GetCreatureListWithEntryInGrid(troops, NPC_PRIEST, 50.0f);
                            for (Creature* unit : troops)
                                unit->SetFacingToObject(malganis);
                        }
                    }
                    case RP2_EVENT_ARTHAS_FACE:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                            me->SetFacingToObject(malganis);
                        break;
                    case RP2_EVENT_MALGANIS2:
                        talkerEntry = NPC_MALGANIS, talkerLine = RP2_LINE_MALGANIS2;
                        break;
                    case RP2_EVENT_MALGANIS_LEAVE1:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                            malganis->CastSpell(malganis, SPELL_SHADOWSTEP_VISUAL);
                        break;
                    case RP2_EVENT_MALGANIS_LEAVE2:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                            malganis->DespawnOrUnsummon(0);
                        if (Creature* bunny = me->FindNearestCreature(NPC_MALGANIS_BUNNY, 80.0f, true))
                            bunny->CastSpell(bunny, SPELL_SHADOWSTEP_VISUAL);
                        break;
                    case RP2_EVENT_ARTHAS4:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS4;
                        break;
                    case RP2_EVENT_ARTHAS4_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        break;
                    case RP2_EVENT_ARTHAS5:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS5;
                        break;
                    case RP2_EVENT_ARTHAS5_2:
                    case RP2_EVENT_ARTHAS5_3:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        break;
                    case RP2_EVENT_WAVE_START:
                        instance->SetData(DATA_START_WAVES, 1);
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

    void AdvanceDungeon(Creature* creature, Player* cause, ProgressStates from, InstanceData command)
    {
        if (InstanceScript* instance = creature->GetInstanceScript())
            if (instance->GetData(DATA_INSTANCE_PROGRESS) == from)
                instance->SetGuidData(command, cause->GetGUID());
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        std::cout << (player ? player->GetName() : "nullptr") << " select " << action << " on " << (creature ? creature->GetName() : "nullptr") << std::endl;
        AdvanceDungeon(creature, player, PURGE_PENDING, DATA_START_PURGE);
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

// @todo sniff
const std::array<Position, NUM_POSITIONS> npc_arthas_stratholme::npc_arthas_stratholmeAI::_positions = {{
    { 1754.485f, 1254.018f, 137.6302f, 0.455525f }, // RP1_UTHER_SPAWN
    { 1891.232f, 1295.902f, 144.1589f, 5.701918f }, // RP1_JAINA_SPAWN
    { 1891.801f, 1286.831f, 143.6345f, 0.082460f }, // RP1_UTHER_WALK_TO
    { 1897.830f, 1295.786f, 143.7800f, 4.728092f }, // RP1_ARTHAS_WALK_TO_START

    { 2047.948f, 1287.598f, 142.8568f, 3.176499f }, // ARTHAS_PURGE_PENDING_POS
    { 2083.011f, 1287.590f, 141.2376f, 5.445427f }, // RP2_ARTHAS_MOVE_1
    { 2087.723f, 1280.895f, 140.6912f }, // RP2_ARTHAS_MOVE_2
    { 2091.994f, 1277.257f, 140.4707f }, // RP2_ARTHAS_MOVE_3
    { 2091.994f, 1277.257f, 140.4707f, 2.234021f }, // RP2_ARTHAS_MOVE_4
    { 2091.994f, 1277.257f, 140.4707f, 1.134464f }, // RP2_ARTHAS_MOVE_5
    { 2074.624f, 1282.958f, 141.6344f }, // RP2_PRIEST1_POS
    { 2074.805f, 1292.172f, 141.6728f }, // RP2_PRIEST2_POS
    { 2077.590f, 1284.609f, 141.5710f }, // RP2_FOOT1_POS
    { 2078.365f, 1281.254f, 141.5182f }, // RP2_FOOT2_POS
    { 2077.737f, 1290.441f, 141.5698f }, // RP2_FOOT3_POS
    { 2078.055f, 1293.624f, 141.5544f }, // RP2_FOOT4_POS
    { 2113.454f, 1287.986f, 136.3829f, 3.071779f } // RP2_MALGANIS_POS
}};

const float npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackDistanceThreshold = 10.0f;
const std::map<ProgressStates, npc_arthas_stratholme::npc_arthas_stratholmeAI::SnapbackInfo> npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackPositions = {
    { PURGE_PENDING, { true, &_positions[ARTHAS_PURGE_PENDING_POS] } }
};

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
