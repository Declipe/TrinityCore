#include "AccountMgr.h"
#include "Channel.h"
#include "ChannelAppenders.h"
#include "CharacterDatabase.h"
#include "Chat.h"
#include "CustomConfig.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "Language.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Mail.h"
#include "ScriptMgr.h"
#include "SocialMgr.h"
#include "StringConvert.h"
#include "AchievementMgr.h"
#include "Item.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "World.h"
#include "WorldSession.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

class HardcoreModePlayerScript : public PlayerScript
{
public:
    HardcoreModePlayerScript() : PlayerScript("HardcoreModePlayerScript") {}

    void OnLogin(Player* player, bool /**/) override
    {
        if (IsHardcoreEnabled() && IsHardcorePlayer(player))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Вы играете в HARDCORE режиме! Смерть = удаление персонажа!");
        if (HasExtraLife(player))
            {
                ChatHandler(player->GetSession()).SendSysMessage(
                    "|cff00ff00У вас есть дополнительная жизнь! При смерти вы воскреснете с 50% здоровья, но потеряете этот бонус.|r");
            }
			CheckMissingLevelRewards(player);
            // Добавляем визуальные эффекты
          //  player->CastSpell(player, HARDCORE_AURA_SPELL, true);
        }

        /* if (Group* group = player->GetGroup())
         {
             group->RemoveMember(player->GetGUID());
             ChatHandler(player->GetSession()).SendSysMessage(
                 "|cffff0000[HARDCORE] Вы автоматически покинули группу.|r");
         }*/
    }

void CheckMissingLevelRewards(Player* player)
{
    uint8 currentLevel = player->GetLevel();

    // Получаем список всех доступных наград
    QueryResult rewards = CharacterDatabase.PQuery(
        "SELECT level FROM hardcore_level_rewards WHERE level <= {} ORDER BY level", currentLevel);

    if (!rewards)
        return;

    do
    {
        Field* fields = rewards->Fetch();
        uint8 rewardLevel = fields[0].GetUInt8();

        // Проверяем, получил ли игрок эту награду
        if (!HasReceivedLevelReward(player, rewardLevel))
        {
            // Выдаем пропущенную награду
            GiveHardcoreLevelReward(player, rewardLevel);
        }
    }
    while (rewards->NextRow());
}

void OnLevelChanged(Player* player, uint8 oldLevel) override
{
    if (!IsHardcorePlayer(player))
        return;

    uint8 newLevel = player->GetLevel();

    // Проверяем, достиг ли игрок нового уровня (не при понижении уровня)
    if (newLevel > oldLevel)
    {
        // Выдаем награду за достижение уровня
        GiveHardcoreLevelReward(player, newLevel);

        // Дополнительное уведомление
        ChatHandler(player->GetSession()).PSendSysMessage(
            "|cff00ff00[HARDCORE] Поздравляем с достижением уровня %u! Вы получили специальную награду.|r",
            newLevel);
    }
}

