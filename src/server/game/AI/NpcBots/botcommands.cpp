#include "bot_ai.h"
#include "botmgr.h"
#include "botdatamgr.h"
#include "Chat.h"
#include "CharacterCache.h"
#include "Language.h"
#include "Group.h"
#include "Map.h"
#include "MapManager.h"
#include "Player.h"
#include "RBAC.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
/*
Name: script_bot_commands
%Complete: ???
Comment: Npc Bot related commands by Trickerer (onlysuffering@gmail.com)
Category: commandscripts/custom/
*/
//RBAC_PERM_GM_COMMANDS = 197
//RBAC_PERM_PLAYER_COMMANDS = 199
#define GM_COMMANDS rbac::RBACPermissions(197)
#define PLAYER_COMMANDS rbac::RBACPermissions(199)

using namespace Trinity::ChatCommands;

class script_bot_commands : public CommandScript
{
private:
    struct BotInfo
    {
        public:
            BotInfo(uint32 Id, std::string Name, uint8 Race) : id(Id), name(Name), race(Race) {}
            uint32 id;
            std::string name;
            uint8 race;
        private:
            BotInfo() {}
            //BotInfo(BotInfo const&);
    };
    static bool sortbots(BotInfo const& p1, BotInfo const& p2)
    {
        return p1.id < p2.id;
    }

public:
    script_bot_commands() : CommandScript("script_bot_commands") { }

    ChatCommandTable GetCommands() const override
    {
        //static std::vector<ChatCommand> npcbotToggleCommandTable =
        //{
        //    { "flags",      GM_COMMANDS,            false, &HandleNpcBotToggleFlagsCommand,         "" },
        //};

        static ChatCommandTable npcbotDebugCommandTable =
        {
            { "raid",       HandleNpcBotDebugRaidCommand,               GM_COMMANDS, Console::No  },
            { "mount",      HandleNpcBotDebugMountCommand,              GM_COMMANDS, Console::No  },
            { "spellvisual",HandleNpcBotDebugSpellVisualCommand,        GM_COMMANDS, Console::No  },
            { "states",     HandleNpcBotDebugStatesCommand,             GM_COMMANDS, Console::No  },
        };

        static ChatCommandTable npcbotSetCommandTable =
        {
            { "faction",    HandleNpcBotSetFactionCommand,              GM_COMMANDS, Console::No  },
            { "owner",      HandleNpcBotSetOwnerCommand,                GM_COMMANDS, Console::No  },
            { "spec",       HandleNpcBotSetSpecCommand,                 GM_COMMANDS, Console::No  },
        };

        static ChatCommandTable npcbotCommandCommandTable =
        {
            { "standstill", HandleNpcBotCommandStandstillCommand,   PLAYER_COMMANDS, Console::No  },
            { "stopfully",  HandleNpcBotCommandStopfullyCommand,    PLAYER_COMMANDS, Console::No  },
            { "follow",     HandleNpcBotCommandFollowCommand,       PLAYER_COMMANDS, Console::No  },
        };

        static ChatCommandTable npcbotAttackDistanceCommandTable =
        {
            { "short",      HandleNpcBotAttackDistanceShortCommand, PLAYER_COMMANDS, Console::No  },
            { "long",       HandleNpcBotAttackDistanceLongCommand,  PLAYER_COMMANDS, Console::No  },
            { "",           HandleNpcBotAttackDistanceExactCommand, PLAYER_COMMANDS, Console::No  },
        };

        static ChatCommandTable npcbotDistanceCommandTable =
        {
            { "attack",     npcbotAttackDistanceCommandTable                                      },
            { "",           HandleNpcBotFollowDistanceCommand,      PLAYER_COMMANDS, Console::No  },
        };

        static ChatCommandTable npcbotOrderCommandTable =
        {
            { "cast",       HandleNpcBotOrderCastCommand,           PLAYER_COMMANDS, Console::No  },
        };

        static ChatCommandTable npcbotCommandTable =
        {
            //{ "debug",      npcbotDebugCommandTable                                               },
            //{ "toggle",     npcbotToggleCommandTable                                              },
            { "set",        npcbotSetCommandTable },
            { "add",        HandleNpcBotAddCommand,                     GM_COMMANDS, Console::No  },
            { "remove",     HandleNpcBotRemoveCommand,                  GM_COMMANDS, Console::No  },
            { "spawn",      HandleNpcBotSpawnCommand,                   GM_COMMANDS, Console::No  },
            { "move",       HandleNpcBotMoveCommand,                    GM_COMMANDS, Console::No  },
            { "delete",     HandleNpcBotDeleteCommand,                  GM_COMMANDS, Console::No  },
            { "lookup",     HandleNpcBotLookupCommand,                  GM_COMMANDS, Console::No  },
            { "revive",     HandleNpcBotReviveCommand,                  GM_COMMANDS, Console::No  },
            { "reloadconfig",HandleNpcBotReloadConfigCommand,           GM_COMMANDS, Console::Yes },
            { "command",    npcbotCommandCommandTable                                             },
            { "info",       HandleNpcBotInfoCommand,                PLAYER_COMMANDS, Console::No  },
            { "hide",       HandleNpcBotHideCommand,                PLAYER_COMMANDS, Console::No  },
            { "unhide",     HandleNpcBotUnhideCommand,              PLAYER_COMMANDS, Console::No  },
            { "show",       HandleNpcBotUnhideCommand,              PLAYER_COMMANDS, Console::No  },
            { "recall",     HandleNpcBotRecallCommand,              PLAYER_COMMANDS, Console::No  },
            { "kill",       HandleNpcBotKillCommand,                PLAYER_COMMANDS, Console::No  },
            { "suicide",    HandleNpcBotKillCommand,                PLAYER_COMMANDS, Console::No  },
            { "distance",   npcbotDistanceCommandTable                                            },
            { "order",      npcbotOrderCommandTable                                               },
        };

        static ChatCommandTable commandTable =
        {
            { "npcbot",     npcbotCommandTable                                                    },
        };
        return commandTable;
    }

