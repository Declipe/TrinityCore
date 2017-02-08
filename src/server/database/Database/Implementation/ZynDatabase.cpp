/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#include "ZynDatabase.h"

void ZynDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_ZYNDATABASE_STATEMENTS);

    //PrepareStatement(ZYN_UPD_CREATURE_FACTION, "UPDATE creature_templates SET faction = ? WHERE entry = ?", CONNECTION_ASYNC);
    //PrepareStatement(ZYN_UPD_CREATURE_NPCFLAG, "UPDATE creature_templates SET npcflag = ? WHERE entry = ?", CONNECTION_ASYNC);
    //PrepareStatement(ZYN_SEL_CREATURE_TEMPLATE, "SELECT entry, difficulty_entry_1, difficulty_entry_2, difficulty_entry_3, KillCredit1, KillCredit2, modelid1, modelid2, modelid3, modelid4, name, subname, IconName, gossip_menu_id, minlevel, maxlevel, exp, faction, npcflag, speed_walk, speed_run, scale, rank, dmgschool, BaseAttackTime, RangeAttackTime, BaseVariance, RangeVariance, unit_class, unit_flags, unit_flags2, dynamicflags, family, trainer_type, trainer_spell, trainer_class, trainer_race, type, type_flags, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, spell1, spell2, spell3, spell4, spell5, spell6, spell7, spell8, PetSpellDataId, VehicleId, mingold, maxgold, AIName, MovementType, InhabitType, HoverHeight, HealthModifier, ManaModifier, ArmorModifier, DamageModifier, ExperienceModifier, RacialLeader, movementId, RegenHealth, mechanic_immune_mask, flags_extra, ScriptName FROM creature_templates WHERE entry = ?", CONNECTION_SYNCH);
    PrepareStatement(Zyn_SEL_ITEM_TEMPLATE_BY_NAME, "SELECT entry FROM item_template WHERE name = ?", CONNECTION_SYNCH);

}
