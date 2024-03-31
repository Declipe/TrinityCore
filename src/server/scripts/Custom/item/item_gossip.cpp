#include "Custom/Dcl.h"

class ItemUse_item_custom : public ItemScript
{
public:

    ItemUse_item_custom() : ItemScript("item_custom") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)

    {
        if ((player->IsInCombat()) || (player->IsInFlight()) || (player->isDead()))
        {
            player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, nullptr);
            return false;
        }
        if (player->IsMounted())
        {
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, nullptr);
            return true;
        }
        {
           // player->SummonCreature(6, player->GetPositionX() + 1, player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12s);
            player->SummonCreature(6, player->GetPositionX() + 10, player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000s);
        }
        return false;
    }
};

class rip : public ItemScript
{
public:
    rip() : ItemScript("rip") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        WorldSession* session = player->GetSession();

        if (player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->isDead())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(session->GetTrinityString2(NOT_USED_9));
            return false;
        }

        // player->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(609), 25);
        if (player->GetReputationRank(70) < REP_NEUTRAL)
            player->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(70), 10000);
        else
            player->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(70), 250);
        player->DestroyItemCount(item->GetEntry(), 1, true);

        return true;
    }
};

enum TPS
{
    SPELL_TPS = 21547
};

class go_tps : public GameObjectScript
{
public:
    go_tps() : GameObjectScript("go_tps") { }

    struct go_tpsAI : public GameObjectAI
    {
        go_tpsAI(GameObject* go) : GameObjectAI(go), timer{ 0 } { }

        void UpdateAI(uint32  diff) override
        {
            timer += diff;
            if (timer > 2000)
            {
                timer = 0;
                std::list<Player*> players;
                GameObject* player = nullptr;
                Trinity::GameObjectInRangeCheck checker(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.3f);
                Trinity::GameObjectLastSearcher<Trinity::GameObjectInRangeCheck> searcher(me, player, checker);
                Cell::VisitGridObjects(me, searcher, 0.3f);

                if (players.size() > 0)
                {
                    std::list<Player*>::iterator itr = players.begin();
                    std::advance(itr, urand(0, players.size() - 1));
                    if (Creature* trigger = me->SummonTrigger((*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), 0, 5s))
                        trigger->CastSpell(trigger, SPELL_TPS);
                }
            }
        }

    private:
        uint32 timer;
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_tpsAI(go);
    }
};

void AddSC_ItemUse_item_custom()
{
    new go_tps();
    new ItemUse_item_custom();
    new rip();
}