void GiveHardcoreLevelReward(Player* player, uint8 level)
{
    // Проверяем, не получил ли игрок уже эту награду
    if (HasReceivedLevelReward(player, level))
        return;

    // Получаем информацию о награде из БД
    QueryResult result = CharacterDatabase.PQuery(
        "SELECT item_id, item_count, money, title_id, achievement_id, spell_id "
        "FROM hardcore_level_rewards WHERE level = {}", level);

    if (!result)
        return;

    Field* fields = result->Fetch();
    uint32 itemId = fields[0].GetUInt32();
    uint32 itemCount = fields[1].GetUInt32();
    uint32 money = fields[2].GetUInt32();
    uint32 titleId = fields[3].GetUInt32();
    uint32 achievementId = fields[4].GetUInt32();
    uint32 spellId = fields[5].GetUInt32();

    // Выдаем предмет, если указан
    if (itemId)
    {
        // Проверяем шаблон предмета
        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);
        if (!itemTemplate)
        {
            TC_LOG_ERROR("sql.sql","Hardcore reward: Item template %u not found!", itemId);
            return;
        }

        // Проверяем место в инвентаре
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, itemCount);
        if (msg != EQUIP_ERR_OK)
        {
            // Создаем предмет
            Item* item = player->StoreNewItem(dest, itemId, true, GenerateItemRandomPropertyId(itemId));
            if (item)
            {
                player->SendNewItem(item, itemCount, true, false);

                // Пытаемся экипировать предмет, если это возможно
                if (itemTemplate->InventoryType != INVTYPE_NON_EQUIP)
                {
                    uint16 dest;
                    InventoryResult msg = player->CanEquipItem(item->GetSlot(), dest, item, false);
                    if (msg == EQUIP_ERR_OK)
                    {
                        player->EquipItem(dest, item, true);
                    }
                }
            }
        }
        else
        {
            // Если инвентарь заполнен, создаем предмет и отправляем по почте
            uint32 guid = player->GetGUID();
            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
            Item* mailItem = Item::CreateItem(itemId, itemCount, player);
            if (mailItem)
            {
                mailItem->SaveToDB(trans); // Сохраняем предмет в БД перед отправкой

                MailSender sender(MAIL_NORMAL, player->GetGUID().GetCounter());
                MailDraft draft("Hardcore Level Reward", "Congratulations on reaching level " + std::to_string(level) + "!");
                draft.AddItem(mailItem); // Теперь передаем указатель на созданный предмет
                draft.SendMailTo(trans, MailReceiver(player, guid), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM));

                ChatHandler(player->GetSession()).PSendSysMessage(
                    "|cff00ff00[HARDCORE] Ваш инвентарь заполнен. Предмет награды отправлен вам по почте.|r");
            }

            //// Создаем предмет
            //Item* item = player->StoreNewItem(dest, itemId, true, GenerateItemRandomPropertyId(itemId));
            //if (item)
            //{
            //    player->SendNewItem(item, itemCount, true, false);

            //    // Пытаемся экипировать предмет, если это возможно
            //    if (itemTemplate->InventoryType != INVTYPE_NON_EQUIP)
            //    {
            //        uint16 dest;
            //        InventoryResult msg = player->CanEquipItem(item->GetSlot(), dest, item, false);
            //        if (msg == EQUIP_ERR_OK)
            //        {
            //            player->EquipItem(dest, item, true);
            //        }
            //    }
            //}
        }
    }

    // Выдаем деньги
    if (money)
    {
        player->ModifyMoney(money);
    }

    // Выдаем титул
    if (titleId)
    {
        CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(titleId);
        if (titleInfo)
        {
            player->SetTitle(titleInfo);
        }
    }

    // Выдаем достижение
    if (achievementId)
    {
        AchievementEntry const* achievement = sAchievementStore.LookupEntry(achievementId);
        if (achievement)
        {
            player->CompletedAchievement(achievement);
        }
    }

    // Применяем заклинание (например, телепорт)
    if (spellId)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (spellInfo)
        {
            player->CastSpell(player, spellId, true);
        }
    }

    // Сохраняем информацию о полученной награде
    SaveReceivedLevelReward(player, level);
}

bool HasReceivedLevelReward(Player* player, uint8 level)
{
    QueryResult result = CharacterDatabase.PQuery(
        "SELECT 1 FROM hardcore_rewards_received WHERE player_guid = {} AND level = {}",
        player->GetGUID().GetCounter(), level);

    return result != nullptr;
}

