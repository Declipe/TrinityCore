/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "culling_of_stratholme.h"
#include "ScriptedEscortAI.h"
#include "PassiveAI.h"
#include "Player.h"
#include "SpellInfo.h"

class npc_arthas : public CreatureScript
{
    public:
        npc_arthas() : CreatureScript("npc_arthas") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
        {
            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            return false;
        }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_arthasAI>(creature);
        }

        struct npc_arthasAI : public ScriptedAI
        {
            npc_arthasAI(Creature* creature) : ScriptedAI(creature)
            {
            }
        };

};

enum CrateMisc
{
    GO_SUSPICIOUS_CRATE     = 190094,
    GO_PLAGUED_CRATE        = 190095,
    SPELL_ARCANE_DISRUPTION =  49590,
    SPELL_CRATES_CREDIT     =  58109
};
class npc_crate_helper : public CreatureScript
{
    public:
        npc_crate_helper() : CreatureScript("npc_create_helper_cot") { }

        struct npc_crate_helperAI : public StratholmeNPCAIWrapper<NullCreatureAI>
        {
            npc_crate_helperAI(Creature* creature) : StratholmeNPCAIWrapper<NullCreatureAI>(creature, CRATES_IN_PROGRESS), _crateRevealed(false) { }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
            {
                if (!_crateRevealed && spell->Id == SPELL_ARCANE_DISRUPTION)
                {
                    _crateRevealed = true;
                    if (InstanceScript* instance = me->GetInstanceScript())
                    {
                        instance->SetData(DATA_CRATE_REVEALED, 1);
                        if (GameObject* crate = me->FindNearestGameObject(GO_SUSPICIOUS_CRATE, 5.0f))
                        {
                            crate->SummonGameObject(GO_PLAGUED_CRATE, *crate, crate->GetWorldRotation(), DAY);
                            crate->Delete();
                        }
                        me->DespawnOrUnsummon(Seconds(1));
                    }
                }
            }
            void _DoAction(int32 /*action*/) override { }

        private:
            bool _crateRevealed;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_crate_helperAI>(creature);
        }
};

void AddSC_culling_of_stratholme()
{
    new npc_arthas();
    new npc_crate_helper();
}
