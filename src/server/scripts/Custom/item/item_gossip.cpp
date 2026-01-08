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
            player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, nullptr);
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

class go_proximity_caster : public GameObjectScript
{
public:
    go_proximity_caster() : GameObjectScript("go_proximity_caster") {}

    struct go_proximity_casterAI : public GameObjectAI
    {
        go_proximity_casterAI(GameObject* go) : GameObjectAI(go)
        {
            checkTimer = 1000;
        }

        uint32 checkTimer;

        void UpdateAI(uint32 diff) override
        {
            if (checkTimer <= diff)
            {
                std::list<Player*> players;
                me->GetPlayerListInGrid(players, 10.0f);

                for (Player* player : players)
                {
                    if (player->IsAlive() && !player->IsGameMaster())
                    {
                        player->AddAura(SPELL_TPS, player); //spell id
                    }
                }
                checkTimer = 5000; //time
            }
            else
                checkTimer -= diff;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_proximity_casterAI(go);
    }
};

class go_proximity_caster2 : public GameObjectScript
{
public:
    go_proximity_caster2() : GameObjectScript("go_proximity_caster2") {}

    struct go_proximity_caster2AI : public GameObjectAI
    {
        go_proximity_caster2AI(GameObject* go) : GameObjectAI(go)
        {
            checkTimer = 1000;
        }

        uint32 checkTimer;

        void UpdateAI(uint32 diff) override
        {
            if (checkTimer <= diff)
            {
                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, 0, 10.0f); // 0 all creature

                for (Creature* creature : creatures)
                {
                    if (creature->IsAlive())
                    {
                        creature->AddAura(SPELL_TPS, creature); // spell id
                    }
                }

                checkTimer = 5000; //time
            }
            else
                checkTimer -= diff;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_proximity_caster2AI(go);
    }
};

class GuildLevelUpItem : public ItemScript
{
public:
    GuildLevelUpItem() : ItemScript("GuildLevelUpItem") {}

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (!player || !item || !player->GetSession() || !player->GetGuild())
            return true;

        Guild* guild = sGuildMgr->GetGuildById(player->GetGuild()->GetId());
        if (!guild) {
            ChatHandler(player->GetSession()).PSendSysMessage("not");
            return true;
        }

        uint32 entry = item->GetEntry();

       // uint32 exp = sGameConfig->GetIntConfig("QueryList.Enabled") * count;
        guild->GiveXp(10000);

        player->DestroyItemCount(entry, 1, true);
        return true;
    }
};

