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
#include "SpellScript.h"
#include "MoveSplineInit.h"

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
    RP1_ARTHAS_WP20,
    RP1_ARTHAS_WP21,
    RP1_ARTHAS_WP22,
    RP1_ARTHAS_WP23,
    RP1_ARTHAS_WP24,
    RP1_ARTHAS_WP25,
    RP1_ARTHAS_WP26,
    RP1_ARTHAS_WP27,
    RP1_ARTHAS_WP28,
    RP1_ARTHAS_WP29,
    RP1_ARTHAS_WP30,
    RP1_ARTHAS_WP31,
    RP1_UTHER_WP20,
    RP1_JAINA_WP20,
    RP1_UTHER_WP40,
    RP1_UTHER_WP41,
    RP1_UTHER_WP42,
    RP1_UTHER_WP43,
    RP1_UTHER_WP44,
    RP1_UTHER_WP45,
    RP1_UTHER_WP46,
    RP1_UTHER_WP47,
    RP1_UTHER_WP48,
    RP1_UTHER_WP49,
    RP1_UTHER_WP50,
    RP1_UTHER_WP51,
    RP1_UTHER_WP52,
    RP1_JAINA_WP40,
    RP1_JAINA_WP41,
    RP1_JAINA_WP60,
    RP1_JAINA_WP61,
    RP1_JAINA_WP62,
    RP1_JAINA_WP63,
    RP1_JAINA_WP64,
    RP1_JAINA_WP65,
    RP1_JAINA_WP66,
    RP1_JAINA_WP67,
    RP1_JAINA_WP68,
    RP1_JAINA_WP69,
    RP1_JAINA_WP70,
    RP1_JAINA_WP71,
    RP1_JAINA_WP72,
    RP1_JAINA_WP73,
    RP1_JAINA_WP74,
    RP1_ARTHAS_WP40,
    RP1_ARTHAS_WP41,
    RP1_ARTHAS_WP42,
    RP1_ARTHAS_WP43,
    RP1_ARTHAS_WP44,
    RP1_ARTHAS_WP45,
    RP1_ARTHAS_WP46,
    RP1_ARTHAS_WP47,
    RP1_ARTHAS_WP48,
    RP1_ARTHAS_WP49,
    RP1_ARTHAS_WP50,
    RP1_ARTHAS_WP51,
    RP1_ARTHAS_WP52,
    RP1_ARTHAS_WP53,
    RP1_ARTHAS_WP54,
    RP1_ARTHAS_WP55,
    RP1_ARTHAS_WP60,
    RP1_ARTHAS_WP61,
    RP1_ARTHAS_WP62,
    RP1_ARTHAS_WP63,
    RP1_ARTHAS_WP64,
    RP1_ARTHAS_WP65,
    RP1_ARTHAS_WP66,
    RP1_ARTHAS_WP67,
    RP1_ARTHAS_WP68,
    RP1_ARTHAS_WP69,
    RP1_ARTHAS_WP70,
    RP1_ARTHAS_WP71,
    RP1_ARTHAS_WP72,
    RP1_ARTHAS_WP73,
    RP1_ARTHAS_WP74,
    RP1_ARTHAS_WP75,
    RP1_ARTHAS_WP76,
    RP1_ARTHAS_WP77,
    RP1_ARTHAS_WP78,
    RP1_ARTHAS_WP79,
    RP1_ARTHAS_WP80,
    RP1_ARTHAS_WP81,
    RP1_ARTHAS_WP82,
    RP1_ARTHAS_WP83,
    RP1_ARTHAS_WP84,
    RP1_ARTHAS_WP85,
    RP1_ARTHAS_WP86,
    RP1_ARTHAS_WP87,
    RP1_ARTHAS_WP88,
    RP1_ARTHAS_WP89,
    RP1_ARTHAS_WP90,
    RP1_ARTHAS_WP91,
    RP1_ARTHAS_WP92,
    RP1_ARTHAS_WP93,
    RP1_ARTHAS_WP94,
    RP1_ARTHAS_WP95,
    RP1_ARTHAS_WP96,
    RP1_ARTHAS_WP97,
    RP1_ARTHAS_WP98,
    RP1_ARTHAS_WP99,
    RP1_ARTHAS_WP100,
    RP1_ARTHAS_WP101,
    RP1_ARTHAS_WP102,
    RP1_ARTHAS_WP103,


    // Arthas/Mal'ganis RP
    ARTHAS_PURGE_PENDING_POS,
    RP2_ARTHAS_MOVE_1,
    RP2_CITIZEN_MOVE_1,
    RP2_CITIZEN_MOVE_2,
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

    // Town Hall
    ARTHAS_TOWN_HALL_POS,
    RP3_ARTHAS_WP1,
    RP3_ARTHAS_WP2,
    RP3_ARTHAS_WP3,
    RP3_ARTHAS_WP4,
    RP3_ARTHAS_WP5,
    RP3_ARTHAS_WP6,
    RP3_ARTHAS_WP7,
    RP3_ARTHAS_WP8,
    RP3_ARTHAS_WP9,
    RP3_ARTHAS_WP10,

    // Array element count
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
    RP1_EVENT_START1 = 1,
    RP1_EVENT_START2,
    RP1_EVENT_ARTHAS1,
    RP1_EVENT_UTHER1,
    RP1_EVENT_ARTHAS2,
    RP1_EVENT_ARTHAS3,
    RP1_EVENT_UTHER2,
    RP1_EVENT_ARTHAS4,
    RP1_EVENT_UTHER3,
    RP1_EVENT_ARTHAS_TURN,
    RP1_EVENT_ARTHAS5,
    RP1_EVENT_UTHER4,
    RP1_EVENT_ARTHAS6,
    RP1_EVENT_UTHER5,
    RP1_EVENT_ARTHAS7,
    RP1_EVENT_JAINA1,
    RP1_EVENT_ARTHAS8,
    RP1_EVENT_ARTHAS8_2,
    RP1_EVENT_UTHER6,
    RP1_EVENT_UTHER_LEAVE,
    RP1_EVENT_JAINA_LEAVE,
    RP1_EVENT_ARTHAS9,
    RP1_EVENT_JAINA2,
    RP1_EVENT_JAINA_LEAVE2,
    RP1_EVENT_ARTHAS_LEAVE,
    RP1_EVENT_ARTHAS10,
    RP1_EVENT_ARTHAS_LEAVE2,
    RP1_EVENT_FINISHED,

    RP2_EVENT_ARTHAS_MOVE_1,
    RP2_EVENT_CITIZEN1,
    RP2_EVENT_ARTHAS2,
    RP2_EVENT_ARTHAS_MOVE_2,
    RP2_EVENT_KILL1,
    RP2_EVENT_ARTHAS_MOVE_3,
    RP2_EVENT_KILL2,
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
    RP2_EVENT_WAVE_START,

    EVENT_TOWN_HALL_REACHED,

    RP3_EVENT_RESIDENT_FACE,
    RP3_EVENT_ARTHAS_FACE,
    RP3_EVENT_CITIZEN1,
    RP3_EVENT_ARTHAS2,
    RP3_EVENT_ARTHAS_KILL,
    RP3_EVENT_INFINITE_LAUGH,
    RP3_EVENT_ARTHAS3,
    RP3_EVENT_CITIZEN2,
    RP3_EVENT_TRANSFORM1,
    RP3_EVENT_TRANSFORM2,
    RP3_EVENT_TRANSFORM3
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
    RP1_LINE_ARTHAS10   =  9, // Take position here, and I will lead a small force inside Stratholme to begin the culling. We must contain and purge the infected for the sake of all of Lordaeron!
    RP1_LINE_ARTHAS11   = 40  // All officers should check in with me when their squads are ready. We'll enter Stratholme on my order.
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

