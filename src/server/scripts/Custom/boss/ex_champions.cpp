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

#include "Custom/Dcl.h"
#include "CreatureAIImpl.h"

#define ExCrScriptName "ExCrScriptName"

enum Spells
{
    // druid healer
    SPELL_LIFEBLOOM                 = 66093,
    SPELL_NOURISH                   = 66066,
    SPELL_REGROWTH                  = 66067,
    SPELL_REJUVENATION              = 66065,
    SPELL_TRANQUILITY               = 66086,
    SPELL_BARKSKIN                  = 65860,
    SPELL_THORNS                    = 66068,
    SPELL_NATURE_GRASP              = 66071
};

enum Events
{

    // druid healer
    EVENT_LIFEBLOOM                 = 1,
    EVENT_NOURISH                   = 2,
    EVENT_REGROWTH                  = 3,
    EVENT_REJUVENATION              = 4,
    EVENT_TRANQUILITY               = 5,
    EVENT_HEAL_BARKSKIN             = 6,
    EVENT_THORNS                    = 7,
    EVENT_NATURE_GRASP              = 8
};


/********************************************************************
                            HEALERS
********************************************************************/
struct npc_dru : public BossAI
{
    npc_dru(Creature* creature) : BossAI(creature, 2) { }

    void Reset() override
    {
        BossAI::Reset();
        events.ScheduleEvent(EVENT_LIFEBLOOM, 5s, 15s);
        events.ScheduleEvent(EVENT_NOURISH, 5s, 15s);
        events.ScheduleEvent(EVENT_REGROWTH, 5s, 15s);
        events.ScheduleEvent(EVENT_REJUVENATION, 5s, 15s);
        events.ScheduleEvent(EVENT_TRANQUILITY, 5s, 20s);
        events.ScheduleEvent(EVENT_HEAL_BARKSKIN, 15s, 25s);
        events.ScheduleEvent(EVENT_THORNS, 2s);
        events.ScheduleEvent(EVENT_NATURE_GRASP, 3s, 20s);
        SetEquipmentSlots(false, 51799, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        BossAI::UpdateAI(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LIFEBLOOM:
                    if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_LIFEBLOOM);
                    events.ScheduleEvent(EVENT_LIFEBLOOM, 5s, 15s);
                    return;
                case EVENT_NOURISH:
                    if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_NOURISH);
                    events.ScheduleEvent(EVENT_NOURISH, 5s, 15s);
                    return;
                case EVENT_REGROWTH:
                    if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_REGROWTH);
                    events.ScheduleEvent(EVENT_REGROWTH, 5s, 15s);
                    return;
                case EVENT_REJUVENATION:
                    if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_REJUVENATION);
                    events.ScheduleEvent(EVENT_REJUVENATION, 5s, 15s);
                    return;
                case EVENT_TRANQUILITY:
                    DoCastAOE(SPELL_TRANQUILITY);
                    events.ScheduleEvent(EVENT_TRANQUILITY, 15s, 40s);
                    return;
                case EVENT_HEAL_BARKSKIN:
                    if (HealthBelowPct(30))
                    {
                        DoCast(me, SPELL_BARKSKIN);
                        events.RescheduleEvent(EVENT_HEAL_BARKSKIN, 60s);
                    }
                    else
                        events.RescheduleEvent(EVENT_HEAL_BARKSKIN, 3s);
                    return;
                case EVENT_NATURE_GRASP:
                    DoCast(me, SPELL_NATURE_GRASP);
                    events.ScheduleEvent(EVENT_NATURE_GRASP, 1min);
                    return;
                default:
                    return;
            }
        }
    }
};

template <class AI, class T>
inline AI* GetexchampionsAI(T* obj)
{
    return GetInstanceAI<AI>(obj, ExCrScriptName);
}

#define RegisterexchampionsAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetexchampionsAI)

void AddSC_ex_champions()
{
    RegisterexchampionsAI(npc_dru);
}
