#include "Custom/Dcl.h"

using namespace std;

#define DEFAULT_MESSAGE 907

struct TitlesData
{
    uint32 Menu;
    uint32 Submenu;
    GossipOptionIcon Icon;
    uint32 Id;
    string Name;
};

TitlesData DKDatas[] =
{
    { 1, 0, GOSSIP_ICON_BATTLE, 78, "The Explorer" },
    { 1, 0, GOSSIP_ICON_BATTLE, 84, "Chef" },
    { 1, 0, GOSSIP_ICON_BATTLE, 83, "Salty" },
    { 1, 0, GOSSIP_ICON_BATTLE, 125, "Loremaster" },
    { 1, 0, GOSSIP_ICON_BATTLE, 81, "The Seeker" },
    { 1, 0, GOSSIP_ICON_BATTLE, 172, "the Patient" },
    { 1, 0, GOSSIP_ICON_BATTLE, 74, "Elder" },
    { 1, 0, GOSSIP_ICON_BATTLE, 135, "The Love Fool" },
    { 1, 0, GOSSIP_ICON_BATTLE, 155, "The Noble" },
    { 1, 0, GOSSIP_ICON_BATTLE, 133, "Brewmaster" },
    { 1, 0, GOSSIP_ICON_BATTLE, 124, "The Hallowed" },
    { 1, 0, GOSSIP_ICON_BATTLE, 168, "Pilgrim" },
    { 1, 0, GOSSIP_ICON_BATTLE, 134, "Merrymaker" },
    { 1, 0, GOSSIP_ICON_BATTLE, 143, "Jenkins" },
    { 1, 2, GOSSIP_ICON_TALK, 0, "Next.." },

    { 2, 0, GOSSIP_ICON_BATTLE, 46, "Scarab Lord" },
    { 2, 0, GOSSIP_ICON_BATTLE, 129, "Champion of the Frozen Wastes" },
    { 2, 0, GOSSIP_ICON_BATTLE, 142, "The Undying" },
    { 2, 0, GOSSIP_ICON_BATTLE, 141, "The Immortal" },
    { 2, 0, GOSSIP_ICON_BATTLE, 140, "of the Nightfall" },
    { 2, 0, GOSSIP_ICON_BATTLE, 121, "Twilight Vanquisher" },
    { 2, 0, GOSSIP_ICON_BATTLE, 164, "Starcaller" },
    { 2, 0, GOSSIP_ICON_BATTLE, 165, "The Astral Walker" },
    { 2, 0, GOSSIP_ICON_BATTLE, 166, "Herald of the Titans" },
    { 2, 0, GOSSIP_ICON_BATTLE, 161, "Champion of Ulduar" },
    { 2, 0, GOSSIP_ICON_BATTLE, 160, "Conqueror of Ulduar" },
    { 2, 0, GOSSIP_ICON_BATTLE, 174, "Bane of the Fallen King" },
    { 2, 0, GOSSIP_ICON_BATTLE, 173, "The Light of Dawn" },
    { 2, 3, GOSSIP_ICON_TALK, 0, "Next.." },
    { 2, 1, GOSSIP_ICON_TALK, 0, "..Back" },

    { 3, 0, GOSSIP_ICON_BATTLE, 175, "The Kingslayer" },
    { 3, 0, GOSSIP_ICON_BATTLE, 63, "of the Shattered Sun" },
    { 3, 0, GOSSIP_ICON_BATTLE, 130, "Ambassador" },
    { 3, 0, GOSSIP_ICON_BATTLE, 144, "Bloodsail Admiral" },
    { 3, 0, GOSSIP_ICON_BATTLE, 79, "The Diplomat" },
    { 3, 0, GOSSIP_ICON_BATTLE, 132, "Guardian of Cenarius" },
    { 3, 0, GOSSIP_ICON_BATTLE, 131, "The Argent Champion" },
    { 3, 0, GOSSIP_ICON_BATTLE, 176, "of the Ashen Verdict" },
    { 3, 0, GOSSIP_ICON_BATTLE, 77, "The Exalted" },
    { 3, 0, GOSSIP_ICON_BATTLE, 145, "The Insane" },
    { 3, 0, GOSSIP_ICON_BATTLE, 72, "Battlemaster" },
    { 3, 0, GOSSIP_ICON_BATTLE, 82, "Arena Master" },
    { 3, 0, GOSSIP_ICON_BATTLE, 43, "Duelist" },
    { 3, 4, GOSSIP_ICON_TALK, 0, "Next.." },
    { 3, 2, GOSSIP_ICON_TALK, 0, "..Back" },

    { 4, 0, GOSSIP_ICON_BATTLE, 44, "Rival" },
    { 4, 0, GOSSIP_ICON_BATTLE, 45, "Challenger" },
    { 4, 0, GOSSIP_ICON_BATTLE, 163, "Vanquisher" },
    { 4, 0, GOSSIP_ICON_BATTLE, 156, "Crusader" },
    { 4, 0, GOSSIP_ICON_BATTLE, 93, "Archmage" },
    { 4, 0, GOSSIP_ICON_BATTLE, 53, "Champion of the Naaru" },
    { 4, 0, GOSSIP_ICON_BATTLE, 64, "Hand of A'dal" },
    { 4, 0, GOSSIP_ICON_BATTLE, 128, "Flawless Victor" },
    { 4, 5, GOSSIP_ICON_TALK, 0, "Next.." },
    { 4, 3, GOSSIP_ICON_TALK, 0, "..Back" },

    { 5, 0, GOSSIP_ICON_BATTLE, 178, "beta titles1" },
    { 5, 0, GOSSIP_ICON_BATTLE, 179, "beta titles2" },
    { 5, 0, GOSSIP_ICON_BATTLE, 180, "beta titles3" },
    { 5, 0, GOSSIP_ICON_BATTLE, 181, "beta titles4" },
    { 5, 0, GOSSIP_ICON_BATTLE, 182, "beta titles5" },
    { 5, 0, GOSSIP_ICON_BATTLE, 183, "beta titles6" },
    { 5, 0, GOSSIP_ICON_BATTLE, 184, "beta titles7" },
    { 5, 0, GOSSIP_ICON_BATTLE, 185, "beta titles8" },
    { 5, 0, GOSSIP_ICON_BATTLE, 186, "beta titles9" },
    { 5, 4, GOSSIP_ICON_TALK, 0, "..Back" }
};