void SaveReceivedLevelReward(Player* player, uint8 level)
{
    CharacterDatabase.PExecute(
        "INSERT INTO hardcore_rewards_received (player_guid, level, received_at) "
        "VALUES ({}, {}, UNIX_TIMESTAMP())",
        player->GetGUID().GetCounter(), level);
}

    void OnPlayerKilledByCreature(Creature* killer, Player* killed) override
    {
        HandleHardcoreDeath(killed, killer);
    }

    void OnPVPKill(Player* killer, Player* killed) override
    {
        if (IsHardcorePlayer(killed))
        {
            HandleHardcoreDeath(killed, killer);
        }
    }

    void OnCreatureKill(Player* player, Creature* /*killed*/) override
    {
        if (IsHardcorePlayer(player))
        {
            // Дополнительные награды за убийство в hardcore режиме
            //uint32 bonusXP = killed->GetExperience(player) * 0.5f; // +50% опыта
            //player->GiveXP(bonusXP, nullptr);

            // Шанс на дополнительный лут
            if (urand(1, 100) <= 10) // 10% шанс
            {
                ChatHandler(player->GetSession()).SendSysMessage("Hardcore бонус: дополнительный лут!");
                // Добавить логику дополнительного лута
            }
        }
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
    {
        if (IsHardcorePlayer(player))
        {
            // Бонус опыта для hardcore игроков
            amount = uint32(amount * 1.5f);
        }
    }

private:
    // static const uint32 HARDCORE_AURA_SPELL = 61573; // Визуальная аура
    static const uint32 HARDCORE_FLAG = 0x10000000;  // Флаг в customFlags

    bool IsHardcoreEnabled()
    {
        return sConfigMgr->GetBoolDefault("Hardcore.Enable", false);
    }

    bool IsHardcorePlayer(Player* player)
    {
        //return player->GetPlayerFlags() & HARDCORE_FLAG;
        return player->HasFlag(PLAYER_FLAGS, HARDCORE_FLAG);
    }

    bool HasExtraLife(Player* player)
    {
        QueryResult result = CharacterDatabase.PQuery(
            "SELECT has_extra_life FROM hardcore_extra_lives WHERE player_guid = {}",
            player->GetGUID().GetCounter()
        );

        return result && (*result)[0].GetBool();
    }

    void UseExtraLife(Player* player)
    {
        CharacterDatabase.PExecute(
            "UPDATE hardcore_extra_lives SET has_extra_life = 0 WHERE player_guid = {}",
            player->GetGUID().GetCounter()
        );
    }
	
    void HandleHardcoreDeath(Player* player, Unit* killer)
    {
        if (!IsHardcorePlayer(player))
            return;

        if (HasExtraLife(player))
        {
            // Воскрешаем игрока с 50% здоровья
            player->ResurrectPlayer(0.5f);
            player->SetHealth(player->GetMaxHealth() * 0.5f);

            // Убираем дополнительную жизнь
            UseExtraLife(player);

            // Уведомляем игрока
            ChatHandler(player->GetSession()).SendSysMessage(
                "|cffff0000Вы использовали дополнительную жизнь! Теперь вы воскрешены, но больше не сможете использовать этот шанс.|r");

            // Логируем использование дополнительной жизни
            LogHardcoreDeath(player, killer);
            return;
        }
		
        LogHardcoreDeath(player, killer);
        std::string announcement = "Hardcore игрок " + player->GetName() +
            " погиб! Персонаж будет удален.";
        sWorld->SendServerMessage(SERVER_MSG_STRING, announcement.c_str());

        DeleteHardcoreCharacter(player);

        player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST);

        ChatHandler(player->GetSession()).SendSysMessage(
            "|cffff0000Ваш hardcore персонаж погиб! Персонаж будет удален через 10 секунд.|r");
    }

    void LogHardcoreDeath(Player* player, Unit* killer)
    {
        std::string killerName = killer ? killer->GetName() : "Unknown";
        uint32 level = player->GetLevel();
        uint32 playTime = player->GetTotalPlayedTime();

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_HARDCORE_DEATH);
        stmt->setUInt32(0, player->GetGUID().GetCounter());
        stmt->setString(1, player->GetName());
        stmt->setUInt32(2, level);
        stmt->setUInt32(3, playTime);
        stmt->setString(4, killerName);
        stmt->setUInt64(5, time(nullptr));
        CharacterDatabase.Execute(stmt);
    }

    static void DeleteHardcoreCharacter(Player* player)
    {
        if (WorldSession* session = player->GetSession())
        {
            session->KickPlayer("Player died during a hardcore session.");

            uint32 guid = player->GetGUID().GetCounter();

            // Сразу удаляем
			if (sGameConfig->GetBoolConfig("config.hardcore.delcharacters"))
{
            Player::DeleteFromDB(ObjectGuid::Create<HighGuid::Player>(guid), 0, true, true);
}
            // Планируем удаление персонажа после отключения
           // uint32 guid = player->GetGUID().GetCounter();
           // sWorld->GetScheduler().Schedule(Milliseconds(5000), [guid](TaskContext /*context*/)
             //   {
               //     Player::DeleteFromDB(ObjectGuid::Create<HighGuid::Player>(guid), 0, true, true);
              //  });
        }
    }
};

