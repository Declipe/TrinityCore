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

#include "culling_of_stratholme.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"

class npc_arthas_stratholme : public CreatureScript
{
    public:
    npc_arthas_stratholme() : CreatureScript("npc_arthas_stratholme") { }

    struct npc_arthas_stratholmeAI : public ScriptedAI
    {
        static const float _snapbackDistanceThreshold; // how far we can be from where we're supposed at start of phase to be before we snap back
        static const std::map<ProgressStates, Position> _snapbackPositions; // positions we should be at when starting a given phase
        npc_arthas_stratholmeAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }
        void AdvanceToPosition(ProgressStates newState)
        {
            std::map<ProgressStates, Position>::const_iterator it = _snapbackPositions.find(newState);
            if (it != _snapbackPositions.end())
            {
                Position const& target = it->second;
                std::cout << "arthas: have snapback, " << target.GetExactDist(me) << "yd away from me" << std::endl;
                if (target.GetExactDist(me) > _snapbackDistanceThreshold)
                    me->NearTeleportTo(target);
            }
            else
                std::cout << "arthas: no snapback for state " << uint32(newState) << std::endl;
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case -ACTION_PROGRESS_UPDATE:
                    AdvanceToPosition(ProgressStates(instance->GetData(DATA_INSTANCE_PROGRESS)));
                    break;
            }
        }

        private:
        InstanceScript* const instance;
    };

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
        return GetInstanceAI<npc_arthas_stratholmeAI>(creature);
    }
};
const float npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackDistanceThreshold = 10.0f;
const std::map<ProgressStates, Position> npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackPositions = {
    { COMPLETE, { 1813.298f, 1283.578f, 142.3258f, 3.878161f } }
};

void AddSC_npc_arthas_stratholme()
{
    new npc_arthas_stratholme();
}
