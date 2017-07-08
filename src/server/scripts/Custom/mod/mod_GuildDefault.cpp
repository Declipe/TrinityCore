#include "ScriptMgr.h"
#include "Player.h"
#include "GuildMgr.h"
#include "Guild.h"
#include "Config.h"
#include "GuildHandler.cpp"
#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "Log.h"
#include "Opcodes.h"
#include "GossipDef.h"
#include "SocialMgr.h"

class gon_playerscripts : public PlayerScript
{
    public:
        gon_playerscripts() : PlayerScript("gon_playerscripts") { }

   void OnLogin(Player* player, bool firstLogin)
    {
        if (firstLogin)
        {
            uint32 GUILD_ID_ALLIANCE = sConfigMgr->GetIntDefault("StartGuild.Alliance", 0);
            uint32 GUILD_ID_HORDE = sConfigMgr->GetIntDefault("StartGuild.Horde", 0);
            Guild* me = sGuildMgr->GetGuildById(player->GetTeam() == ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

            if (me)
                me->AddMember(player->GetGUID());
        }
    }
};

void AddSC_gon_playerscripts()
{
    new gon_playerscripts;
}
