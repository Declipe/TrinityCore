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

// Indices in arthasAI::_positions
// Also used as movement id for MovementInform
// All movement informs are handled by arthas AI (forwarded by Jaina/Uther)
enum PositionIndices
{
    // Arthas/Uther RP
    RP1_ARTHAS_INITIAL = 0,
    RP1_ARTHAS_WP1,
    RP1_ARTHAS_WP2,
    RP1_ARTHAS_WP3,
    RP1_ARTHAS_WP4,
    RP1_ARTHAS_WP5,
    RP1_ARTHAS_WP6,
    RP1_ARTHAS_WP7,
    RP1_ARTHAS_WP8,
    RP1_ARTHAS_WP9,
    RP1_ARTHAS_WP10,
    RP1_ARTHAS_WP11,
    RP1_ARTHAS_WP12,
    RP1_ARTHAS_WP13,
    RP1_ARTHAS_WP14,
    RP1_UTHER_SPAWN,
    RP1_UTHER_WP1,
    RP1_UTHER_WP2,
    RP1_UTHER_WP3,
    RP1_UTHER_WP4,
    RP1_UTHER_WP5,
    RP1_UTHER_WP6,
    RP1_UTHER_WP7,
    RP1_UTHER_WP8,
    RP1_UTHER_WP9,
    RP1_UTHER_WP10,
    RP1_UTHER_WP11,
    RP1_UTHER_WP12,
    RP1_UTHER_WP13,
    RP1_UTHER_WP14,
    RP1_UTHER_WP15,
    RP1_JAINA_SPAWN,
    RP1_JAINA_WP1,
    RP1_JAINA_WP2,
    RP1_JAINA_WP3,
    RP1_JAINA_WP4,
    RP1_JAINA_WP5,
    RP1_JAINA_WP6,
    RP1_JAINA_WP7,
    RP1_JAINA_WP8,

    // Arthas/Mal'ganis RP
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
    DATA_RP_DUMMY_MOVED
};

