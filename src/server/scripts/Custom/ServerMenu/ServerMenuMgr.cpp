#include "AccountMgr.h"
#include "AditionalData.h"
#include "Chat.h"
#include "Custom/Dcl.h"
#include "CustomConfig.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "InstanceSaveMgr.h"
#include "Item.h"
#include "ItemShopMgr.h"
#include "Language.h"
#include "Map.h"
#include "Player.h"
#include "PromotionCodeMgr.h"
#include "RBAC.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "ServerMenuMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "World.h"
#include "WorldSession.h"

//test
constexpr auto CONST_ARENA_RENAME = 100;
constexpr auto CONST_ARENA_CUSTOMIZE = 100;
constexpr auto CONST_ARENA_CHANGE_FACTION = 500;
constexpr auto CONST_ARENA_CHANGE_RACE = 250;

#define GTS1 session->GetTrinityString
#define GTS2 session->GetTrinityString2

enum SpellsAndItemIDsAndCost
{
    FirstAidSkill = 129,
    FirstAidSPell = 65292,

    CookingSkill = 185,
    CookingSpell = 65291,

    FishingSkill = 356,
    FishingSpell = 65293,

    BlackSmithSkill = 164,
    BlackSmithSPell = 51298,

    LeatherWorkingSKill = 165,
    LeatherWorkingSpell = 51301,

    AlchemySkill = 171,
    AlchemySPell = 65281,

    HerbalismSkill = 182,
    HerbalismSpell = 65288,

    MiningSKill = 186,
    MiningSpell = 65289,

    TailoringSKill = 197,
    TailoringSpell = 65283,

    EngSkill = 202,
    EngSPell = 61464,

    EnchanterSkill = 333,
    EnchanterSpell = 51312,

    SKinningSkill = 393,
    SkinningSPell = 50307,

    JewelSKill = 755,
    JewelSpell = 65289,

    InscriptSKill = 773,
    InscriptSpell = 65287,
};

static void HandleLearnSkillRecipesHelper(Player* player, uint32 skillId)
{
    uint32 classmask = player->GetClassMask();

    std::vector<SkillLineAbilityEntry const*> const* skillLineAbilities = GetSkillLineAbilitiesBySkill(skillId);
    if (!skillLineAbilities)
        return;

    for (SkillLineAbilityEntry const* skillLine : *skillLineAbilities)
    {
        // not high rank
        if (skillLine->SupercededBySpell)
            continue;

        // skip racial skills
        if (skillLine->RaceMask != 0)
            continue;

        // skip wrong class skills
        if (skillLine->ClassMask && (skillLine->ClassMask & classmask) == 0)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->Spell);
        if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
            continue;

        player->LearnSpell(skillLine->Spell, false);
    }
}

uint32 constexpr aurassSizer = 13;
uint32 aurassz[aurassSizer] = { 15366, 16609, 48162, 48074, 48170, 43223, 36880, 69994, 33081, 26035, 48469, 57623, 47440 };

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

bool IsVipActive2(Player* player)
{
    if (!player)
        return false;

    return player->GetAditionalData()->isPremium();
}

std::string GetVipTimeLeft2(Player* player, WorldSession* session)
{
    if (!player || !IsVipActive2(player))
        return "";

    time_t endTime = player->GetAditionalData()->getPremiumUnsetdate();
    time_t currentTime = GameTime::GetGameTime();

    if (endTime <= currentTime)
        return "";

    time_t timeLeft = endTime - currentTime;

    uint32 days = timeLeft / 86400;
    uint32 hours = (timeLeft % 86400) / 3600;
    uint32 minutes = (timeLeft % 3600) / 60;

    char buffer[256];

    if (days > 0)
    {
        snprintf(buffer, sizeof(buffer), session->GetTrinityString2(NOT_USED_53), days, hours, minutes);
        return std::string(buffer);
    }
    else if (hours > 0)
    {
        snprintf(buffer, sizeof(buffer), session->GetTrinityString2(NOT_USED_53), hours, minutes);
        return std::string(buffer);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), session->GetTrinityString2(NOT_USED_53), minutes);
        return std::string(buffer);
    }
}

std::string GetVipStatusString2(Player* player, WorldSession* session)
{
    if (!player || !session)
        return "VIP: Error";

    if (IsVipActive2(player))
    {
        time_t endTime = player->GetAditionalData()->getPremiumUnsetdate();
        time_t currentTime = GameTime::GetGameTime();

        if (endTime <= currentTime)
            return session->GetTrinityString2(NOT_USED_56);

        std::string timeLeft = GetVipTimeLeft2(player, session);
        std::string statusActive = session->GetTrinityString2(NOT_USED_52);

        char buffer[512];
        snprintf(buffer, sizeof(buffer), session->GetTrinityString2(NOT_USED_57), timeLeft.c_str());

        return fmt::format("{} ({})", statusActive, std::string(buffer));
    }
    else
    {
        return session->GetTrinityString2(NOT_USED_51);
    }
}

void sServerMenu::GossipHelloMenu(Player* player)
{
    WorldSession* session = player->GetSession();
    player->PlayerTalkClass->ClearMenus();

    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(GTS1(LANG_ITEM_CURRENT_COINS), player->GetCoins()).c_str(), GOSSIP_SENDER_MAIN, 1);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetVipStatusString2(player, session).c_str(), GOSSIP_SENDER_MAIN, 3);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ENTER_PROMO_CODE), GOSSIP_SENDER_MAIN, 0, GTS1(LANG_ENTERED_PROMO_CODE_CORRECT), 0, true);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_MENU_TRADE), GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_7), GOSSIP_SENDER_MAIN, 192);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_11), GOSSIP_SENDER_MAIN, 199, GTS2(NOT_USED_12), 0, false);
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

void BuyItem2(Player* player, uint32 itementry, uint32 coast)
{
    if (!player)
        return;

    uint32 coins = player->GetVerifiedCoins();
    uint32 ostatok = coast - coins;

    if (coins < coast)
    {
        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_ENOUGH_COINS);
        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_HAVE_COINS, coins);
        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_NEED_COINS, ostatok);
        CloseGossipMenuFor(player);
    }
    else
    {
        if (player->AddItem(itementry, 1))
        {
            ostatok = coins - coast;
            player->SetCoins(ostatok);
            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, itementry);
            CloseGossipMenuFor(player);
        }
    }
}

class ServerMenuPlayerGossip : public PlayerScript
{
public:
    ServerMenuPlayerGossip() : PlayerScript("ServerMenuPlayerGossip") {}

