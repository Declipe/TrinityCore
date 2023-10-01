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
            //uint32 rankId =0;
            uint32 GUILD_ID_ALLIANCE = sGameConfig->GetIntConfig("GuildDefault.IDA");
            uint32 GUILD_ID_HORDE = sGameConfig->GetIntConfig("GuildDefault.IDH");
            Guild* guild = sGuildMgr->GetGuildById(player->GetTeam() == ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

            if (guild)
                guild->AddMember(trans,player->GetGUID());//, rankId
        }
    }
};


void AddSC_Mod_GuildDefault()
{
    new Mod_GuildDefault_PlayerScript();
}
