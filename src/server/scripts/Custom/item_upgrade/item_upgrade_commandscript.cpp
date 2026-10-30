/*
 * Credits: silviu20092
 */

#include "Chat.h"
#include "GameTime.h"
#include "item_upgrade.h"
#include "RBAC.h"
#include "ScriptMgr.h"
#include <unordered_map>

using namespace Trinity::ChatCommands;

class item_upgrade_commandscript : public CommandScript
{
public:
    item_upgrade_commandscript() : CommandScript("item_upgrade_commandscript") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable itemUpgradeSubcommandTable =
        {
            { "reload", HandleReloadModItemUpgrade, rbac::RBAC_ROLE_GAMEMASTER, Console::Yes },
            { "lock",   HandleLockItemUpgrade,      rbac::RBAC_ROLE_GAMEMASTER, Console::Yes },
            { "list",   HandleListUpgrades,         rbac::RBAC_ROLE_PLAYER,      Console::No }
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
        handler->SendSysMessage(ItemUpgrade::Localize(handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr, "Item Upgrade NPC is now locked, it is now safe to edit database tables. Release the lock by using .item_upgrade reload command"));
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
            handler->SendSysMessage(fmt::format("{}{}{}", ItemUpgrade::Localize(player, "Please try again in "), (LIST_UPGRADES_COOLDOWN_MS - timeSinceLastRequest) / 1000, ItemUpgrade::Localize(player, " seconds.")));
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
            handler->SendSysMessage(fmt::format("{}{}{}]", ItemUpgrade::ItemLink(player, item), ItemUpgrade::Localize(player, " [equipment slot "), slot));
            for (const ItemUpgrade::UpgradeStat* upgrade : upgrades)
                handler->SendSysMessage(fmt::format("{}{}{}% [{} {}]", sItemUpgrade->StatTypeToString(upgrade->statType, player), ItemUpgrade::Localize(player, " upgraded by "), upgrade->statModPct, ItemUpgrade::Localize(player, "RANK"), upgrade->statRank));
        }

        if (!upgradedItemCount)
            handler->SendSysMessage(player->GetName() + ItemUpgrade::Localize(player, " does not have any upgraded equipped items."));
        else
            handler->SendSysMessage(player->GetName() + ItemUpgrade::Localize(player, " has ") + std::to_string(upgradedItemCount) + ItemUpgrade::Localize(player, " upgraded equipped item(s) and ") + std::to_string(upgradedStatCount) + ItemUpgrade::Localize(player, " upgraded stat(s)."));

        return true;
    }
};

void AddSC_item_upgrade_commandscript()
{
    new item_upgrade_commandscript();
}
