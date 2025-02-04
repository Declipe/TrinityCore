#include "Custom/Dcl.h"

class Mod_GuildDefault_PlayerScript : public PlayerScript
{
    public:
        Mod_GuildDefault_PlayerScript() : PlayerScript("gon_playerscripts") { }

        void OnLogin(Player* player, bool firstLogin)
        {
            if (firstLogin)
            {
                CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

                uint32 GUILD_ID_ALLIANCE = sGameConfig->GetIntConfig("GuildDefault.IDA");
                uint32 GUILD_ID_HORDE = sGameConfig->GetIntConfig("GuildDefault.IDH");
                uint32 GUILD_ID_ALLIANCE_GM = sGameConfig->GetIntConfig("GuildDefault.IDA_GM");
                uint32 GUILD_ID_HORDE_GM = sGameConfig->GetIntConfig("GuildDefault.IDH_GM");

                uint32 guildId = 0;

                if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)
                {
                    //std::cout << "Player is GameMaster" << std::endl;
                    guildId = player->GetTeam() == ALLIANCE ? GUILD_ID_ALLIANCE_GM : GUILD_ID_HORDE_GM;
                }
                else
                {
                    //std::cout << "Player is not GameMaster" << std::endl;
                    guildId = player->GetTeam() == ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE;
                }

                Guild* guild = sGuildMgr->GetGuildById(guildId);

                if (guild)
                    guild->AddMember(trans, player->GetGUID());
            }
        }
};


void AddSC_Mod_GuildDefault()
{
    new Mod_GuildDefault_PlayerScript();
}
