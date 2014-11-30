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

 /* ScriptData
 Name: reload_commandscript
 %Complete: 100
 Comment: All reload related commands
 Category: commandscripts
 EndScriptData */

#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "AchievementMgr.h"
#include "AuctionHouseMgr.h"
#include "BattlegroundMgr.h"
#include "Chat.h"
#include "CreatureTextMgr.h"
#include "DatabaseEnv.h"
#include "DisableMgr.h"
#include "ItemEnchantmentMgr.h"
#include "Language.h"
#include "LFGMgr.h"
#include "Log.h"
#include "LootMgr.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "SmartAI.h"
#include "SpellMgr.h"
#include "StringConvert.h"
#include "TicketMgr.h"
#include "WaypointManager.h"
#include "World.h"
#include "ItemShopMgr.h"
#include "PromotionCodeMgr.h"

#if TRINITY_COMPILER == TRINITY_COMPILER_GNU
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

using namespace Trinity::ChatCommands;

class reload_commandscript : public CommandScript
{
public:
    reload_commandscript() : CommandScript("reload_commandscript") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable reloadAllCommandTable =
        {
            { "achievement",                   HandleReloadAllAchievementCommand,             rbac::RBAC_PERM_COMMAND_RELOAD_ALL_ACHIEVEMENT, Console::Yes },
            { "area",                          HandleReloadAllAreaCommand,                    rbac::RBAC_PERM_COMMAND_RELOAD_ALL_AREA, Console::Yes },
            { "gossips",                       HandleReloadAllGossipsCommand,                 rbac::RBAC_PERM_COMMAND_RELOAD_ALL_GOSSIP, Console::Yes },
            { "item",                          HandleReloadAllItemCommand,                    rbac::RBAC_PERM_COMMAND_RELOAD_ALL_ITEM, Console::Yes },
            { "locales",                       HandleReloadAllLocalesCommand,                 rbac::RBAC_PERM_COMMAND_RELOAD_ALL_LOCALES, Console::Yes },
            { "loot",                          HandleReloadAllLootCommand,                    rbac::RBAC_PERM_COMMAND_RELOAD_ALL_LOOT, Console::Yes },
            { "npc",                           HandleReloadAllNpcCommand,                     rbac::RBAC_PERM_COMMAND_RELOAD_ALL_NPC, Console::Yes },
            { "quest",                         HandleReloadAllQuestCommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_ALL_QUEST, Console::Yes },
            { "scripts",                       HandleReloadAllScriptsCommand,                 rbac::RBAC_PERM_COMMAND_RELOAD_ALL_SCRIPTS, Console::Yes },
            { "spell",                         HandleReloadAllSpellCommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_ALL_SPELL, Console::Yes },
            { "",                              HandleReloadAllCommand,                        rbac::RBAC_PERM_COMMAND_RELOAD_ALL, Console::Yes },
        };

