/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Felblood_Kaelthas
SD%Complete: 80
SDComment: Normal and Heroic Support. Issues: Arcane Spheres do not initially follow targets.
SDCategory: Magisters' Terrace
EndScriptData */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "magisters_terrace.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"

enum Says
{
    SAY_AGGRO                   = 0,                //This yell should be done when the room is cleared. For now, set it as a movelineofsight yell.
    SAY_PHOENIX                 = 1,
    SAY_FLAMESTRIKE             = 2,
    SAY_GRAVITY_LAPSE           = 3,
    SAY_TIRED                   = 4,
    SAY_RECAST_GRAVITY          = 5,
    SAY_DEATH                   = 6
};


enum Spells
{
    // Phase 1 spells
    SPELL_FIREBALL_NORMAL         = 32309,                 // Deals 2700-3300 damage at current target
    SPELL_FIREBALL_HEROIC         = 46164,                 //       4950-6050

    SPELL_PHOENIX                 = 44194,                 // Summons a phoenix (Doesn't work?)
    SPELL_PHOENIX_BURN            = 44197,                 // A spell Phoenix uses to damage everything around
    SPELL_REBIRTH_DMG             = 44196,                 // DMG if a Phoenix rebirth happen

    SPELL_FLAMESTRIKE1_NORMAL     = 69024,                 // Damage part
    SPELL_FLAMESTRIKE1_HEROIC     = 69024,                 // Heroic damage part
    SPELL_FLAMESTRIKE2            = 69024,                 // Flamestrike indicator before the damage
    SPELL_FLAMESTRIKE3            = 69024,                 // Summons the trigger + animation (projectile)

    SPELL_SHOCK_BARRIER           = 46165,                 // Heroic only; 10k damage shield, followed by Pyroblast
    SPELL_PYROBLAST               = 59800,                 // Heroic only; 45-55k fire damage

// Phase 2 spells
    SPELL_GRAVITY_LAPSE_INITIAL   = 44224,
    SPELL_GRAVITY_LAPSE_CHANNEL   = 44251,
    SPELL_TELEPORT_CENTER         = 44218,
    SPELL_GRAVITY_LAPSE_FLY       = 44227,               
    SPELL_GRAVITY_LAPSE_DOT       = 44226,
    SPELL_ARCANE_SPHERE_PASSIVE   = 44263,                 
    SPELL_POWER_FEEDBACK          = 44233                 
};

enum Creatures
{
    CREATURE_PHOENIX              = 24674,
    CREATURE_PHOENIX_EGG          = 24675,
    CREATURE_ARCANE_SPHERE        = 24708
};



class boss_felblood : public CreatureScript
{
public:
    boss_felblood() : CreatureScript("boss_felblood") { }

    CreatureAI* GetAI(Creature* c) const override
	{
		return new boss_felbloodAI(c);
	}
    struct boss_felbloodAI : public ScriptedAI
    {
        boss_felbloodAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
            instance = creature->GetInstanceScript();
        }

        void Initialize()
        {
            /// @todo Timers
            FireballTimer = 0;
            PhoenixTimer = 10000;
            FlameStrikeTimer = 25000;
            CombatPulseTimer = 0;

            PyroblastTimer = 60000;

            GravityLapseTimer = 0;
            GravityLapsePhase = 0;

            FirstGravityLapse = true;
            HasTaunted = false;

            Phase = 0;
        }

        InstanceScript* instance;

        uint32 FireballTimer;
        uint32 PhoenixTimer;
        uint32 FlameStrikeTimer;
        uint32 CombatPulseTimer;

        //Heroic only
        uint32 PyroblastTimer;

        uint32 GravityLapseTimer;
        uint32 GravityLapsePhase;
        // 0 = No Gravity Lapse
        // 1 = Casting Gravity Lapse visual
        // 2 = Teleported people to self
        // 3 = Knocked people up in the air
        // 4 = Applied an aura that allows them to fly, channeling visual, relased Arcane Orbs.

        bool FirstGravityLapse;
        bool HasTaunted;

