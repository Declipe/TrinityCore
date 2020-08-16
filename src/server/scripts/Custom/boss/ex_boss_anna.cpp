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

enum Spells{
	SPELL_HEX = 66054,
	SPELL_SHEEP = 59634,
	SPELL_FLUCH_DER_PEIN = 65814,
	SPELL_SPALTEN = 40504,
	SPELL_SCHATTENWORT_SCHMERZ = 65541,
	SPELL_FROSTFIEBER = 67767,
	SPELL_RUESTUNG_ZERREISSEN = 74367,
	SPELL_VERDERBNIS = 65810,
	SPELL_FLEISCH_EINAESCHERN = 66237,
	SPELL_ZAUBERSCHILD = 33054,
	SPELL_WIRBELWIND = 40219,
	SPELL_VERDERBENDE_SEUCHE = 60588,
	SPELL_WUNDGIFT = 65962,
	SPELL_HAMMER_DER_GERECHTIGKEIT = 66613,
	SPELL_GOETTLICHER_STURM = 66006


};

enum Events{
	EVENT_HEX = 1,
	EVENT_SHEEP = 2,
	EVENT_FLUCH_DER_PEIN = 3,
	EVENT_SPALTEN = 4,
	EVENT_SCHATTENWORT_SCHMERZ = 5,
	EVENT_FROSTFIEBER = 6,
	EVENT_RUESTUNG_ZERREISSEN = 7,
	EVENT_VERDERBNIS = 8,
	EVENT_FLEISCH_EINAESCHERN = 9,
	EVENT_ZAUBERSCHILD = 10,
	EVENT_WIRBELWIND = 11,
	EVENT_VERDERBENDE_SEUCHE = 12,
	EVENT_WUNDGIFT = 13,
	EVENT_HAMMER_DER_GERECHTIGKEIT = 14,
	EVENT_GOETTLICHER_STURM = 15
};

enum Phases{
	PHASE_ONE = 1,
	PHASE_TWO = 2,
	PHASE_THREE = 3,
	PHASE_FOUR = 4,
	PHASE_FIVE = 5
};

class anna : public CreatureScript{
public:
	anna() : CreatureScript("anna") { }

	struct annaAI : public ScriptedAI {

		annaAI(Creature* creature) : ScriptedAI(creature) {}

		void Reset() override
		{
			_events.Reset();
		}

		void JustEngagedWith(Unit* /*who*/) override
		{
			_events.SetPhase(PHASE_ONE);
			_events.ScheduleEvent(EVENT_FLUCH_DER_PEIN, 1000);
			_events.ScheduleEvent(EVENT_SPALTEN, 10000);
			_events.ScheduleEvent(EVENT_FROSTFIEBER, 1000);
		}

		void DamageTaken(Unit* /*attacker*/, uint32& damage) override
		{
			if (me->HealthBelowPctDamaged(80, damage) && _events.IsInPhase(PHASE_ONE))
			{
				_events.SetPhase(PHASE_TWO);
				_events.ScheduleEvent(EVENT_HEX, 0);
				_events.ScheduleEvent(EVENT_FLUCH_DER_PEIN, 1000);
				_events.ScheduleEvent(EVENT_SPALTEN, 8000);
				_events.ScheduleEvent(EVENT_SCHATTENWORT_SCHMERZ, 10000);
			}

			if (me->HealthBelowPctDamaged(60, damage) && _events.IsInPhase(PHASE_TWO))
			{
				_events.SetPhase(PHASE_THREE);
				_events.ScheduleEvent(EVENT_VERDERBNIS, 500);
				_events.ScheduleEvent(EVENT_HEX, 1000);
				_events.ScheduleEvent(EVENT_SHEEP, 1500);
				_events.ScheduleEvent(EVENT_FROSTFIEBER, 5000);
				_events.ScheduleEvent(EVENT_HAMMER_DER_GERECHTIGKEIT, 20000);
				_events.ScheduleEvent(EVENT_WIRBELWIND, 21000);
			}

			if (me->HealthBelowPctDamaged(40, damage) && _events.IsInPhase(PHASE_THREE))
			{
				_events.SetPhase(PHASE_FOUR);
				_events.ScheduleEvent(EVENT_HAMMER_DER_GERECHTIGKEIT, 1000);
				_events.ScheduleEvent(EVENT_HEX, 1000);
				_events.ScheduleEvent(EVENT_SHEEP, 1000);
				_events.ScheduleEvent(EVENT_RUESTUNG_ZERREISSEN, 2000);
				_events.ScheduleEvent(EVENT_FLEISCH_EINAESCHERN, 10000);
				_events.ScheduleEvent(EVENT_SCHATTENWORT_SCHMERZ, 12000);
				_events.ScheduleEvent(EVENT_WIRBELWIND, 15000);
			}

			if (me->HealthBelowPctDamaged(20, damage) && _events.IsInPhase(PHASE_FOUR))
			{
				_events.SetPhase(PHASE_FIVE);
				_events.ScheduleEvent(EVENT_ZAUBERSCHILD, 0);
				_events.ScheduleEvent(EVENT_RUESTUNG_ZERREISSEN, 1000);
				_events.ScheduleEvent(EVENT_FLEISCH_EINAESCHERN, 2000);
				_events.ScheduleEvent(EVENT_VERDERBENDE_SEUCHE, 800);
				_events.ScheduleEvent(EVENT_WUNDGIFT, 1000);
				_events.ScheduleEvent(EVENT_GOETTLICHER_STURM, 15000);

			}
		}

