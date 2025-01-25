#include "Chat.h"
#include "Configuration/Config.h"
#include "CustomConfig.h"
#include "GameTime.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "World.h"

class RebootBuffOnPlayerLogin : public PlayerScript
{
public:
    RebootBuffOnPlayerLogin() : PlayerScript("RebootBuffOnPlayerLogin") {}

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
                if (player->HasAura(buffId)) {
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
        // if (sGameConfig->GetBoolConfig("PvPBuff.Enabled"))
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

class DungeonBuff : public PlayerScript
{
public:
    DungeonBuff() : PlayerScript("DungeonBuff") {}

    std::map<uint32, std::set<uint32>> allowedMaps = {
        {33, {0}},
        {34, {0}},
        {36, {0}},
        {43, {0}},
        {47, {0}},
        {48, {0}},
        {70, {0}},
        {90, {0}},
        {109, {0}},
        {129, {0}},
        {189, {0}},
        {209, {0}},
        {229, {0}},
        {230, {0}},
        {269, {0}},
        {289, {0}},
        {329, {0}},
        {349, {0}},
        {389, {0}},
        {429, {0}},
        {540, {0, 1}},
        {542, {0, 1}},
        {543, {0, 1}},
        {545, {0, 1}},
        {546, {0, 1}},
        {547, {0, 1}},
        {552, {0, 1}},
        {553, {0, 1}},
        {554, {0, 1}},
        {555, {0, 1}},
        {556, {0, 1}},
        {557, {0, 1}},
        {558, {0, 1}},
        {560, {0, 1}},
        {574, {0, 1}},
        {575, {0, 1}},
        {576, {0, 1}},
        {578, {0, 1}},
        {585, {0, 1}},
        {595, {0, 1}},
        {599, {0, 1}},
        {600, {0, 1}},
        {601, {0, 1}},
        {602, {0, 1}},
        {604, {0, 1}},
        {608, {0, 1}},
        {619, {0, 1}},
        {632, {0, 1}},
        {650, {0, 1}},
        {658, {0, 1}},
        {668, {0, 1}},

        {169, {0, 1, 2, 3}},
        {249, {0, 1, 2, 3}},
        {309, {0, 1, 2, 3}},
        {409, {0, 1, 2, 3}},
        {469, {0, 1, 2, 3}},
        {509, {0, 1, 2, 3}},
        {531, {0, 1, 2, 3}},
        {532, {0, 1, 2, 3}},
        {533, {0, 1, 2, 3}},
        {534, {0, 1, 2, 3}},
        {544, {0, 1, 2, 3}},
        {548, {0, 1, 2, 3}},
        {550, {0, 1, 2, 3}},
        {564, {0, 1, 2, 3}},
        {565, {0, 1, 2, 3}},
        {568, {0, 1, 2, 3}},
        {580, {0, 1, 2, 3}},
        {603, {0, 1, 2, 3}},
        {615, {0, 1, 2, 3}},
        {616, {0, 1, 2, 3}},
        {624, {0, 1, 2, 3}},
        {631, {0, 1, 2, 3}},
        {649, {0, 1, 2, 3}},
        {724, {0, 1, 2, 3}}
    };


    const uint32 BUFF_SPELL_ID = 81037;

    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/) override
    {
        if (!player)
            return;

        Map* map = player->GetMap();
        if (!map)
            return;

        uint32 mapId = map->GetId();
        uint32 mapDiff = static_cast<uint32>(map->GetDifficulty());

        auto it = allowedMaps.find(mapId);
        if (it != allowedMaps.end())
        {
            auto& allowedDifficulties = it->second;

            if (allowedDifficulties.find(mapDiff) != allowedDifficulties.end())
            {
                if (!player->HasAura(BUFF_SPELL_ID))
                {
                    player->CastSpell(player, BUFF_SPELL_ID, true);
                }
            }
            else
            {
                if (player->HasAura(BUFF_SPELL_ID))
                {
                    player->RemoveAura(BUFF_SPELL_ID);
                }
            }
        }
        else
        {
            if (player->HasAura(BUFF_SPELL_ID))
            {
                player->RemoveAura(BUFF_SPELL_ID);
            }
        }
    }

};

void AddSC_RebootBuffOnPlayerLogin()
{
    new RebootBuffOnPlayerLogin();
    new PvPBuff();
    new DungeonBuff();
}