        uint8 Phase;
        // 0 = Not started
        // 1 = Fireball; Summon Phoenix; Flamestrike
        // 2 = Gravity Lapses

        void Reset() override
        {
            Initialize();

            instance->SetBossState(DATA_KAELTHAS, NOT_STARTED);
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);

            instance->SetBossState(DATA_KAELTHAS, DONE);

            // Enable the Translocation Orb Exit
            if (GameObject* escapeOrb = ObjectAccessor::GetGameObject(*me, instance->GetGuidData(DATA_ESCAPE_ORB)))
                escapeOrb->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage) override
        {
            if (damage > me->GetHealth())
                RemoveGravityLapse(); // Remove Gravity Lapse so that players fall to ground if they kill him when in air.
        }

        void EnterCombat(Unit* /*who*/) override
        {
            instance->SetBossState(DATA_KAELTHAS, IN_PROGRESS);
        }

        void MoveInLineOfSight(Unit* who) override

        {
            if (!HasTaunted && me->IsWithinDistInMap(who, 40.0f))
            {
                Talk(SAY_AGGRO);
                HasTaunted = true;
            }

            ScriptedAI::MoveInLineOfSight(who);
        }

        void SetThreatList(Creature* summonedUnit)
        {
            if (!summonedUnit)
                return;

            ThreatContainer::StorageType const& threatlist = me->GetThreatManager().getThreatList();
            ThreatContainer::StorageType::const_iterator i = threatlist.begin();
            for (i = threatlist.begin(); i != threatlist.end(); ++i)
            {
                Unit* unit = ObjectAccessor::GetUnit(*me, (*i)->getUnitGuid());
                if (unit && unit->IsAlive())
                {
                    float threat = me->GetThreatManager().getThreat(unit);
                    AddThreat(unit, threat, summonedUnit);
                }
            }
        }


        void CastGravityLapseKnockUp()
        {
            ThreatContainer::StorageType threatlist = me->GetThreatManager().getThreatList();
            ThreatContainer::StorageType::const_iterator i = threatlist.begin();
            for (i = threatlist.begin(); i != threatlist.end(); ++i)
            {
                Unit* unit = ObjectAccessor::GetUnit(*me, (*i)->getUnitGuid());
                if (unit && (unit->GetTypeId() == TYPEID_PLAYER))
                    // Knockback into the air
                    unit->CastSpell(unit, SPELL_GRAVITY_LAPSE_DOT, true, 0, 0, me->GetGUID());
            }
        }

        void CastGravityLapseFly()                              // Use Fly Packet hack for now as players can't cast "fly" spells unless in map 530. Has to be done a while after they get knocked into the air...
        {
            ThreatContainer::StorageType threatlist = me->GetThreatManager().getThreatList();
            ThreatContainer::StorageType::const_iterator i = threatlist.begin();
            for (i = threatlist.begin(); i != threatlist.end(); ++i)
            {
                Unit* unit = ObjectAccessor::GetUnit(*me, (*i)->getUnitGuid());
                if (unit && (unit->GetTypeId() == TYPEID_PLAYER))
                {
                    // Also needs an exception in spell system.
                    unit->CastSpell(unit, SPELL_GRAVITY_LAPSE_FLY, true, 0, 0, me->GetGUID());
                    unit->SetCanFly(true);
                }
            }
        }

        void RemoveGravityLapse()
        {
            ThreatContainer::StorageType threatlist = me->GetThreatManager().getThreatList();
            ThreatContainer::StorageType::const_iterator i = threatlist.begin();
            for (i = threatlist.begin(); i != threatlist.end(); ++i)
            {
                Unit* unit = ObjectAccessor::GetUnit(*me, (*i)->getUnitGuid());
                if (unit && (unit->GetTypeId() == TYPEID_PLAYER))
                {
                    unit->RemoveAurasDueToSpell(SPELL_GRAVITY_LAPSE_FLY);
                    unit->RemoveAurasDueToSpell(SPELL_GRAVITY_LAPSE_DOT);
                    unit->SetCanFly(false);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            switch (Phase)
            {
                case 0:
                {
                    // *Heroic mode only:
                    if (IsHeroic())
                    {
                        if (PyroblastTimer <= diff)
                        {
                            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                            me->InterruptSpell(CURRENT_GENERIC_SPELL);
                            DoCast(me, SPELL_SHOCK_BARRIER, true);
                            DoCastVictim(SPELL_PYROBLAST);
                            PyroblastTimer = 60000;
                        } else PyroblastTimer -= diff;
                    }

                    if (FireballTimer <= diff)
                    {
                        DoCastVictim(SPELL_FIREBALL_NORMAL);
                        FireballTimer = urand(2000, 6000);
                    } else FireballTimer -= diff;

                    if (PhoenixTimer <= diff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1);

                        uint8 random = urand(1, 2);
                        //float x = KaelLocations[random][0];
                        //float y = KaelLocations[random][1];

                       // Creature* Phoenix = me->SummonCreature(CREATURE_PHOENIX, x, y, LOCATION_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
                      //// if (Phoenix)
                      //  {
                           // Phoenix->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_NON_ATTACKABLE);
                           // SetThreatList(Phoenix);
                           // Phoenix->AI()->AttackStart(target);
                       // }

                        Talk(SAY_PHOENIX);

                        PhoenixTimer = 60000;
                    } else PhoenixTimer -= diff;

                    if (FlameStrikeTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                            me->InterruptSpell(CURRENT_GENERIC_SPELL);
                            DoCast(target, SPELL_FLAMESTRIKE3, true);
                            Talk(SAY_FLAMESTRIKE);
                        }
                        FlameStrikeTimer = urand(15000, 25000);
                    } else FlameStrikeTimer -= diff;

                    // Below 50%
                    if (HealthBelowPct(50))
                    {
                        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                        me->StopMoving();
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveIdle();
                        GravityLapseTimer = 0;
                        GravityLapsePhase = 0;
                        Phase = 1;
                    }

                    DoMeleeAttackIfReady();
                }
                break;

                case 1:
                {
                    if (GravityLapseTimer <= diff)
                    {
                        switch (GravityLapsePhase)
                        {
                            case 0:
                                if (FirstGravityLapse)          // Different yells at 50%, and at every following Gravity Lapse
                                {
                                    Talk(SAY_GRAVITY_LAPSE);
                                    FirstGravityLapse = false;

                                    instance->SetData(DATA_KAELTHAS_STATUES, 1);
                                }
                                else
                                    Talk(SAY_RECAST_GRAVITY);

                                DoCast(me, SPELL_GRAVITY_LAPSE_INITIAL);
                                GravityLapseTimer = 2000 + diff;// Don't interrupt the visual spell
                                GravityLapsePhase = 1;
                                break;
                            case 2:
                                CastGravityLapseKnockUp();
                                GravityLapseTimer = 1000;
                                GravityLapsePhase = 3;
                                break;

                            case 3:
                                CastGravityLapseFly();
                                GravityLapseTimer = 30000;
                                GravityLapsePhase = 4;

                                for (uint8 i = 0; i < 3; ++i)
                                {
                                    Unit* target = nullptr;
                                    target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                                    Creature* Orb = DoSpawnCreature(CREATURE_ARCANE_SPHERE, 5, 5, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                                    if (Orb && target)
                                    {
                                        Orb->SetSpeedRate(MOVE_RUN, 0.5f);
                                        AddThreat(target, 1000000.0f, Orb);
                                        Orb->AI()->AttackStart(target);
                                    }
                                }

                                DoCast(me, SPELL_GRAVITY_LAPSE_CHANNEL);
                                break;

                            case 4:
                                me->InterruptNonMeleeSpells(false);
                                Talk(SAY_TIRED);
                                DoCast(me, SPELL_POWER_FEEDBACK);
                                RemoveGravityLapse();
                                GravityLapseTimer = 10000;
                                GravityLapsePhase = 0;
                                break;
                        }
                    } else GravityLapseTimer -= diff;
                }
                break;
            }
        }
    };
};

void AddSC_boss_felblood()
{
    new boss_felblood();
}
