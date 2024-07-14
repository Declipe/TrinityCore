/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "item_upgrade.h"

class item_upgrade_itemscript : public ItemScript
{
public:
    item_upgrade_itemscript() : ItemScript("item_upgrade_itemscript") {}

    bool CanItemRemove(Player* player, Item* item) override
    {
        sItemUpgrade->HandleItemRemove(player, item);
        return true;
    }
};

void AddSC_item_upgrade_itemscript()
{
    new item_upgrade_itemscript();
}
