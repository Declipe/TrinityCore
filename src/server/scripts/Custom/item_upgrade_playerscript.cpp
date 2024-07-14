/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "DatabaseEnv.h"
#include "Player.h"
#include "item_upgrade.h"
#include "ZynDatabase.h"

class item_upgrade_playerscript : public PlayerScript
{
private:
    class SendUpgradePackets : public BasicEvent
    {
    public:
        SendUpgradePackets(Player* player) : player(player)
        {
            player->m_Events.AddEvent(this, player->m_Events.CalculateTime(3000ms));
        }

        bool Execute(uint64 /*e_time*/, uint32 /*p_time*/)
        {
            sItemUpgrade->UpdateVisualCache(player);
            return true;
        }
    private:
        static constexpr uint64 DELAY_MS = 3000;

        Player* player;
    };
public:
    item_upgrade_playerscript() : PlayerScript("item_upgrade_playerscript") {}

    void OnApplyItemModsBefore(Player* player, uint8 slot, bool /*apply*/, uint8 /*itemProtoStatNumber*/, uint32 statType, int32& val) override
    {
        val = sItemUpgrade->HandleStatModifier(player, slot, statType, val);
    }

    void OnApplyEnchantmentItemModsBefore(Player* player, Item* item, EnchantmentSlot slot, bool /*apply*/, uint32 enchant_spell_id, uint32& enchant_amount) override
    {
        enchant_amount = sItemUpgrade->HandleStatModifier(player, item, enchant_spell_id, enchant_amount, slot);
    }

    void OnAfterMoveItemFromInventory(Player* player, Item* it, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/) override
    {
        sItemUpgrade->HandleItemRemove(player, it);
    }

    void OnDeleteFromDB(CharacterDatabaseTransaction trans, uint32 guid) override
    {
        trans->PAppend("DELETE FROM character_item_upgrade WHERE guid = {}", guid);
        sItemUpgrade->HandleCharacterRemove(guid);
    }

    void OnLogin(Player* player, bool) override
    {
        new SendUpgradePackets(player);

        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_ENABLED) && sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES))
        {
            const std::string& loginMsg = sItemUpgrade->GetStringConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOGIN_MSG);
            if (!loginMsg.empty())
                ItemUpgrade::SendMessage(player, loginMsg);
        }
    }

    void OnLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootguid*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOOT))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnQuestRewardItem(Player* player, Item* item, uint32 /*count*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_QUEST_REWARD))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnCreateItem(Player* player, Item* item, uint32 /*count*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CRAFTING))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnAfterStoreOrEquipNewItem(Player* player, uint32 /*vendorslot*/, Item* item, uint8 /*count*/, uint8 /*bag*/, uint8 /*slot*/, ItemTemplate const* /*pProto*/, Creature* /*pVendor*/, VendorItem const* /*crItem*/, bool /*bStore*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_BUY))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }
};

void AddSC_item_upgrade_playerscript()
{
    new item_upgrade_playerscript();
}
