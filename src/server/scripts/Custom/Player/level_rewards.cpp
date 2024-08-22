#include "Chat.h"
#include "Config.h"
#include "CustomConfig.h"
#include "DatabaseEnv.h"
#include "DatabaseEnvFwd.h"
#include "DBCStores.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GameTime.h"
#include "GossipDef.h"
#include "Language.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "ZynDatabase.h"

#define GTS2 session->GetTrinityString2

class level_award : public PlayerScript
{
public:
    level_award() : PlayerScript("level_award") {}

    void OnLevelChanged(Player* player, uint8 oldlevel)
    {
        switch (++oldlevel)
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

                    uint32 pt3 = sGameConfig->GetIntConfig("cost_id2");
                    uint32 pt4 = sGameConfig->GetIntConfig("cost_colvo2");

                    if (player->HasItemCount(pt3, pt4))
                    {
                        hasKey = true;
                        player->DestroyItemCount(pt3, pt4, true);
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

uint32 questId = 70001;
uint32 MaxLevels = 80;

class QuestOnLevelUp : public PlayerScript
{
public:
    QuestOnLevelUp() : PlayerScript("QuestOnLevelUp") { }

    void OnLevelChanged(Player* player, uint8 oldLevel)
    {
        if (player->GetLevel() == MaxLevels && oldLevel < MaxLevels)
        {
            WorldSession* session = player->GetSession();

            const Quest* quest = sObjectMgr->GetQuestTemplate(questId);
            if (!quest)
                return;

            if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE)
            {
                player->AddQuest(quest, player);
                player->SendQuestUpdate(questId);
                player->GetSession()->SendNotification(GTS2(NOT_USED_40));
            }
        }
    }
};

void AddSC_level_award()
{
    new go_grave();
    new level_award();
    new QuestOnLevelUp();
}
