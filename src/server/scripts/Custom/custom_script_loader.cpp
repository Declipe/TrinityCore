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

// This is where scripts' loading functions should be declared:
void AddSC_ZynPlayerScripts();
void AddSC_level_award();
void AddSC_npc_teleport();
//GuildHouse NPC
void AddSC_guildmaster();
// 1v1 Arena
void AddSC_npc_1v1arena();
// VIP
void AddSC_premium_commandscript();
void AddSC_test();
void AddSC_gm_hello();
void AddSC_Mod_HonorRank();
void AddSC_Mod_BossHistory();
void AddSC_npc_title();
void AddSC_Mod_GuildDefault();
void AddSC_npc_hunterpetvendor();
void AddSC_Mod_Tournaments();
void AddSC_lighthardmode();
void AddSC_orrig();
void AddSC_light();
void AddSC_boss_two();
void AddSC_exitare();
void AddSC_eonar();
void AddSC_anna();
void AddSC_event_npc();
void AddSC_Resets();
void AddSC_Mod_ItemUpgrade();
void AddSC_Mod_AutoLearn();
void AddSC_Transmogrification();
void AddSC_mod_DefaultSpec();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
    // GuildHouse NPC
AddSC_guildmaster();
// 1v1 Arena
AddSC_npc_1v1arena();
// VIP
AddSC_premium_commandscript();
AddSC_ZynPlayerScripts();
AddSC_test();
AddSC_gm_hello();
AddSC_Mod_HonorRank();
AddSC_Mod_BossHistory();
AddSC_npc_title();
AddSC_Mod_GuildDefault();
AddSC_npc_hunterpetvendor();
AddSC_lighthardmode();
AddSC_orrig();
AddSC_light();
AddSC_boss_two();
AddSC_exitare();
AddSC_eonar();
AddSC_anna();
AddSC_event_npc();
AddSC_Resets();
AddSC_Mod_Tournaments();
AddSC_Mod_ItemUpgrade();
AddSC_Mod_AutoLearn();
AddSC_npc_teleport();
AddSC_Transmogrification();
AddSC_mod_DefaultSpec();
AddSC_level_award();
}
