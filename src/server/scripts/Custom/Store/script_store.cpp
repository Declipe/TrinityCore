#include "Custom/Dcl.h"
#include "StoreManager.h"
#include "ScriptedGossip.h"
#include "ObjectMgr.h"

#define MAX_ENTRY 10000000

class script_store : public CreatureScript
{
public:
    script_store() : CreatureScript("script_store") { }

    struct script_storeAI : public ScriptedAI
    {
        script_storeAI(Creature* me) : ScriptedAI(me) { }

	//script_store() : CreatureScript("script_store") {}

	bool displayInitialMenu(Player* player) {
		player->PlayerTalkClass->ClearMenus();

		//On rйcupиre les points boutiques du joueur et on les affiche
		uint32 coins = Maelstrom::sStoreMgr->GetAccountCoins(player->GetSession()->GetAccountId());
		std::string msg = "You have " + std::to_string(coins) + " store coins";
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, msg.c_str(), GOSSIP_SENDER_MAIN, 0);
		//On rйcupиre la liste des catйgories
		std::map<uint32, Maelstrom::StoreManager::StoreCategory> categories = Maelstrom::sStoreMgr->GetCategories();
		for (auto it = categories.begin(); it != categories.end(); it++){
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, (it->second.m_name).c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + it->first);
		}

		//player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
		return true;
	}
   // bool OnGossipHello(Player* player) override
	bool OnGossipHello(Player* player) override
    {
		return displayInitialMenu(player);
	}

    bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
        uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
        uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        return GossipSelect(player, sender, action);
    }

	bool GossipSelect(Player* player, uint32 sender, uint32 action)
	{

		if (action == 0) {
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			return true;
		}

		if (action == 9000000) {
			displayInitialMenu(player);
			return true;
		}

		if (action >= MAX_ENTRY && action != 9000000) {
			//On rйcupиre l'entry de l'objet а acheter
			uint32 itemEntryToBuy = action - MAX_ENTRY;
			Maelstrom::StoreManager::PurchaseResult purchaseResult = Maelstrom::sStoreMgr->PurchaseItem(player, itemEntryToBuy);
			switch (purchaseResult) {
			case Maelstrom::StoreManager::PURCHASE_RESULT_ERROR: {
				player->GetSession()->SendAreaTriggerMessage("An error occured");
				player->PlayerTalkClass->ClearMenus();
                CloseGossipMenuFor(player);
				return false;
			}
			break;

			case Maelstrom::StoreManager::PURCHASE_RESULT_NOT_ENOUGH_COINS: {
				player->GetSession()->SendAreaTriggerMessage("You don't have enough coins to purchase this item");
				player->PlayerTalkClass->ClearMenus();
                CloseGossipMenuFor(player);
				return false;
			}
			break;

			case Maelstrom::StoreManager::PURCHASE_RESULT_NOT_ENOUGH_FREE_SLOTS: {
				player->GetSession()->SendAreaTriggerMessage("You don't have enough free slots in your bags to purchase this item");
				player->PlayerTalkClass->ClearMenus();
                CloseGossipMenuFor(player);
				return false;
			}
		    break;

			case Maelstrom::StoreManager::PURCHASE_RESULT_SUCCESS: {
				player->GetSession()->SendAreaTriggerMessage("Thank you for your purchase !");
				player->PlayerTalkClass->ClearMenus();
                CloseGossipMenuFor(player);
				return true;
			}
			break;
			}
		}

		player->PlayerTalkClass->ClearMenus();

		//On rйcupиre les points boutiques du joueur et on les affiche
		uint32 coins = Maelstrom::sStoreMgr->GetAccountCoins(player->GetSession()->GetAccountId());
		std::string msgCoins = "You have " + std::to_string(coins) + " store coins";
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, msgCoins.c_str(), GOSSIP_SENDER_MAIN, 0);

		//On rйcupиre la catйgorie demandйe
		uint32 catId = action - GOSSIP_ACTION_INFO_DEF;

		//On rйcupиre tous les objets de la catйgorie en question
		std::map<uint32, Maelstrom::StoreManager::StoreItem> items = Maelstrom::sStoreMgr->GetItems(catId, player->GetAverageItemLevel());
		//ItemTemplateContainer const* itemTemplateContainer = sObjectMgr->GetItemTemplateStore();
       // ItemTemplateContainer const* itemTemplateContainer = sObjectMgr->GetItemTemplateStore();
        ItemTemplateContainer const& itemTemplateContainer = sObjectMgr->GetItemTemplateStore();
        //for (auto const& itemTemplatePair : its)&
		if (!itemTemplateContainer) {
			player->GetSession()->SendAreaTriggerMessage("Error");
			return false;
		}
		//On affiche tous les items en question
		for (auto elem : items){
			Maelstrom::StoreManager::StoreItem currItem = elem.second;
			const ItemLocale* itemsLoc = sObjectMgr->GetItemLocale(currItem.m_itemEntry);
			if (!itemsLoc)
				continue;
			std::string itemName = itemsLoc->Name.at(player->GetSession()->GetSessionDbLocaleIndex());
			std::string priceText = std::to_string(currItem.m_price);
			std::string message = "[" + priceText + "] " + itemName;
			std::string quantityMsg = currItem.m_quantity == 1 ? "" : " (x" + std::to_string(currItem.m_quantity) + ")";
			std::string gossip = "[" + priceText + " coins] " + itemName + quantityMsg;

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossip.c_str(), GOSSIP_SENDER_MAIN, MAX_ENTRY + currItem.m_itemEntry);
		}

		//On ajoute un bouton retour
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back", GOSSIP_SENDER_MAIN, 9000000);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
		return true;
	}

};
CreatureAI* GetAI(Creature* me) const override
{
    return new script_storeAI(me);
}
};

void AddSC_script_store()
{
	new script_store();
}
