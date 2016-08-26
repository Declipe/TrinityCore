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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureTextMgr.h"
#include "culling_of_stratholme.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "SpellInfo.h"

/* Culling of Stratholme encounters:
0 - Meathook
1 - Salramm the Fleshcrafter
2 - Chrono-Lord Epoch
3 - Mal'Ganis
4 - Infinite Corruptor (Heroic only)
*/

enum Events
{
    EVENT_GUARDIAN_TICK = 1
};

enum Entries
{
    NPC_CRATE_HELPER    =  27827,
    GO_MALGANIS_GATE_2  = 187723,
    GO_EXIT_GATE        = 191788
};

enum Misc
{
    NUM_PLAGUE_CRATES   =  5,
    NUM_SCOURGE_WAVES   = 10,
    WAVE_MEATHOOK       =  5,
    WAVE_SALRAMM        = 10
};

Position const ChromieSummonPos[] =
{
    { 1813.298f, 1283.578f, 142.3258f, 3.878161f },
    { 2273.725f, 1483.684f, 128.7205f, 6.057528f }
};

Position const InfiniteCorruptorPos = { 2335.47f, 1262.04f, 132.921f, 1.42079f };
Position const TimeRiftPos = { 2334.626f, 1280.45f, 133.0066f, 1.727876f };
Position const GuardianOfTimePos = { 2321.489f, 1268.383f, 132.8507f, 0.418879f };

DoorData const doorData[] =
{
    { GO_MALGANIS_GATE_2, DATA_MAL_GANIS, DOOR_TYPE_ROOM },
    { GO_EXIT_GATE,       DATA_MAL_GANIS, DOOR_TYPE_PASSAGE },
    { 0,                  0,              DOOR_TYPE_ROOM } // END
};

class instance_culling_of_stratholme : public InstanceMapScript
{
    public:
        instance_culling_of_stratholme() : InstanceMapScript("instance_culling_of_stratholme", 595) { }

        struct instance_culling_of_stratholme_InstanceMapScript : public InstanceScript
        {
            instance_culling_of_stratholme_InstanceMapScript(Map* map) : InstanceScript(map), _currentState(JUST_STARTED), _infiniteGuardianTimeout(0)
            {
                SetHeaders("CS");
                SetBossNumber(NUM_BOSS_ENCOUNTERS);
                LoadDoorData(doorData);

                _plagueCrates.reserve(NUM_PLAGUE_CRATES);
            }

            void FillInitialWorldStates(WorldPacket& data) override
            {
                std::cout << "FillInitialWorldStates" << std::endl;
                if (_currentState & (CRATES_IN_PROGRESS | CRATES_DONE))
                {
                    data << uint32(WORLDSTATE_SHOW_CRATES) << uint32(1);
                    data << uint32(WORLDSTATE_CRATES_REVEALED) << uint32(missingPlagueCrates());
                }
                else
                {
                    data << uint32(WORLDSTATE_SHOW_CRATES) << uint32(0);
                    data << uint32(WORLDSTATE_CRATES_REVEALED) << uint32(_currentState == JUST_STARTED ? 0 : NUM_PLAGUE_CRATES);
                }

                data << uint32(WORLDSTATE_WAVE_COUNT) << uint32(0);

                if (_infiniteGuardianTimeout)
                {
                    data << uint32(WORLDSTATE_TIME_GUARDIAN_SHOW) << uint32(1);
                    data << uint32(WORLDSTATE_TIME_GUARDIAN) << uint32((_infiniteGuardianTimeout-time(NULL)+MINUTE-1) / MINUTE);
                }
                else
                {
                    data << uint32(WORLDSTATE_TIME_GUARDIAN_SHOW) << uint32(0);
                    data << uint32(WORLDSTATE_TIME_GUARDIAN) << uint32(0);
                }
            }

            void WriteSaveDataMore(std::ostringstream& data) override
            {
                data << _currentState << ' ' << _infiniteGuardianTimeout;
            }

            void ReadSaveDataMore(std::istringstream& data) override
            {
                // read current instance progress from save data, then regress to the previous stable state
                uint32 state = JUST_STARTED, loadState;
                time_t infiniteGuardianTime = 0;
                data >> state;
                data >> infiniteGuardianTime; // UNIX timestamp
                switch(state)
                {
                    case JUST_STARTED:
                    default:
                        loadState = JUST_STARTED;
                        break;
                    case CRATES_IN_PROGRESS:
                        loadState = CRATES_IN_PROGRESS;
                        break;
                    case CRATES_DONE:
                        loadState = CRATES_DONE;
                        break;
                    case UTHER_TALK:
                    case PURGE_PENDING:
                    case WAVES_IN_PROGRESS:
                        loadState = PURGE_PENDING;
                        break;
                    case WAVES_DONE:
                    case TOWN_HALL:
                        loadState = WAVES_DONE;
                        break;
                    case TOWN_HALL_COMPLETE:
                    case GAUNTLET_TRANSITION:
                    case GAUNTLET_PENDING:
                    case GAUNTLET_IN_PROGRESS:
                        loadState = GAUNTLET_PENDING;
                        break;
                    case GAUNTLET_COMPLETE:
                    case MALGANIS_IN_PROGRESS:
                        loadState = GAUNTLET_COMPLETE;
                        break;
                }

                SetInstanceProgress(ProgressStates(loadState));
                
                if (infiniteGuardianTime)
                {
                    _infiniteGuardianTimeout = infiniteGuardianTime;
                    events.ScheduleEvent(EVENT_GUARDIAN_TICK, Seconds(0));
                }

                time_t timediff = (infiniteGuardianTime - time(NULL));
                std::cout << "Loaded with state " << (uint32)loadState << " and guardian timeout at " << timediff/MINUTE << " minutes " << timediff%MINUTE << " seconds  from now." << std::endl;
            }

