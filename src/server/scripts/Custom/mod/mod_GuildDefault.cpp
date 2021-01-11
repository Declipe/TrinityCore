#include "ScriptMgr.h"
#include "Config.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GossipDef.h"
#include "WorldPacket.h"
#include "World.h"
#include "Chat.h"
#include "Mail.h"
#include "WorldSession.h"
#include "ItemTemplate.h"
#include "Item.h"
#include "ObjectMgr.h"
#include <stdarg.h>

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