enum RPEvents
{
    RP1_EVENT_ARTHAS_MOVE = 1,
    RP1_EVENT_UTHER_MOVE,
    RP1_EVENT_JAINA_MOVE,
    RP1_EVENT_START1,
    RP1_EVENT_START2,
    RP1_EVENT_ARTHAS1,
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
        npc_arthas_stratholmeAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()), _arthasWP(0), _utherWP(0), _jainaWP(0) { }

        static const std::array<Position, NUM_POSITIONS> _positions; // all kinds of positions we'll need for RP events (there's a lot of these)
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
                        uther->GetMotionMaster()->MovePoint(RP1_UTHER_WP1, _positions[RP1_UTHER_WP1]);
                    }
                    if (Creature* jaina = me->SummonCreature(NPC_JAINA, _positions[RP1_JAINA_SPAWN], TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        jaina->setActive(true);
                        jaina->SetWalk(true);
                        jaina->GetMotionMaster()->MovePoint(RP1_JAINA_WP1, _positions[RP1_JAINA_WP1]);
                    }
                    me->GetMotionMaster()->MovePoint(RP1_ARTHAS_WP1, _positions[RP1_ARTHAS_WP1]);
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
                case RP1_ARTHAS_WP1:
                case RP1_ARTHAS_WP2:
                case RP1_ARTHAS_WP3:
                case RP1_ARTHAS_WP4:
                case RP1_ARTHAS_WP5:
                case RP1_ARTHAS_WP6:
                case RP1_ARTHAS_WP7:
                case RP1_ARTHAS_WP8:
                case RP1_ARTHAS_WP9:
                case RP1_ARTHAS_WP10:
                case RP1_ARTHAS_WP11:
                case RP1_ARTHAS_WP12:
                case RP1_ARTHAS_WP13:
                    _arthasWP = id+1;
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_MOVE, Seconds(0)); // can't give movement here, needs to be handled outside of movegen finalize - delegate to update
                    break;
                case RP1_UTHER_WP1:
                case RP1_UTHER_WP2:
                case RP1_UTHER_WP3:
                case RP1_UTHER_WP4:
                case RP1_UTHER_WP5:
                case RP1_UTHER_WP6:
                case RP1_UTHER_WP7:
                case RP1_UTHER_WP8:
                case RP1_UTHER_WP9:
                case RP1_UTHER_WP10:
                case RP1_UTHER_WP11:
                case RP1_UTHER_WP12:
                case RP1_UTHER_WP13:
                case RP1_UTHER_WP14:
                    _utherWP = id+1;
                    events.ScheduleEvent(RP1_EVENT_UTHER_MOVE, Seconds(0)); // needs to be handled after movementgenerator finishes
                    break;
                case RP1_JAINA_WP1:
                case RP1_JAINA_WP2:
                case RP1_JAINA_WP3:
                case RP1_JAINA_WP4:
                case RP1_JAINA_WP5:
                case RP1_JAINA_WP6:
                case RP1_JAINA_WP7:
                    _jainaWP = id+1;
                    events.ScheduleEvent(RP1_EVENT_JAINA_MOVE, Seconds(0)); // ditto, same as above
                    break;
                case RP1_UTHER_WP15:
                    events.ScheduleEvent(RP1_EVENT_START1, Seconds(0));
                    events.ScheduleEvent(RP1_EVENT_START2, Seconds(1));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS1, Seconds(4));
                    events.ScheduleEvent(RP1_EVENT_UTHER1, Seconds(8));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS2, Seconds(15));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS3, Seconds(25));
                    events.ScheduleEvent(RP1_EVENT_UTHER2, Seconds(36));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS4, Seconds(38));
                    events.ScheduleEvent(RP1_EVENT_UTHER3, Seconds(42));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS5, Seconds(48));
                    events.ScheduleEvent(RP1_EVENT_UTHER4, Seconds(54));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS6, Seconds(60));
                    events.ScheduleEvent(RP1_EVENT_UTHER5, Seconds(63));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS7, Seconds(68));
                    events.ScheduleEvent(RP1_EVENT_JAINA1, Seconds(81));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS8, Seconds(83));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS8_2, Seconds(89));
                    events.ScheduleEvent(RP1_EVENT_UTHER6, Seconds(95));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS9, Seconds(100));
                    events.ScheduleEvent(RP1_EVENT_JAINA2, Seconds(102));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS10, Seconds(127));
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
                case DATA_RP_DUMMY_MOVED:
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
                    case RP1_EVENT_ARTHAS_MOVE:
                        me->GetMotionMaster()->MovePoint(_arthasWP, _positions[_arthasWP], false);
                        break;
                    case RP1_EVENT_UTHER_MOVE:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 300.0f, true))
                            uther->GetMotionMaster()->MovePoint(_utherWP, _positions[_utherWP], false);
                        break;
                    case RP1_EVENT_JAINA_MOVE:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 200.0f, true))
                            jaina->GetMotionMaster()->MovePoint(_jainaWP, _positions[_jainaWP], false);
                        break;
                    case RP1_EVENT_START1:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            jaina->SetFacingToObject(me);
                        break;
                    case RP1_EVENT_START2:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                        {
                            uther->SetFacingToObject(me);
                            me->SetFacingToObject(uther);
                        }
                        break;
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
            uint32 _arthasWP;
            uint32 _utherWP;
            uint32 _jainaWP;
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
const std::array<Position, NUM_POSITIONS> npc_arthas_stratholme::npc_arthas_stratholmeAI::_positions = { {
    { 1983.857f, 1287.043f, 145.5596f, 3.141593f }, // RP1_ARTHAS_INITIAL
    { 1980.498f, 1287.490f, 145.6067f }, // RP1_ARTHAS_WP1
    { 1964.590f, 1287.431f, 145.6118f }, // RP1_ARTHAS_WP2
    { 1957.130f, 1287.623f, 145.8020f }, // RP1_ARTHAS_WP3
    { 1945.300f, 1288.015f, 145.7205f }, // RP1_ARTHAS_WP4
    { 1941.592f, 1288.343f, 145.7971f }, // RP1_ARTHAS_WP5
    { 1939.092f, 1288.593f, 145.5471f }, // RP1_ARTHAS_WP6
    { 1935.710f, 1288.522f, 145.1396f }, // RP1_ARTHAS_WP7
    { 1926.133f, 1289.097f, 144.1636f }, // RP1_ARTHAS_WP8
    { 1924.944f, 1289.333f, 144.0251f }, // RP1_ARTHAS_WP9
    { 1915.194f, 1290.333f, 142.2751f }, // RP1_ARTHAS_WP10
    { 1913.194f, 1290.333f, 142.5251f }, // RP1_ARTHAS_WP11
    { 1909.194f, 1290.583f, 143.2751f }, // RP1_ARTHAS_WP12
    { 1904.194f, 1291.333f, 143.7751f }, // RP1_ARTHAS_WP13
    { 1903.254f, 1291.568f, 143.3867f }, // RP1_ARTHAS_WP14
    { 1783.282f, 1267.326f, 139.7406f }, // RP1_UTHER_SPAWN
    { 1787.858f, 1269.427f, 140.3383f }, // RP1_UTHER_WP1
    { 1795.608f, 1271.427f, 141.0883f }, // RP1_UTHER_WP2
    { 1801.358f, 1272.927f, 141.3383f }, // RP1_UTHER_WP3
    { 1809.858f, 1274.927f, 142.0883f }, // RP1_UTHER_WP4
    { 1814.059f, 1276.074f, 142.0615f }, // RP1_UTHER_WP5
    { 1820.787f, 1277.925f, 142.9422f }, // RP1_UTHER_WP6
    { 1827.787f, 1279.425f, 143.1922f }, // RP1_UTHER_WP7
    { 1833.537f, 1280.175f, 143.6922f }, // RP1_UTHER_WP8
    { 1846.287f, 1281.425f, 144.4422f }, // RP1_UTHER_WP9
    { 1852.031f, 1281.841f, 144.2432f }, // RP1_UTHER_WP10
    { 1862.359f, 1283.141f, 144.4760f }, // RP1_UTHER_WP11
    { 1880.790f, 1284.917f, 143.8929f }, // RP1_UTHER_WP12
    { 1889.189f, 1286.121f, 143.9258f }, // RP1_UTHER_WP13
    { 1889.939f, 1286.121f, 143.9258f }, // RP1_UTHER_WP14
    { 1899.588f, 1288.324f, 143.4588f }, // RP1_UTHER_WP15
    { 1876.788f, 1305.723f, 146.2474f }, // RP1_JAINA_SPAWN
    { 1878.886f, 1303.033f, 146.1656f }, // RP1_JAINA_WP1
    { 1883.186f, 1299.556f, 145.4580f }, // RP1_JAINA_WP2
    { 1884.462f, 1298.772f, 145.5209f }, // RP1_JAINA_WP3
    { 1885.712f, 1297.772f, 145.2709f }, // RP1_JAINA_WP4
    { 1888.264f, 1296.464f, 144.5988f }, // RP1_JAINA_WP5
    { 1889.259f, 1296.148f, 144.7163f }, // RP1_JAINA_WP6
    { 1891.509f, 1294.898f, 144.2163f }, // RP1_JAINA_WP7
    { 1896.754f, 1292.831f, 143.8338f }, // RP1_JAINA_WP8

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
    { JUST_STARTED, { false, &_positions[RP1_ARTHAS_INITIAL] } },
    { CRATES_IN_PROGRESS, { false, &_positions[RP1_ARTHAS_INITIAL] } },
    { CRATES_DONE, { false, &_positions[RP1_ARTHAS_INITIAL] } },
    { UTHER_TALK, { false, &_positions[RP1_ARTHAS_INITIAL] } },
    { PURGE_PENDING, { true, &_positions[ARTHAS_PURGE_PENDING_POS] } },
    { PURGE_STARTING, { false, &_positions[ARTHAS_PURGE_PENDING_POS] } },
    { WAVES_IN_PROGRESS, { false, &_positions[RP2_ARTHAS_MOVE_5] } },
    { WAVES_DONE, { true, nullptr } },
    { TOWN_HALL, { false, nullptr } },
    { TOWN_HALL_COMPLETE, { true, nullptr } },
    { GAUNTLET_TRANSITION, { false, nullptr } },
    { GAUNTLET_PENDING, { true, nullptr } },
    { GAUNTLET_IN_PROGRESS, { false, nullptr } },
    { GAUNTLET_COMPLETE, { true, nullptr } },
    { MALGANIS_IN_PROGRESS, { false, nullptr } },
    { COMPLETE, { true, nullptr } }
};

// Arthas' AI is the one controlling everything, all this AI does is report any movementinforms back to Arthas AI using SetData
struct npc_stratholme_rp_dummy : public CreatureScript
{
    npc_stratholme_rp_dummy() : CreatureScript("npc_stratholme_rp_dummy") { }
    struct npc_stratholme_rp_dummyAI : public StratholmeNPCAIWrapper<NullCreatureAI>
    {
        npc_stratholme_rp_dummyAI(Creature* creature) : StratholmeNPCAIWrapper<NullCreatureAI>(creature, UTHER_TALK) { }
        void MovementInform(uint32 type, uint32 id) override {
            if (type == POINT_MOTION_TYPE) if (TempSummon* self = me->ToTempSummon()) self->GetSummonerCreatureBase()->AI()->SetData(DATA_RP_DUMMY_MOVED, id);
        }
    };
    CreatureAI* GetAI(Creature* creature) const override { return GetInstanceAI<npc_stratholme_rp_dummyAI>(creature); }
};

void AddSC_npc_arthas_stratholme()
{
    new npc_arthas_stratholme();
    new npc_stratholme_rp_dummy();
}