        static ChatCommandTable  reloadCommandTable =
        {
            { "auctions",                      HandleReloadAuctionsCommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_AUCTIONS, Console::Yes },
            { "access_requirement",            HandleReloadAccessRequirementCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_ACCESS_REQUIREMENT, Console::Yes},
            { "achievement_criteria_data",     HandleReloadAchievementCriteriaDataCommand,    rbac::RBAC_PERM_COMMAND_RELOAD_ACHIEVEMENT_CRITERIA_DATA, Console::Yes },
            { "achievement_reward",            HandleReloadAchievementRewardCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_ACHIEVEMENT_REWARD, Console::Yes },
            { "all", reloadAllCommandTable },
            { "areatrigger_involvedrelation",  HandleReloadQuestAreaTriggersCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_AREATRIGGER_INVOLVEDRELATION, Console::Yes },
            { "areatrigger_tavern",            HandleReloadAreaTriggerTavernCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_AREATRIGGER_TAVERN, Console::Yes },
            { "areatrigger_teleport",          HandleReloadAreaTriggerTeleportCommand,        rbac::RBAC_PERM_COMMAND_RELOAD_AREATRIGGER_TELEPORT, Console::Yes },
            { "autobroadcast",                 HandleReloadAutobroadcastCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_AUTOBROADCAST, Console::Yes },
            { "creature_summon_groups",        HandleReloadCreatureSummonGroupsCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_SUMMON_GROUPS, Console::Yes},
            { "broadcast_text",                HandleReloadBroadcastTextCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_BROADCAST_TEXT, Console::Yes },
            { "battleground_template",         HandleReloadBattlegroundTemplate,              rbac::RBAC_PERM_COMMAND_RELOAD_BATTLEGROUND_TEMPLATE, Console::Yes },
            { "conditions",                    HandleReloadConditions,                        rbac::RBAC_PERM_COMMAND_RELOAD_CONDITIONS, Console::Yes },
            { "config",                        HandleReloadConfigCommand,                     rbac::RBAC_PERM_COMMAND_RELOAD_CONFIG, Console::Yes },
            { "creature_text",                 HandleReloadCreatureText,                      rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_TEXT, Console::Yes },
            { "creature_questender",           HandleReloadCreatureQuestEnderCommand,         rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_QUESTENDER, Console::Yes },
            { "creature_linked_respawn",       HandleReloadLinkedRespawnCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_LINKED_RESPAWN, Console::Yes },
            { "creature_loot_template",        HandleReloadLootTemplatesCreatureCommand,      rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_LOOT_TEMPLATE, Console::Yes },
            { "creature_movement_override",    HandleReloadCreatureMovementOverrideCommand,    rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_MOVEMENT_OVERRIDE, Console::Yes},
            { "creature_onkill_reputation",    HandleReloadOnKillReputationCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_ONKILL_REPUTATION, Console::Yes },
            { "creature_queststarter",         HandleReloadCreatureQuestStarterCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_QUESTSTARTER, Console::Yes },
            { "creature_template",             HandleReloadCreatureTemplateCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_TEMPLATE, Console::Yes },
            { "disables",                      HandleReloadDisablesCommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_DISABLES, Console::Yes },
            { "disenchant_loot_template",      HandleReloadLootTemplatesDisenchantCommand,    rbac::RBAC_PERM_COMMAND_RELOAD_DISENCHANT_LOOT_TEMPLATE, Console::Yes },
            { "event_scripts",                 HandleReloadEventScriptsCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_EVENT_SCRIPTS, Console::Yes },
            { "fishing_loot_template",         HandleReloadLootTemplatesFishingCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_FISHING_LOOT_TEMPLATE, Console::Yes },
            { "graveyard_zone",                HandleReloadGameGraveyardZoneCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_GRAVEYARD_ZONE, Console::Yes },
            { "game_tele",                     HandleReloadGameTeleCommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_GAME_TELE, Console::Yes },
            { "gameobject_questender",         HandleReloadGOQuestEnderCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_GAMEOBJECT_QUESTENDER, Console::Yes },
            { "gameobject_loot_template",      HandleReloadLootTemplatesGameobjectCommand,    rbac::RBAC_PERM_COMMAND_RELOAD_GAMEOBJECT_QUEST_LOOT_TEMPLATE, Console::Yes },
            { "gameobject_queststarter",       HandleReloadGOQuestStarterCommand,             rbac::RBAC_PERM_COMMAND_RELOAD_GAMEOBJECT_QUESTSTARTER, Console::Yes },
            { "gm_tickets",                    HandleReloadGMTicketsCommand,                  rbac::RBAC_PERM_COMMAND_RELOAD_GM_TICKETS, Console::Yes },
            { "gossip_menu",                   HandleReloadGossipMenuCommand,                 rbac::RBAC_PERM_COMMAND_RELOAD_GOSSIP_MENU, Console::Yes },
            { "gossip_menu_option",            HandleReloadGossipMenuOptionCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_GOSSIP_MENU_OPTION, Console::Yes },
            { "item_enchantment_template",     HandleReloadItemEnchantementsCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_ITEM_ENCHANTMENT_TEMPLATE, Console::Yes },
            { "item_loot_template",            HandleReloadLootTemplatesItemCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_ITEM_LOOT_TEMPLATE, Console::Yes },
            { "item_set_names",                HandleReloadItemSetNamesCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_ITEM_SET_NAMES, Console::Yes },
            { "lfg_dungeon_rewards",           HandleReloadLfgRewardsCommand,                 rbac::RBAC_PERM_COMMAND_RELOAD_LFG_DUNGEON_REWARDS, Console::Yes },
            { "achievement_reward_locale",     HandleReloadLocalesAchievementRewardCommand,   rbac::RBAC_PERM_COMMAND_RELOAD_ACHIEVEMENT_REWARD_LOCALE, Console::Yes },
            { "creature_template_locale",      HandleReloadLocalesCreatureCommand,            rbac::RBAC_PERM_COMMAND_RELOAD_CRETURE_TEMPLATE_LOCALE, Console::Yes },
            { "creature_text_locale",          HandleReloadLocalesCreatureTextCommand,        rbac::RBAC_PERM_COMMAND_RELOAD_CRETURE_TEXT_LOCALE, Console::Yes },
            { "gameobject_template_locale",    HandleReloadLocalesGameobjectCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_GAMEOBJECT_TEMPLATE_LOCALE, Console::Yes },
            { "gossip_menu_option_locale",     HandleReloadLocalesGossipMenuOptionCommand,    rbac::RBAC_PERM_COMMAND_RELOAD_GOSSIP_MENU_OPTION_LOCALE, Console::Yes },
            { "item_template_locale",          HandleReloadLocalesItemCommand,                rbac::RBAC_PERM_COMMAND_RELOAD_ITEM_TEMPLATE_LOCALE, Console::Yes },
            { "item_set_name_locale",          HandleReloadLocalesItemSetNameCommand,         rbac::RBAC_PERM_COMMAND_RELOAD_ITEM_SET_NAME_LOCALE, Console::Yes },
            { "npc_text_locale",               HandleReloadLocalesNpcTextCommand,             rbac::RBAC_PERM_COMMAND_RELOAD_NPC_TEXT_LOCALE, Console::Yes },
            { "page_text_locale",              HandleReloadLocalesPageTextCommand,            rbac::RBAC_PERM_COMMAND_RELOAD_PAGE_TEXT_LOCALE, Console::Yes },
            { "points_of_interest_locale",     HandleReloadLocalesPointsOfInterestCommand,    rbac::RBAC_PERM_COMMAND_RELOAD_POINTS_OF_INTEREST_LOCALE, Console::Yes },
            { "quest_template_locale",         HandleReloadLocalesQuestCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_QUEST_TEMPLATE_LOCALE, Console::Yes },
            { "mail_level_reward",             HandleReloadMailLevelRewardCommand,            rbac::RBAC_PERM_COMMAND_RELOAD_MAIL_LEVEL_REWARD, Console::Yes },
            { "mail_loot_template",            HandleReloadLootTemplatesMailCommand,          rbac::RBAC_PERM_COMMAND_RELOAD_MAIL_LOOT_TEMPLATE, Console::Yes },
            { "mail_server_template",          HandleReloadMailServerTemplateCommand,         rbac::RBAC_PERM_COMMAND_RELOAD_MAIL_LOOT_TEMPLATE, Console::Yes },
            { "milling_loot_template",         HandleReloadLootTemplatesMillingCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_MILLING_LOOT_TEMPLATE, Console::Yes },
            { "npc_spellclick_spells",         HandleReloadSpellClickSpellsCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_NPC_SPELLCLICK_SPELLS, Console::Yes },
            { "npc_vendor",                    HandleReloadNpcVendorCommand,                  rbac::RBAC_PERM_COMMAND_RELOAD_NPC_VENDOR, Console::Yes },
            { "quest_greeting_locale",         HandleReloadLocalesQuestGreetingCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_QUEST_GREETING_LOCALE, Console::Yes },
            { "page_text",                     HandleReloadPageTextsCommand,                  rbac::RBAC_PERM_COMMAND_RELOAD_PAGE_TEXT, Console::Yes },
            { "pickpocketing_loot_template",   HandleReloadLootTemplatesPickpocketingCommand, rbac::RBAC_PERM_COMMAND_RELOAD_PICKPOCKETING_LOOT_TEMPLATE, Console::Yes },
            { "points_of_interest",            HandleReloadPointsOfInterestCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_POINTS_OF_INTEREST, Console::Yes },
            { "prospecting_loot_template",     HandleReloadLootTemplatesProspectingCommand,   rbac::RBAC_PERM_COMMAND_RELOAD_PROSPECTING_LOOT_TEMPLATE, Console::Yes },
            { "quest_greeting",                HandleReloadQuestGreetingCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_QUEST_GREETING, Console::Yes },
            { "quest_poi",                     HandleReloadQuestPOICommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_QUEST_POI, Console::Yes },
            { "quest_template",                HandleReloadQuestTemplateCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_QUEST_TEMPLATE, Console::Yes },
            { "rbac",                          HandleReloadRBACCommand,                       rbac::RBAC_PERM_COMMAND_RELOAD_RBAC, Console::Yes },
            { "reference_loot_template",       HandleReloadLootTemplatesReferenceCommand,     rbac::RBAC_PERM_COMMAND_RELOAD_REFERENCE_LOOT_TEMPLATE, Console::Yes },
            { "reserved_name",                 HandleReloadReservedNameCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_RESERVED_NAME, Console::Yes },
            { "reputation_reward_rate",        HandleReloadReputationRewardRateCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_REPUTATION_REWARD_RATE, Console::Yes },
            { "reputation_spillover_template", HandleReloadReputationRewardRateCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_SPILLOVER_TEMPLATE, Console::Yes },
            { "skill_discovery_template",      HandleReloadSkillDiscoveryTemplateCommand,     rbac::RBAC_PERM_COMMAND_RELOAD_SKILL_DISCOVERY_TEMPLATE, Console::Yes },
            { "skill_extra_item_template",     HandleReloadSkillExtraItemTemplateCommand,     rbac::RBAC_PERM_COMMAND_RELOAD_SKILL_EXTRA_ITEM_TEMPLATE, Console::Yes },
            { "skill_fishing_base_level",      HandleReloadSkillFishingBaseLevelCommand,      rbac::RBAC_PERM_COMMAND_RELOAD_SKILL_FISHING_BASE_LEVEL, Console::Yes },
            { "skinning_loot_template",        HandleReloadLootTemplatesSkinningCommand,      rbac::RBAC_PERM_COMMAND_RELOAD_SKINNING_LOOT_TEMPLATE, Console::Yes },
            { "spell_learn_spell",             HandleReloadSpellLearnSpellCommand,            rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_LEARN_SPELL, Console::Yes },
            { "smart_scripts",                 HandleReloadSmartScripts,                      rbac::RBAC_PERM_COMMAND_RELOAD_SMART_SCRIPTS, Console::Yes },
            { "spell_required",                HandleReloadSpellRequiredCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_REQUIRED, Console::Yes },
            { "spell_area",                    HandleReloadSpellAreaCommand,                  rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_AREA, Console::Yes },
            { "spell_bonus_data",              HandleReloadSpellBonusesCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_BONUS_DATA, Console::Yes },
            { "spell_group",                   HandleReloadSpellGroupsCommand,                rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_GROUP, Console::Yes },
            { "spell_loot_template",           HandleReloadLootTemplatesSpellCommand,         rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_LOOT_TEMPLATE, Console::Yes },
            { "spell_linked_spell",            HandleReloadSpellLinkedSpellCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_LINKED_SPELL, Console::Yes },
            { "spell_pet_auras",               HandleReloadSpellPetAurasCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_PET_AURAS, Console::Yes },
            { "spell_proc",                    HandleReloadSpellProcsCommand,                 rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_PROC, Console::Yes },
            { "spell_target_position",         HandleReloadSpellTargetPositionCommand,        rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_TARGET_POSITION, Console::Yes },
            { "spell_threats",                 HandleReloadSpellThreatsCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_THREATS, Console::Yes },
            { "spell_group_stack_rules",       HandleReloadSpellGroupStackRulesCommand,       rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_GROUP_STACK_RULES, Console::Yes },
            { "trainer",                       HandleReloadTrainerCommand,                    rbac::RBAC_PERM_COMMAND_RELOAD_TRAINER, Console::Yes },
            { "trinity_string",                HandleReloadTrinityStringCommand,              rbac::RBAC_PERM_COMMAND_RELOAD_TRINITY_STRING, Console::Yes },
            { "waypoint_scripts",              HandleReloadWpScriptsCommand,                  rbac::RBAC_PERM_COMMAND_RELOAD_WAYPOINT_SCRIPTS, Console::Yes },
            { "waypoint_data",                 HandleReloadWpCommand,                         rbac::RBAC_PERM_COMMAND_RELOAD_WAYPOINT_DATA, Console::Yes },
            { "vehicle_template",              HandleReloadVehicleTemplateCommand,            rbac::RBAC_PERM_COMMAND_RELOAD_VEHICLE_TEMPLATE,  Console::Yes },
            { "vehicle_accessory",             HandleReloadVehicleAccessoryCommand,           rbac::RBAC_PERM_COMMAND_RELOAD_VEHICLE_ACCESORY, Console::Yes },
            { "vehicle_template_accessory",    HandleReloadVehicleTemplateAccessoryCommand,   rbac::RBAC_PERM_COMMAND_RELOAD_VEHICLE_TEMPLATE_ACCESSORY, Console::Yes },
            //CHAT_FILTER
            { "creature_template2",            HandleReloadCreatureTemplateCommand2,          rbac::RBAC_PERM_COMMAND_RELOAD_CREATURE_TEMPLATE2,               Console::Yes },
            { "spell_target_position2",        HandleReloadSpellTargetPosition2Command,       rbac::RBAC_PERM_COMMAND_RELOAD_SPELL_TARGET_POSITION2,           Console::Yes },
            { "item_shop",                     HandleReloadItemShopCommand,                   rbac::RBAC_PERM_COMMAND_RELOAD_ITEM_TEMPLATE_LOCALE,             Console::Yes },
            { "chat_filter",                   HandleReloadLoadChatFilterCommand,             rbac::RBAC_PERM_COMMAND_RELOAD_CHAT_FILTER,                      Console::Yes },
            { "full",                          HandleReloadItemTemplateCommand,               rbac::RBAC_PERM_COMMAND_RELOAD_full,                             Console::Yes },
        };

