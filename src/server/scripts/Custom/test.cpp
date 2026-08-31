#include "Chat.h"
#include "Config.h"
#include "Containers.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "DatabaseEnvFwd.h"
#include "DBCStores.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GameTime.h"
#include "GossipDef.h"
#include "InstanceScript.h"
#include "Language.h"
#include "Log.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerAI.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "Unit.h"
#include "World.h"
#include "ZynDatabase.h"

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
    orrig1() : CreatureScript("orrig1") {}

    struct orrig1AI : public ScriptedAI
    {
        orrig1AI(Creature* creature) : ScriptedAI(creature), Summons(me) {}

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

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
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

enum LazyPeonYells
{
    SAY_SPELL_HIT = 0
};

enum LazyPeon
{
    QUEST_LAZY_PEONS = 70109,
    GO_LUMBERPILE = 175784,
    SPELL_BUFF_SLEEP = 17743,
    SPELL_AWAKEN_PEON = 19938
};

class npcpeon : public CreatureScript
{
public:
    npcpeon() : CreatureScript("npcpeon") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npcpeonAI(creature);
    }

    struct npcpeonAI : public ScriptedAI
    {
        npcpeonAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            RebuffTimer = 0;
            work = false;
        }

        uint32 RebuffTimer;
        bool work;

        void Reset() override
        {
            Initialize();
        }

        void MovementInform(uint32 /*type*/, uint32 id) override
        {
            if (id == 1)
                work = true;
        }

        void SpellHit(WorldObject* caster, SpellInfo const* spellInfo) override
        {
            if (spellInfo->Id != SPELL_AWAKEN_PEON)
                return;

            Player* player = caster->ToPlayer();
            if (player && player->GetQuestStatus(QUEST_LAZY_PEONS) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
                Talk(SAY_SPELL_HIT, caster);
                me->RemoveAllAuras();
                if (GameObject* Lumberpile = me->FindNearestGameObject(GO_LUMBERPILE, 20))
                    me->GetMotionMaster()->MovePoint(1, Lumberpile->GetPositionX() - 0.4f, Lumberpile->GetPositionY() + 0.4f, Lumberpile->GetPositionZ());
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (work == true)
                me->HandleEmoteCommand(EMOTE_ONESHOT_WORK_CHOPWOOD);
            if (RebuffTimer <= diff)
            {
                DoCast(me, SPELL_BUFF_SLEEP);
                RebuffTimer = 300000;                 //Rebuff agian in 5 minutes
            }
            else
                RebuffTimer -= diff;
            if (!UpdateVictim())
                return;
            DoMeleeAttackIfReady();
        }
    };
};

// 456 - SHOWLABEL Only OFF
class spell_gen_showlabel_off : public SpellScript
{
    PrepareSpellScript(spell_gen_showlabel_off);