class hardcore_trade_restrictions : public PlayerScript
{
public:
    hardcore_trade_restrictions() : PlayerScript("hardcore_trade_restrictions") {}

    // Проверяем при обновлении игрока
    void OnUpdate(Player* player, uint32 /*diff*/) override
    {
        // Проверяем активную торговлю каждые несколько секунд
        //static uint32 lastCheck = 0;
        //if (lastCheck++ % 100 != 0) // Проверяем не каждый тик
        //    return;

        if (IsHardcorePlayer(player) && player->GetTrader())
        {
            CancelTrade(player);
        }
    }

private:
    bool IsHardcorePlayer(Player* player)
    {
        return player->HasFlag(PLAYER_FLAGS, 0x10000000);
    }

    void CancelTrade(Player* player)
    {
        TradeStatusInfo info;

        if (Player* trader = player->GetTrader())
        {
            info.Status = TRADE_STATUS_TRADE_CANCELED;
            // Отменяем торговлю для обеих сторон
            player->GetSession()->SendTradeStatus(info);
            trader->GetSession()->SendTradeStatus(info);

            // Очищаем торговлю
            player->TradeCancel(false);

            // Уведомляем игроков
            ChatHandler(player->GetSession()).SendSysMessage(
                "|cffff0000[HARDCORE] Торговля отменена! Hardcore игроки не могут торговать.|r");
            ChatHandler(trader->GetSession()).SendSysMessage(
                "|cffff0000Торговля отменена! Ваш партнер играет в hardcore режиме.|r");
        }
    }
};

class hardcore_activator_ai : public GameObjectScript
{
public:
    hardcore_activator_ai() : GameObjectScript("hardcore_activator_ai") {}

    struct hardcore_activator_aiAI : public GameObjectAI
    {
        hardcore_activator_aiAI(GameObject* go) : GameObjectAI(go) {}


        bool OnGossipHello(Player* player) override
        {
            if (!player || player->HasFlag(PLAYER_FLAGS, 0x10000000))
            {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Disable Hardcore Mode", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            }
            else
            {
            if (player->GetLevel() <= 1)
            {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Enable Hardcore Mode", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                return true;
            }
        }
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            return GossipSelect(player, sender, action);
        }

        bool GossipSelect(Player* player, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1: // off Hardcore
                player->RemoveFlag(PLAYER_FLAGS, 0x10000000);
                player->SaveToDB();
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                //me->Whisper("Hardcore Mode has been disabled.", LANG_UNIVERSAL, player);
                ChatHandler(player->GetSession()).SendSysMessage("Hardcore Mode has been disabled.");
                CloseGossipMenuFor(player);
                break;

            case GOSSIP_ACTION_INFO_DEF + 2: // on Hardcore
                player->SetFlag(PLAYER_FLAGS, 0x10000000);
                std::ostringstream ss;
                ss << "UPDATE characters SET extra_flags = extra_flags | 1 WHERE guid = " << player->GetGUID();
                CharacterDatabase.Execute(ss.str().c_str());
                player->SaveToDB();
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                // me->Whisper("Hardcore Mode has been enabled. Be careful!", LANG_UNIVERSAL, player);
                ChatHandler(player->GetSession()).SendSysMessage("Hardcore Mode has been enabled. Be careful!");
                CloseGossipMenuFor(player);
                break;
            }
            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new hardcore_activator_aiAI(go);
    }
};

void AddSC_hardcore_mode()
{
    new HardcoreModePlayerScript();
    new hardcore_trade_restrictions();
	new hardcore_activator_ai();
}

/*
#
#    Hardcore.Enable
#        Description: Enable hardcore mode functionality
#        Default:     0 (disabled)
#                     1 (enabled)

Hardcore.Enable = 1
*/
