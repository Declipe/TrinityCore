#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"

enum Spells
{
    SPELL_SLIME_BOLT = 32309,
    SPELL_BEBENDE_ERDE = 6524,
    SPELL_BLISTERING_COLD = 70123,
    SPELL_BERSERK = 26662,
    SPELL_ENRAGE = 68335,
    SPELL_TOXIC_WASTE = 69024
};

enum Events
{
    EVENT_COLD = 1,
    EVENT_BEBENDE_ERDE = 2,
    EVENT_SLIME_BOLT = 3,
    EVENT_SUMMONS = 4,
    EVENT_BERSERK = 5,
    EVENT_ENRAGE = 6,
    EVENT_WASTE = 7

};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3
};

enum Summons
{
    NPC_SCHMORRSCHUPPEN = 60766
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

class boss_two : public CreatureScript
{
public:
    boss_two() : CreatureScript("boss_two") { }

    struct boss_twoAI : public ScriptedAI
    {
        boss_twoAI(Creature* creature) : ScriptedAI(creature), Summons(me) { }

        void Reset() override
        {
            _events.Reset();
            Summons.DespawnAll();
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);
            _events.SetPhase(PHASE_ONE);
            _events.ScheduleEvent(EVENT_COLD, 1s);
            _events.ScheduleEvent(EVENT_BEBENDE_ERDE,10s);
            _events.ScheduleEvent(EVENT_WASTE, 8s);
                        
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(75, damage) && _events.IsInPhase(PHASE_ONE))
            {
                _events.SetPhase(PHASE_TWO);
                _events.ScheduleEvent(EVENT_SLIME_BOLT, 10s);
                _events.ScheduleEvent(EVENT_WASTE, 20s);
            
            }

            if (me->HealthBelowPctDamaged(35, damage) && _events.IsInPhase(PHASE_TWO))
            {
                _events.SetPhase(PHASE_THREE);
                _events.ScheduleEvent(EVENT_SUMMONS, 5s);
                _events.ScheduleEvent(EVENT_BERSERK, 10s);
                _events.ScheduleEvent(EVENT_ENRAGE, 120s); //2Minuten sonst alle tot!
            }
        }

        void JustSummoned(Creature* summon) override
        {
            Summons.Summon(summon);

            switch (summon->GetEntry())
            {
            case NPC_SCHMORRSCHUPPEN:
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 300.0f))
                    summon->AI()->AttackStart(target); // I think it means the Tank !
                break;
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            //char msg[250];
            //snprintf(msg, 250, "|cffff0000[Boss System]|r Boss|cffff6060 Galadriel|r wurde getoetet! Der Respawn ist in 7 Tagen ab jetzt! "); 
            //sWorld->SendGlobalText(msg, nullptr);
            Summons.DespawnAll();
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
                case EVENT_SLIME_BOLT:
                    DoCast(me, EVENT_SLIME_BOLT);
                    break;
                case EVENT_BEBENDE_ERDE:
                    DoCastVictim(SPELL_BEBENDE_ERDE);
                    _events.ScheduleEvent(EVENT_BEBENDE_ERDE, 8s);
                    break;
                case EVENT_COLD:
                    Talk(SAY_RANDOM);
                    DoCastVictim(SPELL_BLISTERING_COLD);
                    _events.ScheduleEvent(EVENT_COLD, 30s);
                    break;
                case EVENT_SUMMONS:
                    Talk(SAY_HELP);
                    me->SummonCreature(NPC_SCHMORRSCHUPPEN, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 600s);
                    //me->SummonCreature(NPC_SCHMORRSCHUPPEN, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 600000);
                    //me->SummonCreature(NPC_SCHMORRSCHUPPEN, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 600000);
                    break;
                case EVENT_BERSERK:
                    Talk(SAY_BERSERK);
                    DoCast(me, SPELL_BERSERK);
                    _events.ScheduleEvent(EVENT_BERSERK, 120s);
                    break;
                case EVENT_ENRAGE:
                    Talk(SAY_ENRAGE);
                    DoCast(me, SPELL_ENRAGE);
                    _events.ScheduleEvent(EVENT_ENRAGE, 10s);
                    break;
                case EVENT_WASTE:
                    DoCast(SPELL_TOXIC_WASTE);
                    _events.ScheduleEvent(EVENT_WASTE, 15s);
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
        return new boss_twoAI(creature);
    }
};

void AddSC_boss_two()
{
    new boss_two();
}