        static ChatCommandTable commandTable =
        {
            { "reload",  reloadCommandTable },
        };
        return commandTable;
    }

    //reload commands
    static bool HandleReloadGMTicketsCommand(ChatHandler* /*handler*/)
    {
        sTicketMgr->LoadTickets();
        return true;
    }

    static bool HandleReloadAllCommand(ChatHandler* handler)
    {
        HandleReloadSkillFishingBaseLevelCommand(handler);

        HandleReloadAllAchievementCommand(handler);
        HandleReloadAllAreaCommand(handler);
        HandleReloadAllLootCommand(handler);
        HandleReloadAllNpcCommand(handler);
        HandleReloadAllQuestCommand(handler);
        HandleReloadAllSpellCommand(handler);
        HandleReloadAllItemCommand(handler);
        HandleReloadAllGossipsCommand(handler);
        HandleReloadAllLocalesCommand(handler);

        HandleReloadAccessRequirementCommand(handler);
        HandleReloadMailLevelRewardCommand(handler);
        HandleReloadReservedNameCommand(handler);
        HandleReloadTrinityStringCommand(handler);
        HandleReloadGameTeleCommand(handler);

        HandleReloadCreatureMovementOverrideCommand(handler);
        HandleReloadCreatureSummonGroupsCommand(handler);

        HandleReloadVehicleAccessoryCommand(handler);
        HandleReloadVehicleTemplateAccessoryCommand(handler);

        //CHAT_FILTER
        HandleReloadLoadChatFilterCommand(handler);

        HandleReloadAutobroadcastCommand(handler);
        HandleReloadBattlegroundTemplate(handler);
        return true;
    }

    static bool HandleReloadAllAchievementCommand(ChatHandler* handler)
    {
        HandleReloadAchievementCriteriaDataCommand(handler);
        HandleReloadAchievementRewardCommand(handler);
        return true;
    }

    static bool HandleReloadAllAreaCommand(ChatHandler* handler)
    {
        //HandleReloadQuestAreaTriggersCommand(handler, ""); -- reloaded in HandleReloadAllQuestCommand
        HandleReloadAreaTriggerTeleportCommand(handler);
        HandleReloadAreaTriggerTavernCommand(handler);
        HandleReloadGameGraveyardZoneCommand(handler);
        return true;
    }

    static bool HandleReloadAllLootCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables...");
        LoadLootTables();
        handler->SendGlobalGMSysMessage("DB tables `*_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadAllNpcCommand(ChatHandler* handler)
    {
        HandleReloadTrainerCommand(handler);
        HandleReloadNpcVendorCommand(handler);
        HandleReloadPointsOfInterestCommand(handler);
        HandleReloadSpellClickSpellsCommand(handler);
        return true;
    }

    static bool HandleReloadAllQuestCommand(ChatHandler* handler)
    {
        HandleReloadQuestGreetingCommand(handler);
        HandleReloadQuestAreaTriggersCommand(handler);
        HandleReloadQuestPOICommand(handler);
        HandleReloadQuestTemplateCommand(handler);

        TC_LOG_INFO("misc", "Re-Loading Quests Relations...");
        sObjectMgr->LoadQuestStartersAndEnders();
        handler->SendGlobalGMSysMessage("DB tables `*_queststarter` and `*_questender` reloaded.");
        return true;
    }

    static bool HandleReloadAllScriptsCommand(ChatHandler* handler)
    {
        if (sMapMgr->IsScriptScheduled())
        {
            handler->PSendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        TC_LOG_INFO("misc", "Re-Loading Scripts...");
        HandleReloadEventScriptsCommand(handler);
        handler->SendGlobalGMSysMessage("DB tables `*_scripts` reloaded.");
        HandleReloadWpScriptsCommand(handler);
        HandleReloadWpCommand(handler);
        return true;
    }

    static bool HandleReloadAllSpellCommand(ChatHandler* handler)
    {
        HandleReloadSkillDiscoveryTemplateCommand(handler);
        HandleReloadSkillExtraItemTemplateCommand(handler);
        HandleReloadSpellRequiredCommand(handler);
        HandleReloadSpellAreaCommand(handler);
        HandleReloadSpellGroupsCommand(handler);
        HandleReloadSpellLearnSpellCommand(handler);
        HandleReloadSpellLinkedSpellCommand(handler);
        HandleReloadSpellProcsCommand(handler);
        HandleReloadSpellBonusesCommand(handler);
        HandleReloadSpellTargetPositionCommand(handler);
        HandleReloadSpellTargetPosition2Command(handler);
        HandleReloadSpellThreatsCommand(handler);
        HandleReloadSpellGroupStackRulesCommand(handler);
        HandleReloadSpellPetAurasCommand(handler);
        return true;
    }

    static bool HandleReloadAllGossipsCommand(ChatHandler* handler)
    {
        HandleReloadGossipMenuCommand(handler);
        HandleReloadGossipMenuOptionCommand(handler);
        HandleReloadPointsOfInterestCommand(handler);
        return true;
    }

    static bool HandleReloadAllItemCommand(ChatHandler* handler)
    {
        HandleReloadPageTextsCommand(handler);
        HandleReloadItemEnchantementsCommand(handler);
        return true;
    }

    static bool HandleReloadAllLocalesCommand(ChatHandler* handler)
    {
        HandleReloadLocalesAchievementRewardCommand(handler);
        HandleReloadLocalesCreatureCommand(handler);
        HandleReloadLocalesCreatureTextCommand(handler);
        HandleReloadLocalesGameobjectCommand(handler);
        HandleReloadLocalesGossipMenuOptionCommand(handler);
        HandleReloadLocalesItemCommand(handler);
        HandleReloadLocalesNpcTextCommand(handler);
        HandleReloadLocalesPageTextCommand(handler);
        HandleReloadLocalesPointsOfInterestCommand(handler);
        HandleReloadLocalesQuestCommand(handler);
        HandleReloadLocalesQuestOfferRewardCommand(handler);
        HandleReloadLocalesQuestRequestItemsCommand(handler);
        HandleReloadLocalesQuestGreetingCommand(handler);
        return true;
    }

    static bool HandleReloadConfigCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading config settings...");
        sWorld->LoadConfigSettings(true);
        sMapMgr->InitializeVisibilityDistanceInfo();
        handler->SendGlobalGMSysMessage("World config settings reloaded.");
        return true;
    }

    static bool HandleReloadAccessRequirementCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Access Requirement definitions...");
        sObjectMgr->LoadAccessRequirements();
        handler->SendGlobalGMSysMessage("DB table `access_requirement` reloaded.");
        return true;
    }

    static bool HandleReloadAchievementCriteriaDataCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Additional Achievement Criteria Data...");
        sAchievementMgr->LoadAchievementCriteriaData();
        handler->SendGlobalGMSysMessage("DB table `achievement_criteria_data` reloaded.");
        return true;
    }

    static bool HandleReloadAchievementRewardCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Achievement Reward Data...");
        sAchievementMgr->LoadRewards();
        handler->SendGlobalGMSysMessage("DB table `achievement_reward` reloaded.");
        return true;
    }

    static bool HandleReloadAreaTriggerTavernCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Tavern Area Triggers...");
        sObjectMgr->LoadTavernAreaTriggers();
        handler->SendGlobalGMSysMessage("DB table `areatrigger_tavern` reloaded.");
        return true;
    }

    static bool HandleReloadAreaTriggerTeleportCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Area Trigger Teleports definitions...");
        sObjectMgr->LoadAreaTriggerTeleports();
        handler->SendGlobalGMSysMessage("DB table `areatrigger_teleport` reloaded.");
        return true;
    }

    static bool HandleReloadAutobroadcastCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Autobroadcasts...");
        sWorld->LoadAutobroadcasts();
        handler->SendGlobalGMSysMessage("DB table `autobroadcast` reloaded.");
        return true;
    }

    static bool HandleReloadBattlegroundTemplate(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Battleground Templates...");
        sBattlegroundMgr->LoadBattlegroundTemplates();
        handler->SendGlobalGMSysMessage("DB table `battleground_template` reloaded.");
        return true;
    }

    static bool HandleReloadBroadcastTextCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Broadcast texts...");
        sObjectMgr->LoadBroadcastTexts();
        sObjectMgr->LoadBroadcastTextLocales();
        handler->SendGlobalGMSysMessage("DB table `broadcast_text` reloaded.");
        return true;
    }

    static bool HandleReloadOnKillReputationCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading creature award reputation definitions...");
        sObjectMgr->LoadReputationOnKill();
        handler->SendGlobalGMSysMessage("DB table `creature_onkill_reputation` reloaded.");
        return true;
    }

    static bool HandleReloadCreatureSummonGroupsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading creature summon groups...");
        sObjectMgr->LoadTempSummons();
        handler->SendGlobalGMSysMessage("DB table `creature_summon_groups` reloaded.");
        return true;
    }

    static bool HandleReloadCreatureTemplateCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        for (std::string_view entryStr : Trinity::Tokenize(args, ' ', false))
        {
            uint32 entry = Trinity::StringTo<uint32>(entryStr).value_or(0);

            WorldDatabasePreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CREATURE_TEMPLATE);
            stmt->setUInt32(0, entry);
            PreparedQueryResult result = WorldDatabase.Query(stmt);

            if (!result)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATURETEMPLATE_NOTFOUND, entry);
                continue;
            }

            CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(entry);
            if (!cInfo)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATURESTORAGE_NOTFOUND, entry);
                continue;
            }

            TC_LOG_INFO("misc", "Reloading creature template entry {}", entry);

            Field* fields = result->Fetch();
            sObjectMgr->LoadCreatureTemplate(fields);
            sObjectMgr->CheckCreatureTemplate(cInfo);
        }

        sObjectMgr->InitializeQueriesData(QUERY_DATA_CREATURES);
        handler->SendGlobalGMSysMessage("Creature template reloaded.");
        return true;
    }

    static bool HandleReloadCreatureTemplateCommand2(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        for (std::string_view entryStr : Trinity::Tokenize(args, ' ', false))
        {
            uint32 entry = Trinity::StringTo<uint32>(entryStr).value_or(0);

            WorldDatabasePreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CREATURE_TEMPLATE2);
            stmt->setUInt32(0, entry);
            PreparedQueryResult result = WorldDatabase.Query(stmt);

            if (!result)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATURETEMPLATE_NOTFOUND, entry);
                continue;
            }

            CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(entry);
            if (!cInfo)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATURESTORAGE_NOTFOUND, entry);
                continue;
            }

            TC_LOG_INFO("misc", "Reloading creature template entry {}", entry);

            Field* fields = result->Fetch();
            sObjectMgr->LoadCreatureTemplate2(fields);
            sObjectMgr->CheckCreatureTemplate(cInfo);
        }

        sObjectMgr->InitializeQueriesData(QUERY_DATA_CREATURES);
        handler->SendGlobalGMSysMessage("Creature template reloaded.");
        return true;
    }

    static bool HandleReloadCreatureQuestStarterCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Loading Quests Relations... (`creature_queststarter`)");
        sObjectMgr->LoadCreatureQuestStarters();
        handler->SendGlobalGMSysMessage("DB table `creature_queststarter` reloaded.");
        return true;
    }

    static bool HandleReloadLinkedRespawnCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Loading Linked Respawns... (`creature_linked_respawn`)");
        sObjectMgr->LoadLinkedRespawn();
        handler->SendGlobalGMSysMessage("DB table `creature_linked_respawn` (creature linked respawns) reloaded.");
        return true;
    }

    static bool HandleReloadCreatureQuestEnderCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Loading Quests Relations... (`creature_questender`)");
        sObjectMgr->LoadCreatureQuestEnders();
        handler->SendGlobalGMSysMessage("DB table `creature_questender` reloaded.");
        return true;
    }

    static bool HandleReloadGossipMenuCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `gossip_menu` Table!");
        sObjectMgr->LoadGossipMenu();
        handler->SendGlobalGMSysMessage("DB table `gossip_menu` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadGossipMenuOptionCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `gossip_menu_option` Table!");
        sObjectMgr->LoadGossipMenuItems();
        handler->SendGlobalGMSysMessage("DB table `gossip_menu_option` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadGOQuestStarterCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Loading Quests Relations... (`gameobject_queststarter`)");
        sObjectMgr->LoadGameobjectQuestStarters();
        handler->SendGlobalGMSysMessage("DB table `gameobject_queststarter` reloaded.");
        return true;
    }

    static bool HandleReloadGOQuestEnderCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Loading Quests Relations... (`gameobject_questender`)");
        sObjectMgr->LoadGameobjectQuestEnders();
        handler->SendGlobalGMSysMessage("DB table `gameobject_questender` reloaded.");
        return true;
    }

    static bool HandleReloadQuestAreaTriggersCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Area Triggers...");
        sObjectMgr->LoadQuestAreaTriggers();
        handler->SendGlobalGMSysMessage("DB table `areatrigger_involvedrelation` (quest area triggers) reloaded.");
        return true;
    }

    static bool HandleReloadQuestGreetingCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Greeting ...");
        sObjectMgr->LoadQuestGreetings();
        handler->SendGlobalGMSysMessage("DB table `quest_greeting` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesQuestGreetingCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Greeting locales...");
        sObjectMgr->LoadQuestGreetingLocales();
        handler->SendGlobalGMSysMessage("DB table `quest_greeting_locale` reloaded.");
        return true;
    }

    static bool HandleReloadQuestTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Templates...");
        sObjectMgr->LoadQuests();
        sObjectMgr->InitializeQueriesData(QUERY_DATA_QUESTS);
        handler->SendGlobalGMSysMessage("DB table `quest_template` (quest definitions) reloaded.");

        /// dependent also from `gameobject` but this table not reloaded anyway
        TC_LOG_INFO("misc", "Re-Loading GameObjects for quests...");
        sObjectMgr->LoadGameObjectForQuests();
        handler->SendGlobalGMSysMessage("Data GameObjects for quests reloaded.");
        return true;
    }

    static bool HandleReloadLootTemplatesCreatureCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`creature_loot_template`)");
        LoadLootTemplates_Creature();
        LootTemplates_Creature.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `creature_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadCreatureMovementOverrideCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Creature movement overrides...");
        sObjectMgr->LoadCreatureMovementOverrides();
        handler->SendGlobalGMSysMessage("DB table `creature_movement_override` reloaded.");
        return true;
    }

    static bool HandleReloadLootTemplatesDisenchantCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`disenchant_loot_template`)");
        LoadLootTemplates_Disenchant();
        LootTemplates_Disenchant.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `disenchant_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesFishingCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`fishing_loot_template`)");
        LoadLootTemplates_Fishing();
        LootTemplates_Fishing.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `fishing_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesGameobjectCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`gameobject_loot_template`)");
        LoadLootTemplates_Gameobject();
        LootTemplates_Gameobject.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `gameobject_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesItemCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`item_loot_template`)");
        LoadLootTemplates_Item();
        LootTemplates_Item.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `item_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesMillingCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`milling_loot_template`)");
        LoadLootTemplates_Milling();
        LootTemplates_Milling.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `milling_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesPickpocketingCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`pickpocketing_loot_template`)");
        LoadLootTemplates_Pickpocketing();
        LootTemplates_Pickpocketing.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `pickpocketing_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesProspectingCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`prospecting_loot_template`)");
        LoadLootTemplates_Prospecting();
        LootTemplates_Prospecting.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `prospecting_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesMailCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`mail_loot_template`)");
        LoadLootTemplates_Mail();
        LootTemplates_Mail.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `mail_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadMailServerTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("server.loading", "Re-Loading `server_mail_template` table");
        sObjectMgr->LoadMailServerTemplates();
        handler->SendGlobalGMSysMessage("DB table `server_mail_template` reloaded.");
        return true;
    }

    static bool HandleReloadLootTemplatesReferenceCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`reference_loot_template`)");
        LoadLootTemplates_Reference();
        handler->SendGlobalGMSysMessage("DB table `reference_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesSkinningCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`skinning_loot_template`)");
        LoadLootTemplates_Skinning();
        LootTemplates_Skinning.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `skinning_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesSpellCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Loot Tables... (`spell_loot_template`)");
        LoadLootTemplates_Spell();
        LootTemplates_Spell.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `spell_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadTrinityStringCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading trinity_string Table!");
        sObjectMgr->LoadTrinityStrings();
        sObjectMgr->LoadTrinityStrings2();
        handler->SendGlobalGMSysMessage("DB table `trinity_string` reloaded.");
        return true;
    }

    static bool HandleReloadTrainerCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `trainer` Table!");
        sObjectMgr->LoadTrainers();
        sObjectMgr->LoadCreatureDefaultTrainers();
        handler->SendGlobalGMSysMessage("DB table `trainer` reloaded.");
        handler->SendGlobalGMSysMessage("DB table `trainer_locale` reloaded.");
        handler->SendGlobalGMSysMessage("DB table `trainer_spell` reloaded.");
        handler->SendGlobalGMSysMessage("DB table `creature_default_trainer` reloaded.");
        return true;
    }

    static bool HandleReloadNpcVendorCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `npc_vendor` Table!");
        sObjectMgr->LoadVendors();
        handler->SendGlobalGMSysMessage("DB table `npc_vendor` reloaded.");
        return true;
    }

    static bool HandleReloadPointsOfInterestCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `points_of_interest` Table!");
        sObjectMgr->LoadPointsOfInterest();
        handler->SendGlobalGMSysMessage("DB table `points_of_interest` reloaded.");
        return true;
    }

    static bool HandleReloadQuestPOICommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest POI ...");
        sObjectMgr->LoadQuestPOI();
        sObjectMgr->InitializeQueriesData(QUERY_DATA_POIS);
        handler->SendGlobalGMSysMessage("DB Table `quest_poi` and `quest_poi_points` reloaded.");
        return true;
    }

    static bool HandleReloadSpellClickSpellsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `npc_spellclick_spells` Table!");
        sObjectMgr->LoadNPCSpellClickSpells();
        handler->SendGlobalGMSysMessage("DB table `npc_spellclick_spells` reloaded.");
        return true;
    }

    static bool HandleReloadReservedNameCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Loading ReservedNames... (`reserved_name`)");
        sObjectMgr->LoadReservedPlayersNames();
        handler->SendGlobalGMSysMessage("DB table `reserved_name` (player reserved names) reloaded.");
        return true;
    }

    static bool HandleReloadReputationRewardRateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `reputation_reward_rate` Table!");
        sObjectMgr->LoadReputationRewardRate();
        handler->SendGlobalSysMessage("DB table `reputation_reward_rate` reloaded.");
        return true;
    }

    static bool HandleReloadReputationSpilloverTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading `reputation_spillover_template` Table!");
        sObjectMgr->LoadReputationSpilloverTemplate();
        handler->SendGlobalSysMessage("DB table `reputation_spillover_template` reloaded.");
        return true;
    }

    static bool HandleReloadSkillDiscoveryTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Skill Discovery Table...");
        LoadSkillDiscoveryTable();
        handler->SendGlobalGMSysMessage("DB table `skill_discovery_template` (recipes discovered at crafting) reloaded.");
        return true;
    }

    static bool HandleReloadSkillPerfectItemTemplateCommand(ChatHandler* handler)
    { // latched onto HandleReloadSkillExtraItemTemplateCommand as it's part of that table group (and i don't want to chance all the command IDs)
        TC_LOG_INFO("misc", "Re-Loading Skill Perfection Data Table...");
        LoadSkillPerfectItemTable();
        handler->SendGlobalGMSysMessage("DB table `skill_perfect_item_template` (perfect item procs when crafting) reloaded.");
        return true;
    }

    static bool HandleReloadSkillExtraItemTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Skill Extra Item Table...");
        LoadSkillExtraItemTable();
        handler->SendGlobalGMSysMessage("DB table `skill_extra_item_template` (extra item creation when crafting) reloaded.");

        return HandleReloadSkillPerfectItemTemplateCommand(handler);
    }

    static bool HandleReloadSkillFishingBaseLevelCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Skill Fishing base level requirements...");
        sObjectMgr->LoadFishingBaseSkillLevel();
        handler->SendGlobalGMSysMessage("DB table `skill_fishing_base_level` (fishing base level for zone/subzone) reloaded.");
        return true;
    }

    static bool HandleReloadSpellAreaCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading SpellArea Data...");
        sSpellMgr->LoadSpellAreas();
        handler->SendGlobalGMSysMessage("DB table `spell_area` (spell dependences from area/quest/auras state) reloaded.");
        return true;
    }

    static bool HandleReloadSpellRequiredCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Required Data... ");
        sSpellMgr->LoadSpellRequired();
        handler->SendGlobalGMSysMessage("DB table `spell_required` reloaded.");
        return true;
    }

    static bool HandleReloadSpellGroupsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Groups...");
        sSpellMgr->LoadSpellGroups();
        handler->SendGlobalGMSysMessage("DB table `spell_group` (spell groups) reloaded.");
        return true;
    }

    static bool HandleReloadSpellLearnSpellCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Learn Spells...");
        sSpellMgr->LoadSpellLearnSpells();
        handler->SendGlobalGMSysMessage("DB table `spell_learn_spell` reloaded.");
        return true;
    }

    static bool HandleReloadSpellLinkedSpellCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Linked Spells...");
        sSpellMgr->LoadSpellLinked();
        handler->SendGlobalGMSysMessage("DB table `spell_linked_spell` reloaded.");
        return true;
    }

    static bool HandleReloadSpellProcsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Proc conditions and data...");
        sSpellMgr->LoadSpellProcs();
        handler->SendGlobalGMSysMessage("DB table `spell_proc` (spell proc conditions and data) reloaded.");
        return true;
    }

    static bool HandleReloadSpellBonusesCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Bonus Data...");
        sSpellMgr->LoadSpellBonuses();
        handler->SendGlobalGMSysMessage("DB table `spell_bonus_data` (spell damage/healing coefficients) reloaded.");
        return true;
    }

    static bool HandleReloadSpellTargetPositionCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell target coordinates...");
        sSpellMgr->LoadSpellTargetPositions();
        handler->SendGlobalGMSysMessage("DB table `spell_target_position` (destination coordinates for spell targets) reloaded.");
        return true;
    }

    static bool HandleReloadSpellTargetPosition2Command(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell target coordinates...");
        sSpellMgr->LoadSpellTargetPositions2();
        handler->SendGlobalGMSysMessage("DB table `spell_target_position2` (destination coordinates for spell targets) reloaded.");
        return true;
    }

    static bool HandleReloadSpellThreatsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Aggro Spells Definitions...");
        sSpellMgr->LoadSpellThreats();
        handler->SendGlobalGMSysMessage("DB table `spell_threat` (spell aggro definitions) reloaded.");
        return true;
    }

    static bool HandleReloadSpellGroupStackRulesCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell Group Stack Rules...");
        sSpellMgr->LoadSpellGroupStackRules();
        handler->SendGlobalGMSysMessage("DB table `spell_group_stack_rules` (spell stacking definitions) reloaded.");
        return true;
    }

    static bool HandleReloadSpellPetAurasCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Spell pet auras...");
        sSpellMgr->LoadSpellPetAuras();
        handler->SendGlobalGMSysMessage("DB table `spell_pet_auras` reloaded.");
        return true;
    }

    static bool HandleReloadPageTextsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Page Text...");
        sObjectMgr->LoadPageTexts();
        handler->SendGlobalGMSysMessage("DB table `page_text` reloaded.");
        return true;
    }

    static bool HandleReloadItemEnchantementsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Item Random Enchantments Table...");
        LoadRandomEnchantmentsTable();
        handler->SendGlobalGMSysMessage("DB table `item_enchantment_template` reloaded.");
        return true;
    }

    static bool HandleReloadItemSetNamesCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Item set names...");
        sObjectMgr->LoadItemSetNames();
        handler->SendGlobalGMSysMessage("DB table `item_set_names` reloaded.");
        return true;
    }

    static bool HandleReloadEventScriptsCommand(ChatHandler* handler)
    {
        if (sMapMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        TC_LOG_INFO("misc", "Re-Loading Scripts from `event_scripts`...");
        sObjectMgr->LoadEventScripts();
        handler->SendGlobalGMSysMessage("DB table `event_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadWpScriptsCommand(ChatHandler* handler)
    {
        if (sMapMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        TC_LOG_INFO("misc", "Re-Loading Scripts from `waypoint_scripts`...");
        sObjectMgr->LoadWaypointScripts();
        handler->SendGlobalGMSysMessage("DB table `waypoint_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadWpCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Waypoints data from 'waypoints_data'");
        sWaypointMgr->Load();
        handler->SendGlobalGMSysMessage("DB Table 'waypoint_data' reloaded.");

        return true;
    }

    static bool HandleReloadGameGraveyardZoneCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Graveyard-zone links...");
        sObjectMgr->LoadGraveyardZones();
        handler->SendGlobalGMSysMessage("DB table `graveyard_zone` reloaded.");

        return true;
    }

    static bool HandleReloadGameTeleCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Game Tele coordinates...");
        sObjectMgr->LoadGameTele();
        handler->SendGlobalGMSysMessage("DB table `game_tele` reloaded.");

        return true;
    }

    static bool HandleReloadDisablesCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading disables table...");
        DisableMgr::LoadDisables();
        TC_LOG_INFO("misc", "Checking quest disables...");
        DisableMgr::CheckQuestDisables();
        handler->SendGlobalGMSysMessage("DB table `disables` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesAchievementRewardCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Achievement Reward Data Locale...");
        sAchievementMgr->LoadRewardLocales();
        handler->SendGlobalGMSysMessage("DB table `achievement_reward_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLfgRewardsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading lfg dungeon rewards...");
        sLFGMgr->LoadRewards();
        handler->SendGlobalGMSysMessage("DB table `lfg_dungeon_rewards` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesCreatureCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Creature Template Locale...");
        sObjectMgr->LoadCreatureLocales();
        handler->SendGlobalGMSysMessage("DB table `creature_template_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesCreatureTextCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Creature Texts Locale...");
        sCreatureTextMgr->LoadCreatureTextLocales();
        handler->SendGlobalGMSysMessage("DB table `creature_text_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesGameobjectCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Gameobject Template Locale... ");
        sObjectMgr->LoadGameObjectLocales();
        handler->SendGlobalGMSysMessage("DB table `gameobject_template_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesGossipMenuOptionCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Gossip Menu Option Locale... ");
        sObjectMgr->LoadGossipMenuItemsLocales();
        handler->SendGlobalGMSysMessage("DB table `gossip_menu_option_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesItemCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Item Template Locale... ");
        sObjectMgr->LoadItemLocales();
        handler->SendGlobalGMSysMessage("DB table `item_template_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesItemSetNameCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Item set name Locale... ");
        sObjectMgr->LoadItemSetNameLocales();
        handler->SendGlobalGMSysMessage("DB table `item_set_name_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesNpcTextCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading NPC Text Locale... ");
        sObjectMgr->LoadNpcTextLocales();
        handler->SendGlobalGMSysMessage("DB table `npc_text_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesPageTextCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Page Text Locale... ");
        sObjectMgr->LoadPageTextLocales();
        handler->SendGlobalGMSysMessage("DB table `page_text_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesPointsOfInterestCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Points Of Interest Locale... ");
        sObjectMgr->LoadPointOfInterestLocales();
        handler->SendGlobalGMSysMessage("DB table `points_of_interest_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesQuestCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Template Locale... ");
        sObjectMgr->LoadQuestLocales();
        handler->SendGlobalGMSysMessage("DB table `quest_template_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesQuestOfferRewardCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Offer Reward Locale... ");
        sObjectMgr->LoadQuestOfferRewardLocale();
        handler->SendGlobalGMSysMessage("DB table `quest_offer_reward_locale` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesQuestRequestItemsCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Quest Request Item Locale... ");
        sObjectMgr->LoadQuestRequestItemsLocale();
        handler->SendGlobalGMSysMessage("DB table `quest_request_item_locale` reloaded.");
        return true;
    }

    static bool HandleReloadMailLevelRewardCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Player level dependent mail rewards...");
        sObjectMgr->LoadMailLevelRewards();
        handler->SendGlobalGMSysMessage("DB table `mail_level_reward` reloaded.");
        return true;
    }

    static bool HandleReloadAuctionsCommand(ChatHandler* handler)
    {
        ///- Reload dynamic data tables from the database
        TC_LOG_INFO("misc", "Re-Loading Auctions...");
        sAuctionMgr->LoadAuctionItems();
        sAuctionMgr->LoadAuctions();
        handler->SendGlobalGMSysMessage("Auctions reloaded.");
        return true;
    }

    static bool HandleReloadConditions(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Conditions...");
        sConditionMgr->LoadConditions(true);
        handler->SendGlobalGMSysMessage("Conditions reloaded.");
        return true;
    }

    static bool HandleReloadCreatureText(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Creature Texts...");
        sCreatureTextMgr->LoadCreatureTexts();
        handler->SendGlobalGMSysMessage("Creature Texts reloaded.");
        return true;
    }

    static bool HandleReloadSmartScripts(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Re-Loading Smart Scripts...");
        sSmartScriptMgr->LoadSmartAIFromDB();
        handler->SendGlobalGMSysMessage("Smart Scripts reloaded.");
        return true;
    }

    static bool HandleReloadVehicleTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading vehicle_template table...");
        sObjectMgr->LoadVehicleTemplate();
        handler->SendGlobalGMSysMessage("Vehicle templates reloaded.");
        return true;
    }

    static bool HandleReloadItemShopCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading item_shop table...");
        sItemShopMgr->Reload();
        handler->SendGlobalGMSysMessage("Items from shop reloaded.");
        return true;
    }

    static bool HandleReloadVehicleAccessoryCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading vehicle_accessory table...");
        sObjectMgr->LoadVehicleAccessories();
        handler->SendGlobalGMSysMessage("Vehicle accessories reloaded.");
        return true;
    }

    static bool HandleReloadVehicleTemplateAccessoryCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading vehicle_template_accessory table...");
        sObjectMgr->LoadVehicleTemplateAccessories();
        handler->SendGlobalGMSysMessage("Vehicle template accessories reloaded.");
        return true;
    }

    //CHAT_FILTER
    static bool HandleReloadLoadChatFilterCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading chat_filter table...");
        sObjectMgr->LoadChatFilter();
        handler->SendGlobalGMSysMessage("Chat Filter words reloaded.");
        return true;
    }

    static bool HandleReloadItemTemplateCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading Creature and Item_template..");
        sObjectMgr->LoadEquipmentTemplates();
        sObjectMgr->LoadItemTemplates2();
        sObjectMgr->LoadItemTemplates();
        sObjectMgr->LoadCreatureTemplates();
        sObjectMgr->LoadCreatureTemplates2();
        sObjectMgr->LoadGameObjectTemplate();
        sObjectMgr->LoadAreaCustomFlags();
        sObjectMgr->LoadCreatureSpecialRewards();
        sPromotionCodeMgr->ReloadCodes();
        handler->SendGlobalGMSysMessage("Creature and Item_template has been reloaded!");
        return true;
    }

    static bool HandleReloadRBACCommand(ChatHandler* handler)
    {
        TC_LOG_INFO("misc", "Reloading RBAC tables...");
        sAccountMgr->LoadRBAC();
        sWorld->ReloadRBAC();
        handler->SendGlobalGMSysMessage("RBAC data reloaded.");
        return true;
    }
};

void AddSC_reload_commandscript()
{
    new reload_commandscript();
}
