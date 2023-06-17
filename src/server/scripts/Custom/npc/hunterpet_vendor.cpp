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

enum Creatures
{
        //CREATURE_FELSHETZER                           = 19350,
        CREATURE_FLEDERMAUS                             = 3866,
        CREATURE_NETHERROCHEN                           = 18131,
        CREATURE_RAUBVOGELROT                           = 21804,
        CREATURE_RAUBVOGELSCHWARZ                       = 25817,
        CREATURE_RAUBVOGELWEISS                         = 26369,
        //CREATURE_SCHLANGE                             = 5224,
        CREATURE_SPINNE                                 = 20714,
        CREATURE_GORILLASCHWARZ                         = 2521,
        CREATURE_GORILLAWEISS                           = 6516,
        CREATURE_KREBS                                  = 23929,
        //CREATURE_KROKOLISK                            = 20773,
        //CREATURE_AASVOGEL                             = 16972,
        CREATURE_HYAENE                                 = 27294,
        CREATURE_KATZETIGER                             = 7434,
        CREATURE_KATZELOEWE                             = 28097,
        //CREATURE_KATZELUCHS                           = 24043,
        CREATURE_RAPTORGRUEN                            = 21124,
        CREATURE_RAPTORROT                              = 4352,
        CREATURE_WEITSCHREITER                          = 3246,
        CREATURE_WESPEROT                               = 18283,
        CREATURE_WESPEBLAU                              = 28086,
        CREATURE_WOLFRUESTUNG                           = 17280,
        //CREATURE_WOLFGRAU                             = 24128,
        //CREATURE_WOLFWEI?                             = 26672,
        CREATURE_SCHIMAERE                              = 20932,
        CREATURE_SILITHIDGELB                           = 15230,
        CREATURE_SILITHIDROT                            = 5460,
        CREATURE_RHINOZEROS                             = 25488,
        //CREATURE_WURMBRAUN                            = 34920,
        CREATURE_WURMGELB                               = 14237,
        CREATURE_KERNHUNDWEISS                          = 17447,
        CREATURE_KERNHUNDROT                            = 11671,
        CREATURE_TEUFELSSAURIERWEISS                    = 6498,
        CREATURE_TEUFELSSAURIERSCHWARZ                  = 20931,
        CREATURE_SKOLL                                  = 35189,
        CREATURE_LOQUE                                  = 32517,
        CREATURE_GONDRIA                                = 33776,
        CREATURE_ARCTURIS                               = 38453,
};
 
#define main    100
#define pets    200
#define exotic  300
// Pets
#define felshetzer 201
#define fledermaus 202
#define netherrochen 203
#define raubvogelrot 204
#define raubvogelschwarz 205
#define raubvogelweiss 206
#define schlange 207
#define spinne 208
#define gorillaschwarz 209
#define gorillaweiss 210
#define krebs 211
#define krokolisk 212
#define aasvogel 213
#define hyaene 214
#define katzetiger 215
#define katzeloewe 216
#define katzeluchs 217
#define raptorgruen 218
#define raptorrot 219  
#define weitschreiter 220
#define wesperot 221
#define wespeblau 222
#define wolfruestung 223
#define wolfgrau 224
#define wolfweiss 225
// Exotic
#define schimaere 326
#define silithidgelb 327
#define silithidrot 328
#define rhinozeros 329
#define wurmbraun 330
#define wurmgelb 331
#define kernhundweiss 332
#define kernhundrot 333
#define teufelssaurierweiss 334
#define teufelssaurierschwarz 335
#define skoll 336
#define loque 337
#define gondria 338
#define arcturis 339 
 
 
 //Preise
#define PRICE_PET_TYP_1    250000
#define PRICE_PET_TYP_2    5000000
#define MSG_NOT_MONEY "You have not enough gold!"
#define MSG_PET_SUMMON "The creature will be summoned"
 
class npc_hunterpetvendor : public CreatureScript
{
public:
     npc_hunterpetvendor() : CreatureScript("npc_hunterpetvendor") { }
        
     struct npc_hunterpetvendorAI : public ScriptedAI
    {
        npc_hunterpetvendorAI(Creature* me) : ScriptedAI(me) { }

