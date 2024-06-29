#include "AccountMgr.h"
#include "ArenaTeamMgr.h"
#include "CellImpl.h"
#include "CharacterCache.h"
#include "ChaseMovementGenerator.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DisableMgr.h"
#include "FollowMovementGenerator.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "GroupMgr.h"
#include "InstanceSaveMgr.h"
#include "Item.h"
#include "Language.h"
#include "LFG.h"
#include "Log.h"
#include "MapManager.h"
#include "MMapFactory.h"
#include "MovementGenerator.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "Realm.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "Transport.h"
#include "Weather.h"
#include "WeatherMgr.h"
#include "World.h"
#include "WorldSession.h"

#define GTS2 session->GetTrinityString2

class gm_hello : public PlayerScript
{
public:
    gm_hello() : PlayerScript("gm_hello") {}

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
        WorldSession* session = player->GetSession();

        if (player->GetSession()->GetSecurity() == SEC_MODERATOR)
        {
            std::string sText = (GTS2(NOT_USED_17) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_23));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
        if (player->GetSession()->GetSecurity() == SEC_TESTER)
        {
            std::string sText = (GTS2(NOT_USED_18) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_23));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
        if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)
        {
            std::string sText = (GTS2(NOT_USED_19) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_23));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
        if (player->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)
        {
            std::string sText = (GTS2(NOT_USED_20) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_23));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
        if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
        {
            std::string sText = (GTS2(NOT_USED_21) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_23));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
        if (player->GetSession()->GetSecurity() == SEC_KURATOR)
        {
            std::string sText = (GTS2(NOT_USED_22) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_23));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
    }

    void OnLogout(Player* player)
    {
        WorldSession* session = player->GetSession();

        if (player->GetSession()->GetSecurity() == SEC_MODERATOR)
        {
            std::string sText = (GTS2(NOT_USED_17) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_24));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }

        if (player->GetSession()->GetSecurity() == SEC_TESTER)
        {
            std::string sText = (GTS2(NOT_USED_18) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_24));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }

        if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)
        {
            std::string sText = (GTS2(NOT_USED_19) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_24));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }

        if (player->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)
        {
            std::string sText = (GTS2(NOT_USED_20) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_24));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }

        if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
        {
            std::string sText = (GTS2(NOT_USED_21) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_24));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }

        if (player->GetSession()->GetSecurity() == SEC_KURATOR)
        {
            std::string sText = (GTS2(NOT_USED_22) + std::string(player->GetSession()->GetPlayerName()) + GTS2(NOT_USED_24));
            sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
        }
    }
};

void AddSC_gm_hello()
{
    new gm_hello();
}
