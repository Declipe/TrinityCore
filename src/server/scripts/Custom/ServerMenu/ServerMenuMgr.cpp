#include "ServerMenuMgr.h"
#include "Player.h"
#include "Chat.h"
#include <time.h>
#include "Custom/Dcl.h"

#define GTS1 session->GetTrinityString
#define GTS22 session->GetTrinityString2

std::string getString2(std::string string, uint32 number)
{
    return fmt::format("{}{}", string, number);
}

void sServerMenu::OpenBankSlot(Player* player) 
{
    if (!player)
        return;

    //player->PlayerTalkClass->SendCloseGossip();
	player->GetSession()->SendShowBank(player->GetGUID());
}

void sServerMenu::GossipHelloMenu(Player* player)
{
    WorldSession* session = player->GetSession();
   // player->PlayerTalkClass->ClearMenus();

    //sServerMenuMgr->CanOpenMenu(player);

    // bonus system
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(GTS1(LANG_ITEM_CURRENT_COINS), player->GetCoins()).c_str(), GOSSIP_SENDER_MAIN, 1);
   // AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetVipStatusString(player, session).c_str(), GOSSIP_SENDER_MAIN, 3);
      // promo
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ENTER_PROMO_CODE), GOSSIP_SENDER_MAIN, 0, GTS1(LANG_ENTERED_PROMO_CODE_CORRECT), 0, true);
    // Shops
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_MENU_TRADE), GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS22(NOT_USED_7), GOSSIP_SENDER_MAIN, 192);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS22(NOT_USED_11), GOSSIP_SENDER_MAIN, 199, GTS22(NOT_USED_12), 0, false);
    if (player->IsGameMaster())
    {
        if (player->GetSession()->HasPermission(rbac::RBAC_PERM_COMMAND_SERVER_RESTART))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_MENU_CLASS_SKILLS_GM), GOSSIP_SENDER_MAIN, 7);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_MENU_DELIVERY_OF_BAGS), GOSSIP_SENDER_MAIN, 8);

        if (player->GetLevel() < 80)
            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, GTS1(LANG_MENU_LVL_UP), GOSSIP_SENDER_MAIN, 9);
    }

    if (player->IsPlayer())
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_VIP_MENU), GOSSIP_SENDER_MAIN, 10);

    if (player->GetClass() && player->GetLevel() >= 80)
    {
        switch (player->GetClass())
        {
        case CLASS_WARRIOR:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 83);
            break;
        case CLASS_PALADIN:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 84);
            break;
        case CLASS_HUNTER:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 85);
            break;
        case CLASS_ROGUE:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 86);
            break;
        case CLASS_PRIEST:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 87);
            break;
        case CLASS_DEATH_KNIGHT:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 88);
            break;
        case CLASS_SHAMAN:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 89);
            break;
        case CLASS_MAGE:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 90);
            break;
        case CLASS_WARLOCK:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 91);
            break;
        case CLASS_DRUID:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_START_PACK_ITEM), GOSSIP_SENDER_MAIN, 92);
            break;
        }
    }

    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_SERVER_MENU), GOSSIP_SENDER_MAIN, 11);
    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
    return;
}

bool sServerMenu::CanOpenMenu(Player* player)
{
    if (!player)
        return true;

    if (player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena()
        || player->HasStealthAura() || player->isDead() || (player->GetClass() == CLASS_DEATH_KNIGHT && player->GetMapId() == 609 && !player->IsGameMaster() && !player->HasSpell(50977))) {
        ChatHandler(player->GetSession()).PSendSysMessage(GetCustomText(player, "?????? ??? ??????????.", "Now it is impossible"));
        return true;
    }
    return false;
}
