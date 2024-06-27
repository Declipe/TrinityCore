#include "Chat.h"
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
#include "GossipDef.h"
#include "ScriptedGossip.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "DBCStores.h"
#include "WorldSession.h"

#define GTS2 session->GetTrinityString2

class level_award : public PlayerScript
{
public:
level_award() : PlayerScript("level_award") {}

void OnLevelChanged(Player* player, uint8 newLevel)
{
    switch (++newLevel)
    {
    case 10:
       // player->CastSpell(player, SPELL, true);
        player->ModifyMoney(100000, true);
        player->AddItem(49426, 10);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 20:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 30:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 40:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 50:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 60:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 70:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    case 80:
        //player->CastSpell(player, SPELL, true);
        player->AddItem(49426, 10);
        player->ModifyMoney(100000, true);
        //CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(TITLE_ID);
        //player->SetTitle(titleInfo);
        ChatHandler(player->GetSession()).PSendSysMessage("LANG_VIP_GROUP1");
        break;
    }
}
};

enum ShallowG
{
    NPC_ZOMBIER = 90034,
    NPC_DEAD_HEROES = 90037,
    CHANCE_ZOMBIES = 65,
    CHANCE_DEAD_HEROES = 35
};

class go_grave : public GameObjectScript
{
public:
    go_grave() : GameObjectScript("go_grave") { }

    struct go_graveAI : public GameObjectAI
    {
        go_graveAI(GameObject* go) : GameObjectAI(go) { }

        bool OnGossipHello(Player* player) override
        {
            WorldSession* session = player->GetSession();

            if (me->HasFlag(GO_FLAG_IN_USE))
                return true;

            bool hasKey = true;
            if (LockEntry const* lock = sLockStore.LookupEntry(me->GetGOInfo()->goober.lockId))
            {
                hasKey = false;
                for (uint8 i = 0; i < MAX_LOCK_CASE; ++i)
                {
                    if (!lock->Index[i])
                        continue;

                    if (player->HasItemCount(8886,1))
                    {
                        hasKey = true;
                        ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_15));
                        break;
                    }
                    else
                        player->PlayerTalkClass->SendCloseGossip();
                    player->GetSession()->SendAreaTriggerMessage(GTS2(NOT_USED_16));
                    ChatHandler(player->GetSession()).SendSysMessage(GTS2(NOT_USED_16));

                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
            }

            if (!hasKey)
                return false;

            if (me->GetUseCount() == 0)
            {
                uint32 randomchance = urand(0, 100);
                if (randomchance < CHANCE_ZOMBIES)
                    me->SummonCreature(NPC_ZOMBIER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30s);
                else
                    if ((randomchance - CHANCE_ZOMBIES) < CHANCE_DEAD_HEROES)
                        me->SummonCreature(NPC_DEAD_HEROES, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30s);
            }
            me->AddUse();
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_graveAI(go);
    }
};

void AddSC_level_award()
{
new go_grave();
new level_award();
}
