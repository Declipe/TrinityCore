/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "Chat.h"
#include "item_upgrade.h"
#include "RBAC.h"
#include "GameTime.h"
#include <unordered_map>

using namespace Trinity::ChatCommands;

class item_upgrade_commandscript : public CommandScript
{
public:
    item_upgrade_commandscript() : CommandScript("item_upgrade_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable itemUpgradeSubcommandTable =
        {
            { "reload", HandleReloadModItemUpgrade, rbac::RBAC_ROLE_GAMEMASTER, Console::Yes },
            { "lock",   HandleLockItemUpgrade,      rbac::RBAC_ROLE_GAMEMASTER, Console::Yes },
            { "list",   HandleListUpgrades,         rbac::RBAC_ROLE_PLAYER,      Console::No  }
        };

        static ChatCommandTable itemUpgradeCommandTable =
        {
            { "item_upgrade", itemUpgradeSubcommandTable }
        };

        return itemUpgradeCommandTable;
    }
private:
    static bool HandleReloadModItemUpgrade(ChatHandler* handler)
    {
        ItemUpgrade::PagedDataMap& pagedData = sItemUpgrade->GetPagedDataMap();
        for (auto& itr : pagedData)
            itr.second.reloaded = true;

        sItemUpgrade->SetReloading(true);
        sItemUpgrade->HandleDataReload(false);

        sItemUpgrade->LoadFromDB(true);

        sItemUpgrade->HandleDataReload(true);
        sItemUpgrade->SetReloading(false);

        handler->SendGlobalGMSysMessage("Item Upgrade module data successfully reloaded.");
        return true;
    }

    static bool HandleLockItemUpgrade(ChatHandler* handler)
    {
        sItemUpgrade->SetReloading(true);
        handler->SendSysMessage("Item Upgrade NPC is now locked, it is now safe to edit database tables. Release the lock by using .item_upgrade reload command");
        return true;
    }

    static bool HandleListUpgrades(ChatHandler* handler)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr;
        if (!player)
            return false;

        static std::unordered_map<uint32, uint32> lastListTimeByPlayer;
        static constexpr uint32 LIST_UPGRADES_COOLDOWN_MS = 10000;
        uint32 currentTime = getMSTime();
        uint32& lastTime = lastListTimeByPlayer[player->GetGUID().GetCounter()];
        uint32 timeSinceLastRequest = getMSTimeDiff(lastTime, currentTime);
        if (lastTime != 0 && timeSinceLastRequest < LIST_UPGRADES_COOLDOWN_MS)
        {
            handler->PSendSysMessage("Please try again in {} seconds.", (LIST_UPGRADES_COOLDOWN_MS - timeSinceLastRequest) / 1000);
            return true;
        }
        lastTime = currentTime;

        uint32 upgradedItemCount = 0;
        uint32 upgradedStatCount = 0;
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!item)
                continue;

            std::vector<const ItemUpgrade::UpgradeStat*> upgrades = sItemUpgrade->FindUpgradesForItem(player, item);
            if (upgrades.empty())
                continue;

            ++upgradedItemCount;
            upgradedStatCount += upgrades.size();
            handler->PSendSysMessage("{} [equipment slot {}]", ItemUpgrade::ItemLink(player, item), slot);
            for (const ItemUpgrade::UpgradeStat* upgrade : upgrades)
                handler->PSendSysMessage("{} upgraded by {}% [rank {}]", sItemUpgrade->StatTypeToString(upgrade->statType), upgrade->statModPct, upgrade->statRank);
        }

        if (!upgradedItemCount)
            handler->PSendSysMessage("{} does not have any upgraded equipped items.", player->GetName());
        else
            handler->PSendSysMessage("{} has {} upgraded equipped item(s) and {} upgraded stat(s).", player->GetName(), upgradedItemCount, upgradedStatCount);

        return true;
    }
};

void AddSC_item_upgrade_commandscript()
{
    new item_upgrade_commandscript();
}