    void OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 sender, uint32 action) override
    {
        uint32 coast5 = 5;
        uint32 coast7 = 20;
        uint32 coast14 = 40;
        uint32 coast31 = 80;

        WorldSession* session = player->GetSession();

        std::string XP = GTS1(LANG_ITEM_RATES_XP_KILL);
        std::string XP_quest = GTS1(LANG_ITEM_RATES_XP_QUEST);
        std::string rep = GTS1(LANG_ITEM_RATES_REP);
        std::string gold = GTS1(LANG_ITEM_RATES_GOLD);
        std::string honor = GTS1(LANG_ITEM_RATES_HONOR);
        player->PlayerTalkClass->ClearMenus();
        ItemShopContainer const& itemshopMap = sItemShopMgr->GetItemShopContainer();

        if (sender == GOSSIP_SENDER_MAIN)
        {
            if (action >= 500)
            {
                for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                {
                    if (itr->second.gossipAction == action)
                        BuyItem2(player, itr->second.entryID, itr->second.coinCost);
                }
            }
            else
            {
                switch (action)
                {
                case 1:
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, getString2(GTS1(LANG_ITEM_CURRENT_COINS), player->GetCoins()).c_str(), GOSSIP_SENDER_MAIN, 3);
                    if (!player->IsPlayer())
                        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_VIP_BUY), GOSSIP_SENDER_MAIN, 78);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_VIP_INCREASE), GOSSIP_SENDER_MAIN, 79);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 2:
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/spell_frost_wizardmark:25:25:-15:0|tBuy Honor Token - 2000 Honor points", GOSSIP_SENDER_MAIN, 24);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_chest_chain_07:25:25:-15:0|tBuy Heirloom items", GOSSIP_SENDER_MAIN, 25);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/ability_mount_spectraltiger:25:25:-15:0|tBuy Mounts", GOSSIP_SENDER_MAIN, 32);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_staff_13:25:25:-15:0|tbaka - 200-2000 coins", GOSSIP_SENDER_MAIN, 33);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_10), GOSSIP_SENDER_MAIN, 34);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 3:
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(GTS1(LANG_ITEM_CURRENT_COINS), player->GetCoins()).c_str(), GOSSIP_SENDER_MAIN, 1);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ENTER_PROMO_CODE), GOSSIP_SENDER_MAIN, 0, GTS1(LANG_ENTERED_PROMO_CODE_CORRECT), 0, true);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_MENU_TRADE), GOSSIP_SENDER_MAIN, 2);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_7), GOSSIP_SENDER_MAIN, 192);
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
                        // Start pack
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
                    break;
                }
                case 4: // Trainer class
                {
                    CloseGossipMenuFor(player);
                    // SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
                    // player->PlayerTalkClass->SendCloseGossip();
                    // player->GetSession()->SendTrainerList(nullptr);
                    break;
                }
                case 5: // Teleport
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(0, -8831.966797f, 632.503906f, 94.210480f, 4.61f);
                    player->CastSpell(player, 45523, true);
                    break;
                }
                case 6: // Teleport
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(1, 1377.78f, -4369.87f, 26.0264f, 0.15315f);
                    player->CastSpell(player, 45523, true);
                    break;
                }
                case 7: // Class skills
                {
                    switch (player->GetClass())
                    {
                    case CLASS_WARRIOR:
                        player->LearnSpell(7384, false);
                        player->LearnSpell(47436, false);
                        player->LearnSpell(47450, false);
                        player->LearnSpell(11578, false);
                        player->LearnSpell(47465, false);
                        player->LearnSpell(47502, false);
                        player->LearnSpell(34428, false);
                        player->LearnSpell(1715, false);
                        player->LearnSpell(2687, false);
                        player->LearnSpell(71, false);
                        player->LearnSpell(7386, false);
                        player->LearnSpell(355, false);
                        player->LearnSpell(72, false);
                        player->LearnSpell(47437, false);
                        player->LearnSpell(57823, false);
                        player->LearnSpell(694, false);
                        player->LearnSpell(2565, false);
                        player->LearnSpell(676, false);
                        player->LearnSpell(47520, false);
                        player->LearnSpell(20230, false);
                        player->LearnSpell(12678, false);
                        player->LearnSpell(47471, false);
                        player->LearnSpell(1161, false);
                        player->LearnSpell(871, false);
                        player->LearnSpell(2458, false);
                        player->LearnSpell(20252, false);
                        player->LearnSpell(47475, false);
                        player->LearnSpell(18499, false);
                        player->LearnSpell(1680, false);
                        player->LearnSpell(6552, false);
                        player->LearnSpell(47488, false);
                        player->LearnSpell(1719, false);
                        player->LearnSpell(23920, false);
                        player->LearnSpell(47440, false);
                        player->LearnSpell(3411, false);
                        player->LearnSpell(64382, false);
                        player->LearnSpell(55694, false);
                        player->LearnSpell(57755, false);
                        player->LearnSpell(42459, false);
                        player->LearnSpell(750, false);
                        player->LearnSpell(5246, false);
                        player->LearnSpell(3127, false);
                        if (player->HasSpell(12294))
                            player->LearnSpell(47486, false);
                        if (player->HasSpell(20243))
                            player->LearnSpell(47498, false);
                        player->SaveToDB();
                        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_PALADIN:
                        player->LearnSpell(3127, false);
                        player->LearnSpell(19746, false);
                        player->LearnSpell(19752, false);
                        player->LearnSpell(750, false);
                        player->LearnSpell(48942, false);
                        player->LearnSpell(48782, false);
                        player->LearnSpell(48932, false);
                        player->LearnSpell(20271, false);
                        player->LearnSpell(498, false);
                        player->LearnSpell(10308, false);
                        player->LearnSpell(1152, false);
                        player->LearnSpell(10278, false);
                        player->LearnSpell(48788, false);
                        player->LearnSpell(53408, false);
                        player->LearnSpell(48950, false);
                        player->LearnSpell(48936, false);
                        player->LearnSpell(31789, false);
                        player->LearnSpell(62124, false);
                        player->LearnSpell(54043, false);
                        player->LearnSpell(25780, false);
                        player->LearnSpell(1044, false);
                        player->LearnSpell(20217, false);
                        player->LearnSpell(48819, false);
                        player->LearnSpell(48801, false);
                        player->LearnSpell(48785, false);
                        player->LearnSpell(5502, false);
                        player->LearnSpell(20164, false);
                        player->LearnSpell(10326, false);
                        player->LearnSpell(1038, false);
                        player->LearnSpell(53407, false);
                        player->LearnSpell(48943, false);
                        player->LearnSpell(20165, false);
                        player->LearnSpell(48945, false);
                        player->LearnSpell(642, false);
                        player->LearnSpell(48947, false);
                        player->LearnSpell(20166, false);
                        player->LearnSpell(4987, false);
                        player->LearnSpell(48806, false);
                        player->LearnSpell(6940, false);
                        player->LearnSpell(48817, false);
                        player->LearnSpell(48934, false);
                        player->LearnSpell(48938, false);
                        player->LearnSpell(25898, false);
                        player->LearnSpell(32223, false);
                        player->LearnSpell(31884, false);
                        player->LearnSpell(54428, false);
                        player->LearnSpell(61411, false);
                        player->LearnSpell(53601, false);
                        player->LearnSpell(33388, false);
                        player->LearnSpell(33391, false);
                        if (player->HasSpell(20925))
                            player->LearnSpell(48952, false);
                        if (player->HasSpell(31935))
                            player->LearnSpell(48827, false);
                        if (player->HasSpell(20911))
                            player->LearnSpell(25899, false);
                        if (player->HasSpell(20473))
                            player->LearnSpell(48825, false);
                        if (player->GetCFSTeam() == ALLIANCE)
                        {
                            player->LearnSpell(31801, false);
                            player->LearnSpell(13819, false);
                            player->LearnSpell(23214, false);
                        }
                        if (player->GetCFSTeam() == HORDE)
                        {
                            player->LearnSpell(53736, false);
                            player->LearnSpell(34769, false);
                            player->LearnSpell(34767, false);
                        }
                        player->SaveToDB();
                        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_HUNTER:
                        player->LearnSpell(3043, false);
                        player->LearnSpell(3127, false);
                        player->LearnSpell(3045, false);
                        player->LearnSpell(3034, false);
                        player->LearnSpell(8737, false);
                        player->LearnSpell(1494, false);
                        player->LearnSpell(13163, false);
                        player->LearnSpell(48996, false);
                        player->LearnSpell(49001, false);
                        player->LearnSpell(49045, false);
                        player->LearnSpell(53338, false);
                        player->LearnSpell(5116, false);
                        player->LearnSpell(27044, false);
                        player->LearnSpell(883, false);
                        player->LearnSpell(2641, false);
                        player->LearnSpell(6991, false);
                        player->LearnSpell(982, false);
                        player->LearnSpell(1515, false);
                        player->LearnSpell(19883, false);
                        player->LearnSpell(20736, false);
                        player->LearnSpell(48990, false);
                        player->LearnSpell(2974, false);
                        player->LearnSpell(6197, false);
                        player->LearnSpell(1002, false);
                        player->LearnSpell(14327, false);
                        player->LearnSpell(5118, false);
                        player->LearnSpell(49056, false);
                        player->LearnSpell(53339, false);
                        player->LearnSpell(49048, false);
                        player->LearnSpell(19884, false);
                        player->LearnSpell(34074, false);
                        player->LearnSpell(781, false);
                        player->LearnSpell(14311, false);
                        player->LearnSpell(1462, false);
                        player->LearnSpell(19885, false);
                        player->LearnSpell(19880, false);
                        player->LearnSpell(13809, false);
                        player->LearnSpell(13161, false);
                        player->LearnSpell(5384, false);
                        player->LearnSpell(1543, false);
                        player->LearnSpell(19878, false);
                        player->LearnSpell(49067, false);
                        player->LearnSpell(3034, false);
                        player->LearnSpell(13159, false);
                        player->LearnSpell(19882, false);
                        player->LearnSpell(58434, false);
                        player->LearnSpell(49071, false);
                        player->LearnSpell(49052, false);
                        player->LearnSpell(19879, false);
                        player->LearnSpell(19263, false);
                        player->LearnSpell(19801, false);
                        player->LearnSpell(34026, false);
                        player->LearnSpell(34600, false);
                        player->LearnSpell(34477, false);
                        player->LearnSpell(61006, false);
                        player->LearnSpell(61847, false);
                        player->LearnSpell(53271, false);
                        player->LearnSpell(60192, false);
                        player->LearnSpell(62757, false);
                        if (player->HasSpell(19386))
                            player->LearnSpell(49012, false);
                        if (player->HasSpell(53301))
                            player->LearnSpell(60053, false);
                        if (player->HasSpell(19306))
                            player->LearnSpell(48999, false);
                        if (player->HasSpell(19434))
                            player->LearnSpell(49050, false);
                        player->SaveToDB();
                        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_MAGE:
                        player->LearnSpell(42921, false);
                        player->LearnSpell(42842, false);
                        player->LearnSpell(42995, false);
                        player->LearnSpell(42833, false);
                        player->LearnSpell(27090, false);
                        player->LearnSpell(33717, false);
                        player->LearnSpell(42873, false);
                        player->LearnSpell(42846, false);
                        player->LearnSpell(12826, false);
                        player->LearnSpell(28271, false);
                        player->LearnSpell(61780, false);
                        player->LearnSpell(61721, false);
                        player->LearnSpell(28272, false);
                        player->LearnSpell(42917, false);
                        player->LearnSpell(43015, false);
                        player->LearnSpell(130, false);
                        player->LearnSpell(42926, false);
                        player->LearnSpell(43017, false);
                        player->LearnSpell(475, false);
                        player->LearnSpell(1953, false);
                        player->LearnSpell(42940, false);
                        player->LearnSpell(12051, false);
                        player->LearnSpell(43010, false);
                        player->LearnSpell(43020, false);
                        player->LearnSpell(43012, false);
                        player->LearnSpell(42859, false);
                        player->LearnSpell(2139, false);
                        player->LearnSpell(42931, false);
                        player->LearnSpell(42985, false);
                        player->LearnSpell(43008, false);
                        player->LearnSpell(45438, false);
                        player->LearnSpell(43024, false);
                        player->LearnSpell(43002, false);
                        player->LearnSpell(43046, false);
                        player->LearnSpell(42897, false);
                        player->LearnSpell(42914, false);
                        player->LearnSpell(66, false);
                        player->LearnSpell(58659, false);
                        player->LearnSpell(30449, false);
                        player->LearnSpell(42956, false);
                        player->LearnSpell(47610, false);
                        player->LearnSpell(61316, false);
                        player->LearnSpell(61024, false);
                        player->LearnSpell(55342, false);
                        player->LearnSpell(53142, false);
                        player->LearnSpell(7301, false);
                        if (player->GetCFSTeam() == ALLIANCE)
                        {
                            player->LearnSpell(32271, false); // Teleport: Exodar.
                            player->LearnSpell(49359, false); // Teleport: Theramore.
                            player->LearnSpell(3565, false); // Teleport: Darnassus.
                            player->LearnSpell(33690, false); // Teleport: Shattrath.
                            player->LearnSpell(3562, false); // Teleport: Ironforge.
                            player->LearnSpell(3561, false); // Teleport: Stormwind.
                            player->LearnSpell(53140, false); // Teleport: Dalaran.
                            player->LearnSpell(53142, false); // Portal: Dalaran.
                            player->LearnSpell(10059, false); // Portal: Stormwind.
                            player->LearnSpell(11419, false); // Portal: Darnassus.
                            player->LearnSpell(32266, false); // Portal: Exodar.
                            player->LearnSpell(11416, false); // Portal: Ironforge.
                            player->LearnSpell(33691, false); // Portal: Shattrath.
                            player->LearnSpell(49360, false); // Portal: Theramore.
                        }
                        if (player->GetCFSTeam() == HORDE)
                        {
                            player->LearnSpell(3567, false); // Teleport: Orgrimmar.
                            player->LearnSpell(35715, false); // Teleport: Shattrath.
                            player->LearnSpell(3566, false); // Teleport: Thunder Bluff.
                            player->LearnSpell(49358, false); // Teleport: Stonard.
                            player->LearnSpell(32272, false); // Teleport: Silvermoon.
                            player->LearnSpell(3563, false); // Teleport: Undercity.
                            player->LearnSpell(53140, false); // Teleport: Dalaran.
                            player->LearnSpell(53142, false); // Portal: Dalaran.
                            player->LearnSpell(11417, false); // Portal: Orgrimmar.
                            player->LearnSpell(35717, false); // Portal: Shattrath.
                            player->LearnSpell(32267, false); // Portal: Silvermoon.
                            player->LearnSpell(49361, false); // Portal: Stonard.
                            player->LearnSpell(11420, false); // Portal: Thunder Bluff.
                            player->LearnSpell(11418, false); // Portal: Undercity.
                        }
                        if (player->HasSpell(11366))
                            player->LearnSpell(42891, false);
                        if (player->HasSpell(11426))
                            player->LearnSpell(43039, false);
                        if (player->HasSpell(44457))
                            player->LearnSpell(55360, false);
                        if (player->HasSpell(31661))
                            player->LearnSpell(42950, false);
                        if (player->HasSpell(11113))
                            player->LearnSpell(42945, false);
                        if (player->HasSpell(44425))
                            player->LearnSpell(44781, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_WARLOCK:
                        player->LearnSpell(696, false);
                        player->LearnSpell(47811, false);
                        player->LearnSpell(47809, false);
                        player->LearnSpell(688, false);
                        player->LearnSpell(47813, false);
                        player->LearnSpell(50511, false);
                        player->LearnSpell(57946, false);
                        player->LearnSpell(47864, false);
                        player->LearnSpell(6215, false);
                        player->LearnSpell(47878, false);
                        player->LearnSpell(47855, false);
                        player->LearnSpell(697, false);
                        player->LearnSpell(47856, false);
                        player->LearnSpell(47857, false);
                        player->LearnSpell(5697, false);
                        player->LearnSpell(47884, false);
                        player->LearnSpell(47815, false);
                        player->LearnSpell(47889, false);
                        player->LearnSpell(47820, false);
                        player->LearnSpell(698, false);
                        player->LearnSpell(712, false);
                        player->LearnSpell(126, false);
                        player->LearnSpell(5138, false);
                        player->LearnSpell(5500, false);
                        player->LearnSpell(11719, false);
                        player->LearnSpell(132, false);
                        player->LearnSpell(60220, false);
                        player->LearnSpell(18647, false);
                        player->LearnSpell(61191, false);
                        player->LearnSpell(47823, false);
                        player->LearnSpell(691, false);
                        player->LearnSpell(47865, false);
                        player->LearnSpell(47891, false);
                        player->LearnSpell(47888, false);
                        player->LearnSpell(17928, false);
                        player->LearnSpell(47860, false);
                        player->LearnSpell(47825, false);
                        player->LearnSpell(1122, false);
                        player->LearnSpell(47867, false);
                        player->LearnSpell(18540, false);
                        player->LearnSpell(47893, false);
                        player->LearnSpell(47838, false);
                        player->LearnSpell(29858, false);
                        player->LearnSpell(58887, false);
                        player->LearnSpell(47836, false);
                        player->LearnSpell(61290, false);
                        player->LearnSpell(48018, false);
                        player->LearnSpell(48020, false);
                        player->LearnSpell(33388, false);
                        player->LearnSpell(33391, false);
                        player->LearnSpell(23161, false);
                        if (player->HasSpell(17877))
                            player->LearnSpell(47827, false);
                        if (player->HasSpell(30283))
                            player->LearnSpell(47847, false);
                        if (player->HasSpell(30108))
                            player->LearnSpell(47843, false);
                        if (player->HasSpell(50796))
                            player->LearnSpell(59172, false);
                        if (player->HasSpell(48181))
                            player->LearnSpell(59164, false);
                        if (player->HasSpell(18220))
                            player->LearnSpell(59092, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_ROGUE:
                        player->LearnSpell(3127, false);
                        player->LearnSpell(42459, false);
                        player->LearnSpell(48668, false);
                        player->LearnSpell(48638, false);
                        player->LearnSpell(1784, false);
                        player->LearnSpell(48657, false);
                        player->LearnSpell(921, false);
                        player->LearnSpell(1776, false);
                        player->LearnSpell(26669, false);
                        player->LearnSpell(51724, false);
                        player->LearnSpell(6774, false);
                        player->LearnSpell(11305, false);
                        player->LearnSpell(1766, false);
                        player->LearnSpell(48676, false);
                        player->LearnSpell(48659, false);
                        player->LearnSpell(1804, false);
                        player->LearnSpell(8647, false);
                        player->LearnSpell(48691, false);
                        player->LearnSpell(51722, false);
                        player->LearnSpell(48672, false);
                        player->LearnSpell(1725, false);
                        player->LearnSpell(26889, false);
                        player->LearnSpell(2836, false);
                        player->LearnSpell(1833, false);
                        player->LearnSpell(1842, false);
                        player->LearnSpell(8643, false);
                        player->LearnSpell(2094, false);
                        player->LearnSpell(1860, false);
                        player->LearnSpell(57993, false);
                        player->LearnSpell(48674, false);
                        player->LearnSpell(31224, false);
                        player->LearnSpell(5938, false);
                        player->LearnSpell(57934, false);
                        player->LearnSpell(51723, false);
                        if (player->HasSpell(16511))
                            player->LearnSpell(48660, false);
                        if (player->HasSpell(1329))
                            player->LearnSpell(48666, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_PRIEST:
                        player->LearnSpell(528, false);
                        player->LearnSpell(2053, false);
                        player->LearnSpell(48161, false);
                        player->LearnSpell(48123, false);
                        player->LearnSpell(48125, false);
                        player->LearnSpell(48066, false);
                        player->LearnSpell(586, false);
                        player->LearnSpell(48068, false);
                        player->LearnSpell(48127, false);
                        player->LearnSpell(48171, false);
                        player->LearnSpell(48168, false);
                        player->LearnSpell(10890, false);
                        player->LearnSpell(6064, false);
                        player->LearnSpell(988, false);
                        player->LearnSpell(48300, false);
                        player->LearnSpell(6346, false);
                        player->LearnSpell(48071, false);
                        player->LearnSpell(48135, false);
                        player->LearnSpell(48078, false);
                        player->LearnSpell(453, false);
                        player->LearnSpell(10955, false);
                        player->LearnSpell(10909, false);
                        player->LearnSpell(8129, false);
                        player->LearnSpell(48073, false);
                        player->LearnSpell(605, false);
                        player->LearnSpell(48072, false);
                        player->LearnSpell(48169, false);
                        player->LearnSpell(552, false);
                        player->LearnSpell(1706, false);
                        player->LearnSpell(48063, false);
                        player->LearnSpell(48162, false);
                        player->LearnSpell(48170, false);
                        player->LearnSpell(48074, false);
                        player->LearnSpell(48158, false);
                        player->LearnSpell(48120, false);
                        player->LearnSpell(34433, false);
                        player->LearnSpell(48113, false);
                        player->LearnSpell(32375, false);
                        player->LearnSpell(64843, false);
                        player->LearnSpell(64901, false);
                        player->LearnSpell(53023, false);
                        if (player->HasSpell(34914))
                            player->LearnSpell(48160, false);
                        if (player->HasSpell(47540))
                            player->LearnSpell(53007, false);
                        if (player->HasSpell(724))
                            player->LearnSpell(48087, false);
                        if (player->HasSpell(19236))
                            player->LearnSpell(48173, false);
                        if (player->HasSpell(34861))
                            player->LearnSpell(48089, false);
                        if (player->HasSpell(15407))
                            player->LearnSpell(48156, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_DEATH_KNIGHT:
                        player->LearnSpell(3127, false);
                        player->LearnSpell(50842, false);
                        player->LearnSpell(49941, false);
                        player->LearnSpell(49930, false);
                        player->LearnSpell(47476, false);
                        player->LearnSpell(45529, false);
                        player->LearnSpell(3714, false);
                        player->LearnSpell(56222, false);
                        player->LearnSpell(48743, false);
                        player->LearnSpell(48263, false);
                        player->LearnSpell(49909, false);
                        player->LearnSpell(47528, false);
                        player->LearnSpell(45524, false);
                        player->LearnSpell(48792, false);
                        player->LearnSpell(57623, false);
                        player->LearnSpell(56815, false);
                        player->LearnSpell(47568, false);
                        player->LearnSpell(49895, false);
                        player->LearnSpell(50977, false);
                        player->LearnSpell(49576, false);
                        player->LearnSpell(49921, false);
                        player->LearnSpell(46584, false);
                        player->LearnSpell(49938, false);
                        player->LearnSpell(48707, false);
                        player->LearnSpell(48265, false);
                        player->LearnSpell(61999, false);
                        player->LearnSpell(42650, false);
                        player->LearnSpell(53428, false);
                        player->LearnSpell(53331, false);
                        player->LearnSpell(54447, false);
                        player->LearnSpell(53342, false);
                        player->LearnSpell(54446, false);
                        player->LearnSpell(53323, false);
                        player->LearnSpell(53344, false);
                        player->LearnSpell(70164, false);
                        player->LearnSpell(62158, false);
                        player->LearnSpell(33391, false);
                        player->LearnSpell(48778, false);
                        player->LearnSpell(51425, false);
                        player->LearnSpell(49924, false);
                        if (player->HasSpell(55050))
                            player->LearnSpell(55262, false);
                        if (player->HasSpell(49143))
                            player->LearnSpell(55268, false);
                        if (player->HasSpell(49184))
                            player->LearnSpell(51411, false);
                        if (player->HasSpell(55090))
                            player->LearnSpell(55271, false);
                        if (player->HasSpell(49158))
                            player->LearnSpell(51328, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_SHAMAN:
                        player->LearnSpell(2062, false);
                        player->LearnSpell(8737, false);
                        player->LearnSpell(49273, false);
                        player->LearnSpell(49238, false);
                        player->LearnSpell(10399, false);
                        player->LearnSpell(49231, false);
                        player->LearnSpell(58753, false);
                        player->LearnSpell(2484, false);
                        player->LearnSpell(49281, false);
                        player->LearnSpell(58582, false);
                        player->LearnSpell(49233, false);
                        player->LearnSpell(58790, false);
                        player->LearnSpell(58704, false);
                        player->LearnSpell(58643, false);
                        player->LearnSpell(49277, false);
                        player->LearnSpell(61657, false);
                        player->LearnSpell(8012, false);
                        player->LearnSpell(526, false);
                        player->LearnSpell(2645, false);
                        player->LearnSpell(57994, false);
                        player->LearnSpell(8143, false);
                        player->LearnSpell(49236, false);
                        player->LearnSpell(58796, false);
                        player->LearnSpell(58757, false);
                        player->LearnSpell(49276, false);
                        player->LearnSpell(57960, false);
                        player->LearnSpell(131, false);
                        player->LearnSpell(58745, false);
                        player->LearnSpell(6196, false);
                        player->LearnSpell(58734, false);
                        player->LearnSpell(58774, false);
                        player->LearnSpell(58739, false);
                        player->LearnSpell(58656, false);
                        player->LearnSpell(546, false);
                        player->LearnSpell(556, false);
                        player->LearnSpell(66842, false);
                        player->LearnSpell(51994, false);
                        player->LearnSpell(8177, false);
                        player->LearnSpell(58749, false);
                        player->LearnSpell(20608, false);
                        player->LearnSpell(36936, false);
                        player->LearnSpell(36936, false);
                        player->LearnSpell(58804, false);
                        player->LearnSpell(49271, false);
                        player->LearnSpell(8512, false);
                        player->LearnSpell(6495, false);
                        player->LearnSpell(8170, false);
                        player->LearnSpell(66843, false);
                        player->LearnSpell(55459, false);
                        player->LearnSpell(66844, false);
                        player->LearnSpell(3738, false);
                        player->LearnSpell(2894, false);
                        player->LearnSpell(60043, false);
                        player->LearnSpell(51514, false);
                        if (player->GetCFSTeam() == ALLIANCE)
                            player->LearnSpell(32182, false);
                        if (player->GetCFSTeam() == HORDE)
                            player->LearnSpell(2825, false);
                        if (player->HasSpell(61295))
                            player->LearnSpell(61301, false);
                        if (player->HasSpell(974))
                            player->LearnSpell(49284, false);
                        if (player->HasSpell(30706))
                            player->LearnSpell(57722, false);
                        if (player->HasSpell(51490))
                            player->LearnSpell(59159, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;

                    case CLASS_DRUID:
                        player->LearnSpell(48378, false);
                        player->LearnSpell(48469, false);
                        player->LearnSpell(48461, false);
                        player->LearnSpell(48463, false);
                        player->LearnSpell(48441, false);
                        player->LearnSpell(53307, false);
                        player->LearnSpell(53308, false);
                        player->LearnSpell(5487, false);
                        player->LearnSpell(48560, false);
                        player->LearnSpell(6795, false);
                        player->LearnSpell(48480, false);
                        player->LearnSpell(53312, false);
                        player->LearnSpell(18960, false);
                        player->LearnSpell(5229, false);
                        player->LearnSpell(48443, false);
                        player->LearnSpell(50763, false);
                        player->LearnSpell(8983, false);
                        player->LearnSpell(8946, false);
                        player->LearnSpell(1066, false);
                        player->LearnSpell(48562, false);
                        player->LearnSpell(783, false);
                        player->LearnSpell(770, false);
                        player->LearnSpell(16857, false);
                        player->LearnSpell(18658, false);
                        player->LearnSpell(768, false);
                        player->LearnSpell(1082, false);
                        player->LearnSpell(5215, false);
                        player->LearnSpell(48477, false);
                        player->LearnSpell(49800, false);
                        player->LearnSpell(48465, false);
                        player->LearnSpell(48572, false);
                        player->LearnSpell(26995, false);
                        player->LearnSpell(48574, false);
                        player->LearnSpell(2782, false);
                        player->LearnSpell(50213, false);
                        player->LearnSpell(2893, false);
                        player->LearnSpell(33357, false);
                        player->LearnSpell(5209, false);
                        player->LearnSpell(48575, false);
                        player->LearnSpell(48447, false);
                        player->LearnSpell(48577, false);
                        player->LearnSpell(48579, false);
                        player->LearnSpell(5225, false);
                        player->LearnSpell(22842, false);
                        player->LearnSpell(49803, false);
                        player->LearnSpell(9634, false);
                        player->LearnSpell(20719, false);
                        player->LearnSpell(48467, false);
                        player->LearnSpell(29166, false);
                        player->LearnSpell(62600, false);
                        player->LearnSpell(22812, false);
                        player->LearnSpell(48470, false);
                        player->LearnSpell(33943, false);
                        player->LearnSpell(49802, false);
                        player->LearnSpell(48451, false);
                        player->LearnSpell(48568, false);
                        player->LearnSpell(33786, false);
                        player->LearnSpell(40120, false);
                        player->LearnSpell(62078, false);
                        player->LearnSpell(52610, false);
                        player->LearnSpell(50464, false);
                        player->LearnSpell(48570, false);
                        if (player->HasSpell(50516))
                            player->LearnSpell(61384, false);
                        if (player->HasSpell(48505))
                            player->LearnSpell(53201, false);
                        if (player->HasSpell(48438))
                            player->LearnSpell(53251, false);
                        if (player->HasSpell(5570))
                            player->LearnSpell(48468, false);
                        player->SaveToDB();
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    break;
                }
                case 8: // add bug
                {
                    if (player->HasItemCount(23162, 4))
                    {
                        player->PlayerTalkClass->SendCloseGossip();
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_ALREADY_ITEM);
                    }
                    else
                    {
                        player->PlayerTalkClass->SendCloseGossip();
                        player->AddItem(23162, 4);
                    }
                    break;
                }
                case 9: // Level gm characters
                {
                    player->GiveLevel(80);
                    player->SaveToDB();
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_LEVELUP);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 10: // VIP character management
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_COMMAND_LIST), GOSSIP_SENDER_MAIN, 13);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CHARACTER_MANAGEMENT), GOSSIP_SENDER_MAIN, 14);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CHARACTER_CHANGEING), GOSSIP_SENDER_MAIN, 15);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_REPAIR_EQUIP), GOSSIP_SENDER_MAIN, 16);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_REMOVE_DESERTER), GOSSIP_SENDER_MAIN, 17);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_REMOVE_WEAKNESS), GOSSIP_SENDER_MAIN, 18);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_GIVE_BUFFS), GOSSIP_SENDER_MAIN, 19);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_VIP_BANK), GOSSIP_SENDER_MAIN, 20);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_26), GOSSIP_SENDER_MAIN, 202);
                    //if (!player->IsInCombat() || !player->IsInFlight() || !player->GetMap()->IsBattlegroundOrArena() || !player->HasStealthAura() || !player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || !player->isDead())
                    //	AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_ITEM_REMOVE_COOLDOWN), GOSSIP_SENDER_MAIN, 22);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 11:  // VIP rates info
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_RATES), GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(XP, uint32(sWorld->getRate(RATE_XP_KILL))).c_str(), GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(XP_quest, uint32(sWorld->getRate(RATE_XP_QUEST))).c_str(), GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(rep, uint32(sWorld->getRate(RATE_REPUTATION_GAIN))).c_str(), GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(honor, uint32(sWorld->getRate(RATE_HONOR))).c_str(), GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, getString2(gold, uint32(sWorld->getRate(RATE_DROP_MONEY))).c_str(), GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 12: // dual spec
                {
                    if (player->GetTalentGroupsCount() == 1 && !(player->GetLevel()))
                        player->CastSpell(player, 63680, true);
                    player->CastSpell(player, 63624, true);
                    CloseGossipMenuFor(player);
                    break;
                }
                case 13: // Vip info
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("|cff006699The next VIP commands are available for you:");
                    if (sGameConfig->GetBoolConfig("Vip.Debuff.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip debuff|r - Drop debuff Deserter and Resurrection Sickness");
                    if (sGameConfig->GetBoolConfig("Command.Bank.Premium"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip bank|r - Open bank window");
                    if (sGameConfig->GetBoolConfig("Command.Mail.Premium"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip mail|r - Open mail box");
                    if (sGameConfig->GetBoolConfig("Vip.Repair.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip repair|r - Repair without costs");
                    if (sGameConfig->GetBoolConfig("Vip.Reset.Talents.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip resettalents|r - Reset talents");
                    if (sGameConfig->GetBoolConfig("Vip.Taxi.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip taxi|r - Open taxi window");
                    if (sGameConfig->GetBoolConfig("Vip.Home.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip home|r - Teleport at Home position (Hearthstone)");
                    if (sGameConfig->GetBoolConfig("Vip.Capital.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip capital|r - Teleport at your main city");
                    if (sGameConfig->GetBoolConfig("Vip.Changerace.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip changerace|r - Change Race");
                    if (sGameConfig->GetBoolConfig("Vip.Customize.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip customize|r - Customize character");
                    if (sGameConfig->GetBoolConfig("Vip.Appear.Command"))
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff006699 .vip app|r - Teleport at your mate in Group");
                    CloseGossipMenuFor(player);
                    break;
                }
                case 14: //sub menu
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_DUAL_SPEC), GOSSIP_SENDER_MAIN, 12);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_RESET_TALENT), GOSSIP_SENDER_MAIN, 71, GTS1(LANG_ITEM_RESET_TALENT_SURE), 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_SKILLS_WEAPON), GOSSIP_SENDER_MAIN, 72);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_MAXSKILL), GOSSIP_SENDER_MAIN, 73);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_72), GOSSIP_SENDER_MAIN, 23);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 15: // sub menu
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CHANGE_NAME), GOSSIP_SENDER_MAIN, 74);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CHANGE_OF_APPEARANCE), GOSSIP_SENDER_MAIN, 75);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CHANGE_FACTION), GOSSIP_SENDER_MAIN, 76);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CHANGE_RACE), GOSSIP_SENDER_MAIN, 77);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 16: // repair
                {
                    player->DurabilityRepairAll(false, 0.0f, true);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_REPAIR_CONFIRM);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 17:
                {
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
                }
                case 18:
                {
                    player->RemoveAura(15007);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 19:
                {
                    if (player->HasAura(45523))
                    {
                        player->PlayerTalkClass->SendCloseGossip();
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS1(LANG_ITEM_MSG_COOLDOWN));
                    }
                    else
                    {
                        player->RemoveAurasByType(SPELL_AURA_MOUNTED);
                        for (size_t i = 0; i < aurassSizer; ++i)
                            player->AddAura(aurassz[i], player);
                        player->GetSession()->SendNotification("|cffC67171Buffs received!");
                        player->CastSpell(player, 45523, true);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    break;
                }
                case 20:
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->GetSession()->SendShowBank(player->GetGUID());
                    break;
                }
                case 22:
                {
                    player->RemoveArenaSpellCooldowns(true);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 23:
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    player->ResetDailyQuestStatus();
                    player->ResetWeeklyQuestStatus();
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MSG_RESET_RAID_INSTANCES_COOLDOWN_COMPLETE);
                    break;
                }
                case 24:
                {
                    // honor token
                    // 44115
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/spell_frost_wizardmark:25:25:-15:0|tWintergrasp Commendation - 5 coins", GOSSIP_SENDER_MAIN, 70, "Are you sure you want to buy item?", 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 70:
                {
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = coast5 - coins;

                    if (coins >= coast5)
                    {
                        ostatok = coins - coast5;

                        if (player->IsPlayer())
                        {
                            player->AddItem(44115, 10);
                        }
                        else
                        {
                            player->AddItem(44115, 10);
                        }
                        player->SetCoins(ostatok);
                        AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_VIP_TIME, coast5);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_ENOUGH_COINS);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_HAVE_COINS, coins);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_NEED_COINS, ostatok);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    break;
                }
                case 25:
                {
                    // cloth
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_chest_cloth_25:25:25:-15:0|tCloth armor - 5 coins", GOSSIP_SENDER_MAIN, 26);
                    // leather
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_chest_leather_03:25:25:-15:0|tLeather armor - 5 coins", GOSSIP_SENDER_MAIN, 27);
                    // mail
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_chest_chain_07:25:25:-15:0|tMail armor - 5 coins", GOSSIP_SENDER_MAIN, 28);
                    // plate
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_chest_plate13:25:25:-15:0|tPlate armor - 5 coins", GOSSIP_SENDER_MAIN, 29);
                    // weapon
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_staff_13:25:25:-15:0|tWeapons - 10-20 coins", GOSSIP_SENDER_MAIN, 30);
                    // others
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/ICONS/Inv_gizmo_khoriumpowercore:25:25:-15:0|tOthers - 5 coins", GOSSIP_SENDER_MAIN, 31);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 34: // (FAMILY) cloth armor GTS2(NOT_USED_11)
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "rx")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 33: // (FAMILY) cloth armor
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_baka")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 26: // (FAMILY) cloth armor
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_cloth_armor")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 25);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 27: // (FAMILY) leather armor
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_leather_armor")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 25);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 28: // (FAMILY) mail armor
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_mail_armor")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 25);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 29: // (FAMILY) plate armor
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_plate_armor")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 25);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 30: // (FAMILY) weapon
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_weapon")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 25);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 31: // (FAMILY) trinket and rings
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "family_trinket_and_ring")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string("|TInterface/ICONS/Spell_chargenegative:25:25:-15:0|t ") + "Back", GOSSIP_SENDER_MAIN, 25);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 32:
                {
                    for (ItemShopContainer::const_iterator itr = itemshopMap.begin(); itr != itemshopMap.end(); ++itr)
                    {
                        if (itr->second.groupName == "mounts")
                        {
                            std::string text = fmt::format("{} ({} coins)", itr->second.itemName, itr->second.coinCost);
                            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, text, GOSSIP_SENDER_MAIN, itr->second.gossipAction, "Are you sure you want to buy it?", 0, false);
                        }
                    }
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                // FREE CASES 33-71
                case 71: // reset tall
                {
                    player->ResetTalents(true);
                    player->SendTalentsInfoData(false);
                    CloseGossipMenuFor(player);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_TALENT_RESET_CONFIRM);
                    break;
                }
                case 72: // weapon skill
                {
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
                }
                case 73: // skills max
                {
                    CloseGossipMenuFor(player);
                    player->UpdateWeaponsSkillsToMaxSkillsForLevel();
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_MAXSKILL_CONFIRM);
                    break;
                }
                case 74: // Change name
                {
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
                }
                case 75: // Customize
                {
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
                }
                case 76: // Change Faction
                {
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
                }
                case 77: // Change Race
                {
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
                case 78:
                {
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GTS1(LANG_ITEM_BUY_7), GOSSIP_SENDER_MAIN, 80, GTS1(LANG_ITEM_BUY_7_SURE), 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GTS1(LANG_ITEM_BUY_14), GOSSIP_SENDER_MAIN, 81, GTS1(LANG_ITEM_BUY_14_SURE), 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GTS1(LANG_ITEM_BUY_31), GOSSIP_SENDER_MAIN, 82, GTS1(LANG_ITEM_BUY_31_SURE), 0, false);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 79:
                {
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GTS1(LANG_ITEM_INC_7), GOSSIP_SENDER_MAIN, 80, GTS1(LANG_ITEM_INC_7_SURE), 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GTS1(LANG_ITEM_INC_14), GOSSIP_SENDER_MAIN, 81, GTS1(LANG_ITEM_INC_14_SURE), 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GTS1(LANG_ITEM_INC_31), GOSSIP_SENDER_MAIN, 82, GTS1(LANG_ITEM_INC_31_SURE), 0, false);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 80:
                {
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = coast7 - coins; // 7 day

                    if (coins >= coast7)
                    {
                        ostatok = coins - coast7;
                        time_t unsetdate;
                        if (player->GetAditionalData()->isPremium())
                        {
                            unsetdate = player->GetAditionalData()->getPremiumUnsetdate() + 604800; // 7 day
                            AccountMgr::UpdateVipStatus(player->GetSession()->GetAccountId(), unsetdate);
                        }
                        else
                        {
                            unsetdate = GameTime::GetGameTime() + 604800; // 7 day
                            AccountMgr::SetVipStatus(player->GetSession()->GetAccountId(), unsetdate);
                        }
                        player->GetAditionalData()->setPremiumUnsetdate(unsetdate);
                        player->GetAditionalData()->setPremiumStatus(true);
                        player->SetCoins(ostatok);
                        AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_VIP_TIME, coast7);
                        CloseGossipMenuFor(player);
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_ENOUGH_COINS);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_HAVE_COINS, coins);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_NEED_COINS, ostatok);
                        CloseGossipMenuFor(player);
                    }
                    break;
                }
                case 81:
                {
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = coast14 - coins; // 14 day

                    if (coins >= coast14)
                    {
                        ostatok = coins - coast14;
                        time_t unsetdate;
                        if (player->GetAditionalData()->isPremium())
                        {
                            unsetdate = player->GetAditionalData()->getPremiumUnsetdate() + 1209600; // 14 day
                            AccountMgr::UpdateVipStatus(player->GetSession()->GetAccountId(), unsetdate);
                        }
                        else
                        {
                            unsetdate = GameTime::GetGameTime() + 1209600; // 14 day
                            AccountMgr::SetVipStatus(player->GetSession()->GetAccountId(), unsetdate);
                        }
                        player->GetAditionalData()->setPremiumUnsetdate(unsetdate);
                        player->GetAditionalData()->setPremiumStatus(true);
                        player->SetCoins(ostatok);
                        AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_VIP_TIME, coast14);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_ENOUGH_COINS);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_HAVE_COINS, coins);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_NEED_COINS, ostatok);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    break;
                }
                case 82:
                {
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = coast31 - coins; // 31 day

                    if (coins >= coast31)
                    {
                        ostatok = coins - coast31;
                        time_t unsetdate;
                        //if (player->IsPlayer())
                        if (player->GetAditionalData()->isPremium())
                        {
                            unsetdate = player->GetAditionalData()->getPremiumUnsetdate() + 2678400; // 31 day
                            AccountMgr::UpdateVipStatus(player->GetSession()->GetAccountId(), unsetdate);
                        }
                        else
                        {
                            unsetdate = GameTime::GetGameTime() + 2678400; // 31 day
                            AccountMgr::SetVipStatus(player->GetSession()->GetAccountId(), unsetdate);
                        }
                        player->GetAditionalData()->setPremiumUnsetdate(unsetdate);
                        player->GetAditionalData()->setPremiumStatus(true);
                        player->SetCoins(ostatok);
                        AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_VIP_TIME, coast31);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_NOT_ENOUGH_COINS);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_HAVE_COINS, coins);
                        ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_YOU_NEED_COINS, ostatok);
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                    break;
                }
                case 83: // CLASS_WARRIOR start-pack
                {
                    player->InstallItemPresentBySlot(1);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 84: // CLASS_PALADIN start-pack
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Retribution", GOSSIP_SENDER_MAIN, 93);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Holy", GOSSIP_SENDER_MAIN, 94);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 85: // CLASS_HUNTER start-pack
                {
                    player->InstallItemPresentBySlot(3);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 86: // CLASS_ROGUE start-pack
                {
                    player->InstallItemPresentBySlot(4);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 87: // CLASS_PRIEST start-pack
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Shadow", GOSSIP_SENDER_MAIN, 95);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Healer", GOSSIP_SENDER_MAIN, 96);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 88: // CLASS_DEATH_KNIGHT start-pack
                {
                    player->InstallItemPresentBySlot(6);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 89: // CLASS_SHAMAN start-pack
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Elemental", GOSSIP_SENDER_MAIN, 97);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Enhancement", GOSSIP_SENDER_MAIN, 98);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Restoration", GOSSIP_SENDER_MAIN, 99);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 90: // CLASS_MAGE start-pack
                {
                    player->InstallItemPresentBySlot(8);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 91: // CLASS_WARLOCK start-pack
                {
                    player->InstallItemPresentBySlot(9);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 92: // CLASS_DRUID start-pack
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Balance", GOSSIP_SENDER_MAIN, 100);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Feral", GOSSIP_SENDER_MAIN, 101);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Receive start-pack items For Restoration", GOSSIP_SENDER_MAIN, 102);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 93: // CLASS_PALADIN start-pack retribution
                {
                    player->InstallItemPresentBySlot(2);
                    player->InstallItemPresentBySlot(21);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 94: // CLASS_PALADIN start-pack holy
                {
                    player->InstallItemPresentBySlot(2);
                    player->InstallItemPresentBySlot(22);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 95: // CLASS_PRIEST start-pack shadow
                {
                    player->InstallItemPresentBySlot(5);
                    player->InstallItemPresentBySlot(51);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 96: // CLASS_PRIEST start-pack holy
                {
                    player->InstallItemPresentBySlot(5);
                    player->InstallItemPresentBySlot(52);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 97: // CLASS_SHAMAN start-pack elem
                {
                    player->InstallItemPresentBySlot(7);
                    player->InstallItemPresentBySlot(71);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 98: // CLASS_SHAMAN start-pack ench
                {
                    player->InstallItemPresentBySlot(7);
                    player->InstallItemPresentBySlot(72);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 99: // CLASS_SHAMAN start-pack restor
                {
                    player->InstallItemPresentBySlot(7);
                    player->InstallItemPresentBySlot(73);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 100: // CLASS_DRUID start-pack sova
                {
                    player->InstallItemPresentBySlot(10);
                    player->InstallItemPresentBySlot(101);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 101: // CLASS_DRUID start-pack cat
                {
                    player->InstallItemPresentBySlot(10);
                    player->InstallItemPresentBySlot(102);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 102: // CLASS_DRUID start-pack restor
                {
                    player->InstallItemPresentBySlot(10);
                    player->InstallItemPresentBySlot(103);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case 103: // Guild Menu
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }
                case 104: // Guild Info
                {
                    CloseGossipMenuFor(player);
                    break;
                }
                case 105: // Guild War info
                {
                    CloseGossipMenuFor(player);
                    break;
                }
                case 106: // Register Dalaran Crater queue
                {
                    CloseGossipMenuFor(player);
                    break;
                }
                case 107: // Leave Dalaran Crater queue
                {
                    CloseGossipMenuFor(player);
                    break;
                }
                case 192: // Teleport
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_1/*, loc_idx*/), GOSSIP_SENDER_MAIN, 193);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_2/*, loc_idx*/), GOSSIP_SENDER_MAIN, 194);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_3/*, loc_idx*/), GOSSIP_SENDER_MAIN, 195);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_4/*, loc_idx*/), GOSSIP_SENDER_MAIN, 196);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_5/*, loc_idx*/), GOSSIP_SENDER_MAIN, 197);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_6/*, loc_idx*/), GOSSIP_SENDER_MAIN, 198);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_42/*, loc_idx*/), GOSSIP_SENDER_MAIN, 201);

                    if (player->GetCFSTeam() == ALLIANCE)
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, GTS1(LANG_ITEM_STORMWIND), GOSSIP_SENDER_MAIN, 5, GTS1(LANG_ITEM_STORMWIND_SURE), 0, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, GTS1(LANG_ITEM_ORGRIMMAR), GOSSIP_SENDER_MAIN, 6, GTS1(LANG_ITEM_ORGRIMMAR_SURE), 0, false);

                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }

                case 193:
                {
                    if (player->GetLevel() <= 79)
                    {
                        CloseGossipMenuFor(player);
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_8/*, loc_idx*/));
                        return;
                    }

                    CloseGossipMenuFor(player);
                    player->TeleportTo(1, 5063.1f, -2078.97f, 1369.29f, 5.96975f);
                    break;
                }

                case 194:
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(0, 4262.23f, -2809.17f, 5.14639f, 5.86462f);
                    break;
                }

                case 195:
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(0, -11820.1f, 1232.7f, 1.1f, 5.12f);
                    break;
                }

                case 196:
                {
                    if (player->GetLevel() <= 79)
                    {
                        CloseGossipMenuFor(player);
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_8/*, loc_idx*/));
                        return;
                    }

                    CloseGossipMenuFor(player);
                    player->TeleportTo(1, -6801.19f, -2893.02f, 9.00388f, 0.158639f);
                    break;
                }

                case 197:
                {
                    if (player->GetLevel() <= 79)
                    {
                        CloseGossipMenuFor(player);
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_8));
                        return;
                    }

                    CloseGossipMenuFor(player);
                    player->TeleportTo(0, -11916.7f, -1215.72f, 92.289f, 4.72454f);
                    player->CastSpell(player, 45523, true);
                    break;
                }

                case 198:
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(1, 16265.27f, 16261.16f, 18.925f, 4.77061f);
                    player->CastSpell(player, 45523, true);
                    break;
                }
                case 199:
                {
                    uint32 pt = sGameConfig->GetIntConfig("COST_id"); //49426
                    uint32 pt2 = sGameConfig->GetIntConfig("COST_colvo"); //5
                    if (player->HasItemCount(pt, pt2))
                    {
                        player->DestroyItemCount(pt, pt2, true);
                        if (!player)
                            player = player->GetSession()->GetPlayer();

                        int8 diff = -1;
                        uint16 MapId = 0;

                        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
                        {
                            Player::BoundInstancesMap& binds = player->GetBoundInstances(Difficulty(i));
                            for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
                            {
                                InstanceSave* save = itr->second.save;
                                if (itr->first != player->GetMapId() && (!MapId || MapId == itr->first) && (diff == -1 || diff == save->GetDifficultyID()))
                                {
                                    std::string timeleft = secsToTimeString(save->GetResetTime() - GameTime::GetGameTime(), TimeFormat::ShortText);
                                    player->UnbindInstance(itr, Difficulty(i));
                                }
                                else
                                    ++itr;
                            }
                        }
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_13));
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    else
                        player->PlayerTalkClass->SendCloseGossip();
                    player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_14));
                    ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_14));

                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }

                case 200:
                {
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 + coins;

                    uint32 pt = sGameConfig->GetIntConfig("COST_id"); //49426
                    uint32 pt2 = sGameConfig->GetIntConfig("COST_colvo"); //5
                    if (player->HasItemCount(pt, pt2))
                    {
                        player->DestroyItemCount(pt, pt2, true);
                        {
                            ostatok = coins + 1000;
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 201:
                {
                    CloseGossipMenuFor(player);
                    player->TeleportTo(0, -4137.27f, -1059.16f, 167.925f, 4.77061f);
                    break;
                }

                case 202: //money obmen menu
                {
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_25), GOSSIP_SENDER_MAIN, 200);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_59), GOSSIP_SENDER_MAIN, 203);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_60), GOSSIP_SENDER_MAIN, 204);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_61), GOSSIP_SENDER_MAIN, 205);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_62), GOSSIP_SENDER_MAIN, 206);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_63), GOSSIP_SENDER_MAIN, 207);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_64), GOSSIP_SENDER_MAIN, 208);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_65), GOSSIP_SENDER_MAIN, 209);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS2(NOT_USED_66), GOSSIP_SENDER_MAIN, 210);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS1(LANG_ITEM_CLOSE), GOSSIP_SENDER_MAIN, 3);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }

                case 203:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id1");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id1_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            // ostatok = coins + 1000;
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 204:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id2");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id2_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            // ostatok = coins + 1000;
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 205:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id3");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id3_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 206:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id4");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id4_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 207:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id5");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id5_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 208:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id6");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id6_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 209:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id7");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id7_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 210:
                {
                    uint32 pt = sGameConfig->GetIntConfig("obmen_id8");
                    uint32 pt2 = sGameConfig->GetIntConfig("obmen_id8_col");
                    uint32 coins = player->GetVerifiedCoins();
                    uint32 ostatok = 1000 - coins;

                    if (coins >= 1000)
                    {
                        ostatok = coins - 1000;

                        player->AddItem(pt, pt2);
                        {
                            player->SetCoins(ostatok);
                            AccountMgr::SetCoins(player->GetSession()->GetAccountId(), ostatok);
                            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_SUCCESS_BOUGHT, ostatok);
                            CloseGossipMenuFor(player);
                        }
                    }
                    else
                    {
                        player->GetSession()->SendAreaTriggerMessage("%s", GTS2(NOT_USED_43));
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_43));
                        CloseGossipMenuFor(player);
                    }
                    break;
                }

                case 211:
                {
                    player->PlayerTalkClass->ClearMenus();
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_58), GOSSIP_SENDER_MAIN, 213);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_64), GOSSIP_SENDER_MAIN, 214);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_57), GOSSIP_SENDER_MAIN, 215);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_61), GOSSIP_SENDER_MAIN, 216);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_65), GOSSIP_SENDER_MAIN, 217);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_67), GOSSIP_SENDER_MAIN, 218);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_60), GOSSIP_SENDER_MAIN, 219);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_59), GOSSIP_SENDER_MAIN, 220);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_66), GOSSIP_SENDER_MAIN, 221);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_63), GOSSIP_SENDER_MAIN, 222);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_62), GOSSIP_SENDER_MAIN, 223);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_31), GOSSIP_SENDER_MAIN, 212);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }

                case 212:
                {
                    player->PlayerTalkClass->ClearMenus();
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_70), GOSSIP_SENDER_MAIN, 224);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_69), GOSSIP_SENDER_MAIN, 225);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, GTS1(LANG_GOSSIP_OPTION_71), GOSSIP_SENDER_MAIN, 226);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
                    break;
                }

                case 213:
                {
                    player->CastSpell(player, BlackSmithSPell);
                    player->SetSkill(BlackSmithSkill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, BlackSmithSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 214:
                {
                    player->CastSpell(player, LeatherWorkingSpell);
                    player->SetSkill(LeatherWorkingSKill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, LeatherWorkingSKill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 215:
                {
                    player->CastSpell(player, AlchemySPell);
                    player->SetSkill(AlchemySkill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, AlchemySkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 216:
                {
                    player->CastSpell(player, HerbalismSpell);
                    player->SetSkill(HerbalismSkill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, HerbalismSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 217:
                {
                    player->CastSpell(player, MiningSpell);
                    player->SetSkill(MiningSKill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, MiningSKill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 218:
                {
                    player->CastSpell(player, TailoringSpell);
                    player->SetSkill(TailoringSKill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, TailoringSKill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 219:
                {
                    player->CastSpell(player, EngSPell);
                    player->SetSkill(EngSkill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, EngSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 220:
                {
                    player->CastSpell(player, EnchanterSpell);
                    player->SetSkill(EnchanterSkill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, EnchanterSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 221:
                {
                    player->CastSpell(player, SkinningSPell);
                    player->SetSkill(SKinningSkill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, SKinningSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 222:
                {
                    player->CastSpell(player, JewelSpell);
                    player->SetSkill(JewelSKill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, JewelSKill);
                    player->SetSkill(MiningSKill, 0, 0, 0);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 223:
                {
                    player->CastSpell(player, InscriptSpell);
                    player->SetSkill(InscriptSKill, 1, 450, 450);
                    HandleLearnSkillRecipesHelper(player, InscriptSKill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 224:
                {
                    player->SetSkill(FirstAidSkill, 1, 450, 450);
                    player->CastSpell(player, FirstAidSPell);
                    HandleLearnSkillRecipesHelper(player, FirstAidSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 225:
                {
                    player->SetSkill(CookingSkill, 1, 450, 450);
                    player->CastSpell(player, CookingSpell);
                    HandleLearnSkillRecipesHelper(player, CookingSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                case 226:
                {
                    player->SetSkill(FishingSkill, 1, 450, 450);
                    player->CastSpell(player, FishingSpell);
                    HandleLearnSkillRecipesHelper(player, FishingSkill);
                    CloseGossipMenuFor(player);
                    break;
                }

                }
            }
        }
    }

    void OnGossipSelectCode(Player* player, uint32 /*menu_id*/, uint32 /*sender*/, uint32 action, const char* code) override
    {
        player->PlayerTalkClass->ClearMenus();

        uint32 codeUINT = (uint32)atol(code);
        if (!codeUINT)
            return;
        QueryResult SearchForCode = ZynDatabase.PQuery("SELECT id FROM promotion_codes WHERE code = {} AND collection = {} AND count_of_exists = 0", codeUINT, action);

        if (!*code)
            return;

        if (!action)
        {
            if (!sPromotionCodeMgr->CheckedEnteredCodeByPlayer(code, player, codeUINT))
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_PROMO_CODE_ERROR);
            else
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_PROMO_CODE_ACEPT);

            player->PlayerTalkClass->SendCloseGossip();
            return;
        }
        player->PlayerTalkClass->SendCloseGossip();
    }
};

void AddSC_ServerMenuPlayerGossip()
{
    new ServerMenuPlayerGossip();
}
