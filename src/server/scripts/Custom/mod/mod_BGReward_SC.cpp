/*
 * This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
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

#include "Log.h"
#include "Battleground.h"
#include "Chat.h"
#include "Config.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"

class BGReward_Player : public BGScript
{
public:
    BGReward_Player() : BGScript("BGReward_Player") { }

    void OnBattlegroundEnd(Battleground* bg, uint32 winner) override
    {
        if (!sConfigMgr->GetBoolDefault("BGReward.Enable", false))
            return;

        // Not reward on end arena
        if (bg->isArena())
            return;

        for (auto const& [guid, bgPlayer] : bg->GetPlayers())
        {
            Player* player = ObjectAccessor::FindPlayer(guid);
            if (!player)
                continue;

            auto bgTeam = player->GetTeam();
            uint32 rewardCount = 0;

           // bgTeam == winner ? rewardCount = sConfigMgr->GetIntDefault("BGReward.WinnerTeam.Count", rewardCount) : rewardCount = sConfigMgr->GetIntDefault("BGReward.LoserTeam.Count", rewardCount);
            bgTeam == winner ? rewardCount = sConfigMgr->GetIntDefault("BGReward.WinnerTeam.Count", 3) : rewardCount = sConfigMgr->GetIntDefault("BGReward.LoserTeam.Count", 1);

            switch (player->GetZoneId())
            {
                case 3277: // Warsong Gulch
                    //sConfigMgr->GetIntDefault("Battleground.InvitationType", 0);
                    player->AddItem(sConfigMgr->GetIntDefault("BGReward.ItemID.WSG", ITEM_WS_MARK_OF_HONOR), rewardCount);
                    break;
                case 3358: // Arathi Basin
                    player->AddItem(sConfigMgr->GetIntDefault("BGReward.ItemID.Arathi", ITEM_AB_MARK_OF_HONOR), rewardCount);
                    break;
                case 3820: // Eye of the Storm
                    player->AddItem(sConfigMgr->GetIntDefault("BGReward.ItemID.Eye", ITEM_AV_MARK_OF_HONOR), rewardCount);
                    break;
                case 4710: // Isle of Conquest
                    player->AddItem(sConfigMgr->GetIntDefault("BGReward.ItemID.Isle", ITEM_IC_MARK_OF_HONOR), rewardCount);
                    break;
                case 4384: // Strand of the Ancients
                    player->AddItem(sConfigMgr->GetIntDefault("BGReward.ItemID.Ancients", ITEM_SA_MARK_OF_HONOR), rewardCount);
                    break;
                case 2597: // Alterac Valley
                    player->AddItem(sConfigMgr->GetIntDefault("BGReward.ItemID.Alterac", ITEM_EY_MARK_OF_HONOR), rewardCount);
                    break;
                default:
                    break;
            }
        }
    }
};

class BGReward_World : public WorldScript
{
public:
    BGReward_World() : WorldScript("BGReward_World") { }

    void OnConfigLoad(bool /*reload*/) override
    {
        sConfigMgr->GetBoolDefault("BGReward.Enable",false);
        sConfigMgr->GetIntDefault("BGReward.ItemID.WSG", ITEM_WS_MARK_OF_HONOR);
        sConfigMgr->GetIntDefault("BGReward.ItemID.Arathi", ITEM_AB_MARK_OF_HONOR);
        sConfigMgr->GetIntDefault("BGReward.ItemID.Alterac", ITEM_AV_MARK_OF_HONOR);
        sConfigMgr->GetIntDefault("BGReward.ItemID.Isle", ITEM_IC_MARK_OF_HONOR);
        sConfigMgr->GetIntDefault("BGReward.ItemID.Ancients", ITEM_SA_MARK_OF_HONOR);
        sConfigMgr->GetIntDefault("BGReward.ItemID.Eye", ITEM_EY_MARK_OF_HONOR);
        sConfigMgr->GetIntDefault("BGReward.WinnerTeam.Count", 3);
        sConfigMgr->GetIntDefault("BGReward.LoserTeam.Count", 1);
    }
};

// Group all custom scripts
void AddSC_BGReward()
{
    new BGReward_Player();
    new BGReward_World();
}
