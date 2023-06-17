#include "Config.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "ScriptMgr.h"
#include "Language.h"
#include "ZynDatabase.h"
#include "DatabaseEnvFwd.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "GameTime.h"
#include "SpellMgr.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "Group.h"

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
            uint32 GUILD_ID_ALLIANCE = sConfigMgr->GetIntDefault("GuildDefault.IDA", 0);
            uint32 GUILD_ID_HORDE = sConfigMgr->GetIntDefault("GuildDefault.IDH", 0);
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
