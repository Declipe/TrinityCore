#include "AditionalData.h"
#include "AccountMgr.h"
#include "AuctionHouseMgr.h"
#include "Chat.h"
#include "GameTime.h"
#include "Log.h"
#include "Player.h"
#include "Transport.h"
#include "Unit.h"
#include "WorldSession.h"

AditionalData::AditionalData(Player* player)
{
    pPlayer = player;
    _clean();
}

AditionalData::~AditionalData()
{
    pPlayer = nullptr;
    _clean();
}

void AditionalData::_clean() {
    m_vip = false;
    m_premiumTimer = 0;
    m_unsetdate = 0;
}

void AditionalData::update(uint32 p_time)
{
    if (m_vip && m_premiumTimer > 0)
    {
        if (p_time >= m_premiumTimer)
        {
            time_t currentGameTime = GameTime::GetGameTime();
            time_t unset = getPremiumUnsetdate();
            if (unset <= currentGameTime)
            {
                setPremiumStatus(false);
                setPremiumUnsetdate(0);
                AccountMgr::RemoveVipStatus(pPlayer->GetSession()->GetAccountId());
                ChatHandler(pPlayer->GetSession()).PSendSysMessage(pPlayer->GetSession()->GetTrinityString(LANG_PLAYER_VIP_TIME_EXPIRED));
            }
            else
            {
                time_t diff = unset - currentGameTime;
                time_t days = diff / DAY;
                if (days < 1)
                {
                    time_t hours = diff % DAY / HOUR;
                    if (hours < 1)
                    {
                        time_t minutes = diff % HOUR / MINUTE;
                        //time_t unsetsec = diff % 60;
                        if (minutes == 5)
                            ChatHandler(pPlayer->GetSession()).PSendSysMessage(pPlayer->GetSession()->GetTrinityString(LANG_PLAYER_VIP_TIME_NEAR_END));
                        if (minutes < 5)
                            ChatHandler(pPlayer->GetSession()).PSendSysMessage(pPlayer->GetSession()->GetTrinityString(LANG_PLAYER_VIP_TIME_EXIST), (secsToTimeString(diff, TimeFormat::ShortText)));
                    }
                }
                m_premiumTimer = 1000 * MINUTE;
            }
        }
        else
            m_premiumTimer -= p_time;
    }
}

void AditionalData::setPremiumStatus(bool vipstatus)
{
    m_vip = vipstatus;
    if (m_vip)
        m_premiumTimer = 1000 * MINUTE;
    else
        m_premiumTimer = 0;
}