enum RPEventLines3
{
    RP3_LINE_ARTHAS1    = 16, // Follow me, I know the way through.
    RP3_LINE_CITIZEN1   =  0, // Ah, you've finally arrived Prince Arthas. You're here just in the nick of time.
    RP3_LINE_ARTHAS2    = 17, // Yes, I'm glad I could get to you before the plague.
    RP3_LINE_ARTHAS3    = 18, // What is this sorcery?
    RP3_LINE_CITIZEN2   =  1, // There's no need for you to understand, Arthas. All you need to do is die.
    RP3_LINE_ARTHAS4    = 19, // Mal'Ganis appears to have more than Scourge in his arsenal. We should make haste.

    RP3_LINE_ARTHAS10   = 20, // More vile sorcery! Be ready for anything!
    RP3_LINE_ARTHAS11   = 21, // Let's move on.

    RP3_LINE_ARTHAS20   = 22, // Watch your backs: they have us surrounded in this hall.
    RP3_LINE_ARTHAS21   = 24, // Mal'ganis is not making this easy.
    RP3_LINE_ARTHAS22   = 25, // They're very persistent.
    
    RP3_LINE_ARTHAS30   = 26, // What else can he put in my way?
    RP3_LINE_EPOCH1     =  0, // Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own.
    RP3_LINE_ARTHAS31   = 27, // I do what I must for Lordaeron, and neither your words nor your actions will stop me.
    RP3_LINE_EPOCH2     =  1  // We'll see about that, young prince.
};

enum OtherLines
{
    LINE_TOWN_HALL_PENDING  = 15,
    LINE_AGGRO              = 39
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
    NPC_KNIGHT                  = 27746,
    NPC_PRIEST                  = 27747,
    NPC_SORCERESS               = 27752,
    NPC_RISEN_ZOMBIE            = 27737,
    NPC_CITIZEN_INFINITE        = 28340,
    NPC_RESIDENT_INFINITE       = 28341,
    NPC_INFINITE_HUNTER         = 27743,
    NPC_INFINITE_AGENT          = 27744,
    NPC_EPOCH                   = 26532,

    SPELL_HOLY_LIGHT            = 52444,
    SPELL_EXORCISM              = 52445,
    SPELL_CRUSADER_STRIKE       = 50773,
    SPELL_SHADOWSTEP_VISUAL     = 51908,
    SPELL_TRANSFORM_VISUAL      = 33133
};

