/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "Config.h"
#include "item_upgrade.h"

class item_upgrade_worldscript : public WorldScript
{
public:
    item_upgrade_worldscript() : WorldScript("item_upgrade_worldscript") { }

    void OnConfigLoad(bool reload) override
    {
        if (reload)
        {
            ItemUpgrade::PagedDataMap& pagedData = sItemUpgrade->GetPagedDataMap();
            for (auto& itr : pagedData)
                itr.second.reloaded = true;

            sItemUpgrade->HandleDataReload(false);
        }

        sItemUpgrade->LoadConfig();
        sItemUpgrade->LoadFromDB();

        if (reload)
            sItemUpgrade->HandleDataReload(true);
    }

   // void OnBeforeWorldInitialized()
   // {
   // sItemUpgrade->LoadFromDB();
   // }
};

void AddSC_item_upgrade_worldscript()
{
    new item_upgrade_worldscript();
}
