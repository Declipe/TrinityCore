/*
 * Credits: silviu20092
 */

#include "ItemTemplate.h"
#include "item_upgrade_config.h"
#include "Config.h"

ItemUpgradeConfig::ItemUpgradeConfig()
{
    memset(boolConfigs, 0, sizeof(boolConfigs));
    memset(floatConfigs, 0, sizeof(floatConfigs));
    memset(intConfigs, 0, sizeof(intConfigs));
}

void ItemUpgradeConfig::Initialize()
{
    boolConfigs[CONFIG_ITEM_UPGRADE_ENABLED] = sConfigMgr->GetBoolDefault("ItemUpgrade.Enable", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_SEND_PACKETS] = sConfigMgr->GetBoolDefault("ItemUpgrade.SendUpgradedItemsPackets", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_ALLOW_PURGE] = sConfigMgr->GetBoolDefault("ItemUpgrade.AllowUpgradesPurge", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_REFUND_ALL_ON_PURGE] = sConfigMgr->GetBoolDefault("ItemUpgrade.RefundAllOnPurge", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES] = sConfigMgr->GetBoolDefault("ItemUpgrade.RandomUpgradesOnLoot", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_BUY] = sConfigMgr->GetBoolDefault("ItemUpgrade.RandomUpgradeWhenBuying", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOOT] = sConfigMgr->GetBoolDefault("ItemUpgrade.RandomUpgradeWhenLooting", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_WIN] = sConfigMgr->GetBoolDefault("ItemUpgrade.RandomUpgradeWhenWinning", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_QUEST_REWARD] = sConfigMgr->GetBoolDefault("ItemUpgrade.RandomUpgradeOnQuestReward", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CRAFTING] = sConfigMgr->GetBoolDefault("ItemUpgrade.RandomUpgradeWhenCrafting", true);

    stringConfigs[CONFIG_ITEM_UPGRADE_ALLOWED_STATS] = sConfigMgr->GetStringDefault("ItemUpgrade.AllowedStats", "0,3,4,5,6,7,32,36,45");
    stringConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOGIN_MSG] = sConfigMgr->GetStringDefault("ItemUpgrade.RandomUpgradesBroadcastLoginMsg", "");

    floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] = sConfigMgr->GetFloatDefault("ItemUpgrade.RandomUpgradeChance", 2.0f);
    if (floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] <= 0.0f)
        floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] = 2.0f;
    else if (floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] > 100.0f)
        floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] = 100.0f;

    intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN] = sConfigMgr->GetIntDefault("ItemUpgrade.UpgradePurgeToken", 0);
    if (intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN] < 0)
        intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN] = 0;
    intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN_COUNT] = sConfigMgr->GetIntDefault("ItemUpgrade.UpgradePurgeTokenCount", 1);
    if (intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN_COUNT] < 1)
        intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN_COUNT] = 1;
    intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] = sConfigMgr->GetIntDefault("ItemUpgrade.RandomUpgradeMaxStatCount", 2);
    if (intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] <= 0)
        intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] = 2;
    else if (intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] > MAX_ITEM_PROTO_STATS)
        intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] = MAX_ITEM_PROTO_STATS;
    intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_RANK] = sConfigMgr->GetIntDefault("ItemUpgrade.RandomUpgradeMaxRank", 3);
    if (intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_RANK] <= 0)
        intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_RANK] = 3;
}

bool ItemUpgradeConfig::GetBoolConfig(ItemUpgradeBoolConfigs index) const
{
    return boolConfigs[index];
}

std::string ItemUpgradeConfig::GetStringConfig(ItemUpgradeStringConfigs index) const
{
    return stringConfigs[index];
}

float ItemUpgradeConfig::GetFloatConfig(ItemUpgradeFloatConfigs index) const
{
    return floatConfigs[index];
}

int32 ItemUpgradeConfig::GetIntConfig(ItemUpgradeIntConfigs index) const
{
    return intConfigs[index];
}