    void HandleScriptEffect(SpellEffIndex /* effIndex */)
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->SetGMChat(false);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_showlabel_off::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 2765 - SHOWLABEL Only ON
class spell_gen_showlabel_on : public SpellScript
{
    PrepareSpellScript(spell_gen_showlabel_on);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->SetGMChat(true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_showlabel_on::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 1509 - GM Only OFF
class spell_gen_gm_off : public SpellScript
{
    PrepareSpellScript(spell_gen_gm_off);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            player->SetGameMaster(false);
            player->UpdateTriggerVisibility();
        }
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_gm_off::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 18139 - GM Only ON
class spell_gen_gm_on : public SpellScript
{
    PrepareSpellScript(spell_gen_gm_on);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            player->SetGameMaster(true);
            player->UpdateTriggerVisibility();
        }
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_gm_on::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 6147 - INVIS Only OFF
class spell_gen_invis_off : public SpellScript
{
    PrepareSpellScript(spell_gen_invis_off);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->SetGMVisible(true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_invis_off::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 2763 - INVIS Only ON
class spell_gen_invis_on : public SpellScript
{
    PrepareSpellScript(spell_gen_invis_on);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->SetGMVisible(false);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_invis_on::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 20114, 24675 - BM Only OFF
class spell_gen_bm_off : public SpellScript
{
    PrepareSpellScript(spell_gen_bm_off);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->SetBeastMaster(false);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_bm_off::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 20115, 24676 - BM Only ON
class spell_gen_bm_on : public SpellScript
{
    PrepareSpellScript(spell_gen_bm_on);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->SetBeastMaster(true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_bm_on::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

enum ExchangerConstants
{
    MAX_PER_EXCHANGE = 1000,   // limit per single transaction

    // action/sender encoding
    ACTION_FROM_BASE = GOSSIP_ACTION_INFO_DEF + 100,   // first menu:  pick "pay with"
    ACTION_TO_BASE = GOSSIP_ACTION_INFO_DEF + 200,   // second menu: pick "receive"
    SENDER_FROM_BASE = GOSSIP_SENDER_MAIN + 100,       // carries "from" index
    ACTION_BACK = GOSSIP_ACTION_INFO_DEF + 1,
    ACTION_CLOSE = GOSSIP_ACTION_INFO_DEF + 2
};

struct EmblemInfo
{
    uint32 itemId;
    char const* name;
};

static constexpr uint8 EMBLEM_COUNT = 5;

static EmblemInfo const Emblems[EMBLEM_COUNT] =
{
    { 40752, "Emblem of Heroism"  },
    { 40753, "Emblem of Valor"    },
    { 45624, "Emblem of Conquest" },
    { 47241, "Emblem of Triumph"  },
    { 49426, "Emblem of Frost"    }
};

// ExchangeRate[from][to] = how many 'from' emblems are needed for 1 'to' emblem.
// Diagonal (same emblem) = 0 -> exchange not allowed.
// Adjust these values to your liking.
//static uint32 const ExchangeRate[EMBLEM_COUNT][EMBLEM_COUNT] =
//{
//    //  to:  Heroism  Valor  Conquest  Triumph  Frost      from:
//    {        0,       2,     3,        4,       5     },   // Heroism
//    {        1,       0,     2,        3,       4     },   // Valor
//    {        1,       1,     0,        2,       3     },   // Conquest
//    {        1,       1,     1,        0,       2     },   // Triumph
//    {        1,       1,     1,        1,       0     }    // Frost
//};

struct Ratio
{
    uint32 cost;    // how many 'from' emblems you pay
    uint32 reward;  // how many 'to' emblems you receive
};

static Ratio const ExchangeRatio[EMBLEM_COUNT][EMBLEM_COUNT] =
{
    //   to:   Heroism   Valor     Conquest  Triumph   Frost from:
    /*Heroism */ { {0,0},   {1,1},   {1,10},   {1,10},   {1,10} },
    /*Valor   */ { {1,20},   {0,0},   {1,10},   {3,10},   {1,10} },
    /*Conquest*/ { {1,30},   {1,2},   {0,0},   {1,10},   {1,10} },
    /*Triumph */ { {1,40},   {1,3},   {1,2},   {0,0},   {1,10} },
    /*Frost   */ { {1,30},   {1,40},   {1,30},   {1,20},   {0,0} }    // downgrade: 1 Frost -> 2 Triumph, etc.
};

struct npc_emblem_exchanger2 : public ScriptedAI
{
    npc_emblem_exchanger2(Creature* creature) : ScriptedAI(creature) {}

    // -------- first menu: choose what you pay with --------
    bool OnGossipHello(Player* player) override
    {
        ClearGossipMenuFor(player);

        for (uint8 i = 0; i < EMBLEM_COUNT; ++i)
        {
            std::ostringstream ss;
            ss << "Pay with: " << Emblems[i].name
                << " (you have: " << player->GetItemCount(Emblems[i].itemId) << ")";

            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, ss.str(),
                GOSSIP_SENDER_MAIN, ACTION_FROM_BASE + i);
        }

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Close",
            GOSSIP_SENDER_MAIN, ACTION_CLOSE);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
    }

    // -------- second menu: choose what you receive --------
    bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
        uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
        uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        return GossipSelect(player, sender, action);
    }

    bool GossipSelect(Player* player, uint32 /*sender*/, uint32 action)
    {
        if (action == ACTION_CLOSE)
        {
            CloseGossipMenuFor(player);
            return true;
        }

        if (action == ACTION_BACK)
            return OnGossipHello(player);

        if (action < ACTION_FROM_BASE || action >= ACTION_FROM_BASE + EMBLEM_COUNT)
            return false;

        uint8 from = uint8(action - ACTION_FROM_BASE);

        ClearGossipMenuFor(player);

        for (uint8 to = 0; to < EMBLEM_COUNT; ++to)
        {
            Ratio const& r = ExchangeRatio[from][to];
            if (to == from || r.cost == 0 || r.reward == 0)
                continue;

            std::ostringstream ss;
            ss << "Receive: " << Emblems[to].name
                << " (rate: " << r.cost << " " << Emblems[from].name
                << " -> " << r.reward << ")";

            std::ostringstream box;
            box << "Enter the amount of " << Emblems[from].name
                << " you want to SPEND:";

            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, ss.str(),
                SENDER_FROM_BASE + from, ACTION_TO_BASE + to,
                box.str(), 0, true);
        }

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "<- Back",
            GOSSIP_SENDER_MAIN, ACTION_BACK);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
    }

    // -------- amount entered: perform the exchange --------
    bool OnGossipSelectCode(Player* player, uint32 /*menu_id*/, uint32 gossipListId, const char* code) override
    {
        uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
        uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        return GossipSelectCode(player, me, sender, action, code);
    }

    bool GossipSelectCode(Player* player, Creature* /*creature*/, uint32 sender, uint32 action, const char* code)
    {
            CloseGossipMenuFor(player);

            if (sender < SENDER_FROM_BASE || sender >= SENDER_FROM_BASE + EMBLEM_COUNT ||
                action < ACTION_TO_BASE || action >= ACTION_TO_BASE + EMBLEM_COUNT)
                return false;

            uint8 from = uint8(sender - SENDER_FROM_BASE);
            uint8 to = uint8(action - ACTION_TO_BASE);

            ChatHandler handler(player->GetSession());

            Ratio const& r = ExchangeRatio[from][to];
            if (from == to || r.cost == 0 || r.reward == 0)
                return true;

            // --- input validation ---
            if (!code || !*code)
            {
                handler.PSendSysMessage("|cffff0000You didn't enter anything.|r");
                return true;
            }

            for (char const* c = code; *c; ++c)
            {
                if (!isdigit(*c))
                {
                    handler.PSendSysMessage("|cffff0000Please enter a positive whole number.|r");
                    return true;
                }
            }

            uint32 spend = uint32(atoi(code));   // amount of 'from' emblems the player wants to spend

            if (spend == 0 || spend > MAX_PER_EXCHANGE)
            {
                handler.PSendSysMessage("|cffff0000Amount must be between 1 and %u.|r", uint32(MAX_PER_EXCHANGE));
                return true;
            }

            // --- calculate reward, rounded DOWN; only charge for full bundles ---
            uint32 bundles = spend / r.cost;             // how many full bundles the input covers
            uint32 reward = bundles * r.reward;         // emblems the player receives
            uint32 cost = bundles * r.cost;           // emblems actually deducted (<= spend)

            if (reward == 0)
            {
                handler.PSendSysMessage("|cffff0000Too few. Minimum: %u x %s (for %u x %s).|r",
                    r.cost, Emblems[from].name, r.reward, Emblems[to].name);
                return true;
            }

            // inform the player if the leftover is not spent
            if (cost < spend)
                handler.PSendSysMessage("|cffffff00Note: only %u will be spent (%u is not enough for another exchange).|r",
                    cost, spend - cost);

            // --- currency check ---
            if (!player->HasItemCount(Emblems[from].itemId, cost))
            {
                handler.PSendSysMessage("|cffff0000Not enough %s. Required: %u, you have: %u|r",
                    Emblems[from].name, cost, player->GetItemCount(Emblems[from].itemId));
                return true;
            }

            // --- precheck bag space BEFORE deducting ---
            ItemPosCountVec dest;
            if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, Emblems[to].itemId, reward) != EQUIP_ERR_OK)
            {
                handler.PSendSysMessage("|cffff0000Not enough bag space for %u x %s.|r",
                    reward, Emblems[to].name);
                return true;
            }

            // --- deduct ---
            player->DestroyItemCount(Emblems[from].itemId, cost, true);

            // --- recompute dest AFTER deduction ---
            dest.clear();
            if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, Emblems[to].itemId, reward) != EQUIP_ERR_OK)
            {
                player->AddItem(Emblems[from].itemId, cost);
                handler.PSendSysMessage("|cffff0000Exchange failed, currency refunded.|r");
                return true;
            }

            Item* item = player->StoreNewItem(dest, Emblems[to].itemId, true);
            if (!item)
            {
                player->AddItem(Emblems[from].itemId, cost);
                handler.PSendSysMessage("|cffff0000Exchange failed, currency refunded.|r");
                return true;
            }

            player->SendNewItem(item, reward, true, false);
            handler.PSendSysMessage("|cff00ff00Spent %u x %s -> received %u x %s.|r",
                cost, Emblems[from].name, reward, Emblems[to].name);
            return true;
        }
};

void AddSC_test()
{
    RegisterCreatureAI(npc_emblem_exchanger2);
    new orrig1();
    new npcpeon();
    RegisterSpellScript(spell_gen_showlabel_off);
    RegisterSpellScript(spell_gen_showlabel_on);
    RegisterSpellScript(spell_gen_gm_off);
    RegisterSpellScript(spell_gen_gm_on);
    RegisterSpellScript(spell_gen_invis_off);
    RegisterSpellScript(spell_gen_invis_on);
    RegisterSpellScript(spell_gen_bm_on);
    RegisterSpellScript(spell_gen_bm_off);
}