class npc_arthas_stratholme : public CreatureScript
{
    public:
    npc_arthas_stratholme() : CreatureScript("npc_arthas_stratholme") { }

    struct npc_arthas_stratholmeAI : public ScriptedAI
    {
        npc_arthas_stratholmeAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()), _exorcismCooldown(urandms(7,14)) { }

        static const std::array<Position, NUM_POSITIONS> _positions; // all kinds of positions we'll need for RP events (there's a lot of these)
        static const float _snapbackDistanceThreshold; // how far we can be from where we're supposed at start of phase to be before we snap back
        struct SnapbackInfo
        {
            ReactStates const reactState;
            bool const haveGossip;
            Position const* const snapbackPos;
        };
        static const std::map<ProgressStates, SnapbackInfo> _snapbackPositions; // positions we should be at when starting a given phase

        void InitializeAI() { AdvanceToPosition(ProgressStates(instance->GetData(DATA_INSTANCE_PROGRESS))); }

        void AdvanceToPosition(ProgressStates newState)
        {
            events.Reset();
            std::map<ProgressStates, SnapbackInfo>::const_iterator it = _snapbackPositions.find(newState);
            if (it != _snapbackPositions.end())
            {
                SnapbackInfo const& target = it->second;
                me->SetReactState(target.reactState);
                if (target.haveGossip)
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                else
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                if (target.snapbackPos && target.snapbackPos->GetExactDist(me) > _snapbackDistanceThreshold)
                    me->NearTeleportTo(*target.snapbackPos);
            }

            switch (newState)
            {
                case WAVES_DONE:
                    // @todo proper movement
                    me->SetWalk(false);
                    me->GetMotionMaster()->MovePoint(ARTHAS_TOWN_HALL_POS, _positions[ARTHAS_TOWN_HALL_POS]);
                    break;
                default:
                    break;
            }
        }

