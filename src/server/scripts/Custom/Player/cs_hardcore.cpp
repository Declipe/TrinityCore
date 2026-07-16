#include "ScriptMgr.h"
#include "Chat.h"
#include "ChatCommand.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"

#if TRINITY_COMPILER == TRINITY_COMPILER_GNU
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

using namespace Trinity::ChatCommands;

class hardcore_commandscript : public CommandScript
{
public:
    hardcore_commandscript() : CommandScript("hardcore_commandscript") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable hardcoreCommandTable =
        {
            { "on",          HandleHardcoreEnableCommand,          rbac::RBAC_PERM_COMMAND_RTX108, Console::No },
            { "off",         HandleHardcoreDisableCommand,         rbac::RBAC_PERM_COMMAND_RTX109, Console::No },
            { "status",      HandleHardcoreStatusCommand,          rbac::RBAC_PERM_COMMAND_RTX110, Console::No },
            { "leaderboard", HandleHardcoreLeaderboardCommand,     rbac::RBAC_PERM_COMMAND_RTX111, Console::No },
            { "addlife",     HandleHardcoreExtraLifeGiveCommand,   rbac::RBAC_PERM_COMMAND_RTX113, Console::No },
            { "checklife",   HandleHardcoreExtraLifeCheckCommand,  rbac::RBAC_PERM_COMMAND_RTX114, Console::No },
        };

        static ChatCommandTable commandTable =
        {
            { "hardcore", hardcoreCommandTable },
        };
        return commandTable;
    }

static bool HandleHardcoreExtraLifeGiveCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Player* target = nullptr;
        std::string name = args;
        if (!normalizePlayerName(name))
            return false;

        target = ObjectAccessor::FindPlayerByName(name.c_str());
        if (!target)
        {
            handler->SendSysMessage("Игрок не найден.");
            return false;
        }

        // Проверяем, есть ли у игрока уже дополнительная жизнь
        QueryResult result = CharacterDatabase.PQuery(
            "SELECT has_extra_life FROM hardcore_extra_lives WHERE player_guid = {}",
            target->GetGUID().GetCounter()
        );

        if (result && (*result)[0].GetBool())
        {
            handler->PSendSysMessage("У игрока %s уже есть дополнительная жизнь.", target->GetName().c_str());
            return true;
        }

        // Выдаем дополнительную жизнь
        if (!result)
        {
            CharacterDatabase.PExecute(
                "INSERT INTO hardcore_extra_lives (player_guid, has_extra_life) VALUES ({}, 1)",
                target->GetGUID().GetCounter()
            );
        }
        else
        {
            CharacterDatabase.PExecute(
                "UPDATE hardcore_extra_lives SET has_extra_life = 1 WHERE player_guid = {}",
                target->GetGUID().GetCounter()
            );
        }

        handler->PSendSysMessage("Вы выдали дополнительную жизнь игроку %s.", target->GetName().c_str());
        target->GetSession()->SendNotification("|cff00ff00Вам выдана дополнительная жизнь! Теперь у вас есть один шанс на воскрешение после смерти.|r");
        return true;
    }

    static bool HandleHardcoreExtraLifeCheckCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Player* target = nullptr;
        std::string name = args;
        if (!normalizePlayerName(name))
            return false;

        target = ObjectAccessor::FindPlayerByName(name.c_str());
        if (!target)
        {
            handler->SendSysMessage("Игрок не найден.");
            return false;
        }

        // Проверяем, есть ли у игрока дополнительная жизнь
        QueryResult result = CharacterDatabase.PQuery(
            "SELECT has_extra_life FROM hardcore_extra_lives WHERE player_guid = {}",
            target->GetGUID().GetCounter()
        );

        if (result && (*result)[0].GetBool())
        {
            handler->PSendSysMessage("У игрока %s есть дополнительная жизнь.", target->GetName().c_str());
        }
        else
        {
            handler->PSendSysMessage("У игрока %s нет дополнительной жизни.", target->GetName().c_str());
        }

        return true;
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
        ss << "UPDATE characters SET extra_flags = extra_flags | 1 WHERE guid = " << player->GetGUID().IsEmpty();
        CharacterDatabase.Execute(ss.str().c_str());
        // Сохраняем в БД
        /*PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_HARDCORE_FLAG);
        stmt->setUInt32(0, HARDCORE_FLAG);
        stmt->setUInt32(1, player->GetGUID().GetCounter());
        CharacterDatabase.Execute(stmt);*/

        return true;
    }

    static bool HandleHardcoreDisableCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
            player->RemoveFlag(PLAYER_FLAGS, 0x10000000);
            player->SaveToDB();
            handler->SendSysMessage("Hardcore режим был отключен.");

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


void AddSC_hardcore_commandscript()
{
    new hardcore_commandscript();
}
