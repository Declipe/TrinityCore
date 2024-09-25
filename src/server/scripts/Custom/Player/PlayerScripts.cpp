#include "Custom/Dcl.h"

class ZynPlayerScripts : public PlayerScript
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

uint32 MaxItemLevel = 80;

class item_lvlup : public ItemScript
{
public: item_lvlup() : ItemScript("item_lvlup") {}

      bool OnUse(Player* player, Item* item, SpellCastTargets const& /*Targets*/)
      {
          // WorldSession* session = player->GetSession();
          if (player->GetLevel() != MaxItemLevel)
          {
              if (player->IsInFlight() || player->IsInCombat() || player->isDead())
              {
                  player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, NULL);
                  return true;
              }
              else
              {
                  // int32 level = player->GetLevel();
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

class lfg_solo_announce : public PlayerScript
{
public:
    lfg_solo_announce() : PlayerScript("lfg_solo_announce") {}

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        // Announce Module
        if (sGameConfig->GetBoolConfig("SoloLFG.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00Solo Dungeon Finder|r.");
        }
    }
};

class buff_zones : public PlayerScript
{
public:
    buff_zones() : PlayerScript("buff_zone") {}

    void OnUpdateZone(Player* player, uint32 newZone, uint32 /*newArea*/) override
    {
        LoadZoneBuffs();

        auto it = zoneBuffs.find(newZone);
        if (it != zoneBuffs.end())
        {
            uint32 buffId = it->second;
            player->AddAura(buffId, player);
        }
        else
        {
            for (const auto& pair : zoneBuffs)
            {
                player->RemoveAurasDueToSpell(pair.second);
            }
        }
    }

    void LoadZoneBuffs()
    {
        QueryResult result = ZynDatabase.PQuery("SELECT zone_id, buff_id FROM zone_buffs");
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 zoneId = fields[0].GetUInt32();
                uint32 buffId = fields[1].GetUInt32();
                zoneBuffs[zoneId] = buffId;
            } while (result->NextRow());
        }
    }

private:
    std::map<uint32, uint32> zoneBuffs;
};

void AddSC_ZynPlayerScripts()
{
    new buff_zones();
    new item_lvlup();
    new lfg_solo_announce();
    new ZynPlayerScripts();
}
