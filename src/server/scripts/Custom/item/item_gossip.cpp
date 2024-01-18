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

void AddSC_ItemUse_item_custom()
{
    new ItemUse_item_custom();
}