    static bool HandleNpcBotDebugStatesCommand(ChatHandler* handler, const char* /*args*/)
    {
        Unit* target = handler->getSelectedUnit();
        if (!target)
        {
            handler->SendSysMessage("No target selected");
            return true;
        }

        uint8 const MAX_UNIT_STATES = 29;
        std::ostringstream ostr;
        ostr << "Listing states for " << target->GetName() << ":";
        for (uint8 i = 0; i != MAX_UNIT_STATES; ++i)
        {
            if (target->HasUnitState(1 << i))
                ostr << "\n    0x" << std::hex << (1 << i);
        }

        handler->SendSysMessage(ostr.str().c_str());
        return true;
    }

    static bool HandleNpcBotDebugRaidCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        Group const* gr = owner->GetGroup();
        if (!owner->HaveBot() || !gr)
        {
            handler->SendSysMessage(".npcbot debug raid");
            handler->SendSysMessage("prints your raid bots info");
            return true;
        }
        if (!gr->isRaidGroup())
        {
            handler->SendSysMessage("only usable in raid");
            return true;
        }

        uint8 counter = 0;
        uint8* subBots = new uint8[MAX_RAID_SUBGROUPS];
        memset((void*)subBots, 0, (MAX_RAID_SUBGROUPS)*sizeof(uint8));
        std::ostringstream sstr;
        BotMap const* map = owner->GetBotMgr()->GetBotMap();
        for (BotMap::const_iterator itr = map->begin(); itr != map->end(); ++itr)
        {
            Creature* bot = itr->second;
            if (!bot || !gr->IsMember(itr->second->GetGUID()))
                continue;

            uint8 subGroup = gr->GetMemberGroup(itr->second->GetGUID());
            ++subBots[subGroup];
            sstr << uint32(++counter) << ": " << bot->GetGUID().GetCounter() << " " << bot->GetName()
                << " subgr: " << uint32(subGroup + 1) << "\n";
        }

        for (uint8 i = 0; i != MAX_RAID_SUBGROUPS; ++i)
            if (subBots[i] > 0)
                sstr << uint32(subBots[i]) << " bots in subgroup " << uint32(i + 1) << "\n";

