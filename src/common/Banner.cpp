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

#include "Banner.h"
#include "GitRevision.h"
#include "StringFormat.h"

void Trinity::Banner::Show(char const* applicationName, void(*log)(char const* text), void(*logExtraInfo)())
{
<<<<<<< HEAD
    log(Trinity::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str());
    log("<Ctrl-C> to stop.\n");
    log(" ______                       __");
    log("/\\__  _\\       __          __/\\ \\__");
    log("\\/_/\\ \\/ _ __ /\\_\\    ___ /\\_\\ \\, _\\  __  __");
    log("   \\ \\ \\/\\`'__\\/\\ \\ /' _ `\\/\\ \\ \\ \\/ /\\ \\/\\ \\");
    log("    \\ \\ \\ \\ \\/ \\ \\ \\/\\ \\/\\ \\ \\ \\ \\ \\_\\ \\ \\_\\ \\");
    log("     \\ \\_\\ \\_\\  \\ \\_\\ \\_\\ \\_\\ \\_\\ \\__\\\\/`____ \\");
    log("      \\/_/\\/_/   \\/_/\\/_/\\/_/\\/_/\\/__/ `/___/> \\");
    log("                                 C O R E  /\\___/");
    log("http://TrinityCore.org                    \\/__/\n");
    log("https://github.com/TrinityCore/TrinityCore ");
    log("https://github.com/Declipe/TrinityCore \n");
=======
    log(Trinity::StringFormat("{} ({})", GitRevision::GetFullVersion(), applicationName).c_str());
    log(R"(<Ctrl-C> to stop.)" "\n");
    log(R"( ______                       __)");
    log(R"(/\__  _\       __          __/\ \__)");
    log(R"(\/_/\ \/ _ __ /\_\    ___ /\_\ \, _\  __  __)");
    log(R"(   \ \ \/\`'__\/\ \ /' _ `\/\ \ \ \/ /\ \/\ \)");
    log(R"(    \ \ \ \ \/ \ \ \/\ \/\ \ \ \ \ \_\ \ \_\ \)");
    log(R"(     \ \_\ \_\  \ \_\ \_\ \_\ \_\ \__\\/`____ \)");
    log(R"(      \/_/\/_/   \/_/\/_/\/_/\/_/\/__/ `/___/> \)");
    log(R"(                                 C O R E  /\___/)");
    log(R"(http://TrinityCore.org                    \/__/)" "\n");
>>>>>>> 7a01512b29807bff73c685aab813ebd7be0e8fb7

    if (logExtraInfo)
        logExtraInfo();
}
