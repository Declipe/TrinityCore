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

#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H

#include "Creature.h"

// Note: These are bitmask values to allow combining (spawn state masks on template AI), but only a single bit will ever be true in instance script
enum ProgressStates
{
    JUST_STARTED            = 0x0001, // dungeon just started, crate count not visible yet; pending chromie interaction
    CRATES_IN_PROGRESS      = 0x0002, // freshly started dungeon, players are revealing scourge crates
    CRATES_DONE             = 0x0004, // all crates revealed, chromie spawns at Stratholme entrance; waiting for player input to begin first RP event
    UTHER_TALK              = 0x0008, // RP event in progress, Uther+Arthas talking
    PURGE_PENDING           = 0x0010, // RP event done, pending player input to start wave event
    WAVES_IN_PROGRESS       = 0x0020, // first section is underway, players are battling waves
    WAVES_DONE              = 0x0040, // wave section completed; pending player input to begin Town Hall section
    TOWN_HALL               = 0x0080, // now escorting Arthas through Stratholme Town Hall
    TOWN_HALL_COMPLETE      = 0x0100, // Town Hall event complete, third boss defeated; pending player input to begin gauntlet transition
    GAUNTLET_TRANSITION     = 0x0200, // Arthas is leading players through the secret passage from Town Hall to the gauntlet
    GAUNTLET_PENDING        = 0x0400, // Pending player input to begin escorting Arthas through the final gauntlet section
    GAUNTLET_IN_PROGRESS    = 0x0800, // Arthas is being escorted through the gauntlet section
    GAUNTLET_COMPLETE       = 0x1000, // Arthas has reached the end of the gauntlet section; player input pending to begin Mal'ganis encounter
    MALGANIS_IN_PROGRESS    = 0x2000, // Arthas has moved into the final square and Mal'ganis encounter begins
    COMPLETE                = 0x4000, // Mal'ganis encounter is completed; dungeon over

    ALL                     = 0x7FFF
};

enum Data
{
    DATA_MEATHOOK,
    DATA_SALRAMM,
    DATA_EPOCH,
    DATA_MAL_GANIS,
    DATA_INFINITE_CORRUPTOR,
    NUM_BOSS_ENCOUNTERS,

    DATA_INSTANCE_PROGRESS = NUM_BOSS_ENCOUNTERS, // GET only
    DATA_GM_OVERRIDE,      // sent by chromie #1 in response to GM instance control commands
    DATA_CRATES_START,     // sent by chromie #1 creature script to initiate crate phase
    DATA_CRATE_REVEALED,   // sent by crate helper AI to trigger re-check of crate status
    DATA_UTHER_START,      // sent by chromie #2 creature script to initiate uther RP sequence
    DATA_SKIP_TO_PURGE,    // sent by chromie #1 creature script to skip straight to start of purge

    // old stuff below this, need to figure out if needed
    DATA_ARTHAS
};

// these are understood by all creatures using the instance AI; they are passed as negative values to avoid conflicts with creature script specific actions
enum InstanceActions
{
    ACTION_CHECK_DESPAWN = 1
};

enum InstanceMisc
{
    WORLDSTATE_SHOW_CRATES = 3479,
    WORLDSTATE_CRATES_REVEALED = 3480,
    WORLDSTATE_WAVE_COUNT = 3504,
    WORLDSTATE_TIME_GUARDIAN = 3931,
    WORLDSTATE_TIME_GUARDIAN_SHOW = 3932
};

// These methods are implemented in instance_culling_of_stratholme.cpp so they can access internals of the instance script
void StratholmeAIHello(InstanceScript* /*instance*/, ObjectGuid const& /*me*/, ProgressStates /*myStates*/);
void StratholmeAIGoodbye(InstanceScript* /*instance*/, ObjectGuid const& /*me*/, ProgressStates /*myStates*/);

// Note: instance script is not nullptr checked in this AI - ONLY use this with GetInstanceAI in script AI getter!
template <class ParentAI>
class StratholmeNPCAIWrapper : public ParentAI
{
    public:
        StratholmeNPCAIWrapper(Creature* creature, ProgressStates stateMask) : ParentAI(creature), instance(creature->GetInstanceScript()), _statesMask(stateMask)
        {
            StratholmeAIHello(instance, me->GetGUID(), _statesMask);
            CheckDespawn();
        }
        ~StratholmeNPCAIWrapper()
        {
            StratholmeAIGoodbye(instance, me->GetGUID(), _statesMask);
        }

        void CheckDespawn()
        {
            if (!(_statesMask & instance->GetData(DATA_INSTANCE_PROGRESS)))
                me->DespawnOrUnsummon(0, Seconds(1));
        }

        virtual void _DoAction(int32 /*action*/) { }
        void DoAction(int32 action) final override
        {
            switch (action)
            {
                case -ACTION_CHECK_DESPAWN:
                    CheckDespawn();
                    break;
                default:
                    _DoAction(action);
            }
        }

        bool CanRespawn() override
        {
            return (_statesMask & instance->GetData(DATA_INSTANCE_PROGRESS)) ? true : false;
        }
    protected:
        InstanceScript* const instance;
    private:
        ProgressStates const _statesMask;
};
#endif
