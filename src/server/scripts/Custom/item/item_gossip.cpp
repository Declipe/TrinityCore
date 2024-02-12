#include "Custom/Dcl.h"

class ItemUse_item_custom : public ItemScript
{
public:

    ItemUse_item_custom() : ItemScript("item_custom") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)

    {
        if ((player->IsInCombat()) || (player->IsInFlight()) || (player->isDead()))
        {
            player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, nullptr);
            return false;
        }
        if (player->IsMounted())
        {
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, nullptr);
            return true;
        }
        {
           // player->SummonCreature(6, player->GetPositionX() + 1, player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12s);
            player->SummonCreature(6, player->GetPositionX() + 10, player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000s);
        }
        return false;
    }
};

class rip : public ItemScript
{
public:
    rip() : ItemScript("rip") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
    {
        if (player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->isDead())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Вы не можете использовать это сейчас!");
            return false;
        }

        // кенарий player->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(609), 25);
        if (player->GetReputationRank(70) < REP_NEUTRAL)
            player->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(70), 10000);
        else
            player->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(70), 25);
        player->DestroyItemCount(item->GetEntry(), 1, true);

        return true;
    }
};

void AddSC_ItemUse_item_custom()
{
    new ItemUse_item_custom();
    new rip();
}
