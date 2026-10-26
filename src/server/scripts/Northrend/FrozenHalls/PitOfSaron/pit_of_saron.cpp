/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "PassiveAI.h"
#include "pit_of_saron.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_FIREBALL              = 69583, //Ymirjar Flamebearer
    SPELL_HELLFIRE              = 69586,
    SPELL_TACTICAL_BLINK        = 69584,
    SPELL_FROST_BREATH          = 69527, //Iceborn Proto-Drake
    SPELL_LEAPING_FACE_MAUL     = 69504, // Geist Ambusher
};

enum Events
{
    // Ymirjar Flamebearer
    EVENT_FIREBALL              = 1,
    EVENT_TACTICAL_BLINK        = 2,
};

enum IntroTexts
{
    SAY_TYRANNUS_INTRO_1            = 4,
    SAY_TYRANNUS_INTRO_2            = 5,
    SAY_TYRANNUS_INTRO_3            = 6,
    SAY_TYRANNUS_INTRO_4            = 7,
    SAY_JAINA_INTRO_1               = 2,
    SAY_JAINA_INTRO_2               = 6,
    SAY_JAINA_INTRO_3               = 9,
    SAY_JAINA_INTRO_4               = 10,
    SAY_JAINA_INTRO_5               = 12,
    SAY_SYLVANAS_INTRO_1            = 15,
    SAY_SYLVANAS_INTRO_2            = 7,
    SAY_SYLVANAS_INTRO_3            = 11,
    SAY_SYLVANAS_INTRO_4            = 13
};

bool ScheduledIcicleSummons::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    if (roll_chance_i(12))
    {
        _trigger->CastSpell(_trigger, SPELL_ICICLE_SUMMON, true);
        _trigger->m_Events.AddEvent(new ScheduledIcicleSummons(_trigger), _trigger->m_Events.CalculateTime(randtime(20s, 35s)));
    }
    else
        _trigger->m_Events.AddEvent(new ScheduledIcicleSummons(_trigger), _trigger->m_Events.CalculateTime(randtime(1s, 20s)));

    return true;
}

struct npc_ymirjar_flamebearer: public ScriptedAI
{
    npc_ymirjar_flamebearer(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset() override
    {
        _events.Reset();
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_FIREBALL, 4s);
        _events.ScheduleEvent(EVENT_TACTICAL_BLINK, 15s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FIREBALL:
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                        DoCast(target, SPELL_FIREBALL);
                    _events.RescheduleEvent(EVENT_FIREBALL, 5s);
                    break;
                case EVENT_TACTICAL_BLINK:
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                        DoCast(target, SPELL_TACTICAL_BLINK);
                    DoCast(me, SPELL_HELLFIRE);
                    _events.RescheduleEvent(EVENT_TACTICAL_BLINK, 12s);
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

struct npc_iceborn_protodrake: public ScriptedAI
{
    npc_iceborn_protodrake(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _frostBreathCooldown = 5000;
    }

    void Reset() override
    {
        Initialize();
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        if (Vehicle* _vehicle = me->GetVehicleKit())
            _vehicle->RemoveAllPassengers();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (_frostBreathCooldown < diff)
        {
            DoCastVictim(SPELL_FROST_BREATH);
            _frostBreathCooldown = 10000;
        }
        else
            _frostBreathCooldown -= diff;

        DoMeleeAttackIfReady();
    }

private:
    uint32 _frostBreathCooldown;
};

struct npc_geist_ambusher: public ScriptedAI
{
    npc_geist_ambusher(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _leapingFaceMaulCooldown = 9000;
    }

    void Reset() override
    {
        Initialize();
    }

    void JustEngagedWith(Unit* who) override
    {
        if (who->GetTypeId() != TYPEID_PLAYER)
            return;

        // the max range is determined by aggro range
        if (me->GetDistance(who) > 5.0f)
            DoCast(who, SPELL_LEAPING_FACE_MAUL);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (_leapingFaceMaulCooldown < diff)
        {
            if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 5.0f, true))
                DoCast(target, SPELL_LEAPING_FACE_MAUL);
            _leapingFaceMaulCooldown = urand(9000, 14000);
        }
        else
            _leapingFaceMaulCooldown -= diff;

        DoMeleeAttackIfReady();
    }

private:
    uint32 _leapingFaceMaulCooldown;
};

struct npc_pit_of_saron_icicle : public PassiveAI
{
    npc_pit_of_saron_icicle(Creature* creature) : PassiveAI(creature)
    {
        me->SetDisplayId(me->GetCreatureTemplate()->Modelid1);
    }

    void IsSummonedBy(WorldObject* summoner) override
    {
        _summonerGUID = summoner->GetGUID();

        _scheduler.Schedule(Milliseconds(3650), [this](TaskContext /*context*/)
        {
            DoCastSelf(SPELL_ICICLE_FALL_TRIGGER, true);
            DoCastSelf(SPELL_ICICLE_FALL_VISUAL);

            if (Unit* caster = ObjectAccessor::GetUnit(*me, _summonerGUID))
                caster->RemoveDynObject(SPELL_ICICLE_SUMMON);
        });
    }

