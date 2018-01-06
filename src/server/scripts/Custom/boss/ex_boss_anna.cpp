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
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 4)){
						DoCast(target, SPELL_HEX);
					}
					_events.ScheduleEvent(EVENT_HEX, 16000);
					break;

				case EVENT_SHEEP:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 2)){
						DoCast(target, SPELL_SHEEP);
					}
					_events.ScheduleEvent(EVENT_SHEEP, 20000);
					break;

				case EVENT_FLUCH_DER_PEIN:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){	
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
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
						DoCast(target, SPELL_VERDERBNIS);
					}
					_events.ScheduleEvent(EVENT_VERDERBNIS, 15000);
					break;

				case EVENT_FLEISCH_EINAESCHERN:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
						DoCast(target, SPELL_FLEISCH_EINAESCHERN);
					}
					_events.ScheduleEvent(EVENT_FLEISCH_EINAESCHERN, 15000);
					break;

				case EVENT_WIRBELWIND:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1)){
						DoCast(target, SPELL_WIRBELWIND);
					}
					_events.ScheduleEvent(EVENT_WIRBELWIND, 30000);
					break;

				case EVENT_VERDERBENDE_SEUCHE:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
						DoCast(target, SPELL_VERDERBENDE_SEUCHE);
					}
					_events.ScheduleEvent(EVENT_VERDERBENDE_SEUCHE, 30000);
						break;

				case EVENT_WUNDGIFT:
					DoCastVictim(SPELL_WUNDGIFT);
					_events.ScheduleEvent(EVENT_WUNDGIFT, 10000);
					break;

				case EVENT_HAMMER_DER_GERECHTIGKEIT:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1)){
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


/*######
## npc_thalorien_dawnseeker
######*/

enum ThalorienDawnseekerSays
{
    SAY_THALORIEN_1 = 0,
    SAY_THALORIEN_2 = 1,
    SAY_THALORIEN_3 = 2,
    SAY_THALORIEN_4 = 3,
    SAY_THALORIEN_5 = 4,
    SAY_THALORIEN_6 = 5,
    SAY_THALORIEN_7 = 6,
    SAY_THALORIEN_8 = 7,
    SAY_THALORIEN_9 = 8,
    SAY_THALORIEN_10 = 9
};

enum MorlenSays
{
    SAY_MORLEN_1 = 0,
    SAY_MORLEN_2 = 1,
    SAY_MORLEN_3 = 2,
    SAY_MORLEN_4 = 3,
    SAY_MORLEN_5 = 4
};

enum ThalorienDawnseekerCreatures
{
    NPC_THALORIEN_DAWNSEEKER    = 37205,
    NPC_MORLEN_GOLDGRIP         = 37542,
    NPC_QUEST_CREDIT            = 37601,
    NPC_SUNWELL_DEFENDER        = 37211,
    NPC_SCOURGE_ZOMBIE          = 37538,
    NPC_GHOUL_INVADER           = 37539,
    NPC_CRYPT_RAIDER            = 37541
};

enum Phase
{
    PHASE_SHIFT = 70193
};

enum ThalorienDawnseekerActions
{
    ACTION_START_QUEST = 1
};

enum ThalorienDawnseekerEvents
{
    EVENT_STEP_1    = 1,
    EVENT_STEP_2    = 2,
    EVENT_STEP_3    = 3,
    EVENT_STEP_4    = 4,
    EVENT_STEP_5    = 5,
    EVENT_STEP_6    = 6,
    EVENT_STEP_7    = 7,
    EVENT_STEP_8    = 8,
    EVENT_STEP_9    = 9,
    EVENT_STEP_10   = 10,
    EVENT_STEP_11   = 11,
    EVENT_STEP_12   = 12,
    EVENT_STEP_13   = 13,
    EVENT_STEP_14   = 14,
    EVENT_STEP_15   = 15,
    EVENT_STEP_16   = 16,
    EVENT_STEP_17   = 17,
    EVENT_STEP_18   = 18,
    EVENT_STEP_19   = 19
};

enum ThalorienDawnseekerMisc
{
    OPTION_ID_REMAINS       = 0,
    DISPLAY_MOUNT           = 25678,
    QUEST_THALORIEN_A       = 24535,
    QUEST_THALORIEN_H       = 24563,
    GOSSIP_MENU_ID_REMAINS  = 37552,
    SPELL_BLOOD_PRESENCE    = 50689
};

Position const ZombieLoc[5] =
{
    { 11768.7f, -7065.83f, 24.0971f, 0.125877f },
    { 11769.5f, -7063.83f, 24.1399f, 6.06035f },
    { 11769.8f, -7061.41f, 24.2536f, 6.06035f },
    { 11769.4f, -7057.86f, 24.4624f, 0.00335493f },
    { 11769.4f, -7054.56f, 24.6869f, 0.00335493f }
};

Position const GuardLoc[4] =
{
    { 11807.0f, -7070.37f, 25.372f, 3.218f },
    { 11805.7f, -7073.64f, 25.5694f, 3.218f },
    { 11805.6f, -7077.0f, 25.9643f, 3.218f },
    { 11806.0f, -7079.71f, 26.2067f, 3.218f }
};

class npc_thalorien_dawnseeker : public CreatureScript
{
    public:
        npc_thalorien_dawnseeker() : CreatureScript("npc_thalorien_dawnseeker") { }

        bool GossipHello(Player* player, Creature* creature)
        {
            player->PrepareGossipMenu(creature, 0);

			if ((player->GetQuestStatus(QUEST_THALORIEN_A) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(QUEST_THALORIEN_H) == QUEST_STATUS_INCOMPLETE))
				AddGossipItemFor(player, GOSSIP_MENU_ID_REMAINS, OPTION_ID_REMAINS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SendPreparedGossip(creature);
            return true;
        }

        bool GossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
				CloseGossipMenuFor(player);
                creature->AI()->SetGUID(player->GetGUID());
                creature->CastSpell(creature, PHASE_SHIFT, true);
                player->CastSpell(player, PHASE_SHIFT, true);
                creature->AI()->DoAction(ACTION_START_QUEST);
                break;
            default:
                return false;
            }
            return true;
        }

        struct npc_thalorien_dawnseekerAI : public ScriptedAI
        {
            npc_thalorien_dawnseekerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, uiPlayer))
                    player->SetPhaseMask(1, true);
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->DisappearAndDie();
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetPhaseMask(1, true);
                events.Reset();
            }

            void UpdateAI(uint32 diff) override
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                case EVENT_STEP_1:
                    if (Creature* Thalorien = me->SummonCreature(NPC_THALORIEN_DAWNSEEKER, 11797.0f, -7074.06f, 26.379f, 0.242908f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                    {
                        Thalorien->SetPhaseMask(2, true);
                        uiThalorien = Thalorien->GetGUID();
                    }

                    for (int i = 0; i < 4; ++i)
                        if (Creature* Guard = me->SummonCreature(NPC_SUNWELL_DEFENDER, GuardLoc[i], TEMPSUMMON_TIMED_DESPAWN, 30000))
                            Guard->SetPhaseMask(2, true);

                    if (Creature* Morlen = me->SummonCreature(NPC_MORLEN_GOLDGRIP, 11776.8f, -7050.72f, 25.2394f, 5.13752f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                    {
                        Morlen->Mount(DISPLAY_MOUNT);
                        Morlen->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        Morlen->SetPhaseMask(2, true);
                        Morlen->SetReactState(REACT_PASSIVE);
                        uiMorlen = Morlen->GetGUID();
                    }
                    events.ScheduleEvent(EVENT_STEP_2, 0.1 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_2:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_1);
                    events.ScheduleEvent(EVENT_STEP_3, 5 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_3:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_2);
                    events.ScheduleEvent(EVENT_STEP_4, 5 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_4:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_3);
                    events.ScheduleEvent(EVENT_STEP_5, 10 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_5:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_4);
                    events.ScheduleEvent(EVENT_STEP_6, 6 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_6:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->GetMotionMaster()->MovePoint(0, 11787.3f, -7064.11f, 25.8395f);
                    events.ScheduleEvent(EVENT_STEP_7, 6 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_7:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_5);
                    events.ScheduleEvent(EVENT_STEP_8, 9 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_8:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_6);
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        Morlen->CastSpell(Morlen, SPELL_BLOOD_PRESENCE, true);
                    events.ScheduleEvent(EVENT_STEP_9, 9 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_9:
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        Morlen->AI()->Talk(SAY_MORLEN_1);
                    events.ScheduleEvent(EVENT_STEP_10, 9 * IN_MILLISECONDS);
                    break;
                    // Intro 
                case EVENT_STEP_10:
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        Morlen->AI()->Talk(SAY_MORLEN_2);
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->SetHomePosition(Thalorien->GetPositionX(), Thalorien->GetPositionY(), Thalorien->GetPositionZ(), Thalorien->GetOrientation());
                    SummonWave(NPC_SCOURGE_ZOMBIE);
                    events.ScheduleEvent(EVENT_STEP_11, 30 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_11:
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        Morlen->AI()->Talk(SAY_MORLEN_3);
                    SummonWave(NPC_GHOUL_INVADER);
                    events.ScheduleEvent(EVENT_STEP_12, 30 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_12:
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        Morlen->AI()->Talk(SAY_MORLEN_4);
                    SummonWave(NPC_CRYPT_RAIDER);
                    events.ScheduleEvent(EVENT_STEP_13, 30 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_13:
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        Morlen->AI()->Talk(SAY_MORLEN_5);
                    events.ScheduleEvent(EVENT_STEP_14, 1 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_14:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    {
                        if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                        {
                            Morlen->SetReactState(REACT_AGGRESSIVE);
                            AddThreat(Thalorien, 100, Morlen);
                            Morlen->AI()->AttackStart(Thalorien);
                            Morlen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        }
                    }
                    break;
                    // Outro
                case EVENT_STEP_15:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_7);
                    events.ScheduleEvent(EVENT_STEP_16, 5 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_16:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_8);
                    events.ScheduleEvent(EVENT_STEP_17, 5 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_17:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_9);
                    events.ScheduleEvent(EVENT_STEP_18, 6 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_18:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->AI()->Talk(SAY_THALORIEN_10);
                    events.ScheduleEvent(EVENT_STEP_19, 12 * IN_MILLISECONDS);
                    break;
                case EVENT_STEP_19:
                    if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                        Thalorien->DisappearAndDie();
                    if (Player* player = ObjectAccessor::GetPlayer(*me, uiPlayer))
                        player->KilledMonsterCredit(NPC_QUEST_CREDIT, player->GetGUID());
                    Reset();
                    break;
                default:
                    break;
                }

                DoMeleeAttackIfReady();
            }

            void SummonWave(uint32 entry)
            {
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                {
                    for (int i = 0; i < 5; ++i)
                        if (Creature* Zombie = me->SummonCreature(entry, ZombieLoc[i], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            Zombie->SetPhaseMask(2, true);
                            AddThreat(Thalorien, 100.0f, Zombie);
                            Zombie->AI()->AttackStart(Thalorien);
                        }
                }
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                if (summon->GetEntry() == NPC_THALORIEN_DAWNSEEKER)
                {
                    Reset();
                    return;
                }

                if (summon->GetEntry() == NPC_MORLEN_GOLDGRIP)
                    events.ScheduleEvent(EVENT_STEP_15, 3 * IN_MILLISECONDS);
            }

            void SetGUID(ObjectGuid const& guid, int32 /*id*/) override
            {
                uiPlayer = guid;
            }

            void DoAction(int32 action) override
            {
                switch (action)
                {
                case ACTION_START_QUEST:
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    events.ScheduleEvent(EVENT_STEP_1, 0);
                    break;
                }
            }
        private:
            EventMap events;
            ObjectGuid uiThalorien;
            ObjectGuid uiPlayer;
            ObjectGuid uiMorlen;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_thalorien_dawnseekerAI(creature);
        }
};

void AddSC_anna()
{
	new npc_thalorien_dawnseeker();
	new anna();
}
