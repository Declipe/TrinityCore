#include "StoreManager.h"
#include "WorldSocket.h"
#include "BigNumber.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "CryptoHash.h"
#include "CryptoRandom.h"
#include "IPLocation.h"
#include "Opcodes.h"
#include "PacketLog.h"
#include "Random.h"
#include "RBAC.h"
#include "Realm.h"
#include "ScriptMgr.h"
#include "World.h"
#include "WorldSession.h"
#include <memory>


using namespace Maelstrom;

StoreManager::StoreManager() : m_categories(), m_items() {}

void StoreManager::LoadStore(){
	TC_LOG_INFO("server.loading", ">> Store loading...");
	LoadCategories();
	LoadItems();
	TC_LOG_INFO("server.loading", ">> Store successfully loaded");
}

uint32 StoreManager::GetAccountCoins(uint32 accountId) const {
	QueryResult result = LoginDatabase.PQuery("SELECT coins FROM account WHERE id={}", accountId);
	if (!result)
		return 0;

	Field* field = result->Fetch();
	return field[0].GetUInt32();
}

void StoreManager::SetAccountCoins(uint32 accountId, uint32 newCoinsValue) {
	LoginDatabase.PExecute("UPDATE account SET coins={} WHERE id={}", newCoinsValue, accountId);
}


std::map<uint32, StoreManager::StoreCategory> StoreManager::GetCategories() const {
	return m_categories;
}

std::map<uint32, StoreManager::StoreItem> StoreManager::GetItems() const {
	return m_items;
}

std::map<uint32, StoreManager::StoreItem> StoreManager::GetItems(uint32 catId, uint32 playerAvgItemLevel) const {
	std::map<uint32, StoreManager::StoreItem> itemsToReturn;
	for (auto it = m_items.begin(); it != m_items.end(); it++){
		if (it->second.m_catId != catId)
			continue;
		if (it->second.m_requiredAvgItemLevel > playerAvgItemLevel)
			continue;

		itemsToReturn.insert(std::make_pair(it->first, it->second));
	}

	return itemsToReturn;
}

StoreManager::PurchaseResult StoreManager::PurchaseItem(Player* player, uint32 itemEntry) {
	if (player == nullptr)
		return PURCHASE_RESULT_ERROR;

	auto it = std::find_if(m_items.begin(), m_items.end(), [=](std::pair<uint32, StoreItem> pair) {
		if (pair.second.m_itemEntry == itemEntry)
			return true;
		return false;
	});

	if (it == m_items.end())
		return PURCHASE_RESULT_ERROR;

	uint32 price = it->second.m_price;
	uint32 coins = GetAccountCoins(player->GetSession()->GetAccountId());

	if (price > coins)
		return PURCHASE_RESULT_NOT_ENOUGH_COINS;

	bool couldAddItem = player->AddItem(itemEntry, it->second.m_quantity);
	if (!couldAddItem) {
		return PURCHASE_RESULT_NOT_ENOUGH_FREE_SLOTS;
	}
	else {
		SetAccountCoins(player->GetSession()->GetAccountId(), GetAccountCoins(player->GetSession()->GetAccountId()) - price);
		return PURCHASE_RESULT_SUCCESS;
	}
}

//-----------------------------------------------------------------------------------------------------

void StoreManager::LoadCategories() {
	QueryResult result = CharacterDatabase.PQuery("SELECT * FROM store_categories");
	if (!result)
		return;

	do {

		Field* field = result->Fetch();
		m_categories.insert(std::make_pair(field[0].GetUInt32(), StoreCategory(field[0].GetUInt32(), field[1].GetString())));

	} while (result->NextRow());
}

void StoreManager::LoadItems() {
	QueryResult result = CharacterDatabase.PQuery("SELECT * FROM store_items ORDER BY price");
	if (!result)
		return;

	do {

		Field* field = result->Fetch();
		m_items.insert(std::make_pair(field[0].GetUInt32(), StoreItem(field[0].GetUInt32(), field[1].GetUInt32(), field[2].GetUInt32(), field[3].GetUInt32(), field[4].GetUInt32(), field[5].GetUInt32())));

	} while (result->NextRow());
}
