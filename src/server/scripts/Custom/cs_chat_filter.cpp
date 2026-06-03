/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Cell.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DatabaseEnvFwd.h"
#include "ObjectMgr.h"
#include "RBAC.h"
#include "ScriptMgr.h"

using namespace Trinity::ChatCommands;

class chat_filter_commandscript : public CommandScript
{
public:
    chat_filter_commandscript() : CommandScript("chat_filter_commandscript") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable chatFilterCommandTable =
        {
            { "add",    HandleChatFilterAddCommand,    rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            { "delete", HandleChatFilterDeleteCommand, rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            //{ "remove", HandleChatFilterDeleteCommand, rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            { "info", HandleChatFilterPunishmentsCommand, rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
            { "reload", HandleChatFilterReloadCommand, rbac::RBAC_PERM_COMMAND_GM, Console::Yes },
        };

        static ChatCommandTable commandTable =
        {
            { "chatfilter", chatFilterCommandTable },
        };

        return commandTable;
    }

    // .chatfilter add <word> [punishment]
    static bool HandleChatFilterAddCommand(ChatHandler* handler, std::string word, Optional<uint32> punishment)
    {
        if (word.empty())
            return false;

        ZynDatabase.EscapeString(word);

        QueryResult result = ZynDatabase.PQuery("SELECT word FROM chat_filter WHERE word = '{}'", word);
        if (result)
        {
            handler->SendErrorMessage("Chat filter word \"{}\" already exists.", word);
            return false;
        }

        // uint32 newId = 1;
        uint32 punish = punishment.value_or(0);

        /*QueryResult maxResult = ZynDatabase.Query("SELECT MAX(id) FROM chat_filter");
        if (maxResult)
            newId = (*maxResult)[0].GetUInt32() + 1;*/

            //ZynDatabase.PQuery("INSERT INTO chat_filter (id, word, punishment) VALUES ({}, '{}', {})", newId, escaped, punish);
        ZynDatabase.PQuery("INSERT INTO chat_filter (word, punishment) VALUES ('{}', {})", word, punish);

        sObjectMgr->LoadChatFilter();

        handler->PSendSysMessage("Chat filter word \"{}\" added (punishment: {}).", word, punish);
        return true;
    }

    // .chatfilter delete <word>
    static bool HandleChatFilterDeleteCommand(ChatHandler* handler, std::string word)
    {
        if (word.empty())
            return false;

        ZynDatabase.EscapeString(word);

        QueryResult result = ZynDatabase.PQuery("SELECT word FROM chat_filter WHERE word = '{}'", word);
        if (!result)
        {
            handler->SendErrorMessage("Chat filter word \"{}\" does not exist.", word);
            return false;
        }

        ZynDatabase.PQuery("DELETE FROM chat_filter WHERE word = '{}'", word);

        sObjectMgr->LoadChatFilter();

        handler->PSendSysMessage("Chat filter word \"{}\" deleted.", word);
        return true;
    }

    // .chatfilter reload
    static bool HandleChatFilterReloadCommand(ChatHandler* handler)
    {
        sObjectMgr->LoadChatFilter();
        handler->PSendSysMessage("Chat filter reloaded.");
        return true;
    }

    static bool HandleChatFilterPunishmentsCommand(ChatHandler* handler)
    {
        handler->PSendSysMessage("===== Chat Filter Punishment Flags =====");
        handler->PSendSysMessage("1       - Mute 10 sec");
        handler->PSendSysMessage("2       - Mute 30 sec");
        handler->PSendSysMessage("4       - Mute 1 min");
        handler->PSendSysMessage("8       - Mute 2 min");
        handler->PSendSysMessage("16      - Mute 5 min");
        handler->PSendSysMessage("32      - Mute 10 min");
        handler->PSendSysMessage("64      - Mute 20 min");
        handler->PSendSysMessage("128     - Mute 30 min");
        handler->PSendSysMessage("256     - Mute 1 hour");
        handler->PSendSysMessage("512     - Freeze 5 min");
        handler->PSendSysMessage("1024    - Freeze 10 min");
        handler->PSendSysMessage("2048    - Kick");
        handler->PSendSysMessage("4096    - Ban 1 day");
        handler->PSendSysMessage("8192    - Ban 2 days");
        handler->PSendSysMessage("16384   - Ban 5 days");
        handler->PSendSysMessage("32768   - Ban permanent (~15 years)");
        handler->PSendSysMessage("65536   - Stun 5 min");
        handler->PSendSysMessage("131072  - Stun 10 min");
        handler->PSendSysMessage("Note: values can be combined (e.g. 1+2048 = 2049 for mute + kick).");
        return true;
    }
};

void AddSC_chat_filter_commandscript()
{
    new chat_filter_commandscript();
}