class npc_title : public CreatureScript
{
public:
    npc_title() : CreatureScript("npc_title") { }

    struct npc_titleAI : public ScriptedAI
    {

        npc_titleAI(Creature* me) : ScriptedAI(me) { }

        void GetMenu(Player* player, Creature* /*creature*/, uint32 menuId)
        {
            for (uint8 i = 0; i < (sizeof(DKDatas) / sizeof(*DKDatas)); i++)
            {
                if (DKDatas[i].Menu == menuId)
                    AddGossipItemFor(player, DKDatas[i].Icon, DKDatas[i].Name, GOSSIP_SENDER_MAIN, i);
            }

            SendGossipMenuFor(player, DEFAULT_MESSAGE, me->GetGUID());
        }

        bool OnGossipHello(Player* player) override
        {
            GetMenu(player, me, 1);
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            return GossipSelect(player, sender, action);
        }

        bool GossipSelect(Player* player, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            uint32 token = sConfigMgr->GetIntDefault("NT.Token", 0);
            uint32 count = sConfigMgr->GetIntDefault("NT.Count", 0);

            uint8 menuData = DKDatas[action].Submenu;

            if (menuData == 0)
            {
                if ((int32)!player->GetItemCount(token) < (int32)count)
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("Failure! You not have the token.");
                    CloseGossipMenuFor(player);
                    return false;
                }

                player->SetTitle(sDBCMgr->GetCharTitlesEntry(DKDatas[action].Id));
                player->DestroyItemCount(token, count, true);
                ChatHandler(player->GetSession()).PSendSysMessage("Success! Title added.");
                menuData = DKDatas[action].Menu;
            }
            GetMenu(player, me, menuData);
            return true;
        }
    };

    CreatureAI* GetAI(Creature* me) const override
    {
        return new npc_titleAI(me);
    }
};

struct npc_ten : public ScriptedAI
{
    npc_ten(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _events.ScheduleEvent(1, 6s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
            {
                //FindCreatureOptions& options = FindCreatureOptions().SetStringId("1");
                FindCreatureOptions& options = FindCreatureOptions().SetCreatureId(500146);
                //FindCreatureOptions& options2 = FindCreatureOptions().SetCreatureId(90071);

                if (Creature* creature = me->FindNearestCreatureWithOptions(100.f, options))
                    if (creature)
                    creature->Say("Hello i love spamm", LANG_UNIVERSAL);
                _events.ScheduleEvent(1, 10s);
                break;
            }
            default:
                break;
            }
        }
    }

private:
    EventMap _events;
};

void AddSC_npc_title()
{
    RegisterCreatureAI(npc_ten);
    new npc_title;
}
