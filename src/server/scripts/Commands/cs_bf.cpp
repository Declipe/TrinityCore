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

/* ScriptData
Name: bf_commandscript
%Complete: 100
Comment: All bf related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Chat.h"
#include "Language.h"
#include "RBAC.h"

using namespace Trinity::ChatCommands;

class bf_commandscript : public CommandScript
{
public:
    bf_commandscript() : CommandScript("bf_commandscript") { }

    static std::string GetBattleIdString(BattlefieldId battleId)
    {
        switch (battleId)
        {
        case BATTLEFIELD_BATTLEID_WINTERGRASP:
            return "Wintergrasp";
        default:
            return "";
        }
    }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable battlefieldcommandTable =
        {
            { "start",      HandleBattlefieldStart,  rbac::RBAC_PERM_COMMAND_BF_START,  Console::No },
            { "stop",       HandleBattlefieldEnd,    rbac::RBAC_PERM_COMMAND_BF_STOP,   Console::No },
            { "switch",     HandleBattlefieldSwitch, rbac::RBAC_PERM_COMMAND_BF_SWITCH, Console::No },
            { "timer",      HandleBattlefieldTimer,  rbac::RBAC_PERM_COMMAND_BF_TIMER,  Console::No },
            { "enable",     HandleBattlefieldEnable, rbac::RBAC_PERM_COMMAND_BF_ENABLE, Console::No },
        };
        static ChatCommandTable commandTable =
        {
            { "bf", battlefieldcommandTable },
        };
        return commandTable;
    }
    static bool HandleBattlefieldStart(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* battleidstr = strtok((char*)args, " ");
        if (!battleidstr)
            return false;

        uint32 battlearg = atoul(battleidstr);
        if (battlearg == 0 || battlearg >= BATTLEFIELD_BATTLEID_MAX)
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_INCORRECT_ID);
            return true;
        }

        BattlefieldId battleId = BattlefieldId(battlearg);
        Battlefield* battlefield = sBattlefieldMgr->GetBattlefield(battleId);
        if (!battlefield)
            return true;

        if (!battlefield->IsEnabled())
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_DISABLED, GetBattleIdString(battleId));
            return true;
        }

        if (battlefield->IsWarTime())
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_ACTIVE, GetBattleIdString(battleId));
            return true;
        }

        battlefield->StartBattle();
        handler->PSendSysMessage(LANG_COMMAND_BF_START, GetBattleIdString(battleId));
        return true;
    }

    static bool HandleBattlefieldEnd(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* battleidstr = strtok((char*)args, " ");
        if (!battleidstr)
            return false;

        uint32 battlearg = atoul(battleidstr);
        if (battlearg == 0 || battlearg >= BATTLEFIELD_BATTLEID_MAX)
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_INCORRECT_ID);
            return true;
        }

        BattlefieldId battleId = BattlefieldId(battlearg);
        Battlefield* battlefield = sBattlefieldMgr->GetBattlefield(battleId);
        if (!battlefield)
            return true;

        if (!battlefield->IsEnabled())
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_DISABLED, GetBattleIdString(battleId));
            return true;
        }

        if (!battlefield->IsWarTime())
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_ALREADY_STOPPED, GetBattleIdString(battleId));
            return true;
        }

        battlefield->EndBattle(true);
        handler->PSendSysMessage(LANG_COMMAND_BF_STOP, GetBattleIdString(battleId));
        return true;
    }

    static bool HandleBattlefieldEnable(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* battleidstr = strtok((char*)args, " ");
        if (!battleidstr)
            return false;

        uint32 battlearg = atoul(battleidstr);
        if (battlearg == 0 || battlearg >= BATTLEFIELD_BATTLEID_MAX)
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_INCORRECT_ID);
            return true;
        }

        BattlefieldId battleId = BattlefieldId(battlearg);
        Battlefield* battlefield = sBattlefieldMgr->GetBattlefield(battleId);
        if (!battlefield)
            return true;

        if (battlefield->IsEnabled())
        {
            battlefield->ToggleBattlefield(false);
            handler->PSendSysMessage(LANG_COMMAND_BF_DISABLE, GetBattleIdString(battleId));
        }
        else
        {
            battlefield->ToggleBattlefield(true);
            handler->PSendSysMessage(LANG_COMMAND_BF_ENABLE, GetBattleIdString(battleId));
        }
        return true;
    }

    static bool HandleBattlefieldSwitch(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* battleidstr = strtok((char*)args, " ");
        if (!battleidstr)
            return false;

        uint32 battlearg = atoul(battleidstr);
        if (battlearg == 0 || battlearg >= BATTLEFIELD_BATTLEID_MAX)
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_INCORRECT_ID);
            return true;
        }

        BattlefieldId battleId = BattlefieldId(battlearg);
        Battlefield* battlefield = sBattlefieldMgr->GetBattlefield(battleId);
        if (!battlefield)
            return true;

        if (battlefield->IsWarTime())
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_ACTIVE, GetBattleIdString(battleId));
            return true;
        }

        battlefield->EndBattle(false);
        handler->PSendSysMessage(LANG_COMMAND_BF_SWITCH, GetBattleIdString(battleId));
        return true;
    }

    static bool HandleBattlefieldTimer(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* battleidstr = strtok((char*)args, " ");
        if (!battleidstr)
            return false;

        char* timestr = strtok(nullptr, " ");
        if (!timestr)
            return false;

        uint32 battlearg = atoul(battleidstr);
        if (battlearg == 0 || battlearg >= BATTLEFIELD_BATTLEID_MAX)
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_INCORRECT_ID);
            return true;
        }

        BattlefieldId battleId = BattlefieldId(battlearg);
        Battlefield* battlefield = sBattlefieldMgr->GetBattlefield(battleId);
        if (!battlefield)
            return true;

        if (!battlefield->IsEnabled())
        {
            handler->PSendSysMessage(LANG_COMMAND_BF_DISABLED, GetBattleIdString(battleId));
            return true;
        }

        uint32 time = atoul(timestr);
        battlefield->SetTimer(time * IN_MILLISECONDS);
        battlefield->SendInitWorldStatesToAll();
        handler->PSendSysMessage(LANG_COMMAND_BF_TIMER, GetBattleIdString(battleId), battlefield->GetTimer(), battlefield->IsWarTime() ? "war time" : "no war time");
        return true;
    }
};

void AddSC_bf_commandscript()
{
    new bf_commandscript();
}
