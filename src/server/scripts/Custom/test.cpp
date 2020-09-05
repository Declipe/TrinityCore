#include "ScriptedCreature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "CellImpl.h"
#include "CombatAI.h"
#include "CreatureTextMgr.h"
#include "GameEventMgr.h"
#include "GridNotifiersImpl.h"
#include "Log.h"
#include "MotionMaster.h"
#include "MoveSplineInit.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "PassiveAI.h"
#include "Pet.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SmartAI.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "Vehicle.h"
#include "World.h"
#include "SpellInfo.h"

enum Spells
{
    SPELL_SHADOW_SPIKE = 46589,
    SPELL_FEAR = 68950,
    SPELL_CHAIN_LIGHTNING = 33665,
    SPELL_ENRAGE = 68335,
    SPELL_BLIZZARD = 26607,
    SPELL_ARCANE_BOMB = 56431,
    SPELL_ACID_BLAST = 75637,
    SPELL_POISON_SHOCK = 28741,
    SPELL_BERSERK = 47008,
};

enum Events
{
    EVENT_SHADOW_SPIKE = 1,
    EVENT_ENRAGE = 2,
    EVENT_FEAR = 3,
    EVENT_CHAIN_LIGHTNING = 4,
    EVENT_BLIZZARD = 5,
    EVENT_ARCANE_BOMB = 6,
    EVENT_ACID_BLAST = 7,
    EVENT_POISON_SHOCK = 8,
    EVENT_BERSERK = 9
};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3
};

enum Texts
{
    SAY_AGGRO = 0,
    SAY_RANDOM = 1,
    SAY_HELP = 2,
    SAY_BERSERK = 3,
    SAY_ENRAGE = 4,
    SAY_DEAD = 5
};

class orrig1 : public CreatureScript
{
public:
    orrig1() : CreatureScript("orrig1") { }

    struct orrig1AI : public ScriptedAI
    {
        orrig1AI(Creature* creature) : ScriptedAI(creature), Summons(me) { }

        uint32 kills = 0;
        void Reset() override
        {
            _events.Reset();
            Summons.DespawnAll();

        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            //Talk(SAY_AGGRO);
            _events.SetPhase(PHASE_ONE);
            _events.ScheduleEvent(EVENT_SHADOW_SPIKE, 8s);
            _events.ScheduleEvent(EVENT_FEAR, 10s);
            _events.ScheduleEvent(EVENT_BLIZZARD, 30s);

        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(75, damage) && _events.IsInPhase(PHASE_ONE))
            {
                _events.SetPhase(PHASE_TWO);
                _events.ScheduleEvent(EVENT_BERSERK, 10s);
                _events.ScheduleEvent(EVENT_POISON_SHOCK, 10s);
                _events.ScheduleEvent(EVENT_ACID_BLAST, 15s);
                _events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30s);
                _events.ScheduleEvent(EVENT_FEAR, 45s);

            }

            if (me->HealthBelowPctDamaged(35, damage) && _events.IsInPhase(PHASE_TWO))
            {
                _events.SetPhase(PHASE_THREE);
                _events.ScheduleEvent(EVENT_ARCANE_BOMB, 5s);
                _events.ScheduleEvent(EVENT_ENRAGE, 100s);
                _events.ScheduleEvent(EVENT_ACID_BLAST, 16s);
                _events.ScheduleEvent(EVENT_ENRAGE, 200s);
                _events.ScheduleEvent(EVENT_BLIZZARD, 45s);
            }
        }

        void JustDied(Unit* /*pPlayer*/) override
        {
        }

        void KilledUnit(Unit* victim) override
        {

            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SHADOW_SPIKE:
                    DoCastAOE(SPELL_SHADOW_SPIKE);
                    break;
                case EVENT_FEAR:
                    DoCast(SPELL_FEAR);
                    _events.ScheduleEvent(EVENT_FEAR, 8s);
                    break;
                case EVENT_ENRAGE:
                   // Talk(SAY_RANDOM);
                    DoCast(SPELL_ENRAGE);
                    break;
                case EVENT_BLIZZARD:
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0)) {
                        DoCast(target, SPELL_BLIZZARD);
                    }
                    _events.ScheduleEvent(EVENT_BLIZZARD, 15s);
                    break;
                case EVENT_POISON_SHOCK:
                   // Talk(SAY_BERSERK);
                    DoCastVictim(SPELL_POISON_SHOCK);
                    _events.ScheduleEvent(EVENT_POISON_SHOCK, 12s);
                    break;
                case EVENT_ARCANE_BOMB:
                   // Talk(SAY_ENRAGE);
                    DoCast(SPELL_ARCANE_BOMB);
                    _events.ScheduleEvent(EVENT_ARCANE_BOMB, 10s);
                    break;
                case EVENT_ACID_BLAST:
                    DoCastVictim(SPELL_ACID_BLAST);
                    _events.ScheduleEvent(EVENT_ACID_BLAST, 15s);
                    break;
                case EVENT_CHAIN_LIGHTNING:
                    DoCast(SPELL_CHAIN_LIGHTNING);
                    _events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 10s);
                    break;
                case EVENT_BERSERK:
                    DoCast(SPELL_BERSERK);
                    _events.ScheduleEvent(EVENT_BERSERK, 10s);
                    break;


                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        SummonList Summons;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new orrig1AI(creature);
    }

};



void AddSC_test()
{
    new orrig1();
}
