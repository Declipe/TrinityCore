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
#include "Config.h"

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
            _events.ScheduleEvent(EVENT_SHADOW_SPIKE, 8000);
            _events.ScheduleEvent(EVENT_FEAR, 10000);
            _events.ScheduleEvent(EVENT_BLIZZARD, 30000);

        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(75, damage) && _events.IsInPhase(PHASE_ONE))
            {
                _events.SetPhase(PHASE_TWO);
                _events.ScheduleEvent(EVENT_BERSERK, 10000);
                _events.ScheduleEvent(EVENT_POISON_SHOCK, 10000);
                _events.ScheduleEvent(EVENT_ACID_BLAST, 15000);
                _events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                _events.ScheduleEvent(EVENT_FEAR, 45000);

            }

            if (me->HealthBelowPctDamaged(35, damage) && _events.IsInPhase(PHASE_TWO))
            {
                _events.SetPhase(PHASE_THREE);
                _events.ScheduleEvent(EVENT_ARCANE_BOMB, 5000);
                _events.ScheduleEvent(EVENT_ENRAGE, 100000);
                _events.ScheduleEvent(EVENT_ACID_BLAST, 16000);
                _events.ScheduleEvent(EVENT_ENRAGE, 200000);
                _events.ScheduleEvent(EVENT_BLIZZARD, 45000);
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
                    _events.ScheduleEvent(EVENT_FEAR, 8000);
                    break;
                case EVENT_ENRAGE:
                   // Talk(SAY_RANDOM);
                    DoCast(SPELL_ENRAGE);
                    break;
                case EVENT_BLIZZARD:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(target, SPELL_BLIZZARD);
                    }
                    _events.ScheduleEvent(EVENT_BLIZZARD, 15000);
                    break;
                case EVENT_POISON_SHOCK:
                   // Talk(SAY_BERSERK);
                    DoCastVictim(SPELL_POISON_SHOCK);
                    _events.ScheduleEvent(EVENT_POISON_SHOCK, 12000);
                    break;
                case EVENT_ARCANE_BOMB:
                   // Talk(SAY_ENRAGE);
                    DoCast(SPELL_ARCANE_BOMB);
                    _events.ScheduleEvent(EVENT_ARCANE_BOMB, 10000);
                    break;
                case EVENT_ACID_BLAST:
                    DoCastVictim(SPELL_ACID_BLAST);
                    _events.ScheduleEvent(EVENT_ACID_BLAST, 15000);
                    break;
                case EVENT_CHAIN_LIGHTNING:
                    DoCast(SPELL_CHAIN_LIGHTNING);
                    _events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 10000);
                    break;
                case EVENT_BERSERK:
                    DoCast(SPELL_BERSERK);
                    _events.ScheduleEvent(EVENT_BERSERK, 10000);
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
class Kargatum_Reward_Battleground : public AllCreatureScript
{
public:
    Kargatum_Reward_Battleground() : AllCreatureScript("Kargatum_Reward_Battleground") {}

    void OnBattlegroundEndReward(Battleground* bg, Player* player, uint32 winner)
    {
        if (!sConfigMgr->GetBoolDefault("BG.Reward.Enable", true))
            return;

        // Reward ItemID
        uint32 ItemID_WSG = sConfigMgr->GetBoolDefault("BG.Reward.ItemID.WSG", 20558);
        uint32 ItemID_ARATHI = sConfigMgr->GetIntDefault("BG.Reward.ItemID.Arathi", 20559);
        uint32 ItemID_ALTERAC = sConfigMgr->GetIntDefault("BG.Reward.ItemID.Alterac", 20560);
        uint32 ItemID_ISLE = sConfigMgr->GetIntDefault("BG.Reward.ItemID.Isle", 47395);
        uint32 ItemID_ANCIENT = sConfigMgr->GetIntDefault("BG.Reward.ItemID.Ancients", 42425);
        uint32 ItemID_EYE = sConfigMgr->GetIntDefault("BG.Reward.ItemID.Eye", 29024);

        // Reward Count
        uint32 WinnerCount = sConfigMgr->GetIntDefault("BG.Reward.WinnerTeam.Count", 3);
        uint32 LoserCount = sConfigMgr->GetIntDefault("BG.Reward.LoserTeam.Count", 1);

       // uint32 winner = player->_winnerTeamId();
        uint32 RewardCount = 0;

        winner == winner ? (RewardCount = WinnerCount) : (RewardCount = LoserCount);

        switch (player->GetZoneId())
        {
        case 3277: // Warsong Gulch
            player->AddItem(ItemID_WSG, RewardCount);
            break;
        case 3358: // Arathi Basin
            player->AddItem(ItemID_ARATHI, RewardCount);
            break;
        case 3820: // Eye of the Storm
            player->AddItem(ItemID_EYE, RewardCount);
            break;
        case 4710: // Isle of Conquest
            player->AddItem(ItemID_ISLE, RewardCount);
            break;
        case 4384: // Strand of the Ancients
            player->AddItem(ItemID_ANCIENT, RewardCount);
            break;
        case 2597: // Alterac Valley
            player->AddItem(ItemID_ALTERAC, RewardCount);
            break;
        default:
            break;
        }
    }
};


void AddSC_test()
{
    new orrig1();
    new Kargatum_Reward_Battleground();
}
