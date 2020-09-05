#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "ArenaTeamMgr.h"
#include "CellImpl.h"
#include "CharacterCache.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DisableMgr.h"
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
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "ChaseMovementGenerator.h"
#include "FollowMovementGenerator.h"
#include "Transport.h"
#include "Weather.h"
#include "WeatherMgr.h"
#include "World.h"
#include "WorldSession.h"

 class gm_hello : public PlayerScript  
 {  
 public:  
        gm_hello() : PlayerScript("gm_hello") {}  

    void OnLogin(Player* player, bool firstLogin)  
     {  
       if (player->GetSession()->GetSecurity() == SEC_MODERATOR)  
              {  
                  std::string sText = ("Модератор '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
if (player->GetSession()->GetSecurity() == SEC_TESTER)  
              {  
                  std::string sText = ("Тестер '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }
    if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)  
              {  
                  std::string sText = ("Эвент-мастер '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
    if (player->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)  
              {  
                  std::string sText = ("Гейм-мастер '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              } 
	if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
	{
		std::string sText = ("Администратор Проекта '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' входит в Игровой мир.");
		sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
	}
    if (player->GetSession()->GetSecurity() == SEC_KURATOR)  
              {  
                  std::string sText = ("Куратор Проекта '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
   }    
      
  void OnLogout(Player* player) 
  {  
     if (player->GetSession()->GetSecurity() == SEC_MODERATOR)  
              {  
                  std::string sText = ("Модератор '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  

     if (player->GetSession()->GetSecurity() == SEC_TESTER)  
              {  
                  std::string sText = ("Тестер '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }
               
     if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)  
              {  
                  std::string sText = ("Эвент-мастер '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }
                
     if (player->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)  
              {  
                  std::string sText = ("Гейм-мастер '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }
               
	if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
	 {
		std::string sText = ("Администратор Проекта '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' выходит из Игрового мира.");
		sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
	 }
	 
    if (player->GetSession()->GetSecurity() == SEC_KURATOR)  
              {  
                  std::string sText = ("Куратор Проекта '|cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r' выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }   
  } 
 };  

 void AddSC_gm_hello()  
 {  
        new gm_hello();  
 }