            void SetData(uint32 type, uint32 data) override
            {
                switch (type)
                {
                    case DATA_CRATES_START:
                        if (_currentState == JUST_STARTED)
                            SetInstanceProgress(CRATES_IN_PROGRESS);
                        break;
                    case DATA_CRATE_REVEALED:
                        if (uint32 missingCrates = missingPlagueCrates())
                            DoUpdateWorldState(WORLDSTATE_CRATES_REVEALED, NUM_PLAGUE_CRATES - missingCrates);
                        else
                            SetInstanceProgress(CRATES_DONE);
                        break;
                    case DATA_SKIP_TO_PURGE:
                        if (_currentState <= CRATES_DONE)
                            SetInstanceProgress(PURGE_PENDING);
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type) const override
            {
                if (type == DATA_INSTANCE_PROGRESS)
                    return _currentState;
                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                return true;
            }

            void SetInstanceProgress(ProgressStates state)
            {
                std::cout << "Instance progress is now " << (uint32)state << std::endl;
                ProgressStates oldState = _currentState;
                _currentState = state;
                if (oldState)
                    for (ObjectGuid const& guid : _myCreatures[oldState])
                        if (Creature* creature = instance->GetCreature(guid))
                            creature->AI()->DoAction(-ACTION_CHECK_DESPAWN);

                switch (state)
                {
                    case CRATES_IN_PROGRESS:
                        DoUpdateWorldState(WORLDSTATE_SHOW_CRATES, 1);
                        DoUpdateWorldState(WORLDSTATE_CRATES_REVEALED, missingPlagueCrates());
                        break;
                    case CRATES_DONE:
                        DoUpdateWorldState(WORLDSTATE_CRATES_REVEALED, NUM_PLAGUE_CRATES);
                        break;
                }
            }

            void Update(uint32 diff) override
            {
                if (!_infiniteGuardianTimeout && GetBossState(DATA_INFINITE_CORRUPTOR) != FAIL)
                {
                    _infiniteGuardianTimeout = time(NULL) + 10 * MINUTE;
                    DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN_SHOW, 1);
                    events.ScheduleEvent(EVENT_GUARDIAN_TICK, Seconds(0));
                }
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GUARDIAN_TICK:
                        {
                            time_t secondsToGuardianDeath = _infiniteGuardianTimeout - time(NULL);
                            std::cout << secondsToGuardianDeath << std::endl;
                            if (secondsToGuardianDeath <= 0)
                            {
                                _infiniteGuardianTimeout = 0;
                                DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN_SHOW, 0);
                                DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN, 0);
                                SetBossState(DATA_INFINITE_CORRUPTOR, FAIL);
                            }
                            else
                            {
                                DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN, (secondsToGuardianDeath + (MINUTE - 1)) / MINUTE);
                                events.Repeat(Seconds((secondsToGuardianDeath % MINUTE) + 1));
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case NPC_CRATE_HELPER:
                        _plagueCrates.push_back(creature->GetGUID());
                        break;
                    default:
                        break;
                }
            }

            void CreatureAIHello(ObjectGuid const& me, ProgressStates myStates)
            {
                if (Creature* c = instance->GetCreature(me))
                    std::cout << "Hello: " << c->GetName() << std::endl;
                
                ProgressStates curState = JUST_STARTED;
                while (curState <= COMPLETE)
                {
                    if (myStates & curState)
                        _myCreatures[curState].insert(me);
                    curState = ProgressStates(curState << 1);
                }
            }

            void CreatureAIGoodbye(ObjectGuid const& me, ProgressStates myStates)
            {
                if (Creature* c = instance->GetCreature(me))
                    std::cout << "Goodbye: " << c->GetName() << std::endl;
                
                ProgressStates curState = JUST_STARTED;
                while (curState <= COMPLETE)
                {
                    if (myStates & curState)
                        _myCreatures[curState].erase(me);
                    curState = ProgressStates(curState << 1);
                }
            }

        private:
            EventMap events;
            ProgressStates _currentState;
            time_t _infiniteGuardianTimeout;
            std::map<ProgressStates, std::set<ObjectGuid>> _myCreatures;

            std::vector<ObjectGuid> _plagueCrates;
            uint32 missingPlagueCrates() const
            {
                uint32 num = 0;
                for (ObjectGuid crateHelperGUID : _plagueCrates)
                    if (Creature* crateHelper = instance->GetCreature(crateHelperGUID))
                        if (crateHelper->IsAlive() && !crateHelper->AI()->GetData(DATA_CRATE_REVEALED))
                            ++num;
                return num;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_culling_of_stratholme_InstanceMapScript(map);
        }
};

void StratholmeAIHello(InstanceScript* instance, ObjectGuid const& me, ProgressStates myStates)
{
    reinterpret_cast<instance_culling_of_stratholme::instance_culling_of_stratholme_InstanceMapScript*>(instance)->CreatureAIHello(me, myStates);

}
void StratholmeAIGoodbye(InstanceScript* instance, ObjectGuid const& me, ProgressStates myStates)
{
    reinterpret_cast<instance_culling_of_stratholme::instance_culling_of_stratholme_InstanceMapScript*>(instance)->CreatureAIGoodbye(me, myStates);
}

void AddSC_instance_culling_of_stratholme()
{
    new instance_culling_of_stratholme();
}
