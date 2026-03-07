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
#include "BattlefieldMgr.h"
#include "Chat.h"
#include "RBAC.h"
#include "Language.h"

using namespace Trinity::ChatCommands;

class bf_commandscript : public CommandScript
{
public:
    bf_commandscript() : CommandScript("bf_commandscript") { }

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

    static bool HandleBattlefieldStart(ChatHandler* handler, uint32 battleId)
    {
        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleId);

        if (!bf)
        {
            handler->SendErrorMessage(LANG_BF_NOT_FOUND, battleId);
            return false;
        }

        bf->StartBattle();

        sWorld->SendWorldText(LANG_BF_STARTED, battleId);
        if (handler->IsConsole())
            handler->PSendSysMessage(LANG_BF_STARTED, battleId);

        return true;
    }

    static bool HandleBattlefieldEnd(ChatHandler* handler, uint32 battleId)
    {
        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleId);

        if (!bf)
        {
            handler->SendErrorMessage(LANG_BF_NOT_FOUND, battleId);
            return false;
        }

        bf->EndBattle(true);

        sWorld->SendWorldText(LANG_BF_STOPPED, battleId);
        if (handler->IsConsole())
            handler->PSendSysMessage(LANG_BF_STOPPED, battleId);

        return true;
    }

    static bool HandleBattlefieldEnable(ChatHandler* handler, uint32 battleId)
    {
        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleId);

        if (!bf)
        {
            handler->SendErrorMessage(LANG_BF_NOT_FOUND, battleId);
            return false;
        }

        if (bf->IsEnabled())
        {
            bf->ToggleBattlefield(false);

            sWorld->SendWorldText(LANG_BF_DISABLED, battleId);
            if (handler->IsConsole())
                handler->PSendSysMessage(LANG_BF_DISABLED, battleId);
        }
        else
        {
            bf->ToggleBattlefield(true);
  
            sWorld->SendWorldText(LANG_BF_ENABLED, battleId);
            if (handler->IsConsole())
                handler->PSendSysMessage(LANG_BF_ENABLED, battleId);
        }

        return true;
    }

    static bool HandleBattlefieldSwitch(ChatHandler* handler, uint32 battleId)
    {
        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleId);

        if (!bf)
        {
            handler->SendErrorMessage(LANG_BF_NOT_FOUND, battleId);
            return false;
        }

        //bf->EndBattle(false);
        bf->switchSide();

        sWorld->SendWorldText(LANG_BF_SWITCHED, battleId);
        if (handler->IsConsole())
            handler->PSendSysMessage(LANG_BF_SWITCHED, battleId);

        return true;
    }

    static bool HandleBattlefieldTimer(ChatHandler* handler, uint32 battleId, uint32 time)
    {
        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleId);

        if (!bf)
        {
            handler->SendErrorMessage(LANG_BF_NOT_FOUND, battleId);
            return false;
        }

        bf->SetTimer(time * IN_MILLISECONDS);
        bf->SendInitWorldStatesToAll();

        sWorld->SendWorldText(LANG_BF_TIMER_SET, battleId, time);
        if (handler->IsConsole())
            handler->PSendSysMessage(LANG_BF_TIMER_SET, battleId, time);

        return true;
    }
};

void AddSC_bf_commandscript()
{
    new bf_commandscript();
}