    void UpdateAI(uint32 diff) override
    {
        _scheduler.Update(diff);
    }

private:
    TaskScheduler _scheduler;
    ObjectGuid _summonerGUID;
};

enum StartIntroEvents
{
    EVENT_INTRO_SPAWN = 1,
    EVENT_TYRANNUS_INTRO_1,
    EVENT_TYRANNUS_INTRO_2,
    EVENT_LEADER_INTRO_1,
    EVENT_TYRANNUS_INTRO_3,
    EVENT_LEADER_INTRO_2,
    EVENT_LEADER_INTRO_3,
    EVENT_TYRANNUS_INTRO_4,
    EVENT_LEADER_INTRO_4,
    EVENT_FINISH_INTRO
};

static Position const StartIntroPortalPos = { 424.46f, 212.16f, 528.8f, 0.0f };
static Position const StartIntroLeaderPos = { 440.788f, 213.76f, 528.711f, 0.0f };

struct IntroSummonPosition
{
    uint32 Entry[2];
    Position Destination;
};

static IntroSummonPosition const IntroSummons[] =
{
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 452.884f, 209.141f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 450.541f, 212.28f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 449.835f, 206.68f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_2_HORDE }, { 446.542f, 209.986f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_2_HORDE }, { 447.29f, 213.916f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_2_HORDE }, { 445.794f, 206.057f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 446.74f, 228.577f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 449.19f, 226.21f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 447.352f, 222.754f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 443.346f, 192.343f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 446.293f, 195.047f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_1_ALLIANCE, NPC_CHAMPION_1_HORDE }, { 444.035f, 197.67f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_3_HORDE }, { 442.69f, 223.525f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_3_HORDE }, { 442.967f, 219.535f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_3_HORDE }, { 442.526f, 199.361f, 528.84f, 0.0f } },
    { { NPC_CHAMPION_2_ALLIANCE, NPC_CHAMPION_3_HORDE }, { 442.843f, 203.193f, 528.84f, 0.0f } },
    { { NPC_ELANDRA, NPC_KILARA }, { 438.505f, 211.54f, 528.71f, 0.0f } },
    { { NPC_KORLAEN, NPC_KORALEN }, { 438.946f, 215.427f, 528.71f, 0.0f } }
};

class npc_pos_leader : public CreatureScript
{
public:
    npc_pos_leader() : CreatureScript("npc_pos_leader") { }

    struct npc_pos_leaderAI : public NullCreatureAI
    {
        npc_pos_leaderAI(Creature* creature) : NullCreatureAI(creature), _instance(creature->GetInstanceScript()), _summons(creature) { }

        void Reset() override
        {
            _events.Reset();
            _summons.DespawnAll();
            _summonIndex = 0;
            _started = _instance && _instance->GetData(DATA_INSTANCE_PROGRESS) != INSTANCE_PROGRESS_NONE;

            if (!_started)
                me->SetVisible(false);
        }

