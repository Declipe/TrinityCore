#include "Define.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"

#define MENU_ID 123 // Our menuID used to match the sent menu to select hook (playerscript)

class example_PlayerGossip : public PlayerScript
{
public:
    example_PlayerGossip() : PlayerScript("example_PlayerGossip") {}

    void OnLevelChanged(Player* player, uint8 /*oldlevel*/) override            // Any hook here
    {
        ClearGossipMenuFor(player);                              // Clears old options
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Morph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demorph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demorp", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demor", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Dem", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "De", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D1", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D2", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D3", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D4", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D6", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D7", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
        //AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D8", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
        // SetMenuId must be after clear menu and before send menu!!
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(MENU_ID);        // Sets menu ID so we can identify our menu in Select hook. Needs unique number for the menu
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
    }

    void OnGossipSelect(Player* player, uint32 menu_id, uint32 /*sender*/, uint32 action) override
    {
        if (menu_id != MENU_ID) // Not the menu coded here? stop.
            return;
        ClearGossipMenuFor(player);

        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->SetDisplayId(999);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->DeMorph();
            break;
        }
        CloseGossipMenuFor(player);
    }
};

void AddSC_example_PlayerGossip() // Add to scriptloader normally
{
    new example_PlayerGossip();
}
