#include "Config.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "ScriptedGossip.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "Group.h"
#include "AchievementMgr.h"
#include "ObjectAccessor.h"
#include "Unit.h"
#include "SharedDefines.h"
#include "Creature.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Language.h"
#include "Chat.h"
#include "Channel.h"
#include "MapManager.h"
#include "CreatureTextMgr.h"
#include "SmartScriptMgr.h"
#include "ZynDatabase.h"
#include "DatabaseEnvFwd.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "GameTime.h"
#include "DBCStructure.h"
#include "GameEventMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "MotionMaster.h"
#include "ScriptedGossip.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Item.h"
#include "ItemDefines.h"
#include "ItemEnchantmentMgr.h"
#include "ItemTemplate.h"  

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
