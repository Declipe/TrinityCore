#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"

#define CONST_ARENA_RENAME 100
#define CONST_ARENA_CUSTOMIZE 100
#define CONST_ARENA_CHANGE_FACTION 500
#define CONST_ARENA_CHANGE_RACE 250
#define CONST_ARENA_POINT 100
#define CONST_ARENA_POINT2 1000
#define CONST_HONOR_POINT 2000
#define CONST_HONOR_POINT2 20000
#define CONST_HONOR_POINT3 10000
#define CONST_HONOR_POINT4 100000
#define CONST_HONOR_POINT5 200000

#define GTS session->GetTrinityString

uint32 constexpr aurassSize = 13;
uint32 aurass[aurassSize] = { 15366, 16609, 48162, 48074, 48170, 43223, 36880, 69994, 33081, 26035, 48469, 57623, 47440 };

class example_ItemGossip : public ItemScript
{
public:
    example_ItemGossip() : ItemScript("example_ItemGossip") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override // Any hook here
    {
        if (player->GetMap()->IsBattlegroundOrArena())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_NOT_USED_BG);
            CloseGossipMenuFor(player);
            return false;
        }
        if (player->IsInCombat())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_YOU_IN_COMBAT);
            CloseGossipMenuFor(player);
            return false;
        }
        if (player->IsInFlight())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_YOU_IN_FLIGHT);
            CloseGossipMenuFor(player);
            return false;
        }
        if (player->HasStealthAura())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_USED_STEALTH);
            CloseGossipMenuFor(player);
            return false;
        }
        if (player->isDead() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_USED_DEAD);
            CloseGossipMenuFor(player);
            return false;
        }

        ClearGossipMenuFor(player); // Clears old options
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "test", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
        return false; // Cast the spell on use normally
    }

    void OnGossipSelect(Player* player, Item* /*item*/, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);

        switch (action)
        {
                break;
            case GOSSIP_ACTION_INFO_DEF + 10:
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_COMMAND_LIST), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CHARACTER_MANAGEMENT), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CHARACTER_CHANGEING), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_REPAIR_EQUIP), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_REMOVE_DESERTER), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_REMOVE_WEAKNESS), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_GIVE_BUFFS), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_VIP_BANK), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
                if (!player->IsInCombat() || !player->IsInFlight() || !player->GetMap()->IsBattlegroundOrArena() || !player->HasStealthAura() || !player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || !player->isDead())
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_REMOVE_COOLDOWN), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
                break;

            case GOSSIP_ACTION_INFO_DEF + 12:
                if (player->GetSpecsCount() == 1 && !(player->GetLevel()))
                    player->CastSpell(player, 63680, true);
                player->CastSpell(player, 63624, true);
                CloseGossipMenuFor(player);
                break;
                break;
            case GOSSIP_ACTION_INFO_DEF + 13:
                player->PlayerTalkClass->SendCloseGossip();
                break;
            case GOSSIP_ACTION_INFO_DEF + 14:
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_DUAL_SPEC), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_RESET_TALENT), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 71, GTS(LANG_ITEM_RESET_TALENT_SURE), 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_SKILLS_WEAPON), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 72);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_MAXSKILL), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 73);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 10);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 15:
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CHANGE_NAME), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 74);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CHANGE_OF_APPEARANCE), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 75);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CHANGE_FACTION), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 76);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CHANGE_RACE), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 77);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 10);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 16:
                player->DurabilityRepairAll(false, 0.0f, true);
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_REPAIR_CONFIRM);
                player->PlayerTalkClass->SendCloseGossip();
                break;
            case GOSSIP_ACTION_INFO_DEF + 17:
                if (!player->HasAura(26013))
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_DESERTER_NOT_FOUND);
                }
                else
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->RemoveAurasDueToSpell(26013);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_DESERTER_REMOVED);
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 18:
                player->RemoveAura(15007);
                player->PlayerTalkClass->SendCloseGossip();
                break;
            case GOSSIP_ACTION_INFO_DEF + 19:
                if (player->HasAura(45523))
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->GetSession()->SendAreaTriggerMessage("%s", GTS(LANG_ITEM_MSG_COOLDOWN));
                }
                else
                {
                    player->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    for (size_t i = 0; i < aurassSize; ++i)
                        player->AddAura(aurass[i], player);
                    player->GetSession()->SendNotification("|cffC67171Buffs received!");
                    player->CastSpell(player, 45523, true);
                    player->PlayerTalkClass->SendCloseGossip();
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 20:
                player->PlayerTalkClass->SendCloseGossip();
                player->GetSession()->SendShowBank(player->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 22:
                if (player->HasAura(45523))
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->GetSession()->SendAreaTriggerMessage("%s", GTS(LANG_ITEM_MSG_RESET_COOLDOWN));
                }
                else
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->RemoveArenaSpellCooldowns(true);
                    player->GetSession()->SendAreaTriggerMessage("%s", GTS(LANG_ITEM_MSG_RESET_COOLDOWN));
                    player->CastSpell(player, 45523, true);
                }
                break;

            case GOSSIP_ACTION_INFO_DEF + 71:
                player->ResetTalents(true);
                player->SendTalentsInfoData(false);
                CloseGossipMenuFor(player);
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_TALENT_RESET_CONFIRM);
                break;
            case GOSSIP_ACTION_INFO_DEF + 72:
                switch (player->GetClass())
                {
                case CLASS_WARRIOR:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(5011, false); // Crossbows
                    player->LearnSpell(200, false); // Polearms
                    player->LearnSpell(15590, false);
                    player->LearnSpell(264, false);
                    player->LearnSpell(266, false);
                    player->LearnSpell(227, false);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_PALADIN:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(197, false); // Two-handed axes
                    player->LearnSpell(200, false); // Polearms
                    player->LearnSpell(196, false); // One - handed axes
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_WARLOCK:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(201, false); // One - handed sword
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_PRIEST:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(1180, false);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_HUNTER:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(5011, false); // Crossbows
                    player->LearnSpell(202, false); // Dual - handed sword
                    player->LearnSpell(200, false); // Polearms
                    player->LearnSpell(15590, false);
                    player->LearnSpell(264, false);
                    player->LearnSpell(2567, false);
                    player->LearnSpell(227, false);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_MAGE:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(1180, false);
                    player->LearnSpell(201, false); // One - handed sword
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_SHAMAN:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(199, false);
                    player->LearnSpell(197, false); // Two-handed axes
                    player->LearnSpell(1180, false);
                    player->LearnSpell(15590, false);
                    player->LearnSpell(196, false); // One - handed axes
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_ROGUE:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(5011, false); // Crossbows
                    player->LearnSpell(198, false);
                    player->LearnSpell(15590, false);
                    player->LearnSpell(264, false);
                    player->LearnSpell(201, false); // One - handed sword
                    player->LearnSpell(266, false);
                    player->LearnSpell(196, false); // One - handed axes
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_DEATH_KNIGHT:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(199, false);
                    player->LearnSpell(198, false);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                case CLASS_DRUID:
                    CloseGossipMenuFor(player);
                    player->LearnSpell(199, false);
                    player->LearnSpell(200, false); // Polearms
                    player->LearnSpell(15590, false);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SKILLS_WEAPON_CONFIRM);
                    break;
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 73:
                CloseGossipMenuFor(player);
                player->UpdateWeaponsSkillsToMaxSkillsForLevel();
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MAXSKILL_CONFIRM);
                break;
            case GOSSIP_ACTION_INFO_DEF + 74:
                if (player->GetArenaPoints() < CONST_ARENA_RENAME)
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_NO_ARENA_POINTS);
                    player->PlayerTalkClass->SendCloseGossip();
                }
                else
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->SetAtLoginFlag(AT_LOGIN_RENAME);
                    player->ModifyArenaPoints(-CONST_ARENA_RENAME);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_RENAME_COMPLETE);
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 75:
                if (player->GetArenaPoints() < CONST_ARENA_CUSTOMIZE)
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_NO_ARENA_POINTS);
                    player->PlayerTalkClass->SendCloseGossip();
                }
                else
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                    player->ModifyArenaPoints(-CONST_ARENA_CUSTOMIZE);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_CUSTOMIZE_COMPLETE);
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 76:
                if (player->GetArenaPoints() < CONST_ARENA_CHANGE_FACTION)
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_NO_ARENA_POINTS);
                    player->PlayerTalkClass->SendCloseGossip();
                }
                else
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                    player->ModifyArenaPoints(-CONST_ARENA_CHANGE_FACTION);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_CHANGE_FACTION_COMPLETE);
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 77:
                if (player->GetArenaPoints() < CONST_ARENA_CHANGE_RACE)
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_NO_ARENA_POINTS);
                    player->PlayerTalkClass->SendCloseGossip();
                }
                else
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                    player->ModifyArenaPoints(-CONST_ARENA_CHANGE_RACE);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_CHANGE_RACE_COMPLETE);
                }
                break;
           
        }
        CloseGossipMenuFor(player);
    }
};

void AddSC_example_ItemGossip() // Add to scriptloader normally
{
    new example_ItemGossip();
}
