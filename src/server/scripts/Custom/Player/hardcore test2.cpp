#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"
#include "WorldSession.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Channel.h"
#include "AccountMgr.h"
#include "ChannelAppenders.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Language.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "SocialMgr.h"
#include "CharacterDatabase.h"
#include "StringConvert.h"
#include "Group.h"
#include "Guild.h"


class HardcoreModePlayerScript : public PlayerScript
{
public:
    HardcoreModePlayerScript() : PlayerScript("HardcoreModePlayerScript") {}

    void OnLogin(Player* player, bool /**/) override
    {
        if (IsHardcoreEnabled() && IsHardcorePlayer(player))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Вы играете в HARDCORE режиме! Смерть = удаление персонажа!");

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

    void HandleHardcoreDeath(Player* player, Unit* killer)
    {
        if (!IsHardcorePlayer(player))
            return;

        // Логирование смерти
        LogHardcoreDeath(player, killer);

        // Уведомление всего сервера
        std::string announcement = "Hardcore игрок " + player->GetName() +
            " погиб! Персонаж будет удален.";
        sWorld->SendServerMessage(SERVER_MSG_STRING, announcement.c_str());

        // Задержка перед удалением для скриншота/прощания
       // player->GetScheduler().Schedule(Milliseconds(10000), [player](TaskContext /*context*/)
          //  {
                DeleteHardcoreCharacter(player);
          //  });

        // Отключаем возможность воскрешения
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
            Player::DeleteFromDB(ObjectGuid::Create<HighGuid::Player>(guid), 0, true, true);

            // Планируем удаление персонажа после отключения
           // uint32 guid = player->GetGUID().GetCounter();
           // sWorld->GetScheduler().Schedule(Milliseconds(5000), [guid](TaskContext /*context*/)
             //   {
               //     Player::DeleteFromDB(ObjectGuid::Create<HighGuid::Player>(guid), 0, true, true);
              //  });
        }
    }
};

// Система достижений для hardcore игроков
class HardcoreAchievements
{
public:
    enum HardcoreAchievementIds
    {
        ACHIEVEMENT_HARDCORE_LEVEL_10 = 90001,
        ACHIEVEMENT_HARDCORE_LEVEL_20 = 90002,
        ACHIEVEMENT_HARDCORE_LEVEL_30 = 90003,
        ACHIEVEMENT_HARDCORE_LEVEL_40 = 90004,
        ACHIEVEMENT_HARDCORE_LEVEL_50 = 90005,
        ACHIEVEMENT_HARDCORE_LEVEL_60 = 90006,
        ACHIEVEMENT_HARDCORE_LEVEL_70 = 90007,
        ACHIEVEMENT_HARDCORE_LEVEL_80 = 90008,
        ACHIEVEMENT_HARDCORE_FIRST_DUNGEON = 90009,
        ACHIEVEMENT_HARDCORE_FIRST_RAID = 90010,
    };

    static void CheckLevelAchievements(Player* player)
    {
        if (!IsHardcorePlayer(player))
            return;

        uint32 level = player->GetLevel();

        if (level >= 10 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_10))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_10));

        if (level >= 20 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_20))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_20));

        if (level >= 30 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_30))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_30));

        if (level >= 40 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_40))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_40));

        if (level >= 50 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_50))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_50));

        if (level >= 60 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_60))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_60));

        if (level >= 70 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_70))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_70));

        if (level >= 80 && !player->HasAchieved(ACHIEVEMENT_HARDCORE_LEVEL_80))
            player->CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_HARDCORE_LEVEL_80));
        // ... и так далее для других уровней
    }

private:
    static bool IsHardcorePlayer(Player* player)
    {
        return player->HasFlag(PLAYER_FLAGS, 0x10000000);
    }
};

// Команда для включения hardcore режима
class HardcoreModeCommand : public CommandScript
{
public:
    HardcoreModeCommand() : CommandScript("HardcoreModeCommand") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> hardcoreCommandTable =
        {
            { "enable",        rbac::RBAC_PERM_COMMAND_RTX108, false, &HandleHardcoreEnableCommand,   "" },
            { "disable",       rbac::RBAC_PERM_COMMAND_RTX109, false, &HandleHardcoreDisableCommand,   "" },
            { "status",        rbac::RBAC_PERM_COMMAND_RTX110, false, &HandleHardcoreStatusCommand,   "" },
            { "leaderboard",   rbac::RBAC_PERM_COMMAND_RTX111, false, &HandleHardcoreLeaderboardCommand,   "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "hardcore", rbac::RBAC_PERM_COMMAND_RTX112, false, nullptr, "", hardcoreCommandTable },
        };
        return commandTable;
    }

    static bool HandleHardcoreEnableCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (player->GetLevel() > 1)
        {
            handler->SendSysMessage("Hardcore режим можно включить только на 1 уровне!");
            return true;
        }