        handler->SendSysMessage(sstr.str().c_str());
        delete[] subBots;
        return true;
    }

    static bool HandleNpcBotDebugMountCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        //float speed = 1.f;
        uint32 num = 0;

        num = atoi((char*)args);
        if (!num)
            return false;

        Unit* target = handler->getSelectedUnit();
        if (!target)
        {
            handler->SendSysMessage("No target selected");
            return true;
        }

        target->Mount(num);
        return true;
    }

    static bool HandleNpcBotDebugSpellVisualCommand(ChatHandler* handler, const char* args)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        Unit* target = owner->GetSelectedUnit();
        if (!target)
        {
            handler->SendSysMessage("No target selected");
            return true;
        }

        const std::string intStr = args;
        uint32 kit = (uint32)atoi((char*)args);

        target->SendPlaySpellVisual(kit);
        return true;
    }

    static bool HandleNpcBotOrderCastCommand(ChatHandler* handler, const char* args)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        char* bot_name = !*args ? nullptr : strtok((char*)args, " ");
        char* spell_name = !*args ? nullptr : strtok(NULL, " ");
        char* target_token = !*args ? nullptr : strtok(NULL, " ");

        if (!owner->HaveBot() || !*args || !bot_name || !spell_name)
        {
            handler->SendSysMessage(".npcbot order cast #bot_name #spell_underscored_name #[target_token]");
            handler->SendSysMessage("Orders bot to cast a spell immediately");
            return true;
        }

        for (uint32 i = 0; i < strlen(spell_name); ++i)
            if (spell_name[i] == '_')
                spell_name[i] = ' ';

        Creature* bot = owner->GetBotMgr()->GetBotByName(bot_name);
        if (!bot || !bot->IsInWorld())
        {
            handler->PSendSysMessage("Bot %s is not found!", bot_name);
            return true;
        }
        if (!bot->IsAlive())
        {
            handler->PSendSysMessage("%s is dead!", bot->GetName().c_str());
            return true;
        }

        uint32 basespell = 0;
        std::string sname = spell_name;
        std::wstring wname;
        if (Utf8toWStr(sname, wname))
        {
            wstrToLower(wname);
            bot_ai::BotSpellMap const& spellmap = bot->GetBotAI()->GetSpellMap();
            for (bot_ai::BotSpellMap::const_iterator itr = spellmap.begin(); itr != spellmap.end(); ++itr)
            {
                //we ignore enabled state since this is exactly what we want
                if (itr->second->spellId == 0) //not init'ed
                    continue;
                sname = sSpellMgr->GetSpellInfo(itr->first)->SpellName[handler->GetSessionDbcLocale()];
                std::wstring wcname;
                if (!Utf8toWStr(sname, wcname))
                    continue;
                wstrToLower(wcname);
                if (wcname == wname)
                {
                    basespell = itr->first;
                    break;
                }
            }
        }
        if (!basespell)
        {
            handler->PSendSysMessage("%s doesn't have spell named '%s'!", bot->GetName().c_str(), spell_name);
            return true;
        }
        //we ignore GCD for now
        if (bot->GetBotAI()->GetSpellCooldown(basespell) > bot->GetBotAI()->GetLastDiff())
        {
            handler->PSendSysMessage("%s's %s is not ready yet!", bot->GetName().c_str(), sSpellMgr->GetSpellInfo(basespell)->SpellName[handler->GetSessionDbcLocale()]);
            return true;
        }

        ObjectGuid target_guid;
        if (!target_token || !stricmp(target_token, "bot") || !stricmp(target_token, "self"))
            target_guid = bot->GetGUID();
        else if (!stricmp(target_token, "me") || !stricmp(target_token, "master"))
            target_guid = owner->GetGUID();
        else if (!stricmp(target_token, "target"))
            target_guid = bot->GetTarget();
        else if (!stricmp(target_token, "mytarget"))
            target_guid = owner->GetTarget();
        else
        {
            handler->PSendSysMessage("Invalid target token '%s'!", target_token);
            handler->SendSysMessage("Valid target tokens:\n    '','bot','self', 'me','master', 'target', 'mytarget'");
            return true;
        }

        Unit* target = ObjectAccessor::GetUnit(*owner, target_guid);
        if (!target || !bot->FindMap() || target->FindMap() != bot->FindMap())
        {
            handler->PSendSysMessage("Invalid target '%s'!", target ? target->GetName().c_str() : "unknown");
            return true;
        }

        bot_ai::BotOrder* order = new bot_ai::BotOrder(BOT_ORDER_SPELLCAST);
        order->params.spellCastParams.baseSpell = basespell;
        order->params.spellCastParams.targetGuid = target_guid.GetRawValue();

        if (bot->GetBotAI()->AddOrder(order))
        {
            if (DEBUG_BOT_ORDERS)
                handler->PSendSysMessage("Order given: %s: %s on %s", bot->GetName().c_str(),
                    sSpellMgr->GetSpellInfo(basespell)->SpellName[handler->GetSessionDbcLocale()], target ? target->GetName().c_str() : "unknown");
        }
        else
        {
            if (DEBUG_BOT_ORDERS)
                handler->PSendSysMessage("Order failed: %s: %s on %s", bot->GetName().c_str(),
                    sSpellMgr->GetSpellInfo(basespell)->SpellName[handler->GetSessionDbcLocale()], target ? target->GetName().c_str() : "unknown");
        }

        return true;
    }

    static bool HandleNpcBotFollowDistanceCommand(ChatHandler* handler, const char* args)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        char* dist_str = strtok((char*)args, " ");

        if (!owner->HaveBot() || !dist_str)
        {
            handler->SendSysMessage(".npcbot distance #[attack] #newdist");
            handler->SendSysMessage("Sets follow / attack distance for bots");
            return true;
        }

        uint8 newdist = (uint8)std::min<int32>(std::max<int32>(atoi(dist_str), 0), 100);
        owner->GetBotMgr()->SetBotFollowDist(newdist);

        handler->PSendSysMessage("Bots' follow distance is set to %u", uint32(newdist));
        return true;
    }

    static bool HandleNpcBotAttackDistanceShortCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot distance attack short");
            handler->SendSysMessage("Sets attack distance for bots");
            return true;
        }

        owner->GetBotMgr()->SetBotAttackRangeMode(BOT_ATTACK_RANGE_SHORT);

        handler->SendSysMessage("Bots' attack distance is set to 'short'");
        return true;
    }

    static bool HandleNpcBotAttackDistanceLongCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot distance attack long");
            handler->SendSysMessage("Sets attack distance for bots");
            return true;
        }

        owner->GetBotMgr()->SetBotAttackRangeMode(BOT_ATTACK_RANGE_LONG);

        handler->SendSysMessage("Bots' attack distance is set to 'long'");
        return true;
    }

    static bool HandleNpcBotAttackDistanceExactCommand(ChatHandler* handler, const char* args)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        char* dist_str = strtok((char*)args, " ");

        if (!owner->HaveBot() || !dist_str)
        {
            handler->SendSysMessage(".npcbot distance attack #newdist");
            handler->SendSysMessage("Sets attack distance for bots");
            return true;
        }

        uint8 newdist = (uint8)std::min<int32>(std::max<int32>(atoi(dist_str), 0), 50);
        owner->GetBotMgr()->SetBotAttackRangeMode(BOT_ATTACK_RANGE_EXACT, newdist);

        handler->PSendSysMessage("Bots' attack distance is set to %u", uint32(newdist));
        return true;
    }

    static bool HandleNpcBotHideCommand(ChatHandler* handler, const char* /*args*/)
    {
        // Hiding/unhiding bots should be allowed only out of combat
        // Currenly bots can teleport to master in combat
        // This creates potential for some serious trolls
        Player* owner = handler->GetSession()->GetPlayer();
        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot hide");
            handler->SendSysMessage("Removes your owned npcbots from world temporarily");
            //handler->SendSysMessage("You have no bots!");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (!owner->IsAlive())
        {
            handler->GetSession()->SendNotification("You are dead");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (owner->GetBotMgr()->IsPartyInCombat())
        {
            handler->GetSession()->SendNotification(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        owner->GetBotMgr()->SetBotsHidden(true);
        handler->SendSysMessage("Bots hidden");
        return true;
    }

    static bool HandleNpcBotUnhideCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot unhide | show");
            handler->SendSysMessage("Returns your temporarily hidden bots back");
            //handler->SendSysMessage("You have no bots!");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (!owner->IsAlive())
        {
            handler->GetSession()->SendNotification("You are dead");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (owner->GetBotMgr()->IsPartyInCombat())
        {
            handler->GetSession()->SendNotification(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        owner->GetBotMgr()->SetBotsHidden(false);
        handler->SendSysMessage("Bots unhidden");
        return true;
    }

    static bool HandleNpcBotKillCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();

        ObjectGuid guid = owner->GetTarget();
        if (!guid || !owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot recall");
            handler->SendSysMessage("Makes your npcbot just drop dead. If you select yourself ALL your bots will die");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (guid == owner->GetGUID())
        {
            owner->GetBotMgr()->KillAllBots();
            return true;
        }
        if (Creature* bot = owner->GetBotMgr()->GetBot(guid))
        {
            owner->GetBotMgr()->KillBot(bot);
            return true;
        }

        handler->SendSysMessage("You must select one of your bots or yourself");
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleNpcBotRecallCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();

        ObjectGuid guid = owner->GetTarget();
        if (!guid || !owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot recall");
            handler->SendSysMessage("Forces npcbots to move directly on your position. Select a npcbot you want to move or select yourself to move all bots");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (owner->GetBotMgr()->IsPartyInCombat())
        {
            handler->GetSession()->SendNotification(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (guid == owner->GetGUID())
        {
            owner->GetBotMgr()->RecallAllBots();
            return true;
        }
        if (Creature* bot = owner->GetBotMgr()->GetBot(guid))
        {
            owner->GetBotMgr()->RecallBot(bot);
            return true;
        }

        handler->SendSysMessage("You must select one of your bots or yourself");
        handler->SetSentErrorMessage(true);
        return false;
    }

    //static bool HandleNpcBotToggleFlagsCommand(ChatHandler* handler, const char* args)
    //{
    //    Player* chr = handler->GetSession()->GetPlayer();
    //    Unit* unit = chr->GetSelectedUnit();
    //    if (!unit || unit->GetTypeId() != TYPEID_UNIT || !*args)
    //    {
    //        handler->SendSysMessage(".npcbot toggle flags #flag");
    //        handler->SendSysMessage("This is a debug command");
    //        handler->SetSentErrorMessage(true);
    //        return false;
    //    }

    //    const std::string facStr = args;
    //    int32 flag = (int32)atoi((char*)args);

    //    uint32 setFlags = 0;

    //    switch (flag)
    //    {
    //        case 6:
    //            setFlags = UNIT_FLAG_UNK_6;
    //            break;
    //        case 14:
    //            setFlags = UNIT_FLAG_UNK_14;
    //            break;
    //        case 15:
    //            setFlags = UNIT_FLAG_UNK_15;
    //            break;
    //        case 16:
    //            setFlags = UNIT_FLAG_UNK_16;
    //            break;
    //        default:
    //            break;
    //    }

    //    if (!setFlags)
    //        return false;

    //    handler->PSendSysMessage("Toggling flag %u on %s", setFlags, unit->GetName().c_str());
    //    unit->ToggleFlag(UNIT_FIELD_FLAGS, setFlags);
    //    return true;
    //}

    static bool HandleNpcBotSetFactionCommand(ChatHandler* handler, const char* args)
    {
        Player* chr = handler->GetSession()->GetPlayer();
        Unit* ubot = chr->GetSelectedUnit();
        if (!ubot || !*args)
        {
            handler->SendSysMessage(".npcbot set faction #faction");
            handler->SendSysMessage("Sets faction for selected npcbot (saved in DB)");
            handler->SendSysMessage("Use 'a', 'h', 'm' or 'f' as argument to set faction to alliance, horde, monsters (hostile to all) or friends (friendly to all)");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* bot = ubot->ToCreature();
        if (!bot || !bot->IsNPCBot() || !bot->IsFreeBot())
        {
            handler->SendSysMessage("You must select uncontrolled npcbot");
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 factionId = 0;
        const std::string facStr = args;
        char const* factionChar = facStr.c_str();

        if (factionChar[0] == 'a')
            factionId = 1802; //Alliance
        else if (factionChar[0] == 'h')
            factionId = 1801; //Horde
        else if (factionChar[0] == 'm')
            factionId = 14; //Monsters
        else if (factionChar[0] == 'f')
            factionId = 35; //Friendly to all

        if (!factionId)
        {
            char* pfactionid = handler->extractKeyFromLink((char*)args, "Hfaction");
            factionId = atoi(pfactionid);
        }

        if (!sFactionTemplateStore.LookupEntry(factionId))
        {
            handler->PSendSysMessage(LANG_WRONG_FACTION, factionId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BotDataMgr::UpdateNpcBotData(bot->GetEntry(), NPCBOT_UPDATE_FACTION, &factionId);
        bot->GetBotAI()->ReInitFaction();

        handler->PSendSysMessage("%s's faction set to %u", bot->GetName().c_str(), factionId);
        return true;
    }

    static bool HandleNpcBotSetOwnerCommand(ChatHandler* handler, const char* args)
    {
        Player* chr = handler->GetSession()->GetPlayer();
        Unit* ubot = chr->GetSelectedUnit();
        if (!ubot || !*args)
        {
            handler->SendSysMessage(".npcbot set owner #guid | #name");
            handler->SendSysMessage("Binds selected npcbot to new player owner using guid or name and updates owner in DB");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* bot = ubot->ToCreature();
        if (!bot || !bot->IsNPCBot())
        {
            handler->SendSysMessage("You must select a npcbot");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (bot->GetBotAI()->GetBotOwnerGuid())
        {
            handler->SendSysMessage("This npcbot already has owner");
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* characterName_str = strtok((char*)args, " ");
        if (!characterName_str)
            return false;

        std::string characterName = characterName_str;
        uint32 guidlow = (uint32)atoi(characterName_str);

        bool found = true;
        if (guidlow)
            found = sCharacterCache->GetCharacterNameByGuid(ObjectGuid(HighGuid::Player, 0, guidlow), characterName);
        else
            guidlow = sCharacterCache->GetCharacterGuidByName(characterName);

        if (!guidlow || !found)
        {
            handler->SendSysMessage("Player not found");
            handler->SetSentErrorMessage(true);
            return false;
        }

        BotDataMgr::UpdateNpcBotData(bot->GetEntry(), NPCBOT_UPDATE_OWNER, &guidlow);
        bot->GetBotAI()->ReinitOwner();
        //bot->GetBotAI()->Reset();

        handler->PSendSysMessage("%s's new owner is %s (guidlow: %u)", bot->GetName().c_str(), characterName.c_str(), guidlow);
        return true;
    }

    static bool HandleNpcBotSetSpecCommand(ChatHandler* handler, const char* args)
    {
        Player* chr = handler->GetSession()->GetPlayer();
        Unit* ubot = chr->GetSelectedUnit();
        if (!ubot || !*args)
        {
            handler->SendSysMessage(".npcbot set spec #specnumber");
            handler->SendSysMessage("Changes talent spec for selected npcbot");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* bot = ubot->ToCreature();
        if (!bot || !bot->IsNPCBot())
        {
            handler->SendSysMessage("You must select a npcbot");
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* specStr = strtok((char*)args, " ");
        if (!specStr)
            return false;

        uint8 spec = (uint8)atoi(specStr);
        if (spec < BOT_SPEC_BEGIN || spec > BOT_SPEC_END)
        {
            handler->SendSysMessage("Spec is out of range (1 to 3)!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        bot->GetBotAI()->SetSpec(spec);

        handler->PSendSysMessage("%s's new spec is %u", bot->GetName().c_str(), uint32(spec));
        return true;
    }

    static bool HandleNpcBotLookupCommand(ChatHandler* handler, const char* args)
    {
        //this is just a modified '.lookup creature' command
        if (!*args)
        {
            handler->SendSysMessage(".npcbot lookup #class");
            handler->SendSysMessage("Looks up npcbots by #class, and returns all matches with their creature ID's");
            handler->PSendSysMessage("BOT_CLASS_WARRIOR = %u", uint32(BOT_CLASS_WARRIOR));
            handler->PSendSysMessage("BOT_CLASS_PALADIN = %u", uint32(BOT_CLASS_PALADIN));
            handler->PSendSysMessage("BOT_CLASS_HUNTER = %u", uint32(BOT_CLASS_HUNTER));
            handler->PSendSysMessage("BOT_CLASS_ROGUE = %u", uint32(BOT_CLASS_ROGUE));
            handler->PSendSysMessage("BOT_CLASS_PRIEST = %u", uint32(BOT_CLASS_PRIEST));
            handler->PSendSysMessage("BOT_CLASS_DEATH_KNIGHT = %u", uint32(BOT_CLASS_DEATH_KNIGHT));
            handler->PSendSysMessage("BOT_CLASS_SHAMAN = %u", uint32(BOT_CLASS_SHAMAN));
            handler->PSendSysMessage("BOT_CLASS_MAGE = %u", uint32(BOT_CLASS_MAGE));
            handler->PSendSysMessage("BOT_CLASS_WARLOCK = %u", uint32(BOT_CLASS_WARLOCK));
            handler->PSendSysMessage("BOT_CLASS_DRUID = %u", uint32(BOT_CLASS_DRUID));
            handler->PSendSysMessage("BOT_CLASS_BLADEMASTER = %u", uint32(BOT_CLASS_BM));
            handler->PSendSysMessage("BOT_CLASS_SPHYNX = %u", uint32(BOT_CLASS_SPHYNX));
            handler->PSendSysMessage("BOT_CLASS_ARCHMAGE = %u", uint32(BOT_CLASS_ARCHMAGE));
            handler->PSendSysMessage("BOT_CLASS_DREADLORD = %u", uint32(BOT_CLASS_DREADLORD));
            handler->PSendSysMessage("BOT_CLASS_SPELLBREAKER = %u", uint32(BOT_CLASS_SPELLBREAKER));
            handler->PSendSysMessage("BOT_CLASS_DARK_RANGER = %u", uint32(BOT_CLASS_DARK_RANGER));
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* classstr = strtok((char*)args, " ");
        uint8 botclass = BOT_CLASS_NONE;

        if (classstr)
            botclass = (uint8)atoi(classstr);

        if (botclass == BOT_CLASS_NONE || botclass >= BOT_CLASS_END)
        {
            handler->PSendSysMessage("Unknown bot class %u", uint32(botclass));
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("Looking for bots of class %u...", uint32(botclass));

        uint8 localeIndex = handler->GetSessionDbLocaleIndex();
        CreatureTemplateContainer const& ctc = sObjectMgr->GetCreatureTemplates();
        typedef std::list<BotInfo> BotList;
        BotList botlist;
        for (CreatureTemplateContainer::const_iterator itr = ctc.begin(); itr != ctc.end(); ++itr)
        {
            uint32 id = itr->second.Entry;
            if (id < BOT_ENTRY_BEGIN || id > BOT_ENTRY_END)
                continue;

            if (id == BOT_ENTRY_MIRROR_IMAGE_BM)
                continue;
            //Blademaster disabled
            if (botclass == BOT_CLASS_BM)
                continue;

            //TC_LOG_ERROR("entities.unit", "NpcBotLookup: cur %u", id);

            NpcBotExtras const* _botExtras = BotDataMgr::SelectNpcBotExtras(id);
            if (!_botExtras)
                continue;

            //TC_LOG_ERROR("entities.unit", "NpcBotLookup: found extras...");

            if (_botExtras->bclass != botclass)
                continue;

            //TC_LOG_ERROR("entities.unit", "NpcBotLookup: class matches...");

            uint8 race = _botExtras->race;

            if (CreatureLocale const* creatureLocale = sObjectMgr->GetCreatureLocale(id))
            {
                if (creatureLocale->Name.size() > localeIndex && !creatureLocale->Name[localeIndex].empty())
                {
                    botlist.push_back(BotInfo(id, creatureLocale->Name[localeIndex], race));
                    continue;
                }
            }

            std::string name = itr->second.Name;
            if (name.empty())
                continue;

            //TC_LOG_ERROR("entities.unit", "NpcBotLookup: ading to list");

            botlist.push_back(BotInfo(id, name, race));
        }

        if (botlist.empty())
        {
            handler->SendSysMessage(LANG_COMMAND_NOCREATUREFOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        botlist.sort(&script_bot_commands::sortbots);

        for (BotList::const_iterator itr = botlist.begin(); itr != botlist.end(); ++itr)
        {
            uint32 id = itr->id;
            char const* name = itr->name.c_str();
            uint8 race = itr->race;

            //TODO:
            if (race >= MAX_RACES)
                race = RACE_NONE;

            char const* raceName;
            switch (race)
            {
                case RACE_HUMAN:        raceName = "Human";     break;
                case RACE_ORC:          raceName = "Orc";       break;
                case RACE_DWARF:        raceName = "Dwarf";     break;
                case RACE_NIGHTELF:     raceName = "Night Elf"; break;
                case RACE_UNDEAD_PLAYER:raceName = "Forsaken";  break;
                case RACE_TAUREN:       raceName = "Tauren";    break;
                case RACE_GNOME:        raceName = "Gnome";     break;
                case RACE_TROLL:        raceName = "Troll";     break;
                case RACE_BLOODELF:     raceName = "Blood Elf"; break;
                case RACE_DRAENEI:      raceName = "Draenei";   break;
                case RACE_NONE:         raceName = "No Race";   break;
                default:                raceName = "Unknown";   break;
            }

            handler->PSendSysMessage("%d - |cffffffff|Hcreature_entry:%d|h[%s]|h|r %s", id, id, name, raceName);
        }

        return true;
    }

    static bool HandleNpcBotDeleteCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* chr = handler->GetSession()->GetPlayer();
        Unit* ubot = chr->GetSelectedUnit();
        if (!ubot)
        {
            handler->SendSysMessage(".npcbot delete");
            handler->SendSysMessage("Deletes selected npcbot spawn from world and DB");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* bot = ubot->ToCreature();
        if (!bot || !bot->IsNPCBot())
        {
            handler->SendSysMessage("No npcbot selected");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* botowner = bot->GetBotOwner()->ToPlayer();

        ObjectGuid::LowType receiver =
            botowner ? botowner->GetGUID().GetCounter() :
            bot->GetBotAI()->GetBotOwnerGuid() != 0 ? bot->GetBotAI()->GetBotOwnerGuid() :
            chr->GetGUID().GetCounter();
        if (!bot->GetBotAI()->UnEquipAll(receiver))
        {
            handler->PSendSysMessage("%s is unable to unequip some gear. Please remove equips before deleting bot!", bot->GetName().c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (botowner)
            botowner->GetBotMgr()->RemoveBot(bot->GetGUID(), BOT_REMOVE_DISMISS);

        bot->CombatStop();
        bot->GetBotAI()->Reset();
        bot->GetBotAI()->canUpdate = false;
        Creature::DeleteFromDB(bot->GetSpawnId());
        bot->AddObjectToRemoveList();

        BotDataMgr::UpdateNpcBotData(bot->GetEntry(), NPCBOT_UPDATE_ERASE);

        handler->SendSysMessage("Npcbot successfully deleted");
        return true;
    }

    static bool HandleNpcBotMoveCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();
        Creature* creature = handler->getSelectedCreature();

        if (!*args && !creature)
        {
            handler->SendSysMessage(".npcbot move");
            handler->SendSysMessage("Moves npcbot to your location");
            handler->SendSysMessage("Syntax: .npcbot move [#ID]");
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* charID = *args ? handler->extractKeyFromLink((char*)args, "Hcreature_entry") : nullptr;
        if (!charID && !creature)
            return false;

        uint32 id = charID ? atoi(charID) : creature->GetEntry();

        CreatureTemplate const* creInfo = sObjectMgr->GetCreatureTemplate(id);
        if (!creInfo)
        {
            handler->PSendSysMessage("creature id %u does not exist!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!(creInfo->flags_extra & CREATURE_FLAG_EXTRA_NPCBOT))
        {
            handler->PSendSysMessage("creature id %u is not a npcbot!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!BotDataMgr::SelectNpcBotData(id))
        {
            handler->PSendSysMessage("NpcBot %u is not spawned!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature const* bot = BotDataMgr::FindBot(id);
        ASSERT(bot);

        uint32 lowguid = bot->GetSpawnId();

        CreatureData const* data = sObjectMgr->GetCreatureData(lowguid);
        if (!data)
        {
            handler->PSendSysMessage(LANG_COMMAND_CREATGUIDNOTFOUND, lowguid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        CreatureData* cdata = const_cast<CreatureData*>(data);
        cdata->spawnPoint.Relocate(player);
        cdata->spawnPoint.SetOrientation(player->GetOrientation());
        cdata->mapId = player->GetMapId();

        WorldDatabase.PExecute(
            "UPDATE creature SET position_x = %.3f, position_y = %.3f, position_z = %.3f, orientation = %.3f, map = %u WHERE guid = %u",
            cdata->spawnPoint.GetPositionX(), cdata->spawnPoint.GetPositionY(), cdata->spawnPoint.GetPositionZ(), cdata->spawnPoint.GetOrientation(), uint32(cdata->mapId), lowguid);

        if (bot->GetBotAI()->IAmFree() && bot->IsInWorld() && !bot->IsInCombat() && bot->IsAlive())
            BotMgr::TeleportBot(const_cast<Creature*>(bot), player->GetMap(), player);

        handler->PSendSysMessage("NpcBot %u (guid %u) was moved", id, lowguid);
        return true;
    }

    static bool HandleNpcBotSpawnCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            handler->SendSysMessage(".npcbot spawn");
            handler->SendSysMessage("Adds new npcbot spawn of given entry in world. You can shift-link the npc");
            handler->SendSysMessage("Syntax: .npcbot spawn #entry");
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* charID = handler->extractKeyFromLink((char*)args, "Hcreature_entry");
        if (!charID)
            return false;

        uint32 id = atoi(charID);

        CreatureTemplate const* creInfo = sObjectMgr->GetCreatureTemplate(id);

        if (!creInfo)
        {
            handler->PSendSysMessage("creature %u does not exist!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!(creInfo->flags_extra & CREATURE_FLAG_EXTRA_NPCBOT))
        {
            handler->PSendSysMessage("creature %u is not a npcbot!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (BotDataMgr::SelectNpcBotData(id))
        {
            handler->PSendSysMessage("Npcbot %u already exists in `characters_npcbot` table!", id);
            handler->SendSysMessage("If you want to move this bot to a new location use '.npcbot move' command");
            handler->SetSentErrorMessage(true);
            return false;
        }

        WorldDatabasePreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CREATURE_BY_ID);
        //"SELECT guid FROM creature WHERE id = ?", CONNECTION_SYNCH
        stmt->setUInt32(0, id);
        PreparedQueryResult res2 = WorldDatabase.Query(stmt);
        if (res2)
        {
            handler->PSendSysMessage("Npcbot %u already exists in `creature` table!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* chr = handler->GetSession()->GetPlayer();

        if (/*Transport* trans = */chr->GetTransport())
        {
            handler->SendSysMessage("Cannot spawn bots on transport!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        //float x = chr->GetPositionX();
        //float y = chr->GetPositionY();
        //float z = chr->GetPositionZ();
        //float o = chr->GetOrientation();
        Map* map = chr->GetMap();

        if (map->Instanceable())
        {
            handler->SendSysMessage("Cannot spawn bots in instances!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* creature = new Creature();
        if (!creature->Create(map->GenerateLowGuid<HighGuid::Unit>(), map, chr->GetPhaseMaskForSpawn(), id, *chr))
        {
            delete creature;
            handler->SendSysMessage("Creature is not created!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        NpcBotExtras const* _botExtras = BotDataMgr::SelectNpcBotExtras(id);
        if (!_botExtras)
        {
            handler->PSendSysMessage("No class/race data found for bot %u!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BotDataMgr::AddNpcBotData(id, bot_ai::DefaultRolesForClass(_botExtras->bclass), bot_ai::DefaultSpecForClass(_botExtras->bclass), creature->GetCreatureTemplate()->faction);

        creature->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), chr->GetPhaseMaskForSpawn());

        uint32 db_guid = creature->GetSpawnId();
        if (!creature->LoadBotCreatureFromDB(db_guid, map))
        {
            handler->SendSysMessage("Cannot load npcbot from DB!");
            handler->SetSentErrorMessage(true);
            delete creature;
            return false;
        }

        sObjectMgr->AddCreatureToGrid(db_guid, sObjectMgr->GetCreatureData(db_guid));

        handler->SendSysMessage("NpcBot successfully spawned");
        return true;
    }

    static bool HandleNpcBotInfoCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        if (!owner->GetTarget())
        {
            handler->SendSysMessage(".npcbot info");
            handler->SendSysMessage("Lists NpcBots count of each class owned by selected player. You can use this on self and your party members");
            handler->SetSentErrorMessage(true);
            return false;
        }
        Player* master = owner->GetSelectedPlayer();
        if (!master)
        {
            handler->SendSysMessage("No player selected");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (handler->HasLowerSecurity(master, ObjectGuid::Empty))
        {
            handler->SendSysMessage("Invalid target");
            handler->SetSentErrorMessage(true);
            return false;
        }
        if (!master->HaveBot())
        {
            handler->PSendSysMessage("%s has no NpcBots!", master->GetName().c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("Listing NpcBots for %s", master->GetName().c_str());
        handler->PSendSysMessage("Owned NpcBots: %u", master->GetNpcBotsCount());
        for (uint8 i = BOT_CLASS_WARRIOR; i != BOT_CLASS_END; ++i)
        {
            uint8 count = 0;
            uint8 alivecount = 0;
            BotMap const* map = master->GetBotMgr()->GetBotMap();
            for (BotMap::const_iterator itr = map->begin(); itr != map->end(); ++itr)
            {
                if (Creature* cre = itr->second)
                {
                    if (cre->GetBotClass() == i)
                    {
                        ++count;
                        if (cre->IsAlive())
                            ++alivecount;
                    }
                }
            }
            if (count == 0)
                continue;

            char const* bclass;
            switch (i)
            {
                case BOT_CLASS_WARRIOR:         bclass = "Warriors";        break;
                case BOT_CLASS_PALADIN:         bclass = "Paladins";        break;
                case BOT_CLASS_MAGE:            bclass = "Mages";           break;
                case BOT_CLASS_PRIEST:          bclass = "Priests";         break;
                case BOT_CLASS_WARLOCK:         bclass = "Warlocks";        break;
                case BOT_CLASS_DRUID:           bclass = "Druids";          break;
                case BOT_CLASS_DEATH_KNIGHT:    bclass = "Death Knights";   break;
                case BOT_CLASS_ROGUE:           bclass = "Rogues";          break;
                case BOT_CLASS_SHAMAN:          bclass = "Shamans";         break;
                case BOT_CLASS_HUNTER:          bclass = "Hunters";         break;
                case BOT_CLASS_BM:              bclass = "Blademasters";    break;
                case BOT_CLASS_SPHYNX:          bclass = "Destroyers";      break;
                case BOT_CLASS_ARCHMAGE:        bclass = "Archmagi";        break;
                case BOT_CLASS_DREADLORD:       bclass = "Dreadlords";      break;
                case BOT_CLASS_SPELLBREAKER:    bclass = "Spell Breakers";  break;
                case BOT_CLASS_DARK_RANGER:     bclass = "Dark Rangers";    break;
                default:                        bclass = "Unknown Class";   break;
            }
            handler->PSendSysMessage("%s: %u (alive: %u)", bclass, count, alivecount);
        }
        return true;
    }

    static bool HandleNpcBotCommandStandstillCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();

        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot command standstill");
            handler->SendSysMessage("Forces your npcbots to stop all movement and remain stationed");
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string msg;
        Unit* target = owner->GetSelectedUnit();
        if (target && owner->GetBotMgr()->GetBot(target->GetGUID()))
        {
            target->ToCreature()->GetBotAI()->SetBotCommandState(BOT_COMMAND_STAY);
            msg = target->GetName() + "'s command state set to 'STAY'";
        }
        else
        {
            owner->GetBotMgr()->SendBotCommandState(BOT_COMMAND_STAY);
            msg = "Bots' command state set to 'STAY'";
        }

        handler->SendSysMessage(msg.c_str());
        return true;
    }

    static bool HandleNpcBotCommandStopfullyCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();

        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot command stopfully");
            handler->SendSysMessage("Forces your npcbots to stop all activity");
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string msg;
        Unit* target = owner->GetSelectedUnit();
        if (target && owner->GetBotMgr()->GetBot(target->GetGUID()))
        {
            target->ToCreature()->GetBotAI()->SetBotCommandState(BOT_COMMAND_FULLSTOP);
            msg = target->GetName() + "'s command state set to 'FULLSTOP'";
        }
        else
        {
            owner->GetBotMgr()->SendBotCommandState(BOT_COMMAND_FULLSTOP);
            msg = "Bots' command state set to 'FULLSTOP'";
        }

        handler->SendSysMessage(msg.c_str());
        return true;
    }

    static bool HandleNpcBotCommandFollowCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();

        if (!owner->HaveBot())
        {
            handler->SendSysMessage(".npcbot command follow");
            handler->SendSysMessage("Allows npcbots to follow you again if stopped");
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string msg;
        Unit* target = owner->GetSelectedUnit();
        if (target && owner->GetBotMgr()->GetBot(target->GetGUID()))
        {
            target->ToCreature()->GetBotAI()->SetBotCommandState(BOT_COMMAND_FOLLOW);
            msg = target->GetName() + "'s command state set to 'FOLLOW'";
        }
        else
        {
            owner->GetBotMgr()->SendBotCommandState(BOT_COMMAND_FOLLOW);
            msg = "Bots' command state set to 'FOLLOW'";
        }

        handler->SendSysMessage(msg.c_str());
        return true;
    }

    static bool HandleNpcBotRemoveCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        Unit* u = owner->GetSelectedUnit();
        if (!u)
        {
            handler->SendSysMessage(".npcbot remove");
            handler->SendSysMessage("Frees selected npcbot from it's owner. Select player to remove all npcbots");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* master = u->ToPlayer();
        if (master)
        {
            if (master->HaveBot())
            {
                master->RemoveAllBots(BOT_REMOVE_DISMISS);

                if (!master->HaveBot())
                {
                    handler->SendSysMessage("Npcbots were successfully removed");
                    handler->SetSentErrorMessage(true);
                    return true;
                }
                handler->SendSysMessage("Some npcbots were not removed!");
                handler->SetSentErrorMessage(true);
                return false;
            }
            handler->SendSysMessage("Npcbots are not found!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* cre = u->ToCreature();
        if (cre && cre->IsNPCBot() && !cre->IsFreeBot())
        {
            master = cre->GetBotOwner();
            master->GetBotMgr()->RemoveBot(cre->GetGUID(), BOT_REMOVE_DISMISS);
            if (master->GetBotMgr()->GetBot(cre->GetGUID()) == nullptr)
            {
                handler->SendSysMessage("NpcBot successfully removed");
                handler->SetSentErrorMessage(true);
                return true;
            }
            handler->SendSysMessage("NpcBot was NOT removed for some stupid reason!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->SendSysMessage("You must select player or controlled npcbot");
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleNpcBotReviveCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        Unit* u = owner->GetSelectedUnit();
        if (!u)
        {
            handler->SendSysMessage(".npcbot revive");
            handler->SendSysMessage("Revives selected npcbot. If player is selected, revives all selected player's npcbots");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (Player* master = u->ToPlayer())
        {
            if (!master->HaveBot())
            {
                handler->PSendSysMessage("%s has no npcbots!", master->GetName().c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            master->GetBotMgr()->ReviveAllBots();
            handler->SendSysMessage("Npcbots revived");
            return true;
        }
        else if (Creature* bot = u->ToCreature())
        {
            if (bot->GetBotAI())
            {
                if (bot->IsAlive())
                {
                    handler->PSendSysMessage("%s is not dead", bot->GetName().c_str());
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                BotMgr::ReviveBot(bot, (bot->GetBotOwner() == owner) ? owner->ToUnit() : bot->ToUnit());
                handler->PSendSysMessage("%s revived", bot->GetName().c_str());
                return true;
            }
        }

        handler->SendSysMessage("You must select player or npcbot");
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleNpcBotAddCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* owner = handler->GetSession()->GetPlayer();
        Unit* cre = owner->GetSelectedUnit();

        if (!cre || cre->GetTypeId() != TYPEID_UNIT)
        {
            handler->SendSysMessage(".npcbot add");
            handler->SendSysMessage("Allows to hire selected uncontrolled bot");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* bot = cre->ToCreature();
        if (!bot || !bot->IsNPCBot() || bot->GetBotAI()->GetBotOwnerGuid())
        {
            handler->SendSysMessage("You must select uncontrolled npcbot");
            handler->SetSentErrorMessage(true);
            return false;
        }

        BotMgr* mgr = owner->GetBotMgr();
        if (!mgr)
            mgr = new BotMgr(owner);

        if (mgr->AddBot(bot, false) == BOT_ADD_SUCCESS)
        {
            handler->PSendSysMessage("%s is now your npcbot", bot->GetName().c_str());
            return true;
        }

        handler->SendSysMessage("NpcBot is NOT added for some reason!");
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleNpcBotReloadConfigCommand(ChatHandler* handler, const char* /*args*/)
    {
        TC_LOG_INFO("misc", "Re-Loading config settings...");
        sWorld->LoadConfigSettings(true);
        sMapMgr->InitializeVisibilityDistanceInfo();
        handler->SendGlobalGMSysMessage("World config settings reloaded.");
        BotMgr::ReloadConfig();
        handler->SendGlobalGMSysMessage("NpcBot config settings reloaded.");
        return true;
    }
};

void AddSC_script_bot_commands()
{
    new script_bot_commands();
}
