/*
 * Copyright (C) 2016-2020 AtieshCore <https://gitlab.com/healthstone/>
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

#include "AccountMgr.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "PromotionCodeMgr.h"
#include "Player.h"
#include "Log.h"
#include "WorldSession.h"
#include <utf8.h>

PromotionCodeMgr* PromotionCodeMgr::instance()
{
    static PromotionCodeMgr instance;
    return &instance;
}

PromotionCodeMgr::~PromotionCodeMgr()
{
    promoCodesStore.clear();
}

void PromotionCodeMgr::Initialize()
{
    LoadPromoCodes();
    LoadPromoCodesHistory();
}

void PromotionCodeMgr::ReloadCodes()
{
    LoadPromoCodes();
}

void PromotionCodeMgr::LoadPromoCodes()
{
    uint32 oldMSTime = getMSTime();

    promoCodesStore.clear();                                  // for reload case

    //                                                0       1        2     3      4      5      6       7       8         9              10           11         12     13       14      15       16    17
    QueryResult result = ZynDatabase.Query("SELECT id, collection, code, honor, arena, money, item_1, item_2, item_3, item_4, item_5, item_6, item_7, item_8, item_9, item_10, item_count_1, item_count_2, item_count_3, item_count_4, item_count_5, item_count_6, item_count_7, item_count_8, item_count_9, item_count_10, aura, spell_1, spell_2, spell_3, coin, count_of_exists FROM promotion_codes");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Promo Codes. DB table `promotion_codes` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();

        PromotionCodes pc;

        pc.collection = fields[1].GetUInt32();
        pc.code = fields[2].GetString();
        pc.honor = fields[3].GetUInt32();
        pc.arena = fields[4].GetUInt32();
        pc.money = fields[5].GetUInt32();
        pc.item_1 = fields[6].GetUInt32();
        pc.item_2 = fields[7].GetUInt32();
        pc.item_3 = fields[8].GetUInt32();
        pc.item_4 = fields[9].GetUInt32();
        pc.item_5 = fields[10].GetUInt32();
        pc.item_6 = fields[11].GetUInt32();
        pc.item_7 = fields[12].GetUInt32();
        pc.item_8 = fields[13].GetUInt32();
        pc.item_9 = fields[14].GetUInt32();
        pc.item_10 = fields[15].GetUInt32();
        pc.item_count_1 = fields[16].GetUInt32();
        pc.item_count_2 = fields[17].GetUInt32();
        pc.item_count_3 = fields[18].GetUInt32();
        pc.item_count_4 = fields[19].GetUInt32();
        pc.item_count_5 = fields[20].GetUInt32();
        pc.item_count_6 = fields[21].GetUInt32();
        pc.item_count_7 = fields[22].GetUInt32();
        pc.item_count_8 = fields[23].GetUInt32();
        pc.item_count_9 = fields[24].GetUInt32();
        pc.item_count_10 = fields[25].GetUInt32();

        pc.aura = fields[26].GetUInt32();
        pc.spell_1 = fields[27].GetUInt32();
        pc.spell_2 = fields[28].GetUInt32();
        pc.spell_3 = fields[29].GetUInt32();
        pc.coin = fields[30].GetUInt32();
        pc.exist_count = fields[31].GetUInt32();

        promoCodesStore[id] = pc;

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} Promotion Codes in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void PromotionCodeMgr::LoadPromoCodesHistory()
{
    uint32 oldMSTime = getMSTime();

    promoHistoryStore.clear();                                  // for reload case

    //                                                0     1      2        3       4        5
    QueryResult result = ZynDatabase.Query("SELECT id, codeID, code, accountID, playerID, use_unix_time FROM promotion_codes_history");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Promo Codes History count. DB table `promotion_codes_history` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();

        PromoHistory ph;
        ph.codeId = fields[1].GetUInt32();
        ph.code = fields[2].GetString();
        ph.accountId = fields[3].GetUInt32();
        ph.playerGUID = fields[4].GetUInt32();
        ph.time = fields[5].GetUInt64();
        promoHistoryStore[id] = ph;

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} Promotion Codes history count in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

PromotionCodes const* PromotionCodeMgr::GetPromoCode(const std::string& name, uint32& id) const
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return nullptr;

    id = 0;

    for (PromotionCodesContainer::const_iterator itr = promoCodesStore.begin(); itr != promoCodesStore.end(); ++itr)
    {
        if (itr->second.code == name)
        {
            id = itr->first;
            return &itr->second;
        }
    }

    return nullptr;
}

bool PromotionCodeMgr::AddPromoCode(PromotionCodes& promo)
{
    // find max id
    uint32 new_id = 0;
    for (PromotionCodesContainer::const_iterator itr = promoCodesStore.begin(); itr != promoCodesStore.end(); ++itr)
        if (itr->first > new_id)
            new_id = itr->first;

    // use next
    ++new_id;

    promoCodesStore[new_id] = promo;

    ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZYN_INS_PROMO_CODE);

    stmt->setUInt32(0, new_id);
    stmt->setUInt32(1, promo.collection);
    stmt->setString(2, promo.code);
    stmt->setUInt32(3, promo.honor);
    stmt->setUInt32(4, promo.arena);
    stmt->setUInt32(5, promo.money);
    stmt->setUInt32(6, promo.item_1);
    stmt->setUInt32(7, promo.item_2);
    stmt->setUInt32(8, promo.item_3);

    stmt->setUInt32(9, promo.item_4);
    stmt->setUInt32(10, promo.item_5);
    stmt->setUInt32(11, promo.item_6);
    stmt->setUInt32(12, promo.item_7);
    stmt->setUInt32(13, promo.item_8);
    stmt->setUInt32(14, promo.item_9);
    stmt->setUInt32(15, promo.item_10);

    stmt->setUInt32(16, promo.item_count_1);
    stmt->setUInt32(17, promo.item_count_2);
    stmt->setUInt32(18, promo.item_count_3);

    stmt->setUInt32(19, promo.item_count_4);
    stmt->setUInt32(20, promo.item_count_5);
    stmt->setUInt32(21, promo.item_count_6);
    stmt->setUInt32(22, promo.item_count_7);
    stmt->setUInt32(23, promo.item_count_8);
    stmt->setUInt32(24, promo.item_count_9);
    stmt->setUInt32(25, promo.item_count_10);

    stmt->setUInt32(26, promo.aura);
    stmt->setUInt32(27, promo.spell_1);
    stmt->setUInt32(28, promo.spell_2);
    stmt->setUInt32(29, promo.spell_3);
    stmt->setUInt32(30, promo.coin);
    stmt->setUInt32(31, promo.exist_count);

    ZynDatabase.Execute(stmt);

    return true;
}

bool PromotionCodeMgr::DeletePromoCode(const std::string& name)
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    for (PromotionCodesContainer::iterator itr = promoCodesStore.begin(); itr != promoCodesStore.end(); ++itr)
    {
        if (itr->second.code == name)
        {
            ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZYN_DEL_PROMO_CODE);

            stmt->setString(0, itr->second.code);

            ZynDatabase.Execute(stmt);

            promoCodesStore.erase(itr);
            return true;
        }
    }

    return false;
}

bool PromotionCodeMgr::UpdateCountOfExistPromoCode(uint32 id, Player* player)
{
    bool result = false;
    std::string code = "";
    for (PromotionCodesContainer::iterator itr = promoCodesStore.begin(); itr != promoCodesStore.end(); ++itr)
    {
        if (itr->first == id)
        {
            if (itr->second.exist_count > 0)
            {
                itr->second.exist_count -= 1;
                code = itr->second.code;

                ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZYN_UPD_PROMO_CODE_USED);
                stmt->setUInt32(0, itr->second.exist_count);
                stmt->setUInt32(1, id);
                ZynDatabase.Execute(stmt);

                result = true;
            }
        }
    }

    if (!result)
        return false;

    return AddCodeInHistory(id, code, player);
}

bool PromotionCodeMgr::CheckedEnteredCodeByPlayer(std::string const& code, Player* player, uint32 collection)
{
    if (!player || !player->GetSession())
        return false;

    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(code, wname))
        return false;

    // check from promotioncode history (1 player can not use 1 code more then 1 time)
    if (!CanUseCode(code, player->GetGUID().GetCounter()))
        return false;

    uint32 codeid = TryToRewardForCode(code, player, collection);
    if (!codeid)
        return false;

    return UpdateCountOfExistPromoCode(codeid, player);
}

uint32 PromotionCodeMgr::TryToRewardForCode(std::string const& code, Player* player, uint32 collection)
{
    uint32 result = 0;
    for (PromotionCodesContainer::const_iterator itr = promoCodesStore.begin(); itr != promoCodesStore.end(); ++itr)
    {
        if (itr->second.code == code && itr->second.exist_count)
        {
            //check on collection (gossipId)
            if (itr->second.collection)
            {
                if (!collection)
                    continue;

                if (collection && collection != itr->second.collection)
                    continue;
            }
            //code exist and count > 0, need to reward a player
            result = itr->first;
            if (itr->second.arena)
                player->ModifyArenaPoints(itr->second.arena);
            if (itr->second.honor)
                player->ModifyHonorPoints(itr->second.honor);
            if (itr->second.item_1)
            {
                uint32 count = 1;
                if (itr->second.item_count_1 && itr->second.item_count_1 > 1)
                    count = itr->second.item_count_1;
                player->AddItem(itr->second.item_1, count);
            }
            if (itr->second.item_2)
            {
                uint32 count = 1;
                if (itr->second.item_count_2 && itr->second.item_count_2 > 1)
                    count = itr->second.item_count_2;
                player->AddItem(itr->second.item_2, count);
            }
            if (itr->second.item_3)
            {
                uint32 count = 1;
                if (itr->second.item_count_3 && itr->second.item_count_3 > 1)
                    count = itr->second.item_count_3;
                player->AddItem(itr->second.item_3, count);
            }

            if (itr->second.item_4)
            {
                uint32 count = 1;
                if (itr->second.item_count_4 && itr->second.item_count_4 > 1)
                    count = itr->second.item_count_4;
                player->AddItem(itr->second.item_4, count);
            }

            if (itr->second.item_5)
            {
                uint32 count = 1;
                if (itr->second.item_count_5 && itr->second.item_count_5 > 1)
                    count = itr->second.item_count_5;
                player->AddItem(itr->second.item_5, count);
            }

            if (itr->second.item_6)
            {
                uint32 count = 1;
                if (itr->second.item_count_6 && itr->second.item_count_6 > 1)
                    count = itr->second.item_count_6;
                player->AddItem(itr->second.item_6, count);
            }

            if (itr->second.item_7)
            {
                uint32 count = 1;
                if (itr->second.item_count_7 && itr->second.item_count_7 > 1)
                    count = itr->second.item_count_7;
                player->AddItem(itr->second.item_7, count);
            }

            if (itr->second.item_8)
            {
                uint32 count = 1;
                if (itr->second.item_count_8 && itr->second.item_count_8 > 1)
                    count = itr->second.item_count_8;
                player->AddItem(itr->second.item_8, count);
            }

            if (itr->second.item_9)
            {
                uint32 count = 1;
                if (itr->second.item_count_9 && itr->second.item_count_9 > 1)
                    count = itr->second.item_count_9;
                player->AddItem(itr->second.item_9, count);
            }


            if (itr->second.item_10)
            {
                uint32 count = 1;
                if (itr->second.item_count_10 && itr->second.item_count_10 > 1)
                    count = itr->second.item_count_10;
                player->AddItem(itr->second.item_10, count);
            }

            if (itr->second.money)
                player->ModifyMoney(itr->second.money);
            if (itr->second.coin)
            {
                uint32 coins = player->GetCoins();
                coins += itr->second.coin;
                player->SetCoins(coins);
                AccountMgr::SetCoins(player->GetSession()->GetAccountId(), coins);
            }
            if (itr->second.spell_1)
                player->LearnSpell(itr->second.spell_1, false, false);
            if (itr->second.spell_2)
                player->LearnSpell(itr->second.spell_2, false, false);
            if (itr->second.spell_3)
                player->LearnSpell(itr->second.spell_3, false, false);
            if (itr->second.aura)
                player->AddAura(itr->second.aura, player);
        }
    }

    return result;
}

bool PromotionCodeMgr::AddCodeInHistory(uint32 id, std::string const& code, Player* player)
{
    // find max id
    uint32 new_id = 0;
    for (PromotionHistoryContainer::const_iterator itr = promoHistoryStore.begin(); itr != promoHistoryStore.end(); ++itr)
        if (itr->first > new_id)
            new_id = itr->first;

    // use next
    ++new_id;

    PromoHistory ph;
    ph.codeId = id;
    ph.code = code;
    ph.accountId = player->GetSession()->GetAccountId();
    ph.playerGUID = player->GetGUID().GetCounter();
    ph.time = GameTime::GetGameTime();
    promoHistoryStore[new_id] = ph;

    ZynDatabasePreparedStatement* stmt = ZynDatabase.GetPreparedStatement(ZYN_INS_PROMO_CODE_HISTORY);

    stmt->setUInt32(0, new_id);
    stmt->setUInt32(1, ph.codeId);
    stmt->setString(2, ph.code);
    stmt->setUInt32(3, ph.accountId);
    stmt->setUInt32(4, ph.playerGUID);
    stmt->setUInt64(5, uint64(ph.time));

    ZynDatabase.Execute(stmt);

    return true;
}

bool PromotionCodeMgr::CanUseCode(std::string const& code, ObjectGuid::LowType plrGUID)
{
    //uint32 useCount = 0;
    bool result = true;
    for (PromotionHistoryContainer::const_iterator itr = promoHistoryStore.begin(); itr != promoHistoryStore.end(); ++itr)
    {
        if (itr->second.playerGUID == plrGUID && itr->second.code == code)
            result = false;
            //useCount++;
    }

    /*return useCount < 2;*/
    return result;
}