        if (player->HasFlag(PLAYER_FLAGS, HARDCORE_FLAG))
        {
            handler->SendSysMessage("Hardcore режим уже включен!");
            return true;
        }

        player->SetFlag(PLAYER_FLAGS, HARDCORE_FLAG);
        //player->CastSpell(player, 61573, true); // Визуальная аура

        handler->SendSysMessage("|cffff0000Hardcore режим включен! Смерть = удаление персонажа!|r");

        std::ostringstream ss;
        ss << "UPDATE characters SET extra_flags = extra_flags | 1 WHERE guid = " << player->GetGUID();
        CharacterDatabase.Execute(ss.str().c_str());
        // Сохраняем в БД
        /*PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_HARDCORE_FLAG);
        stmt->setUInt32(0, HARDCORE_FLAG);
        stmt->setUInt32(1, player->GetGUID().GetCounter());
        CharacterDatabase.Execute(stmt);*/

        return true;
    }

    static bool HandleHardcoreDisableCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        std::string playerName = args;
        Player* target = ObjectAccessor::FindPlayerByName(playerName);

        if (!target)
        {
            handler->SendSysMessage("Игрок не найден!");
            return true;
        }

        target->RemoveFlag(PLAYER_FLAGS, HARDCORE_FLAG);
        //target->RemoveAurasDueToSpell(61573);

        handler->PSendSysMessage("Hardcore режим отключен для игрока %s", playerName.c_str());
        ChatHandler(target->GetSession()).SendSysMessage("Hardcore режим был отключен администратором.");

        return true;
    }

    static bool HandleHardcoreStatusCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (player->HasFlag(PLAYER_FLAGS, HARDCORE_FLAG))
        {
            handler->SendSysMessage("|cffff0000Hardcore режим: ВКЛЮЧЕН|r");

            // Показываем статистику
            uint32 playTime = player->GetTotalPlayedTime();
            uint32 hours = playTime / 3600;
            uint32 minutes = (playTime % 3600) / 60;

            handler->PSendSysMessage("Время игры: %u часов %u минут", hours, minutes);
            handler->PSendSysMessage("Уровень: %u", player->GetLevel());
        }
        else
        {
            handler->SendSysMessage("Hardcore режим: отключен");
        }

        return true;
    }

    static bool HandleHardcoreLeaderboardCommand(ChatHandler* handler, const char* /*args*/)
    {
        // Показываем топ живых hardcore игроков
        QueryResult result = CharacterDatabase.Query(
            "SELECT name, level, totaltime FROM characters "
            "WHERE playerFlags & 0x10000000 AND deleteDate IS NULL "
            "ORDER BY level DESC, totaltime ASC LIMIT 10");

        if (!result)
        {
            handler->SendSysMessage("Нет hardcore игроков.");
            return true;
        }

        handler->SendSysMessage("=== Топ Hardcore игроков ===");
        uint32 rank = 1;

        do
        {
            Field* fields = result->Fetch();
            std::string name = fields[0].GetString();
            uint32 level = fields[1].GetUInt32();
            uint32 playTime = fields[2].GetUInt32();
            uint32 hours = playTime / 3600;

            handler->PSendSysMessage("%u. %s - Уровень %u (%u часов)",
                rank++, name.c_str(), level, hours);
        } while (result->NextRow());

        return true;
    }

private:
    static const uint32 HARDCORE_FLAG = 0x10000000;
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

void AddSC_hardcore_mode()
{
    new HardcoreModePlayerScript();
    new HardcoreModeCommand();
    new hardcore_trade_restrictions();
}
/*
-- Таблица для логирования смертей hardcore игроков
CREATE TABLE IF NOT EXISTS `hardcore_deaths` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `char_guid` int(11) NOT NULL,
    `char_name` varchar(12) NOT NULL,
    `level` tinyint(3) unsigned NOT NULL,
    `playtime` int(11) unsigned NOT NULL,
    `killer_name` varchar(50) NOT NULL,
    `death_time` bigint(20) unsigned NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
*/

/*

#
#    Hardcore.Enable
#        Description: Enable hardcore mode functionality
#        Default:     0 (disabled)
#                     1 (enabled)

Hardcore.Enable = 1

#
#    Hardcore.BonusXP
#        Description: Bonus XP multiplier for hardcore players
#        Default:     1.5 (50% bonus)

Hardcore.BonusXP = 1.5

#
#    Hardcore.BonusLootChance
#        Description: Chance for bonus loot (percentage)
#        Default:     10

Hardcore.BonusLootChance = 10

#
#    Hardcore.DeathDelay
#        Description: Delay before character deletion (seconds)
#        Default:     10

Hardcore.DeathDelay = 10
*/
