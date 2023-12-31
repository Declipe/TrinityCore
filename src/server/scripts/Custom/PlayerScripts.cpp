#include "Custom/Dcl.h"

#define buffid 48162
#define buffid1 46668
#define buffid2 48074
#define buffid3 48469
#define zoneid 14
#define zoneid1 85
#define zoneid2 4080
#define zoneid3 12

class ZynPlayerScripts: public PlayerScript
{
    public:
        ZynPlayerScripts() : PlayerScript("ZynPlayerScripts") { }

        void OnCreatureKill(Player* killer, Creature* killed)
        {

            CreatureSpecialRewards reward = sObjectMgr->GetSpecialReward(killed->GetEntry());
            if (reward.type >= CSR_TYPE_ITEM && reward.type <= CSR_TYPE_ARENA)
            {
                switch (reward.type)
                {

                case CSR_TYPE_ITEM:
                    killer->AddItem(reward.param1, reward.param2);
                    break;

                case CSR_TYPE_HONOR:
                    killer->SetHonorPoints(killer->GetHonorPoints() + reward.param1);
                    break;

                case CSR_TYPE_ARENA:
                    killer->SetArenaPoints(killer->GetArenaPoints() + reward.param1);
                    break;
                }
            }
        }
};

uint32 MaxItemLevel = 255;

class item_lvlup : public ItemScript
{
public: item_lvlup() : ItemScript("item_lvlup") {}

        bool OnUse(Player* player, Item* item, SpellCastTargets const& /*Targets*/)
        {
            WorldSession* session = player->GetSession();
            if (player->GetLevel() != MaxItemLevel)
            {
                if (player->IsInFlight() || player->IsInCombat() || player->isDead())
                {
                    player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, NULL);
                    return true;
                }
                else
                {
                    int32 level = player->GetLevel();
                    player->GiveLevel(player->GetSession()->GetPlayer()->GetLevel() + 1);
                    player->DestroyItemCount(item->GetEntry(), 1, true);
                   // ChatHandler(session).PSendSysMessage(LANG_SAY_LVL_UP_ITEM);
                    return true;
                }
                return true;
            }
            else
            {
               // ChatHandler(session).PSendSysMessage(LANG_ERROR_LVL_UP_ITEM);
                return true;
            }
            return true;
        }

};

class lfg_solo_announce : public PlayerScript
{
public:
    lfg_solo_announce() : PlayerScript("lfg_solo_announce") {}

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        // Announce Module
        if (sConfigMgr->GetBoolDefault("SoloLFG.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running |cff4CFF00Solo Dungeon Finder|r.");
        }
    }
};

class lfg_solo : public PlayerScript
{
public:
    lfg_solo() : PlayerScript("lfg_solo") { }

    void OnLogin(Player* /*player*/, bool /*firstLogin*/) override
    {
        if (sConfigMgr->GetIntDefault("SoloLFG.Enable", true))
        {
            if (!sLFGMgr->IsSoloLFG())
            {
                sLFGMgr->ToggleSoloLFG();
            }
        }
    }
};

class buff_zone : public PlayerScript
{
public:
    buff_zone() : PlayerScript("buff_zone") {}

    void OnUpdateZone(Player* player, uint32 newZone, uint32 /*newArea*/)
    {
        if (newZone == zoneid) {
            player->AddAura(buffid, player);
        }
        else {
            player->RemoveAurasDueToSpell(buffid);
        }
        if (newZone == zoneid1) {
            player->AddAura(buffid1, player);
        }
        else {
            player->RemoveAurasDueToSpell(buffid1);
        }
        if (newZone == zoneid2) {
            player->AddAura(buffid2, player);
        }
        else {
            player->RemoveAurasDueToSpell(buffid2);
        }
        if (newZone == zoneid3) {
            player->AddAura(buffid3, player);
        }
        else {
            player->RemoveAurasDueToSpell(buffid3);
        }
    }
};

void AddSC_ZynPlayerScripts()
{
    new buff_zone();
    new item_lvlup();
    new lfg_solo();
    new lfg_solo_announce();
    new ZynPlayerScripts();
}
