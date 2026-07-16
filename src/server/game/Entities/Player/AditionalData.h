#ifndef AditionalData_h__
#define AditionalData_h__

#include "Define.h"

class Player;
class Transport;

class TC_GAME_API AditionalData
{
public:
    AditionalData(Player* player);
    ~AditionalData();

private:
    void _clean();
public:
    void update(uint32 p_time);

    // VIP
    void setPremiumStatus(bool vipstatus);
    bool isPremium() const { return m_vip; }
    void setPremiumUnsetdate(time_t unsetdate) { m_unsetdate = unsetdate; }
    time_t getPremiumUnsetdate() const { return m_unsetdate; }

private:
    Player* pPlayer;

    // VIP
    bool m_vip;                 // Used for VIP func
    uint32 m_premiumTimer;
    time_t m_unsetdate;         // time (unixtime) of unsetdate vip previlegies
};

#endif // AditionalData_h__
