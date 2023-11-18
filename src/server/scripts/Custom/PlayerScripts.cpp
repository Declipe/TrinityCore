#include "Custom/Dcl.h"  

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
