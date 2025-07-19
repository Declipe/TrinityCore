#include "ScriptMgr.h"
#include "Player.h"
#include "AccountMgr.h"
#include "Common.h"
#include "Configuration/Config.h"
#include <vector>
#include <cstdlib>

class Hyjal_AutoRevive : public PlayerScript
{
public:
    Hyjal_AutoRevive() : PlayerScript("Hyjal_AutoRevive") {}

    void OnUpdate(Player* player, uint32 /*diff*/)
    {  
        if (AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            return;

        if (player->GetHealth() <= 1)
        {
            if (player->GetZoneId() == 616)
            {
                player->ResurrectPlayer(1.0f);
                player->SetHealth(player->GetMaxHealth());

                teleportToRandomLocation(player);
                player->SaveToDB();
            }
        }
    }

private:
    void teleportToRandomLocation(Player* player)
    {
        std::vector<std::tuple<float, float, float, float>> resurrectionCoordinates = {
        {5063, -2078.97f, 1369.29f, 5.96975f},
	    {4970.97f, -1743.45f, 1342.04f, 1.29307f},
	    {5607.63f, -3054.61f, 1556.31f, 2.04434f},
	    {4518.5f, -2490.53f, 1130.75f, 3.716f},
	    {4216.3f, -2918.71f, 1094.54f, 0.134588f},
	    {5289.57f, -1537.27f, 1366.23f, 2.30076f}
        };

        int randomIndex = std::rand() % resurrectionCoordinates.size();
        auto coordinates = resurrectionCoordinates[randomIndex];

        player->TeleportTo(1, std::get<0>(coordinates), std::get<1>(coordinates), std::get<2>(coordinates), std::get<3>(coordinates));
    }
};

void AddAutoReviveScripts()
{
    new Hyjal_AutoRevive();
}
