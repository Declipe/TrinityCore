/*
<--------------------------------------------------------------------------->
- Developer(s): Lille Carl, Grim/Render
- Complete: %100
- ScriptName: 'AccountAchievements'
- Comment: Tested and Works.
- Orginial Creator: Lille Carl
- Edited: Render/Grim
<--------------------------------------------------------------------------->
*/

#include "Chat.h"
#include "CustomConfig.h"
#include "Player.h"
#include "ScriptMgr.h"

class AccountAchievements : public PlayerScript
{
    static const bool limitrace = true; // This set to true will only achievements from chars on the same team, do what you want. NOT RECOMMANDED TO BE CHANGED!!!
    static const bool limitlevel = false; // This checks the player's level and will only add achievements to players of that level.
    int minlevel = 80; // It's set to players of the level 60. Requires limitlevel to be set to true.
    int setlevel = 1; // Dont Change

public:
    AccountAchievements() : PlayerScript("AccountAchievements") { }

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
        if (sGameConfig->GetBoolConfig("Account.Achievements.Enable"))
        {

            std::vector<uint32> Guids;
            QueryResult result1 = CharacterDatabase.PQuery("SELECT guid, race FROM characters WHERE account = {}", player->GetSession()->GetAccountId());
            if (!result1)
                return;

            do
            {
                Field* fields = result1->Fetch();

                uint32 race = fields[1].GetUInt8();

                if ((Player::TeamForRace(race) == Player::TeamForRace(player->GetRace())) || !limitrace)
                    Guids.push_back(result1->Fetch()[0].GetUInt32());

            } while (result1->NextRow());

            std::vector<uint32> Achievement;

            for (auto& i : Guids)
            {
                QueryResult result2 = CharacterDatabase.PQuery("SELECT achievement FROM character_achievement WHERE guid = {}", i);
                if (!result2)
                    continue;

                do
                {
                    Achievement.push_back(result2->Fetch()[0].GetUInt32());
                } while (result2->NextRow());
            }

            for (auto& i : Achievement)
            {
                auto sAchievement = sAchievementStore.LookupEntry(i);
                AddAchievements(player, sAchievement->ID);
            }
        }
    }

    void AddAchievements(Player* player, uint32 AchievementID)
    {
        if (sGameConfig->GetBoolConfig("Account.Achievements.Enable"))
        {
            if (limitlevel)
                setlevel = minlevel;

            if (player->GetLevel() >= setlevel)
                player->CompletedAchievement(sAchievementStore.LookupEntry(AchievementID));
        }
    }
};

void AddAccountAchievementsScripts()
{
    new AccountAchievements;
}
