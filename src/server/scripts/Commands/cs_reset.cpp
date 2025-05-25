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
Name: reset_commandscript
%Complete: 100
Comment: All reset related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "AchievementMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "Language.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Player.h"
#include "RBAC.h"
#include "World.h"
#include "WorldSession.h"
#include "CharacterCache.h"
#include "ChatCommandTags.h"
#include "DBCStores.h"
#include "GroupMgr.h"
#include "LFG.h"
#include "Map.h"

#if TRINITY_COMPILER == TRINITY_COMPILER_GNU
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

using namespace Trinity::ChatCommands;

class reset_commandscript : public CommandScript
{
public:
    reset_commandscript() : CommandScript("reset_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> resetItemsCommandTable =
        {
            { "equipped",       rbac::RBAC_PERM_COMMAND_RTX100, true, &HandleResetItemsEquippedCommand,           "" },
            { "bags",           rbac::RBAC_PERM_COMMAND_RTX101, true, &HandleResetItemsInBagsCommand,             "" },
            { "bank",           rbac::RBAC_PERM_COMMAND_RTX102, true, &HandleResetItemsInBankCommand,             "" },
            { "keyring",        rbac::RBAC_PERM_COMMAND_RTX103, true, &HandleResetItemsKeyringCommand,            "" },
            { "currency",       rbac::RBAC_PERM_COMMAND_RTX104, true, &HandleResetItemsInCurrenciesListCommand,   "" },
            { "vendor_buyback", rbac::RBAC_PERM_COMMAND_RTX105, true, &HandleResetItemsInVendorBuyBackTabCommand, "" },
            { "all",            rbac::RBAC_PERM_COMMAND_RTX106, true, &HandleResetItemsAllCommand,                "" },
            { "allbags",        rbac::RBAC_PERM_COMMAND_RTX107, true, &HandleResetItemsAllAndDeleteBagsCommand,   "" },
        };

        static std::vector<ChatCommand> resetCommandTable =
        {
            { "achievements", rbac::RBAC_PERM_COMMAND_RESET_ACHIEVEMENTS, true, &HandleResetAchievementsCommand, "" },
            { "honor",        rbac::RBAC_PERM_COMMAND_RESET_HONOR,        true, &HandleResetHonorCommand,        "" },
            { "level",        rbac::RBAC_PERM_COMMAND_RESET_LEVEL,        true, &HandleResetLevelCommand,        "" },
            { "spells",       rbac::RBAC_PERM_COMMAND_RESET_SPELLS,       true, &HandleResetSpellsCommand,       "" },
            { "stats",        rbac::RBAC_PERM_COMMAND_RESET_STATS,        true, &HandleResetStatsCommand,        "" },
            { "talents",      rbac::RBAC_PERM_COMMAND_RESET_TALENTS,      true, &HandleResetTalentsCommand,      "" },
            { "items",          resetItemsCommandTable                                          },
            { "all",          rbac::RBAC_PERM_COMMAND_RESET_ALL,          true, &HandleResetAllCommand,          "" },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "reset", rbac::RBAC_PERM_COMMAND_RESET, true, nullptr, "", resetCommandTable },
        };
        return commandTable;
    }

    static bool HandleResetAchievementsCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        ObjectGuid targetGuid;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid))
            return false;

        if (target)
            target->ResetAchievements();
        else
            AchievementMgr::DeleteFromDB(targetGuid);

        return true;
    }

    static bool HandleResetHonorCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        target->SetHonorPoints(0);
        target->SetUInt32Value(PLAYER_FIELD_KILLS, 0);
        target->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, 0);
        target->SetUInt32Value(PLAYER_FIELD_TODAY_CONTRIBUTION, 0);
        target->SetUInt32Value(PLAYER_FIELD_YESTERDAY_CONTRIBUTION, 0);
        target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL);

        return true;
    }

    static bool HandleResetStatsOrLevelHelper(Player* player)
    {
        ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(player->GetClass());
        if (!classEntry)
        {
            TC_LOG_ERROR("misc", "Class {} not found in DBC (Wrong DBC files?)", player->GetClass());
            return false;
        }

        uint8 powerType = classEntry->DisplayPower;

        // reset m_form if no aura
        if (!player->HasAuraType(SPELL_AURA_MOD_SHAPESHIFT))
            player->SetShapeshiftForm(FORM_NONE);

        player->SetFactionForRace(player->GetRace());

        player->SetPowerType(Powers(powerType), false);

        // reset only if player not in some form;
        if (player->GetShapeshiftForm() == FORM_NONE)
            player->InitDisplayIds();

        player->ReplaceAllPvpFlags(UNIT_BYTE2_FLAG_PVP);

        player->ReplaceAllUnitFlags(UNIT_FLAG_PLAYER_CONTROLLED);

        //-1 is default value
        player->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, uint32(-1));
        return true;
    }

    static bool HandleResetLevelCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        if (!HandleResetStatsOrLevelHelper(target))
            return false;

        uint8 oldLevel = target->GetLevel();

        // set starting level
        uint32 startLevel = target->GetClass() != CLASS_DEATH_KNIGHT
            ? sWorld->getIntConfig(CONFIG_START_PLAYER_LEVEL)
            : sWorld->getIntConfig(CONFIG_START_DEATH_KNIGHT_PLAYER_LEVEL);

        target->_ApplyAllLevelScaleItemMods(false);
        target->SetLevel(startLevel);
        target->InitRunes();
        target->InitStatsForLevel(true);
        target->InitTaxiNodesForLevel();
        target->InitGlyphsForLevel();
        target->InitTalentForLevel();
        target->SetXP(0);

        target->_ApplyAllLevelScaleItemMods(true);

        // reset level for pet
        if (Pet* pet = target->GetPet())
            pet->SynchronizeLevelWithOwner();

        sScriptMgr->OnPlayerLevelChanged(target, oldLevel);

        return true;
    }

    static bool HandleResetSpellsCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        if (target)
        {
            target->ResetSpells(/* bool myClassOnly */);

            ChatHandler(target->GetSession()).SendSysMessage(LANG_RESET_SPELLS);
            if (!handler->GetSession() || handler->GetSession()->GetPlayer() != target)
                handler->PSendSysMessage(LANG_RESET_SPELLS_ONLINE, handler->GetNameLink(target).c_str());
        }
        else
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
            stmt->setUInt16(0, uint16(AT_LOGIN_RESET_SPELLS));
            stmt->setUInt32(1, targetGuid.GetCounter());
            CharacterDatabase.Execute(stmt);

            handler->PSendSysMessage(LANG_RESET_SPELLS_OFFLINE, targetName.c_str());
        }

        return true;
    }

    static bool HandleResetStatsCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        if (!HandleResetStatsOrLevelHelper(target))
            return false;

        target->InitRunes();
        target->InitStatsForLevel(true);
        target->InitTaxiNodesForLevel();
        target->InitGlyphsForLevel();
        target->InitTalentForLevel();

        return true;
    }

    static bool HandleResetTalentsCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
        {
            // Try reset talents as Hunter Pet
            Creature* creature = handler->getSelectedCreature();
            if (!*args && creature && creature->IsPet())
            {
                Unit* owner = creature->GetOwner();
                if (owner && owner->GetTypeId() == TYPEID_PLAYER && creature->ToPet()->IsPermanentPetFor(owner->ToPlayer()))
                {
                    creature->ToPet()->resetTalents(true);
                    owner->ToPlayer()->SendTalentsInfoData(true);

                    if (!handler->GetSession() || handler->GetSession()->GetPlayer() != owner->ToPlayer())
                        handler->PSendSysMessage(LANG_RESET_PET_TALENTS_ONLINE, handler->GetNameLink(owner->ToPlayer()).c_str());
                }
                return true;
            }

            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target)
        {
            target->ResetTalents(true);
            target->SendTalentsInfoData(false);
            if (!handler->GetSession() || handler->GetSession()->GetPlayer() != target)
                handler->PSendSysMessage(LANG_RESET_TALENTS_ONLINE, handler->GetNameLink(target).c_str());

            Pet* pet = target->GetPet();
            Pet::resetTalentsForAllPetsOf(target, pet, true);
            if (pet)
                target->SendTalentsInfoData(true);
            return true;
        }
        else if (targetGuid)
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
            stmt->setUInt16(0, uint16(AT_LOGIN_NONE | AT_LOGIN_RESET_PET_TALENTS));
            stmt->setUInt32(1, targetGuid.GetCounter());
            CharacterDatabase.Execute(stmt);

            std::string nameLink = handler->playerLink(targetName);
            handler->PSendSysMessage(LANG_RESET_TALENTS_OFFLINE, nameLink.c_str());
            return true;
        }

        handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleResetAllCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string caseName = args;

        AtLoginFlags atLogin;

        // Command specially created as single command to prevent using short case names
        if (caseName == "spells")
        {
            atLogin = AT_LOGIN_RESET_SPELLS;
            sWorld->SendWorldText(LANG_RESETALL_SPELLS);
            if (!handler->GetSession())
                handler->SendSysMessage(LANG_RESETALL_SPELLS);
        }
        else if (caseName == "talents")
        {
            atLogin = AtLoginFlags(AT_LOGIN_RESET_TALENTS | AT_LOGIN_RESET_PET_TALENTS);
            sWorld->SendWorldText(LANG_RESETALL_TALENTS);
            if (!handler->GetSession())
               handler->SendSysMessage(LANG_RESETALL_TALENTS);
        }
        else
        {
            handler->PSendSysMessage(LANG_RESETALL_UNKNOWN_CASE, args);
            handler->SetSentErrorMessage(true);
            return false;
        }

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ALL_AT_LOGIN_FLAGS);
        stmt->setUInt16(0, uint16(atLogin));
        CharacterDatabase.Execute(stmt);

        std::shared_lock<std::shared_mutex> lock(*HashMapHolder<Player>::GetLock());
        HashMapHolder<Player>::MapType const& plist = ObjectAccessor::GetPlayers();
        for (HashMapHolder<Player>::MapType::const_iterator itr = plist.begin(); itr != plist.end(); ++itr)
            itr->second->SetAtLoginFlag(atLogin);

        return true;
    }

    //test command
    static bool HandleResetItemsEquippedCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {
            int16 deletedItemsCount = ResetItemsEquipped(targetPlayer);
            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_EQUIPPED, deletedItemsCount, handler->GetNameLink(targetPlayer));
        }

        return true;
    }

    static bool HandleResetItemsInBagsCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {
            int16 deletedItemsCount = ResetItemsInBags(targetPlayer);
            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_BAGS, deletedItemsCount, handler->GetNameLink(targetPlayer));
        }

        return true;
    }

    static bool HandleResetItemsKeyringCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {
            int16 deletedItemsCount = ResetItemsInKeyring(targetPlayer);
            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_KEYRING, deletedItemsCount, handler->GetNameLink(targetPlayer));
        }

        return true;
    }

    static bool HandleResetItemsInCurrenciesListCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {
            int16 deletedItemsCount = ResetItemsInCurrenciesList(targetPlayer);
            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_CURRENCY, deletedItemsCount, handler->GetNameLink(targetPlayer));
        }

        return true;
    }

    static bool HandleResetItemsInBankCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {
            int16 deletedItemsCount = ResetItemsInBank(targetPlayer);
            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_BANK, deletedItemsCount, handler->GetNameLink(targetPlayer));
        }

        return true;
    }

    static bool HandleResetItemsInVendorBuyBackTabCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {
            int16 deletedItemsCount = ResetItemsInVendorBuyBackTab(targetPlayer);
            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_BUYBACK, deletedItemsCount, handler->GetNameLink(targetPlayer));
        }

        return true;
    }

    static bool HandleResetItemsAllCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {

            // Delete all items destinations
            int16 deletedItemsEquippedCount = ResetItemsEquipped(targetPlayer);
            int16 deletedItemsInBagsCount = ResetItemsInBags(targetPlayer);
            int16 deletedItemsInBankCount = ResetItemsInBank(targetPlayer);
            int16 deletedItemsInKeyringCount = ResetItemsInKeyring(targetPlayer);
            int16 deletedItemsInCurrenciesListCount = ResetItemsInCurrenciesList(targetPlayer);
            int16 deletedItemsInVendorBuyBackTabCount = ResetItemsInVendorBuyBackTab(targetPlayer);

            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_ALL, handler->GetNameLink(targetPlayer),
                deletedItemsEquippedCount,
                deletedItemsInBagsCount,
                deletedItemsInBankCount,
                deletedItemsInKeyringCount,
                deletedItemsInCurrenciesListCount,
                deletedItemsInVendorBuyBackTabCount);
        }

        return true;
    }

    static bool HandleResetItemsAllAndDeleteBagsCommand(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = GetPlayerFromIdentifierOrSelectedTarget(handler, target);

        if (!targetPlayer)
        {
            return false;
        }
        else
        {

            // Delete all items destinations
            int16 deletedItemsEquippedCount = ResetItemsEquipped(targetPlayer);
            int16 deletedItemsInBagsCount = ResetItemsInBags(targetPlayer);
            int16 deletedItemsInBankCount = ResetItemsInBank(targetPlayer);
            int16 deletedItemsInKeyringCount = ResetItemsInKeyring(targetPlayer);
            int16 deletedItemsInCurrenciesListCount = ResetItemsInCurrenciesList(targetPlayer);
            int16 deletedItemsInVendorBuyBackTabCount = ResetItemsInVendorBuyBackTab(targetPlayer);
            int16 deletedItemsStandardBagsCount = ResetItemsDeleteStandardBags(targetPlayer);
            int16 deletedItemsBankBagsCount = ResetItemsDeleteBankBags(targetPlayer);

            handler->PSendSysMessage(LANG_COMMAND_RESET_ITEMS_ALL_BAGS, handler->GetNameLink(targetPlayer),
                deletedItemsEquippedCount,
                deletedItemsInBagsCount,
                deletedItemsInBankCount,
                deletedItemsInKeyringCount,
                deletedItemsInCurrenciesListCount,
                deletedItemsInVendorBuyBackTabCount,
                deletedItemsStandardBagsCount,
                deletedItemsBankBagsCount);
        }

        return true;
    }
    //, Optional<std::string_view> playerName = {}
