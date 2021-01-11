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
    SPELL_POISON_NOVA = 68989,
    SPELL_ENRAGE = 68335,
    SPELL_TOXIC_WASTE = 69024,
    SPELL_RAIN_OF_FIRE = 59971,
    SPELL_FLAME_BURST = 41131,
    SPELL_ARCANE_BOMB = 56431,
    SPELL_MOONFIRE = 48463,
    SPELL_SPALTEN = 56909,
    SPELL_SARGERAS = 28342,
    SPELL_BURN = 46218,
    SPELL_FLAME_BREATH = 56908
};

enum Events
{
    EVENT_POISON_NOVA = 1,
    EVENT_ENRAGE = 2,
    EVENT_TOXIC_WASTE = 3,
    EVENT_RAIN_OF_FIRE = 4,
    EVENT_FLAME_BURST = 5,
    EVENT_ARCANE_BOMB = 6,
    EVENT_MOONFIRE = 8,
    EVENT_SUMMONS = 9,
    EVENT_SPALTEN= 10,
    EVENT_BURN = 11,
    EVENT_BREATH = 12


};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3
};

enum Summons
{
    NPC_PUSTELIGER_SCHRECKEN = 60765
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

class exitare : public CreatureScript
{
public:
    exitare() : CreatureScript("exitare") { }

    struct exitareAI : public ScriptedAI
    {
        exitareAI(Creature* creature) : ScriptedAI(creature), Summons(me) { }

        void Reset() override
        {
            me->SetFaction(FACTION_MONSTER);
            _events.Reset();
            Summons.DespawnAll();
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            me->Yell("Ich bin gerade auf Kaffee bei Kayoula. Bitte versucht es spaeter noch einmal!", LANG_UNIVERSAL, nullptr);
            me->SetFaction(FACTION_FRIENDLY);
            Reset();
            Talk(SAY_AGGRO);
            _events.SetPhase(PHASE_ONE);
            _events.ScheduleEvent(EVENT_TOXIC_WASTE, 20s);
            _events.ScheduleEvent(EVENT_POISON_NOVA, 40s);
            _events.ScheduleEvent(EVENT_SPALTEN, 30s);
            _events.ScheduleEvent(EVENT_BREATH, 35s);

        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(75, damage) && _events.IsInPhase(PHASE_ONE))
            {
                _events.SetPhase(PHASE_TWO);
                _events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 8s);
                _events.ScheduleEvent(EVENT_FLAME_BURST, 12s);
                _events.ScheduleEvent(EVENT_BREATH, 35s);
                _events.ScheduleEvent(EVENT_SUMMONS, 45s);

            }

            if (me->HealthBelowPctDamaged(35, damage) && _events.IsInPhase(PHASE_TWO))
            {
                _events.SetPhase(PHASE_THREE);
                _events.ScheduleEvent(EVENT_POISON_NOVA, 60s);
                _events.ScheduleEvent(EVENT_TOXIC_WASTE, 45s);
                _events.ScheduleEvent(EVENT_ENRAGE, 440s);
                _events.ScheduleEvent(EVENT_SPALTEN, 30s);
            }
        }

        void JustSummoned(Creature* summon) override
        {
            Summons.Summon(summon);

            switch (summon->GetEntry())
            {
            case NPC_PUSTELIGER_SCHRECKEN:
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 300.0f))
                    summon->AI()->AttackStart(target); 
                break;
            }
        }

        void JustDied(Unit* /*pPlayer*/) override
        {
            //char msg[250];
            //snprintf(msg, 250, "|cffff0000[Boss System]|r Boss|cffff6060 Exitares Schatten|r wurde getoetet! Respawn in 6h 33min.");
            //sWorld->SendGlobalText(msg, nullptr);
            
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
                case EVENT_POISON_NOVA:
                    DoCastAOE(SPELL_POISON_NOVA);
                    _events.ScheduleEvent(EVENT_TOXIC_WASTE, 30s);
                    break;
                case EVENT_TOXIC_WASTE:
                    DoCast(SPELL_TOXIC_WASTE);
                    _events.ScheduleEvent(EVENT_TOXIC_WASTE, 45s);
                    break;
                case EVENT_ENRAGE:
                    Talk(SAY_RANDOM);
                    DoCast(SPELL_ENRAGE);
                    break;
                case EVENT_RAIN_OF_FIRE:
                    me->FinishSpell(CURRENT_CHANNELED_SPELL, true);
                    DoCast(SPELL_RAIN_OF_FIRE);
                    _events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 10s);
                    break;
                case EVENT_FLAME_BURST:
                    Talk(SAY_BERSERK);
                    DoCast(me, SPELL_FLAME_BURST);
                    _events.ScheduleEvent(EVENT_FLAME_BURST, 12s);
                    break;
                case EVENT_SUMMONS:
                    Talk(SAY_HELP);
                    me->SummonCreature(NPC_PUSTELIGER_SCHRECKEN, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 12s);
                    me->SummonCreature(NPC_PUSTELIGER_SCHRECKEN, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 12s);
                    me->SummonCreature(NPC_PUSTELIGER_SCHRECKEN, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 12s);
                    _events.ScheduleEvent(EVENT_SUMMONS, 60s);
                    break;
                case EVENT_ARCANE_BOMB:
                    Talk(SAY_ENRAGE);
                    DoCast(SPELL_ARCANE_BOMB);
                    _events.ScheduleEvent(EVENT_ARCANE_BOMB, 15s);
                    break;
                case EVENT_SPALTEN:
                    DoCast(SPELL_SPALTEN);
                    _events.ScheduleEvent(EVENT_SPALTEN, 30s);
                    break;
                case EVENT_BURN:
                    DoCastVictim(SPELL_BURN);
                    _events.ScheduleEvent(EVENT_BURN, 5s);
                    break;
                case EVENT_BREATH:
                    if (Unit* target = SelectTarget(SelectTargetMethod::MaxThreat,0)){
                        DoCast(target,SPELL_FLAME_BREATH);
                    }
                    
                    _events.ScheduleEvent(EVENT_BREATH, 35s);
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
        return new exitareAI(creature);
    }
};

void AddSC_exitare()
{
    new exitare();
}