        void MoveAlongPath(Creature* creature, PositionIndices start, PositionIndices end, bool walk = false)
        {
            if (start == end)
            {
                creature->SetWalk(walk);
                creature->GetMotionMaster()->MovePoint(end, _positions[end], false);
            }
            else
            {
                Movement::PointsArray const path(_positions.begin() + start, _positions.begin() + end + 1);
                creature->GetMotionMaster()->MoveSmoothPath(end, path, walk);
            }
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
                        MoveAlongPath(uther, RP1_UTHER_WP1, RP1_UTHER_WP15);
                    }
                    if (Creature* jaina = me->SummonCreature(NPC_JAINA, _positions[RP1_JAINA_SPAWN], TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        jaina->setActive(true);
                        MoveAlongPath(jaina, RP1_JAINA_WP1, RP1_JAINA_WP6, true);
                    }
                    MoveAlongPath(me, RP1_ARTHAS_WP1, RP1_ARTHAS_WP14);
                    break;
                case -ACTION_START_RP_EVENT2:
                    Talk(RP2_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_1, Seconds(9));
                    break;
                case -ACTION_START_RP_EVENT3:
                    Talk(RP3_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    MoveAlongPath(me, RP3_ARTHAS_WP1, RP3_ARTHAS_WP9);
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
                return;
            switch (id)
            {
                case RP1_UTHER_WP15:
                    events.ScheduleEvent(RP1_EVENT_START1, Seconds(0));
                    events.ScheduleEvent(RP1_EVENT_START2, Seconds(1));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS1, Seconds(4));
                    events.ScheduleEvent(RP1_EVENT_UTHER1, Seconds(8));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS2, Seconds(15));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS3, Seconds(25));
                    events.ScheduleEvent(RP1_EVENT_UTHER2, Seconds(36));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_TURN, Seconds(37));
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
                    events.ScheduleEvent(RP1_EVENT_UTHER_LEAVE, Seconds(98));
                    events.ScheduleEvent(RP1_EVENT_JAINA_LEAVE, Seconds(99));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS9, Seconds(100));
                    events.ScheduleEvent(RP1_EVENT_JAINA2, Seconds(102));
                    events.ScheduleEvent(RP1_EVENT_JAINA_LEAVE2, Seconds(106));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_LEAVE, Seconds(112));
                    break;
                case RP1_UTHER_WP20:
                    if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                        uther->SetFacingToObject(me);
                    break;
                case RP1_JAINA_WP20:
                    if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                        jaina->SetFacingToObject(me);
                    break;
                case RP1_UTHER_WP52:
                    if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 500.0f, true))
                        uther->DespawnOrUnsummon();
                    break;
                case RP1_JAINA_WP74:
                    if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 500.0f, true))
                        jaina->DespawnOrUnsummon();
                    break;
                case RP1_ARTHAS_WP55:
                    events.ScheduleEvent(RP1_EVENT_ARTHAS10, Seconds(0));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_LEAVE2, Seconds(12));
                    break;
                case ARTHAS_PURGE_PENDING_POS:
                    events.ScheduleEvent(RP1_EVENT_FINISHED, Seconds(0));
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
                    events.ScheduleEvent(RP2_EVENT_KILL1, Seconds(0));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_3, Seconds(2));
                    break;
                case RP2_ARTHAS_MOVE_3:
                    events.ScheduleEvent(RP2_EVENT_KILL2, Seconds(1));
                    events.ScheduleEvent(RP2_EVENT_REACT1, Seconds(2));
                    events.ScheduleEvent(RP2_EVENT_REACT2, Seconds(3));
                    events.ScheduleEvent(RP2_EVENT_REACT3, Seconds(4));
                    events.ScheduleEvent(RP2_EVENT_REACT4, Seconds(6));
                    events.ScheduleEvent(RP2_EVENT_REACT5, Seconds(6));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_4, Seconds(4));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS3, Seconds(6));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS1, Seconds(10));
                    events.ScheduleEvent(RP2_EVENT_TROOPS_FACE, Seconds(11));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_FACE, Seconds(13));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS2, Seconds(22));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS_LEAVE1, Seconds(34));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS_LEAVE2, Seconds(35));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS4, Seconds(35));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS4_2, Seconds(39));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_5, Seconds(45));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5, Seconds(45));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5_2, Seconds(51));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5_3, Seconds(57));
                    events.ScheduleEvent(RP2_EVENT_WAVE_START, Seconds(64));
                    break;
                case ARTHAS_TOWN_HALL_POS:
                    events.ScheduleEvent(EVENT_TOWN_HALL_REACHED, Seconds(1));
                    break;
                case RP3_ARTHAS_WP9:
                {
                    std::list<Creature*> infinites;
                    me->GetCreatureListWithEntryInGrid(infinites, NPC_CITIZEN_INFINITE, 100.0f);
                    for (Creature* infinite : infinites)
                        infinite->SetFacingToObject(me);
                    events.ScheduleEvent(RP3_EVENT_RESIDENT_FACE, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_FACE, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_CITIZEN1, Seconds(3));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS2, Seconds(12));
                    break;
                }
                case RP3_ARTHAS_WP10:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_KILL, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_INFINITE_LAUGH, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS3, Seconds(6));
                    events.ScheduleEvent(RP3_EVENT_CITIZEN2, Seconds(8));
                    events.ScheduleEvent(RP3_EVENT_TRANSFORM1, Seconds(10));
                    events.ScheduleEvent(RP3_EVENT_TRANSFORM2, Seconds(12));
                    events.ScheduleEvent(RP3_EVENT_TRANSFORM3, Seconds(14));
                    break;
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
                        MoveAlongPath(me, RP1_ARTHAS_WP20, RP1_ARTHAS_WP31, true);
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            MoveAlongPath(uther, RP1_UTHER_WP20, RP1_UTHER_WP20, true);
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            MoveAlongPath(jaina, RP1_JAINA_WP20, RP1_JAINA_WP20, true);
                        break;
                    case RP1_EVENT_ARTHAS3:
                    {
                        me->SetFacingTo(_positions[RP1_ARTHAS_WP31].GetOrientation());
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS3;
                        std::list<Creature*> troops;
                        me->GetCreatureListWithEntryInGrid(troops, NPC_FOOTMAN, 100.0f);
                        me->GetCreatureListWithEntryInGrid(troops, NPC_SORCERESS, 100.0f);
                        me->GetCreatureListWithEntryInGrid(troops, NPC_KNIGHT, 100.0f);
                        me->GetCreatureListWithEntryInGrid(troops, NPC_PRIEST, 100.0f);
                        for (Creature* unit : troops)
                            unit->SetFacingToObject(me);
                        break;
                    }
                    case RP1_EVENT_UTHER2:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER2;
                        break;
                    case RP1_EVENT_ARTHAS4:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS4;
                        break;
                    case RP1_EVENT_UTHER3:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER3;
                        break;
                    case RP1_EVENT_ARTHAS_TURN:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            me->SetFacingToObject(uther);
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
                    case RP1_EVENT_UTHER_LEAVE:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            MoveAlongPath(uther, RP1_UTHER_WP40, RP1_UTHER_WP52, true);
                        break;
                    case RP1_EVENT_JAINA_LEAVE:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            MoveAlongPath(jaina, RP1_JAINA_WP40, RP1_JAINA_WP41, true);
                        break;
                    case RP1_EVENT_ARTHAS9:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS9;
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            me->SetFacingToObject(jaina);
                        break;
                    case RP1_EVENT_JAINA2:
                        talkerEntry = NPC_JAINA, talkerLine = RP1_LINE_JAINA2;
                        break;
                    case RP1_EVENT_JAINA_LEAVE2:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            MoveAlongPath(jaina, RP1_JAINA_WP60, RP1_JAINA_WP74, true);
                        break;
                    case RP1_EVENT_ARTHAS_LEAVE:
                        MoveAlongPath(me, RP1_ARTHAS_WP40, RP1_ARTHAS_WP55, true);
                        break;
                    case RP1_EVENT_ARTHAS10:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS10;
                        me->SetFacingTo(_positions[RP1_ARTHAS_WP55].GetOrientation());
                        break;
                    case RP1_EVENT_ARTHAS_LEAVE2:
                        MoveAlongPath(me, RP1_ARTHAS_WP60, ARTHAS_PURGE_PENDING_POS);
                        break;
                    case RP1_EVENT_FINISHED:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS11;
                        me->SetFacingTo(_positions[ARTHAS_PURGE_PENDING_POS].GetOrientation());
                        instance->SetData(DATA_UTHER_FINISHED, 1);
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_1:
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_1, _positions[RP2_ARTHAS_MOVE_1]);
                        break;
                    case RP2_EVENT_CITIZEN1:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                        {
                            MoveAlongPath(citizen, RP2_CITIZEN_MOVE_1, RP2_CITIZEN_MOVE_2, true);
                            citizen->AI()->Talk(RP2_LINE_CITIZEN1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                        }
                        talkerEntry = NPC_CITIZEN, talkerLine = RP2_LINE_CITIZEN1;
                        break;
                    case RP2_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS2;
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_2:
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_2, _positions[RP2_ARTHAS_MOVE_2]);
                        break;
                    case RP2_EVENT_KILL1:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                            DoCast(citizen, SPELL_CRUSADER_STRIKE);
                        talkerEntry = NPC_RESIDENT, talkerLine = RP2_LINE_RESIDENT1;
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_3:
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(RP2_ARTHAS_MOVE_3, _positions[RP2_ARTHAS_MOVE_3]);
                        break;
                    case RP2_EVENT_KILL2:
                        if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT, 100.0f, true))
                            DoCast(resident, SPELL_CRUSADER_STRIKE);
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
                    case EVENT_TOWN_HALL_REACHED:
                        me->SetFacingTo(_positions[ARTHAS_TOWN_HALL_POS].GetOrientation());
                        Talk(LINE_TOWN_HALL_PENDING);
                        instance->SetData(DATA_REACH_TOWN_HALL, 1);
                        break;
                    case RP3_EVENT_RESIDENT_FACE:
                        if (Creature* infinite = me->FindNearestCreature(NPC_RESIDENT_INFINITE, 100.0f, true))
                            infinite->SetFacingToObject(me);
                        break;
                    case RP3_EVENT_ARTHAS_FACE:
                        me->SetFacingTo(_positions[RP3_ARTHAS_WP9].GetOrientation());
                        break;
                    case RP3_EVENT_CITIZEN1:
                        talkerEntry = NPC_CITIZEN_INFINITE, talkerLine = RP3_LINE_CITIZEN1;
                        break;
                    case RP3_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS2;
                        MoveAlongPath(me, RP3_ARTHAS_WP10, RP3_ARTHAS_WP10, true);
                        break;
                    case RP3_EVENT_ARTHAS_KILL:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN_INFINITE, 100.0f, true))
                            DoCast(citizen, SPELL_CRUSADER_STRIKE);
                        break;
                    case RP3_EVENT_INFINITE_LAUGH:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN_INFINITE, 100.0f, true))
                            citizen->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                        break;
                    case RP3_EVENT_ARTHAS3:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS3;
                        break;
                    case RP3_EVENT_CITIZEN2:
                        talkerEntry = NPC_CITIZEN_INFINITE, talkerLine = RP3_LINE_CITIZEN2;
                        break;
                    case RP3_EVENT_TRANSFORM1:
                    case RP3_EVENT_TRANSFORM3:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN_INFINITE, 100.0f, true))
                        {
                            citizen->CastSpell(citizen, SPELL_TRANSFORM_VISUAL);
                            citizen->UpdateEntry(NPC_INFINITE_HUNTER, nullptr, false);
                        }
                        break;
                    case RP3_EVENT_TRANSFORM2:
                        if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT_INFINITE, 100.0f, true))
                        {
                            resident->CastSpell(resident, SPELL_TRANSFORM_VISUAL);
                            resident->UpdateEntry(NPC_INFINITE_AGENT, nullptr, false);
                        }
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

            if (me->HasReactState(REACT_PASSIVE))
                return;

            if (!UpdateVictim())
                return;

            if (HealthBelowPct(40))
                DoCastSelf(SPELL_HOLY_LIGHT);
            if (_exorcismCooldown <= diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    _exorcismCooldown = 0;
                else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_EXORCISM);
                    _exorcismCooldown = urandms(7, 14);
                }
            }
            else
                _exorcismCooldown -= diff;

            DoMeleeAttackIfReady();
        }

        void EnterCombat(Unit* who) override
        {
            if (who && who->GetEntry() == NPC_RISEN_ZOMBIE)
                Talk(LINE_AGGRO, who);
        }

        private:
            InstanceScript* const instance;
            EventMap events;
            ObjectGuid _eventStarterGuid;
            uint32 _exorcismCooldown; // no EventMap entry for this, it's reserved for RP handling
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
        AdvanceDungeon(creature, player, TOWN_HALL_PENDING, DATA_START_TOWN_HALL);
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
    { 1903.773f, 1295.073f, 143.8806f }, // RP1_ARTHAS_WP20
    { 1904.243f, 1297.470f, 143.7825f }, // RP1_ARTHAS_WP21
    { 1905.602f, 1301.860f, 144.8910f }, // RP1_ARTHAS_WP22
    { 1905.852f, 1302.610f, 145.6410f }, // RP1_ARTHAS_WP23
    { 1906.102f, 1303.360f, 146.6410f }, // RP1_ARTHAS_WP24
    { 1906.352f, 1304.360f, 147.1410f }, // RP1_ARTHAS_WP25
    { 1906.605f, 1305.008f, 147.3347f }, // RP1_ARTHAS_WP26
    { 1907.279f, 1306.383f, 148.3546f }, // RP1_ARTHAS_WP27
    { 1907.779f, 1307.383f, 148.3546f }, // RP1_ARTHAS_WP28
    { 1908.279f, 1309.133f, 149.1046f }, // RP1_ARTHAS_WP29
    { 1909.779f, 1311.633f, 150.1046f }, // RP1_ARTHAS_WP30
    { 1909.953f, 1312.258f, 149.8745f, 6.248279f }, // RP1_ARTHAS_WP31
    { 1903.980f, 1297.119f, 143.5117f }, // RP1_UTHER_WP20
    { 1899.490f, 1298.720f, 143.8338f }, // RP1_JAINA_WP20
    { 1894.333f, 1289.897f, 143.6610f }, // RP1_UTHER_WP40
    { 1891.336f, 1287.669f, 143.8724f }, // RP1_UTHER_WP41
    { 1880.168f, 1285.746f, 144.0307f }, // RP1_UTHER_WP42
    { 1878.394f, 1285.508f, 144.1768f }, // RP1_UTHER_WP43
    { 1868.367f, 1283.260f, 144.2789f }, // RP1_UTHER_WP44
    { 1866.074f, 1282.787f, 144.4259f }, // RP1_UTHER_WP45
    { 1847.381f, 1281.428f, 144.0926f }, // RP1_UTHER_WP46
    { 1845.679f, 1281.301f, 144.2642f }, // RP1_UTHER_WP47
    { 1833.179f, 1281.301f, 144.2642f }, // RP1_UTHER_WP48
    { 1829.435f, 1279.402f, 143.1175f }, // RP1_UTHER_WP49
    { 1827.441f, 1279.227f, 143.2142f }, // RP1_UTHER_WP50
    { 1819.691f, 1277.227f, 142.4642f }, // RP1_UTHER_WP51
    { 1809.446f, 1274.552f, 141.8109f }, // RP1_UTHER_WP52
    { 1896.806f, 1295.515f, 144.0838f }, // RP1_JAINA_WP40
    { 1894.122f, 1293.809f, 143.8338f }, // RP1_JAINA_WP41
    { 1883.056f, 1288.990f, 144.0463f }, // RP1_JAINA_WP60
    { 1881.201f, 1288.203f, 144.2471f }, // RP1_JAINA_WP61
    { 1868.281f, 1286.438f, 144.3624f }, // RP1_JAINA_WP62
    { 1864.452f, 1285.841f, 144.5302f }, // RP1_JAINA_WP63
    { 1850.351f, 1284.098f, 144.2387f }, // RP1_JAINA_WP64
    { 1847.541f, 1283.823f, 144.4623f }, // RP1_JAINA_WP65
    { 1834.041f, 1281.823f, 143.7123f }, // RP1_JAINA_WP66
    { 1832.474f, 1281.457f, 143.4485f }, // RP1_JAINA_WP67
    { 1829.620f, 1280.964f, 143.3172f }, // RP1_JAINA_WP68
    { 1823.120f, 1280.214f, 143.8172f }, // RP1_JAINA_WP69
    { 1817.593f, 1279.304f, 142.3018f }, // RP1_JAINA_WP70
    { 1815.349f, 1278.991f, 142.2914f }, // RP1_JAINA_WP71
    { 1804.849f, 1276.741f, 141.7914f }, // RP1_JAINA_WP72
    { 1799.849f, 1275.741f, 141.5414f }, // RP1_JAINA_WP73
    { 1794.105f, 1274.177f, 140.7811f }, // RP1_JAINA_WP74
    { 1909.149f, 1310.354f, 149.3120f }, // RP1_ARTHAS_WP40
    { 1908.953f, 1309.581f, 148.9193f }, // RP1_ARTHAS_WP41
    { 1908.301f, 1307.853f, 148.5280f }, // RP1_ARTHAS_WP42
    { 1906.801f, 1305.353f, 147.7780f }, // RP1_ARTHAS_WP43
    { 1906.051f, 1303.853f, 146.5280f }, // RP1_ARTHAS_WP44
    { 1905.301f, 1303.103f, 146.0280f }, // RP1_ARTHAS_WP45
    { 1904.976f, 1302.552f, 145.2282f }, // RP1_ARTHAS_WP46
    { 1904.597f, 1302.058f, 144.8074f }, // RP1_ARTHAS_WP47
    { 1903.847f, 1300.308f, 144.3074f }, // RP1_ARTHAS_WP48
    { 1903.347f, 1299.558f, 144.0574f }, // RP1_ARTHAS_WP49
    { 1903.495f, 1294.424f, 143.4848f }, // RP1_ARTHAS_WP50
    { 1904.043f, 1291.139f, 143.5607f }, // RP1_ARTHAS_WP51
    { 1905.880f, 1289.570f, 143.0584f }, // RP1_ARTHAS_WP52
    { 1910.900f, 1288.434f, 142.5332f }, // RP1_ARTHAS_WP53
    { 1914.900f, 1287.184f, 142.2832f }, // RP1_ARTHAS_WP54
    { 1916.920f, 1287.297f, 142.0080f, 3.141593f }, // RP1_ARTHAS_WP55
    { 1922.920f, 1287.426f, 143.7628f }, // RP1_ARTHAS_WP60
    { 1926.420f, 1287.426f, 144.2628f }, // RP1_ARTHAS_WP61
    { 1926.315f, 1287.547f, 144.1820f }, // RP1_ARTHAS_WP62
    { 1929.387f, 1287.536f, 144.9325f }, // RP1_ARTHAS_WP63
    { 1939.387f, 1287.536f, 145.6825f }, // RP1_ARTHAS_WP64
    { 1941.637f, 1287.536f, 145.9325f }, // RP1_ARTHAS_WP65
    { 1945.476f, 1287.539f, 145.7195f }, // RP1_ARTHAS_WP66
    { 1952.274f, 1287.516f, 145.9143f }, // RP1_ARTHAS_WP67
    { 1955.102f, 1287.516f, 145.6653f }, // RP1_ARTHAS_WP68
    { 1964.744f, 1287.494f, 145.6109f }, // RP1_ARTHAS_WP69
    { 1976.952f, 1287.353f, 145.7911f }, // RP1_ARTHAS_WP70
    { 1983.987f, 1287.299f, 145.5024f }, // RP1_ARTHAS_WP71
    { 1988.481f, 1287.522f, 145.7339f }, // RP1_ARTHAS_WP72
    { 1989.231f, 1287.522f, 145.7339f }, // RP1_ARTHAS_WP73
    { 1990.481f, 1291.272f, 145.7339f }, // RP1_ARTHAS_WP74
    { 1990.737f, 1291.360f, 145.4654f }, // RP1_ARTHAS_WP75
    { 1991.269f, 1292.252f, 145.7454f }, // RP1_ARTHAS_WP76
    { 1993.269f, 1297.502f, 145.7454f }, // RP1_ARTHAS_WP77
    { 1994.019f, 1300.752f, 145.7454f }, // RP1_ARTHAS_WP78
    { 1996.158f, 1309.712f, 143.8715f }, // RP1_ARTHAS_WP79
    { 1997.341f, 1313.685f, 143.4305f }, // RP1_ARTHAS_WP80
    { 1998.091f, 1316.185f, 143.4305f }, // RP1_ARTHAS_WP81
    { 1998.841f, 1316.935f, 143.4305f }, // RP1_ARTHAS_WP82
    { 1999.630f, 1318.427f, 142.9734f }, // RP1_ARTHAS_WP83
    { 2003.284f, 1322.536f, 143.2183f }, // RP1_ARTHAS_WP84
    { 2004.534f, 1323.286f, 143.2183f }, // RP1_ARTHAS_WP85
    { 2010.034f, 1326.286f, 143.2183f }, // RP1_ARTHAS_WP86
    { 2015.219f, 1323.498f, 142.9729f }, // RP1_ARTHAS_WP87
    { 2019.570f, 1321.246f, 143.2407f }, // RP1_ARTHAS_WP88
    { 2021.570f, 1313.496f, 143.2407f }, // RP1_ARTHAS_WP89
    { 2022.320f, 1310.746f, 143.2407f }, // RP1_ARTHAS_WP90
    { 2022.820f, 1308.996f, 143.2407f }, // RP1_ARTHAS_WP91
    { 2023.178f, 1307.211f, 143.2767f }, // RP1_ARTHAS_WP92
    { 2023.797f, 1306.225f, 143.6581f }, // RP1_ARTHAS_WP93
    { 2025.672f, 1297.923f, 143.5070f }, // RP1_ARTHAS_WP94
    { 2026.215f, 1296.983f, 143.6298f }, // RP1_ARTHAS_WP95
    { 2028.965f, 1293.233f, 143.6298f }, // RP1_ARTHAS_WP96
    { 2030.715f, 1290.983f, 143.6298f }, // RP1_ARTHAS_WP97
    { 2031.299f, 1290.355f, 143.5597f }, // RP1_ARTHAS_WP98
    { 2032.124f, 1289.977f, 143.7082f }, // RP1_ARTHAS_WP99
    { 2033.874f, 1289.477f, 143.4582f }, // RP1_ARTHAS_WP100
    { 2035.624f, 1288.727f, 143.4582f }, // RP1_ARTHAS_WP101
    { 2038.124f, 1287.727f, 143.4582f }, // RP1_ARTHAS_WP102
    { 2045.124f, 1287.727f, 143.2082f }, // RP1_ARTHAS_WP103

    { 2047.948f, 1287.598f, 142.8568f, 3.176499f }, // ARTHAS_PURGE_PENDING_POS
    { 2083.011f, 1287.590f, 141.2376f, 5.445427f }, // RP2_ARTHAS_MOVE_1
    { 2090.946f, 1276.898f, 140.7805f }, // RP2_CITIZEN_MOVE_1
    { 2088.873f, 1279.260f, 140.7279f }, // RP2_CITIZEN_MOVE_2
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
    { 2113.454f, 1287.986f, 136.3829f, 3.071779f }, // RP2_MALGANIS_POS

    { 2366.240f, 1195.253f, 132.0441f, 3.159046f }, // ARTHAS_TOWN_HALL_POS
    { 2371.452f, 1199.571f, 134.8580f }, // RP3_ARTHAS_WP1
    { 2373.452f, 1200.071f, 134.8580f }, // RP3_ARTHAS_WP2
    { 2375.452f, 1200.571f, 134.3580f }, // RP3_ARTHAS_WP3
    { 2376.452f, 1200.571f, 134.1080f }, // RP3_ARTHAS_WP4
    { 2377.202f, 1200.821f, 134.1080f }, // RP3_ARTHAS_WP5
    { 2379.192f, 1201.169f, 134.0411f }, // RP3_ARTHAS_WP6
    { 2384.146f, 1202.468f, 134.2909f }, // RP3_ARTHAS_WP7
    { 2386.146f, 1202.718f, 134.2909f }, // RP3_ARTHAS_WP8
    { 2392.101f, 1203.767f, 134.0407f, 0.541052f }, // RP3_ARTHAS_WP9
    { 2396.516f, 1206.148f, 134.0400f, 0.494561f }, // RP3_ARTHAS_WP10
}};

