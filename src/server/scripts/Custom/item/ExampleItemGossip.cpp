#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"

class example_ItemGossip : public ItemScript
{
public:
    example_ItemGossip() : ItemScript("example_ItemGossip") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override // Any hook here
    {
        ClearGossipMenuFor(player); // Clears old options
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Morph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demorph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demorp", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demor", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Demo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Dem", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "De", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D1", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D2", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D3", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D4", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D6", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D7", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "D8", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
        return false; // Cast the spell on use normally
    }

    void OnGossipSelect(Player* player, Item* /*item*/, uint32 /*sender*/, uint32 action) override
    {
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

class item_lv : public ItemScript
{
public: item_lv() : ItemScript("item_lv") {}

      bool OnUse(Player* player, Item* item, SpellCastTargets const& /*Targets*/)
      {
          if (player->IsPlayer())
          {
              if (player->IsInFlight() || player->IsInCombat() || player->isDead())
              {
                  player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, NULL);
                  return true;
              }
              else
              {
                  player->LearnSpell(46917, false);
                  player->DestroyItemCount(item->GetEntry(), 1, true);
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

void AddSC_example_ItemGossip() // Add to scriptloader normally
{
    new example_ItemGossip();
    new item_lv();
}