		void JustDied(Unit* /*killer*/) override
		{
			//char msg[250];
			//snprintf(msg, 250, "|cffff0000[Boss System]|r Boss|cffff6060 Anna|r wurde getoetet! Der Respawn erfolgt in 7 Tagen.");
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

				case EVENT_HEX:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 4)){
						DoCast(target, SPELL_HEX);
					}
					_events.ScheduleEvent(EVENT_HEX, 16000);
					break;

				case EVENT_SHEEP:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 2)){
						DoCast(target, SPELL_SHEEP);
					}
					_events.ScheduleEvent(EVENT_SHEEP, 20000);
					break;

				case EVENT_FLUCH_DER_PEIN:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0)){
						DoCast(target, SPELL_FLUCH_DER_PEIN);
					}
					_events.ScheduleEvent(EVENT_FLUCH_DER_PEIN, 12000);
					break;

				case EVENT_SPALTEN:
					DoCastVictim(SPELL_SPALTEN);
					_events.ScheduleEvent(EVENT_SPALTEN, 25000);
					break;

				case EVENT_SCHATTENWORT_SCHMERZ:
					DoCast(SPELL_SCHATTENWORT_SCHMERZ);
					_events.ScheduleEvent(EVENT_SCHATTENWORT_SCHMERZ, 20000);
					break;

				case EVENT_RUESTUNG_ZERREISSEN:
					DoCastVictim(SPELL_RUESTUNG_ZERREISSEN);
					_events.ScheduleEvent(EVENT_RUESTUNG_ZERREISSEN, 20000);
					break;

				case EVENT_VERDERBNIS:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0)){
						DoCast(target, SPELL_VERDERBNIS);
					}
					_events.ScheduleEvent(EVENT_VERDERBNIS, 15000);
					break;

				case EVENT_FLEISCH_EINAESCHERN:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0)){
						DoCast(target, SPELL_FLEISCH_EINAESCHERN);
					}
					_events.ScheduleEvent(EVENT_FLEISCH_EINAESCHERN, 15000);
					break;

				case EVENT_WIRBELWIND:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 1)){
						DoCast(target, SPELL_WIRBELWIND);
					}
					_events.ScheduleEvent(EVENT_WIRBELWIND, 30000);
					break;

				case EVENT_VERDERBENDE_SEUCHE:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0)){
						DoCast(target, SPELL_VERDERBENDE_SEUCHE);
					}
					_events.ScheduleEvent(EVENT_VERDERBENDE_SEUCHE, 30000);
						break;

				case EVENT_WUNDGIFT:
					DoCastVictim(SPELL_WUNDGIFT);
					_events.ScheduleEvent(EVENT_WUNDGIFT, 10000);
					break;

				case EVENT_HAMMER_DER_GERECHTIGKEIT:
					if (Unit* target = SelectTarget(SelectTargetMethod::Random, 1)){
						DoCast(target, SPELL_HAMMER_DER_GERECHTIGKEIT);
					}
					_events.ScheduleEvent(EVENT_HAMMER_DER_GERECHTIGKEIT, 12000);
					break;

				case EVENT_GOETTLICHER_STURM:
					DoCastVictim(SPELL_GOETTLICHER_STURM);
					_events.ScheduleEvent(EVENT_GOETTLICHER_STURM, 20000);
					break;

				case EVENT_ZAUBERSCHILD:
					DoCast(me, SPELL_ZAUBERSCHILD);
					_events.ScheduleEvent(EVENT_ZAUBERSCHILD, 30000);
					break;

				default:
					break;
				}
			}

			DoMeleeAttackIfReady();
		}

	private:
		EventMap _events;
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new annaAI(creature);
	}

};

void AddSC_anna()
{
	new anna();
}