private:
    static Player* GetPlayerFromIdentifierOrSelectedTarget(ChatHandler* handler, Optional<PlayerIdentifier> target)
    {
        Player* targetPlayer = nullptr;

        // Check if there is an optional target player name
        // Do not use TargetOrSelf, we must be sure to select ourself
        if (!target)
        {
            // No optional target, so try to get selected target
            target = PlayerIdentifier::FromTarget(handler);

            if (!target)
            {
                // No character selected
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                return targetPlayer;
            }

            targetPlayer = target->GetConnectedPlayer();
        }
        else
        {
            targetPlayer = target->GetConnectedPlayer();

            if (!targetPlayer || !target->IsConnected())
            {
                // No character selected
                handler->SendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE);
            }
        }

        return targetPlayer;
    }

    static int16 ResetItemsEquipped(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
        {
            Item* pItem = playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pItem)
            {
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        return count;
    }

    static int16 ResetItemsInBags(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        // Default bagpack :
        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
        {
            Item* pItem = playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pItem)
            {
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        // Bag slots
        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
        {
            Bag* pBag = (Bag*)playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pBag)
            {
                for (uint8 j = 0; j < pBag->GetBagSize(); ++j)
                {
                    Item* pItem = pBag->GetItemByPos(j);
                    if (pItem)
                    {
                        playerTarget->DestroyItem(i, j, true);
                        ++count;
                    }
                }
            }
        }

        return count;
    }

    static int16 ResetItemsInBank(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        // Normal bank slot
        for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
        {
            Item* pItem = playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pItem)
            {
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        // Bank bagslots
        for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
        {
            Bag* pBag = (Bag*)playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pBag)
            {
                for (uint8 j = 0; j < pBag->GetBagSize(); ++j)
                {
                    Item* pItem = pBag->GetItemByPos(j);
                    if (pItem)
                    {
                        playerTarget->DestroyItem(i, j, true);
                        ++count;
                    }
                }
            }
        }

        return count;
    }

    static int16 ResetItemsInKeyring(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        for (uint8 i = KEYRING_SLOT_START; i < KEYRING_SLOT_END; ++i)
        {
            Item* pItem = playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pItem)
            {
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        return count;
    }

    static int16 ResetItemsInCurrenciesList(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        for (uint8 i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; ++i)
        {
            Item* pItem = playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pItem)
            {
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        return count;
    }

    static int16 ResetItemsInVendorBuyBackTab(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        for (uint8 i = BUYBACK_SLOT_START; i < BUYBACK_SLOT_END; ++i)
        {
            Item* pItem = playerTarget->GetItemFromBuyBackSlot(i);
            if (pItem)
            {
                playerTarget->RemoveItemFromBuyBackSlot(i, true);
                ++count;
            }
        }

        return count;
    }

    static int16 ResetItemsDeleteStandardBags(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        // Standard bag slots
        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
        {
            Bag* pBag = (Bag*)playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pBag)
            {
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        return count;
    }

    static int16 ResetItemsDeleteBankBags(Player* playerTarget)
    {
        if (!playerTarget)
        {
            return -1;
        }

        int16 count = 0;
        // Bank bags
        for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
        {
            Bag* pBag = (Bag*)playerTarget->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (pBag)
            {
                // prevent no empty ?
                playerTarget->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                ++count;
            }
        }

        return count;
    }
};

void AddSC_reset_commandscript()
{
    new reset_commandscript();
}