//class rip2 : public ItemScript
//{
//public:
//    rip2() : ItemScript("rip2") {}
//
//    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
//    {
//        WorldSession* session = player->GetSession();
//
//        if (player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->isDead())
//        {
//            ChatHandler(player->GetSession()).PSendSysMessage(session->GetTrinityString2(NOT_USED_9));
//            return false;
//        }
//
//        std::vector<uint32> factionIds = {
//            72,    // Stormwind
//            47,    // Ironforge  
//            54,    // Gnomeregan
//            69,    // Darnassus
//            930,   // Exodar
//        };
//
//        int32 reputationLoss = -25000;
//
//        for (uint32 factionId : factionIds)
//        {
//            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId))
//            {
//                player->GetReputationMgr().ModifyReputation(factionEntry, reputationLoss);
//            }
//        }
//
//        ChatHandler(player->GetSession()).PSendSysMessage("???? ????????? ? ??????????? ????????? ???????????!");
//
//        player->DestroyItemCount(item->GetEntry(), 1, true);
//
//        return true;
//    }
//};
//
//class rip3 : public ItemScript
//{
//private:
//    struct FactionGroup
//    {
//        std::vector<uint32> allianceFactions;
//        std::vector<uint32> hordeFactions;
//        std::map<uint32, std::string> factionNames;
//    };
//
//    FactionGroup m_factions;
//
//public:
//    rip3() : ItemScript("rip3")
//    {
//        InitializeFactions();
//    }
//
//    void InitializeFactions()
//    {
//        // ??????? ???????
//        m_factions.allianceFactions = { 72, 47, 54, 69, 930 };
//
//        // ??????? ????
//        m_factions.hordeFactions = { 76, 68, 81, 530, 911 };
//
//        // ???????? ???????
//        m_factions.factionNames[72] = "Stormwind";
//        m_factions.factionNames[47] = "Ironforge";
//        m_factions.factionNames[54] = "Gnomeregan";
//        m_factions.factionNames[69] = "Darnassus";
//        m_factions.factionNames[930] = "Exodar";
//
//        m_factions.factionNames[76] = "Orgrimmar";
//        m_factions.factionNames[68] = "Undercity";
//        m_factions.factionNames[81] = "Thunder Bluff";
//        m_factions.factionNames[530] = "Darkspear Trolls";
//        m_factions.factionNames[911] = "Silvermoon City";
//    }
//
//    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
//    {
//        if (!player || !item)
//            return false;
//
//        // ??????? ????????
//        if (player->GetLevel() < 10)
//        {
//            ChatHandler(player->GetSession()).SendSysMessage("?? ?????? ???? ??????? 10 ?????? ??? ????????????? ????? ????????.");
//            return false;
//        }
//
//        if (player->IsInCombat())
//        {
//            ChatHandler(player->GetSession()).SendSysMessage("?????? ???????????? ? ???!");
//            return false;
//        }
//
//        std::vector<uint32> targetFactions;
//        std::string factionType;
//
//        if (player->GetTeamId() == TEAM_ALLIANCE)
//        {
//            targetFactions = m_factions.allianceFactions;
//            factionType = "???????";
//        }
//        else if (player->GetTeamId() == TEAM_HORDE)
//        {
//            targetFactions = m_factions.hordeFactions;
//            factionType = "????";
//        }
//        else
//        {
//            ChatHandler(player->GetSession()).SendSysMessage("?????? ??????????? ??????? ??????!");
//            return false;
//        }
//
//        int32 reputationLoss = -25000;
//        uint32 modifiedFactions = 0;
//        std::string affectedFactions;
//
//        for (uint32 factionId : targetFactions)
//        {
//            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId))
//            {
//                ReputationRank currentRank = player->GetReputationMgr().GetRank(factionEntry);
//                if (currentRank > REP_HOSTILE)
//                {
//                    player->GetReputationMgr().ModifyReputation(factionEntry, reputationLoss);
//
//                    modifiedFactions++;
//                    if (!affectedFactions.empty())
//                        affectedFactions += ", ";
//
//                    auto it = m_factions.factionNames.find(factionId);
//                    if (it != m_factions.factionNames.end())
//                        affectedFactions += it->second;
//                    else
//                        affectedFactions += std::to_string(factionId);
//                }
//            }
//        }
//
//        if (modifiedFactions > 0)
//        {
//            ChatHandler(player->GetSession()).PSendSysMessage(
//                "????????? %s ????????? ? %u ?????????: %s",
//                factionType.c_str(),
//                modifiedFactions,
//                affectedFactions.c_str()
//            );
//
//            player->CastSpell(player, 27827, true);
//
//            player->DestroyItemCount(item->GetEntry(), 1, true);
//            return true;
//        }
//        else
//        {
//            ChatHandler(player->GetSession()).PSendSysMessage("????????? ? ????????? %s ??? ?????????? ??? ???????????.", factionType.c_str());
//            return false;
//        }
//    }
//};
//
//class rip3_advanced : public ItemScript
//{
//private:
//    struct FactionGroup
//    {
//        std::vector<uint32> allianceFactions;
//        std::vector<uint32> hordeFactions;
//        std::map<uint32, std::string> factionNames;
//    };
//
//    FactionGroup m_factions;
//
//    struct ReputationSettings
//    {
//        int32 ownFactionLoss;
//        int32 enemyFactionGain;
//        uint32 cooldownHours;
//        uint32 minLevel;
//    };
//
//public:
//    rip3_advanced() : ItemScript("rip3_advanced")
//    {
//        InitializeFactions();
//    }
//
//    void InitializeFactions()
//    {
//        m_factions.allianceFactions = { 72, 47, 54, 69, 930 };
//
//        m_factions.hordeFactions = { 76, 68, 81, 530, 911 };
//
//        m_factions.factionNames[1] = "Stormwind";
//        m_factions.factionNames[3] = "Ironforge";
//        m_factions.factionNames[4] = "Gnomeregan";
//        m_factions.factionNames[115] = "Darnassus";
//        m_factions.factionNames[930] = "Exodar";
//
//        m_factions.factionNames[2] = "Orgrimmar";
//        m_factions.factionNames[5] = "Undercity";
//        m_factions.factionNames[6] = "Thunder Bluff";
//        m_factions.factionNames[116] = "Darkspear Trolls";
//        m_factions.factionNames[911] = "Silvermoon City";
//    }
//
//    ReputationSettings GetReputationSettings(Player* /*player*/, Item* item)
//    {
//        ReputationSettings settings;
//
//        uint32 itemQuality = item->GetTemplate()->Quality;
//
//        switch (itemQuality)
//        {
//        case ITEM_QUALITY_UNCOMMON:
//            settings = { -25000, 25000, 12, 15 };
//            break;
//        case ITEM_QUALITY_RARE:
//            settings = { -25000, 25000, 12, 15 };
//            break;
//        case ITEM_QUALITY_EPIC:
//            settings = { -25000, 25000, 12, 15 };
//            break;
//        case ITEM_QUALITY_LEGENDARY:
//            settings = { -25000, 25000, 12, 15 };
//            break;
//        default:
//            settings = { -25000, 25000, 12, 15 };
//            break;
//        }
//
//        return settings;
//    }
//
//    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
//    {
//        if (!player || !item)
//            return false;
//
//        ReputationSettings settings = GetReputationSettings(player, item);
//
//        if (player->GetLevel() < settings.minLevel)
//        {
//            ChatHandler(player->GetSession()).PSendSysMessage("?? ?????? ???? ??????? %u ?????? ??? ????????????? ????? ????????.", settings.minLevel);
//            return false;
//        }
//
//        if (player->IsInCombat())
//        {
//            ChatHandler(player->GetSession()).SendSysMessage("?????? ???????????? ? ???!");
//            return false;
//        }
//
//        std::vector<uint32> ownFactions, enemyFactions;
//        std::string ownFactionType, enemyFactionType;
//
//        if (player->GetTeamId() == TEAM_ALLIANCE)
//        {
//            ownFactions = m_factions.allianceFactions;
//            enemyFactions = m_factions.hordeFactions;
//            ownFactionType = "???????";
//            enemyFactionType = "????";
//        }
//        else if (player->GetTeamId() == TEAM_HORDE)
//        {
//            ownFactions = m_factions.hordeFactions;
//            enemyFactions = m_factions.allianceFactions;
//            ownFactionType = "????";
//            enemyFactionType = "???????";
//        }
//        else
//        {
//            ChatHandler(player->GetSession()).SendSysMessage("?????? ??????????? ??????? ??????!");
//            return false;
//        }
//
//        uint32 decreasedFactions = ModifyFactionsReputation(player, ownFactions, settings.ownFactionLoss);
//        uint32 increasedFactions = ModifyFactionsReputation(player, enemyFactions, settings.enemyFactionGain);
//
//        if (decreasedFactions > 0 || increasedFactions > 0)
//        {
//            // ?????????
//            if (decreasedFactions > 0)
//            {
//                ChatHandler(player->GetSession()).PSendSysMessage(
//                    "|cffff0000?????????????!|r ????????? %s ????????? ? %u ????????? ?? %d ?????",
//                    ownFactionType.c_str(),
//                    decreasedFactions,
//                    abs(settings.ownFactionLoss)
//                );
//            }
//
//            if (increasedFactions > 0)
//            {
//                ChatHandler(player->GetSession()).PSendSysMessage(
//                    "|cff00ff00????? ?????!|r ????????? %s ????????? ? %u ????????? ?? %d ?????",
//                    enemyFactionType.c_str(),
//                    increasedFactions,
//                    settings.enemyFactionGain
//                );
//            }
//
//            ApplyVisualEffects(player);
//
//            player->DestroyItemCount(item->GetEntry(), 1, true);
//            return true;
//        }
//        else
//        {
//            ChatHandler(player->GetSession()).SendSysMessage("????????? ????????? ? ?????? ?????? ??????????.");
//            return false;
//        }
//    }
//
//private:
//    uint32 ModifyFactionsReputation(Player* player, const std::vector<uint32>& factions, int32 change)
//    {
//        uint32 modifiedCount = 0;
//
//        for (uint32 factionId : factions)
//        {
//            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId))
//            {
//                ReputationRank currentRank = player->GetReputationMgr().GetRank(factionEntry);
//
//                if ((change < 0 && currentRank > REP_HATED) ||
//                    (change > 0 && currentRank < REP_EXALTED))
//                {
//                    player->GetReputationMgr().ModifyReputation(factionEntry, change);
//                    modifiedCount++;
//                }
//            }
//        }
//
//        return modifiedCount;
//    }
//
//    void ApplyVisualEffects(Player* player)
//    {
//        player->CastSpell(player, 5782, true);
//
//        if (player->GetTeamId() == TEAM_ALLIANCE)
//        {
//            player->CastSpell(player, 20549, true);
//        }
//        else
//        {
//            player->CastSpell(player, 1126, true);
//        }
//    }
//};

void AddSC_ItemUse_item_custom()
{
    new GuildLevelUpItem();
    new go_proximity_caster();
    new go_proximity_caster2();
    new go_tps();
    new ItemUse_item_custom();
    new rip();
    //new rip2();
    //new rip3();
    //new rip3_advanced();
}
