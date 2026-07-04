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

//class ItemInfectionSystem : public PlayerScript
//{
//public:
//    ItemInfectionSystem() : PlayerScript("ItemInfectionSystem") {}
//
//    void OnLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootGuid*/) override
//    {
//        std::vector<uint32> infectionSpells = { 60842, 30843, 16867, 31977, 3269, 69391, 51442 };
//
//        if (urand(1, 100) <= 1)
//        {
//            uint32 selectedSpell = infectionSpells[urand(0, infectionSpells.size() - 1)];
//
//
//            player->AddAura(selectedSpell, player);
//            ChatHandler(player->GetSession()).SendSysMessage("While collecting loot, you were infected with an unknown plague, next time be extremely careful!");
//
//        }
//    }
//};

using namespace Trinity::ChatCommands;

class bg_queue_commandscript : public CommandScript
{
public:
    bg_queue_commandscript() : CommandScript("bg_queue_commandscript") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> bgqCommandTable =
        {
            { "queue",    HandleBgQueueCommand,    rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            { "leave",    HandleBgLeaveCommand,    rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            { "window",  HandleBgWindowCommand,    rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            //{ "",  HandleBgWindowCommand,    rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "rgb", bgqCommandTable },
        };
        return commandTable;
    }

    static bool HandleBgWindowCommand(ChatHandler* handler, char const* args)
    {
        WorldSession* session = handler->GetSession();
        Player* player = handler->GetSession()->GetPlayer();

        uint32 bgId = args && *args ? atoi(args) : 2;

        switch (bgId)
        {
        case 1:  // AV
        case 2:  // WSG
        case 3:  // AB
        case 7:  // EotS
        case 9:  // SotA
        case 30: // IoC
            break; //NOT_USED_73
        default:
            handler->SendSysMessage("1=AV, 2=WSG, 3=AB, 7=EotS, 9=SotA, 30=IoC");
            return true;
        }

        BattlegroundTypeId bgTypeId = BattlegroundTypeId(bgId);

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId, nullptr))
        {
            handler->SendSysMessage(session->GetTrinityString2(NOT_USED_73));
            return true;
        }

        player->GetSession()->SendBattleGroundList(player->GetGUID(), bgTypeId);
        return true;
    }

    static bool HandleBgQueueCommand(ChatHandler* handler, char const* args)
    {
        WorldSession* session = handler->GetSession();
        Player* player = handler->GetSession()->GetPlayer();

        // ID ??: 1=AV, 2=WSG, 3=AB, 7=EotS, 9=SotA, 30=IoC
        uint32 bgId = args && *args ? atoi(args) : 2;
        switch (bgId)
        {
        case 1:
        case 2:
        case 3:
        case 7:
        case 9:
        case 30:
            break;
        default:
            handler->SendSysMessage("1=AV, 2=WSG, 3=AB, 7=EotS, 9=SotA, 30=IoC");
            return true;
        }
        BattlegroundTypeId bgTypeId = BattlegroundTypeId(bgId);

        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
        if (!bg)
        {
            handler->SendSysMessage("1=AV, 2=WSG, 3=AB, 7=EotS, 9=SotA, 30=IoC");
            return true;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId, nullptr))
        {
            handler->SendSysMessage(session->GetTrinityString2(NOT_USED_73));
            return true;
        }

        if (player->InBattleground() || player->InBattlegroundQueue())
        {
            handler->SendSysMessage("?.");
            return true;
        }

        if (player->HasAura(26013))
        {
            handler->SendSysMessage("?.");
            return true;
        }

        PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->GetLevel());
        if (!bracketEntry)
            bracketEntry = GetBattlegroundBracketById(bg->GetMapId(), BG_BRACKET_ID_FIRST);

        if (!bracketEntry)
        {
            handler->SendSysMessage(session->GetTrinityString2(NOT_USED_73));
            return true;
        }

        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

        GroupQueueInfo* ginfo = bgQueue.AddGroup(player, nullptr, bgTypeId, bracketEntry, 0, false, false, 0, 0);
        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, 0, 0);
        player->GetSession()->SendPacket(&data);

        sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        handler->PSendSysMessage("?? ?????? ? ???????: %s", bg->GetName());
        return true;
    }

    static bool HandleBgLeaveCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
        {
            BattlegroundQueueTypeId qId = player->GetBattlegroundQueueTypeId(i);
            if (qId == BATTLEGROUND_QUEUE_NONE)
                continue;

            BattlegroundQueue& queue = sBattlegroundMgr->GetBattlegroundQueue(qId);
            queue.RemovePlayer(player->GetGUID(), true);
            player->RemoveBattlegroundQueueId(qId);
        }

        handler->SendSysMessage("?.");
        return true;
    }
};

void AddSC_ZynPlayerScripts()
{
    new bg_queue_commandscript();
    new buff_zones();
    new item_lvlup();
    new lfg_solo_announce();
    new ZynPlayerScripts();
}