const float npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackDistanceThreshold = 10.0f;
const std::map<ProgressStates, npc_arthas_stratholme::npc_arthas_stratholmeAI::SnapbackInfo> npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackPositions = {
    { JUST_STARTED, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { CRATES_IN_PROGRESS, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { CRATES_DONE, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { UTHER_TALK, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { PURGE_PENDING, { REACT_PASSIVE, true, &_positions[ARTHAS_PURGE_PENDING_POS] } },
    { PURGE_STARTING, { REACT_PASSIVE, false, &_positions[ARTHAS_PURGE_PENDING_POS] } },
    { WAVES_IN_PROGRESS, { REACT_AGGRESSIVE, false, &_positions[RP2_ARTHAS_MOVE_5] } },
    { WAVES_DONE, { REACT_DEFENSIVE, false, &_positions[RP2_ARTHAS_MOVE_5] } },
    { TOWN_HALL_PENDING, { REACT_DEFENSIVE, true, &_positions[ARTHAS_TOWN_HALL_POS] } },
    { TOWN_HALL, { REACT_DEFENSIVE, false, &_positions[ARTHAS_TOWN_HALL_POS] } },
    { TOWN_HALL_COMPLETE, { REACT_PASSIVE, true, nullptr } },
    { GAUNTLET_TRANSITION, { REACT_PASSIVE, false, nullptr } },
    { GAUNTLET_PENDING, { REACT_PASSIVE, true, nullptr } },
    { GAUNTLET_IN_PROGRESS, { REACT_DEFENSIVE, false, nullptr } },
    { GAUNTLET_COMPLETE, { REACT_PASSIVE, true, nullptr } },
    { MALGANIS_IN_PROGRESS, { REACT_DEFENSIVE, false, nullptr } },
    { COMPLETE, { REACT_PASSIVE, true, nullptr } }
};

// Arthas' AI is the one controlling everything, all this AI does is report any movementinforms back to Arthas AI using SetData
struct npc_stratholme_rp_dummy : public StratholmeCreatureScript<NullCreatureAI>
{
    npc_stratholme_rp_dummy() : StratholmeCreatureScript<NullCreatureAI>("npc_stratholme_rp_dummy", ProgressStates(UTHER_TALK | PURGE_PENDING)) { }
    struct npc_stratholme_rp_dummyAI : public StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper
    {
        npc_stratholme_rp_dummyAI(Creature* creature) : StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper(creature, ProgressStates(UTHER_TALK | PURGE_PENDING)) { }
        void MovementInform(uint32 type, uint32 id) override {
            if (type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE) if (TempSummon* self = me->ToTempSummon()) self->GetSummonerCreatureBase()->AI()->SetData(DATA_RP_DUMMY_MOVED, id);
        }
    };
    CreatureAI* GetAI(Creature* creature) const override { return GetInstanceAI<npc_stratholme_rp_dummyAI>(creature); }
};

class spell_stratholme_crusader_strike : public SpellScriptLoader
{
    public:
        spell_stratholme_crusader_strike() : SpellScriptLoader("spell_stratholme_crusader_strike") { }

        class spell_stratholme_crusader_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_stratholme_crusader_strike_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    if (target->GetEntry() == NPC_CITIZEN || target->GetEntry() == NPC_RESIDENT)
                        GetCaster()->Kill(target);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_stratholme_crusader_strike_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_stratholme_crusader_strike_SpellScript();
        }
};

void AddSC_npc_arthas_stratholme()
{
    new npc_arthas_stratholme();
    new npc_stratholme_rp_dummy();
    new spell_stratholme_crusader_strike();
}
