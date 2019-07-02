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

class ZynPlayerScripts: public PlayerScript
{
    public:
        ZynPlayerScripts() : PlayerScript("ZynPlayerScripts") { }

        void OnCreatureKill(Player* killer, Creature* killed)
        {

            CreatureSpecialRewards reward = sObjectMgr->GetSpecialReward(killed->GetEntry());
            if (reward.type >= CSR_TYPE_ITEM && reward.type <= CSR_TYPE_ARENA)
            {
                switch (reward.type)
                {

                case CSR_TYPE_ITEM:
                    killer->AddItem(reward.param1, reward.param2);
                    break;

                case CSR_TYPE_HONOR:
                    killer->SetHonorPoints(killer->GetHonorPoints() + reward.param1);
                    break;

                case CSR_TYPE_ARENA:
                    killer->SetArenaPoints(killer->GetArenaPoints() + reward.param1);
                    break;
                }
            }
        }
};

class item_lvlup : public ItemScript
{
public: item_lvlup() : ItemScript("item_lvlup") {}

        bool OnUse(Player* player, Item* item, SpellCastTargets const& /*Targets*/)
        {
            WorldSession* session = player->GetSession();
            if (player->GetLevel() != 255)
            {
                if (player->IsInFlight() || player->IsInCombat() || player->isDead())
                {
                    player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, NULL);
                    return true;
                }
                else
                {
                    int32 level = player->GetLevel();
                    player->GiveLevel(player->GetSession()->GetPlayer()->GetLevel() + 1);
                    player->DestroyItemCount(item->GetEntry(), 1, true);
                   // ChatHandler(session).PSendSysMessage(LANG_SAY_LVL_UP_ITEM);
                    return true;
                }
                return true;
            }
            else
            {
               // ChatHandler(session).PSendSysMessage(LANG_ERROR_LVL_UP_ITEM);
                return true;
            }
            return true;
        }

};

void AddSC_ZynPlayerScripts()
{
    new item_lvlup();
    new ZynPlayerScripts();
}
