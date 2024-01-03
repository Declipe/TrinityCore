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

#ifndef PROMOTIONCODE_MGR_H
#define PROMOTIONCODE_MGR_H

#include "ObjectGuid.h"
#include <unordered_map>

struct PromotionCodes
{
    uint32 collection;
    std::string code;
    uint32 honor;
    uint32 arena;
    uint32 money;
    uint32 item_1;
    uint32 item_2;
    uint32 item_3;
    uint32 item_count_1;
    uint32 item_count_2;
    uint32 item_count_3;
    uint32 aura;
    uint32 spell_1;
    uint32 spell_2;
    uint32 spell_3;
    uint32 coin;
    uint32 exist_count;
};
typedef std::unordered_map<uint32, PromotionCodes> PromotionCodesContainer;

struct PromoHistory
{
    uint32 codeId;
    uint32 accountId;
    uint32 playerGUID;
    std::string code;
    time_t time;
};
typedef std::unordered_map<uint32, PromoHistory> PromotionHistoryContainer;

class Player;

class TC_GAME_API PromotionCodeMgr
{
    private:
        PromotionCodeMgr() { }
        ~PromotionCodeMgr();

    public:
        static PromotionCodeMgr* instance();

        void Initialize();
        void ReloadCodes();

        //Promo codes start
        PromotionCodes const* GetPromoCode(uint32 id) const
        {
            PromotionCodesContainer::const_iterator itr = _promoCodesStore.find(id);
            if (itr == _promoCodesStore.end()) return nullptr;
            return &itr->second;
        }
        PromotionCodes const* GetPromoCode(std::string const& name, uint32& id) const;
        PromotionCodesContainer const& GetPromotionCodesMap() const { return _promoCodesStore; }
        bool AddPromoCode(PromotionCodes& data);
        bool DeletePromoCode(std::string const& name);
        //Promo codes end

        bool CheckedEnteredCodeByPlayer(std::string const& code, Player* player, uint32 collection = 0);

    protected:
        void _LoadPromoCodes();
        void _LoadPromoCodesHistory();

        uint32 _TryToRewardForCode(std::string const& code, Player* player, uint32 collection = 0);
        bool _UpdateCountOfExistPromoCode(uint32 id, Player* player);

        bool _AddCodeInHistory(uint32 id, std::string const& code, Player* player);
        bool _CanUseCode(std::string const& code, ObjectGuid::LowType plrGUID);

    private:
        PromotionCodesContainer _promoCodesStore;
        PromotionHistoryContainer _promoHistoryStore;
};

#define sPromotionCodeMgr PromotionCodeMgr::instance()

#endif