    bool OnGossipHello(Player* player) override
    {
        if ((player->GetClass() == CLASS_HUNTER))
        {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cff00ff00|TInterface\\icons\\Ability_Hunter_Pet_Devilsaur:30|t|r Normal Creatures (Cost 25 Gold)", GOSSIP_SENDER_MAIN, pets);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cff00ff00|TInterface\\icons\\Ability_Hunter_BeastMastery:30|t|r Exotic Creatures (Cost 500 Gold)", GOSSIP_SENDER_MAIN, exotic);
        SendGossipMenuFor(player, 40009, me->GetGUID());
    return true;
        }
        else
        {
                SendGossipMenuFor(player, 40010, me->GetGUID());
                return true;
        }
}
 
bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
        uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
        uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
player->PlayerTalkClass->ClearMenus();
    switch(action)
    {
                case pets:
                        // 2, "Felshetzer", GOSSIP_SENDER_MAIN, felshetzer);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Vile Bat", GOSSIP_SENDER_MAIN, fledermaus);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Marshfang Slicer", GOSSIP_SENDER_MAIN, netherrochen);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Skettis Kaliri", GOSSIP_SENDER_MAIN, raubvogelrot);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Oiled Fledgeling", GOSSIP_SENDER_MAIN, raubvogelschwarz);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Imperial Eagle", GOSSIP_SENDER_MAIN, raubvogelweiss);
                        //AddGossipItemFor(player, 2, "Schlange", GOSSIP_SENDER_MAIN, schlange);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Ridgespine Stalker", GOSSIP_SENDER_MAIN, spinne);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Skymane Gorilla", GOSSIP_SENDER_MAIN, gorillaschwarz);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Un'Goro Thunderer", GOSSIP_SENDER_MAIN, gorillaweiss);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Giant Tidecrawler", GOSSIP_SENDER_MAIN, krebs);
                        //AddGossipItemFor(player, 2, "Krokodil", GOSSIP_SENDER_MAIN, krokolisk);
                        //AddGossipItemFor(player, 2, "Aasvogel", GOSSIP_SENDER_MAIN, aasvogel);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Tundra Scavenger", GOSSIP_SENDER_MAIN, hyaene);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Frostsaber Pride Watcher", GOSSIP_SENDER_MAIN, katzetiger);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Pitch", GOSSIP_SENDER_MAIN, katzeloewe);
                        //AddGossipItemFor(player, 2, "Katze (Luchs)", GOSSIP_SENDER_MAIN, katzeluchs);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Felsworn Daggermaw", GOSSIP_SENDER_MAIN, raptorgruen);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Bloodfen Screecher", GOSSIP_SENDER_MAIN, raptorrot);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Fleeting Plainstrider", GOSSIP_SENDER_MAIN, weitschreiter);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Blacksting", GOSSIP_SENDER_MAIN, wesperot);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Sapphire Hive Wasp", GOSSIP_SENDER_MAIN, wespeblau);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Shattered Hand Warhound", GOSSIP_SENDER_MAIN, wolfruestung);
                        //AddGossipItemFor(player, 2, "Wolf (Wilder Worg)", GOSSIP_SENDER_MAIN, wolfgrau);
                        //AddGossipItemFor(player, 2, "Wolf (Blutdurstiger Tundrawolf)", GOSSIP_SENDER_MAIN, wolfwei?);
                        //AddGossipItemFor(player, 0, "I would prefer a exotic list...", GOSSIP_SENDER_MAIN, exotic);
 
                        SendGossipMenuFor(player, 40011, me->GetGUID());
                break;
               
                case exotic:
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Nuramoc", GOSSIP_SENDER_MAIN, schimaere);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Vekniss Warrior", GOSSIP_SENDER_MAIN, silithidgelb);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Centipaar Sandreaver", GOSSIP_SENDER_MAIN, silithidrot);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Wooly Rhino Calf", GOSSIP_SENDER_MAIN, rhinozeros);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Oozeworm", GOSSIP_SENDER_MAIN, wurmgelb);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "The Kurken", GOSSIP_SENDER_MAIN, kernhundweiss);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Core Hound", GOSSIP_SENDER_MAIN, kernhundrot);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Devilsaur", GOSSIP_SENDER_MAIN, teufelssaurierweiss);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Tyrantus", GOSSIP_SENDER_MAIN, teufelssaurierschwarz);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Skoll", GOSSIP_SENDER_MAIN, skoll);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Loque'nahak", GOSSIP_SENDER_MAIN, loque);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Gondria", GOSSIP_SENDER_MAIN, gondria);
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Arcturis", GOSSIP_SENDER_MAIN, arcturis);
                        //AddGossipItemFor(player,0, "I would prefer a non-exotic list...", GOSSIP_SENDER_MAIN, pets);
                       
                        SendGossipMenuFor(player, 40011, me->GetGUID());
                break;
                /*
                case felshetzer:
                        me->SummonCreature(CREATURE_FELSHETZER, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                        CloseGossipMenuFor(player); break;
                */
                case fledermaus:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_FLEDERMAUS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;				

                case netherrochen:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_NETHERROCHEN, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;				

                case raubvogelrot:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_RAUBVOGELROT, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;				

                case raubvogelschwarz:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_RAUBVOGELSCHWARZ, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case raubvogelweiss:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_RAUBVOGELWEISS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;					
                /*      
                case schlange:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_SCHLANGE, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                */      
                case spinne:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_SPINNE, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;				
                       
                case gorillaschwarz:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_GORILLASCHWARZ, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;							
                       
                case gorillaweiss:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_GORILLAWEISS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;					
                       
                case krebs:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_KREBS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case hyaene:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_HYAENE, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case katzetiger:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_KATZETIGER, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case katzeloewe:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_KATZELOEWE, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                        /*
                case katzeluchs:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_KATZELUCHS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                        */
                case raptorgruen:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_RAPTORGRUEN, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;							
                       
                case raptorrot:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_RAPTORROT, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;							
                       
                case weitschreiter:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_WEITSCHREITER, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case wesperot:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_WESPEROT, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case wespeblau:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_WESPEBLAU, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case wolfruestung:
                    if (player->GetMoney() < PRICE_PET_TYP_1)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_1);
                        me->SummonCreature(CREATURE_WOLFRUESTUNG, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;					
                       
                        // -- Exotics -- //
                       
                case schimaere:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_SCHIMAERE, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case silithidgelb:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_SILITHIDGELB, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;							
                       
                case silithidrot:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_SILITHIDROT, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case rhinozeros:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_RHINOZEROS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;								
       
                case wurmgelb:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_WURMGELB, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;								
                       
                case kernhundweiss:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_KERNHUNDWEISS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case kernhundrot:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_KERNHUNDROT, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case teufelssaurierweiss:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_TEUFELSSAURIERWEISS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;							
                       
                case teufelssaurierschwarz:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_TEUFELSSAURIERSCHWARZ, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;					
                       
                case skoll:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_SKOLL, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
                       
                case loque:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_LOQUE, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;					
                       
                case gondria:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_GONDRIA, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;							
                       
                case arcturis:
                    if (player->GetMoney() < PRICE_PET_TYP_2)
                    {
                        me->Whisper(MSG_NOT_MONEY, LANG_UNIVERSAL, player);
                    }else{
                        me->Whisper(MSG_PET_SUMMON, LANG_UNIVERSAL, player);
                        player->ModifyMoney(-PRICE_PET_TYP_2);
                        me->SummonCreature(CREATURE_ARCTURIS, me->GetPositionX()+4, me->GetPositionY(), me->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_DESPAWN, 60s);
                    }
                    CloseGossipMenuFor(player);
                    break;						
    }
 
    return true;
     }
};
    
    CreatureAI* GetAI(Creature* me) const
    {
        return new npc_hunterpetvendorAI(me);
    }
};

enum GOSSIPS
{
    FALL_OF_THE_LICH_KING,
};

#define GOSSIP_TEXT_SEE_FALL_OF_THE_LICH_KING  "See the fall of the Lich King."
#define GOSSIP_MESSAGE_MEMORIAL  15921

class go_dedication_of_honor : public GameObjectScript
{
public:
    go_dedication_of_honor() : GameObjectScript("go_dedication_of_honor") { }
    
    struct  go_dedication_of_honorAI : public GameObjectAI
    {
        go_dedication_of_honorAI(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_TEXT_SEE_FALL_OF_THE_LICH_KING, GOSSIP_SENDER_MAIN, FALL_OF_THE_LICH_KING);
        SendGossipMenuFor(player, GOSSIP_MESSAGE_MEMORIAL, me->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
        uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
        uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case FALL_OF_THE_LICH_KING:
            player->SendMovieStart(16);
            CloseGossipMenuFor(player);
            break;
        }
        return true;
}
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_dedication_of_honorAI(go);
    }
};

void AddSC_npc_hunterpetvendor()
{
        new go_dedication_of_honor();
        new npc_hunterpetvendor;
}