        void SetData(uint32 type, uint32 /*value*/) override
        {
            if (type != DATA_START_INTRO || !_instance || _instance->GetData(DATA_INSTANCE_PROGRESS) != INSTANCE_PROGRESS_NONE || _started)
                return;

            _started = true;
            _instance->SetData(DATA_INSTANCE_PROGRESS, INSTANCE_PROGRESS_INTRO_IN_PROGRESS);
            me->SetVisible(true);
            me->setActive(true);
            me->GetMotionMaster()->MovePoint(1, StartIntroLeaderPos);
            _events.ScheduleEvent(EVENT_INTRO_SPAWN, 1s);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_INTRO_SPAWN:
                    {
                        if (_summonIndex < std::size(IntroSummons))
                        {
                            uint8 teamIndex = _instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE ? 0 : 1;
                            IntroSummonPosition const& spawn = IntroSummons[_summonIndex++];
                            if (Creature* summon = me->SummonCreature(spawn.Entry[teamIndex], StartIntroPortalPos))
                            {
                                summon->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                                summon->SetSpeed(MOVE_RUN, 0.8f);
                                summon->GetMotionMaster()->MovePoint(1, spawn.Destination);
                                summon->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                            }
                            _events.ScheduleEvent(EVENT_INTRO_SPAWN, 150ms);
                        }
                        else
                            _events.ScheduleEvent(EVENT_TYRANNUS_INTRO_1, 2500ms);
                        break;
                    }
                    case EVENT_TYRANNUS_INTRO_1:
                        TalkTyrannus(SAY_TYRANNUS_INTRO_1);
                        _events.ScheduleEvent(EVENT_TYRANNUS_INTRO_2, 7s);
                        break;
                    case EVENT_TYRANNUS_INTRO_2:
                        TalkTyrannus(SAY_TYRANNUS_INTRO_2);
                        _events.ScheduleEvent(EVENT_LEADER_INTRO_1, 14s);
                        break;
                    case EVENT_LEADER_INTRO_1:
                        TalkLeader(SAY_JAINA_INTRO_1, SAY_SYLVANAS_INTRO_1);
                        _events.ScheduleEvent(EVENT_TYRANNUS_INTRO_3, 5s);
                        break;
                    case EVENT_TYRANNUS_INTRO_3:
                        TalkTyrannus(SAY_TYRANNUS_INTRO_3);
                        _events.ScheduleEvent(EVENT_LEADER_INTRO_2, 5s);
                        break;
                    case EVENT_LEADER_INTRO_2:
                        TalkLeader(SAY_JAINA_INTRO_2, SAY_SYLVANAS_INTRO_2);
                        _events.ScheduleEvent(EVENT_LEADER_INTRO_3, 7s);
                        break;
                    case EVENT_LEADER_INTRO_3:
                        TalkLeader(SAY_JAINA_INTRO_3, SAY_SYLVANAS_INTRO_3);
                        _events.ScheduleEvent(EVENT_TYRANNUS_INTRO_4, 7s);
                        break;
                    case EVENT_TYRANNUS_INTRO_4:
                        TalkTyrannus(SAY_TYRANNUS_INTRO_4);
                        _events.ScheduleEvent(EVENT_LEADER_INTRO_4, 4s);
                        break;
                    case EVENT_LEADER_INTRO_4:
                        TalkLeader(SAY_JAINA_INTRO_4, SAY_SYLVANAS_INTRO_4);
                        _events.ScheduleEvent(EVENT_FINISH_INTRO, 8s);
                        break;
                    case EVENT_FINISH_INTRO:
                        if (me->GetEntry() == NPC_JAINA_PART1)
                            Talk(SAY_JAINA_INTRO_5);
                        _instance->SetData(DATA_INSTANCE_PROGRESS, INSTANCE_PROGRESS_FINISHED_INTRO);
                        me->setActive(false);
                        break;
                    default:
                        break;
                }
            }
        }

        void JustSummoned(Creature* summon) override
        {
            _summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            _summons.Despawn(summon);
        }

        void AttackStart(Unit* /*who*/) override { }
        void MoveInLineOfSight(Unit* /*who*/) override { }

    private:
        void TalkTyrannus(uint8 text)
        {
            if (Creature* tyrannus = ObjectAccessor::GetCreature(*me, _instance->GetGuidData(DATA_TYRANNUS_EVENT)))
            {
                tyrannus->setActive(true);
                tyrannus->AI()->Talk(text);
            }
        }

        void TalkLeader(uint8 allianceText, uint8 hordeText)
        {
            Talk(me->GetEntry() == NPC_JAINA_PART1 ? allianceText : hordeText);
        }

        EventMap _events;
        InstanceScript* _instance;
        SummonList _summons;
        uint8 _summonIndex = 0;
        bool _started = false;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetPitOfSaronAI<npc_pos_leaderAI>(creature);
    }
};

// 70827 - Ice Shards
class spell_pos_ice_shards : public SpellScript
{
    PrepareSpellScript(spell_pos_ice_shards);

    bool Load() override
    {
        // This script should execute only in Pit of Saron
        return InstanceHasScript(GetCaster(), PoSScriptName);
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (GetHitPlayer())
            GetCaster()->GetInstanceScript()->SetData(DATA_ICE_SHARDS_HIT, 1);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pos_ice_shards::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

enum TyrannusEventCavernEmote
{
    SAY_TYRANNUS_CAVERN_ENTRANCE = 3
};

class at_pit_cavern_entrance : public AreaTriggerScript
{
    public:
        at_pit_cavern_entrance() : AreaTriggerScript("at_pit_cavern_entrance") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
        {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_CAVERN_ACTIVE))
                    return true;

                instance->SetData(DATA_CAVERN_ACTIVE, 1);

                if (Creature* tyrannus = ObjectAccessor::GetCreature(*player, instance->GetGuidData(DATA_TYRANNUS_EVENT)))
                    tyrannus->AI()->Talk(SAY_TYRANNUS_CAVERN_ENTRANCE);
            }
            return true;
        }
};

class at_pit_cavern_end : public AreaTriggerScript
{
public:
    at_pit_cavern_end() : AreaTriggerScript("at_pit_cavern_end") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            instance->SetData(DATA_CAVERN_ACTIVE, 0);

            if (!instance->GetData(DATA_ICE_SHARDS_HIT))
                instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_DONT_LOOK_UP_ACHIEV_CREDIT, 0, player);
        }

        return true;
    }
};

void AddSC_pit_of_saron()
{
    RegisterPitOfSaronCreatureAI(npc_ymirjar_flamebearer);
    RegisterPitOfSaronCreatureAI(npc_iceborn_protodrake);
    RegisterPitOfSaronCreatureAI(npc_geist_ambusher);
    RegisterPitOfSaronCreatureAI(npc_pit_of_saron_icicle);
    new npc_pos_leader();
    RegisterSpellScript(spell_pos_ice_shards);
    new at_pit_cavern_entrance();
    new at_pit_cavern_end();
}
