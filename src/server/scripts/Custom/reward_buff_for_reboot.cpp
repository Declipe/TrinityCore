#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "Configuration/Config.h"
#include "SpellAuraEffects.h"
#include "GameTime.h"
#include "CustomConfig.h"

class RebootBuffOnPlayerLogin : public PlayerScript
{
public:
    RebootBuffOnPlayerLogin() : PlayerScript("RebootBuffOnPlayerLogin") { }

    void OnLogin(Player* player, bool /*loginFirst*/) override
    {        

        if (sGameConfig->GetBoolConfig("RebootbuffEnable"))
        {
            uint32 buffId = 81034;
            uint32 maxBuffDuration = 2160 * 10000;

            uint32 startTime = GameTime::GetStartTime();

            uint32 currentTime = time(nullptr);

            uint32 startTimeMs = startTime * 1000;
            uint32 currentTimeMs = currentTime * 1000;

            uint32 serverUptime = currentTimeMs - startTimeMs;

            if (serverUptime >= maxBuffDuration)
            {
                if(player->HasAura(buffId)) {
                    player->RemoveAura(buffId);
                }

                return;
            }

            uint32 remainingBuffTime = maxBuffDuration - serverUptime;

            if (!player->HasAura(buffId))
            {

                if (Aura* aura = player->AddAura(buffId, player))
                {
                    aura->SetDuration(remainingBuffTime);
                    aura->SetMaxDuration(remainingBuffTime);
                }
            } 
            else 
            {
                player->RemoveAura(buffId);

                if (Aura* aura = player->AddAura(buffId, player))
                {
                    aura->SetDuration(remainingBuffTime);
                    aura->SetMaxDuration(remainingBuffTime);
                }
            }
        }
    }
};

class PvPBuff : public PlayerScript
{

public:
    PvPBuff() : PlayerScript("PvPBuff") {}

    void OnUpdate(Player* player, uint32 /*p_time*/)
    {
        //if (sConfigMgr->GetOption<uint32>("PvPBuff.Enabled", true))
        //{
            if (player->IsPvP() && player->InArena() == false) {
                player->RemoveAura(80866);
                player->AddAura(80865, player);
            }
            else {
                player->RemoveAura(80865);
                player->AddAura(80866, player);
            }
       // }
    }

};

void AddSC_RebootBuffOnPlayerLogin()
{
    new RebootBuffOnPlayerLogin();
    new PvPBuff();
}
