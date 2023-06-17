#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "ScriptMgr.h"
#include "Language.h"
#include "ZynDatabase.h"
#include "DatabaseEnvFwd.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "GameTime.h"
#include "SpellMgr.h"
#include "GossipDef.h"
#include "ScriptedGossip.h"
#include "GameObject.h"
#include "GameObjectAI.h"

class level_award : public PlayerScript
{
public:
level_award() : PlayerScript("level_award") {}

void OnLevelChanged(Player* player, uint8 newLevel)
{
    switch (++newLevel)
    {
    case 10:
       // player->CastSpell(player, SPELL, true);
        player->ModifyMoney(100000, true);
        player->AddItem(49426, 10);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 20:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 30:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 40:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 50:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 60:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 70:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 80:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        //CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(TITLE_ID);
        //player->SetTitle(titleInfo);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    }
}
};

void AddSC_level_award()
{
new level_award();
}
