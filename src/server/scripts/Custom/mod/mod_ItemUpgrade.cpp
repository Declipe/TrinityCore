#include "Custom/Dcl.h"
#include "DBCStructure.h"

enum ItemUpgradeStrings
{
    ARE_YOU_SURE    = 11300, // Are you sure you want to remove the effect?
    NO_EFFECT       = 11301, // no effect
    EFFECT_NOW      = 11302, // Effect ?%i (now: %s)
    EFFECT_REMOVE   = 11303, // Remove effect
};

struct ItemUpgradeTemplate
{
    uint32 enchantId;
    uint32 prevEnchantId;
    std::string description;
   // char const* description;
    uint32 charges;
    uint32 duration;
    uint32 golds;
};

int ItemUpgradeTextAreYouSure = 0;
int ItemUpgradeTextNoEffect = 0;
int ItemUpgradeTextEffectNow = 0;
int ItemUpgradeTextEffectRemove = 0;
bool ItemUpgradeEnable = false;
std::vector<ItemUpgradeTemplate> ItemUpgradeInfo;

class Mod_ItemUpgrade_WorldScript : public WorldScript
{
    public:
        Mod_ItemUpgrade_WorldScript() : WorldScript("Mod_ItemUpgrade_WorldScript") { }

    void LoadDataFromDataBase()
    {
        ItemUpgradeInfo.clear();

        TC_LOG_INFO("misc", "Loading ItemUpgrade...");
        uint32 oldMSTime = getMSTime();

        //QueryResult result = ZynDatabase.PQuery("SELECT `enchant_id`, `prev_enchant_id`, `golds` FROM `world_item_upgrade`");
        ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZynDatabase1);
        PreparedQueryResult result = ZynDatabase.Query(stmt);

        if (!result)
        {
            TC_LOG_ERROR("misc", ">> `world_item_upgrade` is empty");
            return;
        }

        uint16 count = 0;

        do
        {
            Field* fields = result->Fetch();

            ItemUpgradeTemplate ItemUpgradeTemp;

            ItemUpgradeTemp.enchantId       = fields[0].GetUInt32();
            ItemUpgradeTemp.prevEnchantId   = fields[1].GetUInt32();
            ItemUpgradeTemp.description     = fields[2].GetCString();
            ItemUpgradeTemp.golds           = fields[3].GetUInt32() * GOLD;
            ItemUpgradeTemp.charges         = 0;
            ItemUpgradeTemp.duration        = 0;

            SpellItemEnchantmentEntry const* enchantEntry = sDBCMgr->GetSpellItemEnchantmentEntry(ItemUpgradeTemp.enchantId);
            if (!enchantEntry) {
                TC_LOG_ERROR("misc", "Item Upgrade: not exists enchantment id {}", ItemUpgradeTemp.enchantId);
                continue;
            }

          //  for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
            //    if (strlen(enchantEntry->Name[i]))
                    ItemUpgradeTemp.description = fields[2].GetCString();//*/ = enchantEntry->Name[i];

            ItemUpgradeInfo.push_back(ItemUpgradeTemp);
            ++count;
        }
        while (result->NextRow());

        TC_LOG_INFO("misc", ">> Loaded {} count for ItemUpgrade in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    void OnConfigLoad(bool /*reload*/)
    {
        ItemUpgradeTextAreYouSure    = sGameConfig->GetIntConfig("ItemUpgrade.Text.AreYouSure");
        ItemUpgradeTextNoEffect      = sGameConfig->GetIntConfig("ItemUpgrade.Text.NoEffect");
        ItemUpgradeTextEffectNow     = sGameConfig->GetIntConfig("ItemUpgrade.Text.EffectNow");
        ItemUpgradeTextEffectRemove  = sGameConfig->GetIntConfig("ItemUpgrade.Text.EffectRemove");
        //ItemUpgradeEnable            = sConfigMgr->GetBoolDefault("ItemUpgrade.Enable", false);
        ItemUpgradeEnable            = sGameConfig->GetBoolConfig("ItemUpgrade.Enable");

        if (ItemUpgradeEnable)
            LoadDataFromDataBase();
    }

};

class go_item_upgrade : public GameObjectScript
{
    public:
        go_item_upgrade() : GameObjectScript("go_item_upgrade") { }

        struct go_item_upgradeAI : public GameObjectAI
        {
            go_item_upgradeAI(GameObject* go) : GameObjectAI(go) { }
            
    uint16 getSlot(uint32 sender) {
        return (uint16) ((sender - GOSSIP_SENDER_MAIN) >> 16);
    }
    uint16 getEnchant(uint32 sender) {
        return (uint16) ((sender - GOSSIP_SENDER_MAIN) & 0xFFFF);
    }
    uint32 senderValue(uint16 slot, uint16 enchant) {
        return (uint32) (GOSSIP_SENDER_MAIN + ((slot << 16) | (enchant & 0xFFFF)));
    }
 
    bool OnGossipHello(Player* player) override
    {
        if (!ItemUpgradeEnable) {
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
            return true;
        }
        
        LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

        for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
        {
            Item* item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, i);
            WorldSession* session = player->GetSession();

            if (item)
            {
                ItemTemplate const *itemTemplate = item->GetTemplate();
                std::string Name = itemTemplate->Name1;
                if (loc_idx >= 0)
                    if (ItemLocale const* il = sObjectMgr->GetItemLocale(itemTemplate->ItemId))
                        ObjectMgr::GetLocaleString(il->Name, loc_idx, Name);
                
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(item->GetEntry(), 30, 30, -18, 0) + sTransmogrification->GetItemLink(item, session), senderValue(i, 0), GOSSIP_ACTION_INFO_DEF);
               // AddGossipItemFor(player, GOSSIP_ICON_CHAT, Name.c_str(), senderValue(i,0), GOSSIP_ACTION_INFO_DEF);
            }
        }
        SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
        //return false;
        return true;
    }

    bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
           uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
           uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
           return GossipSelect(player, sender, action);
    }

    bool GossipSelect(Player* player, uint32 sender, uint32 action)
    //bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
        //uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
        //uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        if (!ItemUpgradeEnable) {
            CloseGossipMenuFor(player);
            return true;
        }

        player->PlayerTalkClass->ClearMenus();

        uint16 itemSlot = getSlot(sender);
        uint16 itemEnchantSlot = getEnchant(sender);

        if (itemSlot >= EQUIPMENT_SLOT_END) {
            CloseGossipMenuFor(player);
            return false;
        }

        Item* item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, itemSlot);

        if (!item) {
            CloseGossipMenuFor(player);
            return false;
        }

        if (itemEnchantSlot < PROP_ENCHANTMENT_SLOT_0 || itemEnchantSlot >= MAX_ENCHANTMENT_SLOT) {
            //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "<< ...", senderValue(0, 0), GOSSIP_ACTION_INFO_DEF);

            for (uint8 i = PROP_ENCHANTMENT_SLOT_0; i < MAX_ENCHANTMENT_SLOT; ++i) {
                std::string oldEffect = sObjectMgr->GetTrinityStringForDBCLocale(ItemUpgradeTextNoEffect);
                uint32 enchantId = item->GetEnchantmentId(EnchantmentSlot(i));
                if (enchantId != 0)
                {
                    bool isExists = false;

                    for (uint32 j = 0; j < ItemUpgradeInfo.size(); ++j)
                    {
                        if (ItemUpgradeInfo[j].enchantId != enchantId)
                            continue;

                        oldEffect = ItemUpgradeInfo[j].description;
                        isExists = true;
                    }

                    if (!isExists) {
                        SpellItemEnchantmentEntry const* enchantEntry = sDBCMgr->GetSpellItemEnchantmentEntry(enchantId);

                        if (enchantEntry)
                            for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
                                if (strlen(enchantEntry->Name[i]))
                                    oldEffect = enchantEntry->Name[i];
                    }
                }

                char gossipTextFormat[100];
                snprintf(gossipTextFormat, 100, sObjectMgr->GetTrinityStringForDBCLocale(ItemUpgradeTextEffectNow), i - PROP_ENCHANTMENT_SLOT_0 + 1, oldEffect.c_str());
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossipTextFormat, senderValue(itemSlot, i), GOSSIP_ACTION_INFO_DEF);
            }
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
            return true;
        }

        uint32 currentEnchantId = item->GetEnchantmentId(EnchantmentSlot(itemEnchantSlot));

        if (action <= GOSSIP_ACTION_INFO_DEF)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "<< ...", senderValue(itemSlot, 0), GOSSIP_ACTION_INFO_DEF);

            if (currentEnchantId != 0)
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(ItemUpgradeTextEffectRemove), senderValue(itemSlot, itemEnchantSlot), GOSSIP_ACTION_INFO_DEF + 1, sObjectMgr->GetTrinityStringForDBCLocale(ItemUpgradeTextAreYouSure), 100 * GOLD, 0);

            for (uint32 i = 0; i < ItemUpgradeInfo.size(); ++i)
            {
                if (ItemUpgradeInfo[i].prevEnchantId != currentEnchantId)
                    continue;

                AddGossipItemFor(player, GOSSIP_ICON_CHAT, ItemUpgradeInfo[i].description.c_str(), senderValue(itemSlot, itemEnchantSlot), GOSSIP_ACTION_INFO_DEF + ItemUpgradeInfo[i].enchantId, ItemUpgradeInfo[i].description.c_str(), ItemUpgradeInfo[i].golds, 0);
            }

            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
            return true;
        }

        uint32 golds = 0;
        uint32 enchantId = action - GOSSIP_ACTION_INFO_DEF;

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        if (enchantId == 1)
        {
            golds = 100 * GOLD;
            if (!player->HasEnoughMoney(golds))
            {
                player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                CloseGossipMenuFor(player);
                return false;
            }
            player->ApplyEnchantment(item, EnchantmentSlot(itemEnchantSlot), false);
            item->ClearEnchantment(EnchantmentSlot(itemEnchantSlot));
        } else {
            for (uint32 i = 0; i < ItemUpgradeInfo.size(); ++i)
            {
                if (ItemUpgradeInfo[i].prevEnchantId != currentEnchantId)
                    continue;

                if (ItemUpgradeInfo[i].enchantId != enchantId)
                    continue;

                golds = ItemUpgradeInfo[i].golds;

                if (!player->HasEnoughMoney(golds))
                {
                    player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                    CloseGossipMenuFor(player);
                    return false;
                }
                player->ApplyEnchantment(item, EnchantmentSlot(itemEnchantSlot), false);
                item->SetEnchantment(EnchantmentSlot(itemEnchantSlot), ItemUpgradeInfo[i].enchantId, ItemUpgradeInfo[i].duration, ItemUpgradeInfo[i].charges);
                player->ApplyEnchantment(item, EnchantmentSlot(itemEnchantSlot), true);
            }
        }

        if (golds > 0)
        {
            player->ModifyMoney(-(int32)golds);
            player->SaveInventoryAndGoldToDB(trans);
        }
        item->SaveToDB(trans);
        CharacterDatabase.CommitTransaction(trans);
        CloseGossipMenuFor(player);
        return true;
      }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_item_upgradeAI(go);
    }
};

void AddSC_Mod_ItemUpgrade()
{
    new Mod_ItemUpgrade_WorldScript();
    new go_item_upgrade();
}
