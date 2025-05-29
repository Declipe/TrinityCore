#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"

class hardcore_commandscript : public CommandScript
{
public:
    hardcore_commandscript() : CommandScript("hardcore_commandscript") {}

        std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> hardcoreCommandTable =
        {
            { "on",        rbac::RBAC_PERM_COMMAND_RTX108, false, &HandleHardcoreEnableCommand,   "" },
            { "off",       rbac::RBAC_PERM_COMMAND_RTX109, false, &HandleHardcoreDisableCommand,   "" },
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


void AddSC_hardcore_commandscript()
{
    new hardcore_commandscript();
}
