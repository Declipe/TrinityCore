/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
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

/* Script Data Start
SDName: Boss mal_ganis
SDAuthor: Tartalo
SD%Complete: 80
SDComment: @todo Intro & outro
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "culling_of_stratholme.h"
#include "Player.h"

enum Spells
{
    SPELL_CARRION_SWARM                         = 52720, //A cresting wave of chaotic magic splashes over enemies in front of the caster, dealing 3230 to 3570 Shadow damage and 380 to 420 Shadow damage every 3 sec. for 15 sec.
    SPELL_MIND_BLAST                            = 52722, //Inflicts 4163 to 4837 Shadow damage to an enemy.
    SPELL_SLEEP                                 = 52721, //Puts an enemy to sleep for up to 10 sec. Any damage caused will awaken the target.
    SPELL_VAMPIRIC_TOUCH                        = 52723, //Heals the caster for half the damage dealt by a melee attack.
    SPELL_MAL_GANIS_KILL_CREDIT                 = 58124, // Quest credit
    SPELL_KILL_CREDIT                           = 58630  // Non-existing spell as encounter credit, created in spell_dbc
};

enum Yells
{
    SAY_KILL                                    = 3,
    SAY_SLAY                                    = 4,
    SAY_SLEEP                                   = 5,
    SAY_30HEALTH                                = 6,
    SAY_15HEALTH                                = 7,
};

class boss_mal_ganis : public CreatureScript
{
    public:
        boss_mal_ganis() : CreatureScript("boss_mal_ganis") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                if (instance->GetData(DATA_INSTANCE_PROGRESS) < MALGANIS_IN_PROGRESS)
                    return new NullCreatureAI(creature);
                else
                    return new boss_mal_ganisAI(creature);
            }
            else
                return nullptr;
        }

        struct boss_mal_ganisAI : public BossAI
        {
            boss_mal_ganisAI(Creature* creature) : BossAI(creature, DATA_MAL_GANIS), defeated(false)
            {
                Initialize();
            }

            void Initialize()
            {
                bYelled = false;
                bYelled2 = false;
                uiCarrionSwarmTimer = 6000;
                uiMindBlastTimer = 11000;
                uiVampiricTouchTimer = urand(10000, 15000);
                uiSleepTimer = urand(15000, 20000);
            }

            uint32 uiCarrionSwarmTimer;
            uint32 uiMindBlastTimer;
            uint32 uiVampiricTouchTimer;
            uint32 uiSleepTimer;

            bool bYelled;
            bool bYelled2;

            void Reset() override
            {
                if (defeated)
                    return;
                Initialize();
                instance->SetBossState(DATA_MAL_GANIS, NOT_STARTED);
            }

            void DamageTaken(Unit* source, uint32 &damage) override
            {
                if (damage >= me->GetHealth())
                {
                    damage = me->GetHealth()-1;
                    defeated = true;
                    if (InstanceMap* map = instance->instance->ToInstanceMap())
                        if (map->IsHeroic())
                        {
                            Player* sourcePlayer = source->GetCharmerOrOwnerPlayerOrPlayerItself();
                            if (!sourcePlayer)
                            {
                                MapRefManager const& players = map->GetPlayers();
                                // Find any player with a valid instance state
                                for (MapRefManager::const_iterator it = players.begin(); !sourcePlayer && it != players.end(); ++it)
                                {
                                    Player* p = it->GetSource();
                                    if (p && !p->IsGameMaster() && p->m_InstanceValid)
                                        sourcePlayer = p;
                                }
                                // Fall back to any player, regardless of instance state
                                for (MapRefManager::const_iterator it = players.begin(); !sourcePlayer && it != players.end(); ++it)
                                {
                                    Player* p = it->GetSource();
                                    if (p && !p->IsGameMaster())
                                        sourcePlayer = p;
                                }
                            }

                            if (sourcePlayer)
                                map->PermBindAllPlayers(sourcePlayer);
                        }
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (defeated)
                {
                    if (me->IsInCombat())
                    {
                        EnterEvadeMode();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                    }
                    return;
                }

                if (!UpdateVictim())
                    return;

                if (!bYelled && HealthBelowPct(30))
                {
                    Talk(SAY_30HEALTH);
                    bYelled = true;
                }

                if (!bYelled2 && HealthBelowPct(15))
                {
                    Talk(SAY_15HEALTH);
                    bYelled2 = true;
                }

                if (uiCarrionSwarmTimer < diff)
                {
                    DoCastVictim(SPELL_CARRION_SWARM);
                    uiCarrionSwarmTimer = 7000;
                } else uiCarrionSwarmTimer -= diff;

                if (uiMindBlastTimer < diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_MIND_BLAST);
                    uiMindBlastTimer = 6000;
                } else uiMindBlastTimer -= diff;

                if (uiVampiricTouchTimer < diff)
                {
                    DoCast(me, SPELL_VAMPIRIC_TOUCH);
                    uiVampiricTouchTimer = 32000;
                } else uiVampiricTouchTimer -= diff;

                if (uiSleepTimer < diff)
                {
                    Talk(SAY_SLEEP);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_SLEEP);
                    uiSleepTimer = urand(15000, 20000);
                } else uiSleepTimer -= diff;

                DoMeleeAttackIfReady();
            }

            void KilledUnit(Unit* victim) override
            {
                if (!defeated && victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            bool defeated;
        };

};

void AddSC_boss_mal_ganis()
{
    new boss_mal_ganis();
}
