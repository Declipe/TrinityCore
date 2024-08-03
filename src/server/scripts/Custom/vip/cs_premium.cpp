#include "AccountMgr.h"
#include "AditionalData.h"
#include "Custom/Dcl.h"
#include "SpellHistory.h"

#if TRINITY_COMPILER == TRINITY_COMPILER_GNU
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

using namespace Trinity::ChatCommands;

class premium_commandscript : public CommandScript
{
public:
    premium_commandscript() : CommandScript("premium_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable premiumCommandTable =
        {
            { "bank",          HandlePremiumBankCommand,     rbac::RBAC_PERM_COMMAND_VIP_BANK, Console::No},
            { "mail",          HandlePremiumMailCommand,     rbac::RBAC_PERM_COMMAND_VIP_MAIL, Console::No },
            { "buffs",         HandleVipbuffsCommand,        rbac::RBAC_PERM_COMMAND_VIP_buffs, Console::No },
            { "arena",         HandleVipjoinArenaCommand,    rbac::RBAC_PERM_COMMAND_VIP_arena, Console::No },
            { "warsong",       HandleVipjoinWarsongCommand,  rbac::RBAC_PERM_COMMAND_VIP_warsong, Console::No },
            { "arathi",        HandleVipjoinArathiCommand,   rbac::RBAC_PERM_COMMAND_VIP_arathi, Console::No },
            { "eye",           HandleVipjoinEyeCommand,      rbac::RBAC_PERM_COMMAND_VIP_eye, Console::No },
            { "alterac",       HandleVipjoinAlteracCommand,  rbac::RBAC_PERM_COMMAND_VIP_alterac, Console::No },
            { "debuff",        HandleVipDebuffCommand,       rbac::RBAC_PERM_COMMAND_VIP_DEBUFF, Console::No },
            { "map",           HandleVipMapCommand,          rbac::RBAC_PERM_COMMAND_VIP_MAP, Console::No },
            { "resettalents",  HandleVipResetTalentsCommand, rbac::RBAC_PERM_COMMAND_VIP_RESETTALENTS, Console::No },
            { "repair",        HandleVipRepairCommand,       rbac::RBAC_PERM_COMMAND_VIP_REPAIR, Console::No },
            { "capital",       HandleVipCapitalCommand,      rbac::RBAC_PERM_COMMAND_VIP_CAPITAL, Console::No },
            { "changerace",    HandleChangeRaceCommand,      rbac::RBAC_PERM_COMMAND_VIP_CHANGERACE, Console::No },
            { "customize",     HandleCustomizeCommand,       rbac::RBAC_PERM_COMMAND_VIP_CUSTOMIZE, Console::No },
            { "app",           HandleAppearCommand,          rbac::RBAC_PERM_COMMAND_VIP_ARPPEAR, Console::No },
            { "taxi",          HandleVipTaxiCommand,         rbac::RBAC_PERM_COMMAND_VIP_TAXI,  Console::No },
            { "home",          HandleVipHomeCommand,         rbac::RBAC_PERM_COMMAND_VIP_HOME,  Console::No },
            { "teles",         HandleTelesNameCommand,       rbac::RBAC_PERM_COMMAND_VIP_HOMEs,  Console::No },
            { "status",        HandleVipStatusCommand,       rbac::RBAC_HandleVipStatusCommand,  Console::No },
            { "gbuff",         HandleGuildBuffCommand,       rbac::RBAC_PERM_COMMAND_GXP_BUFF,  Console::No },
            { "set",           HandleSetVipCommand,          rbac::RBAC_PERM_COMMAND_VIP_SET,  Console::No },
            { "del",           HandleDelVipCommand,          rbac::RBAC_PERM_COMMAND_VIP_REMOVE,  Console::No },
            { "Activate",      HandleActivateCommand,        rbac::RBAC_PERM_COMMAND_GM,  Console::No },
            { "transfer",      HandleTransferCommand,        rbac::RBAC_ROLE_PLAYER,  Console::No },
        };

        static ChatCommandTable coinCommandTable =
        {
            { "add",          HandleCoinAddCommand,    rbac::RBAC_PERM_COMMAND_ADDCOIN,         Console::No },
            { "del",          HandleCoinDelCommand,   rbac::RBAC_PERM_COMMAND_ADDCOIN,          Console::No },
        };

        static ChatCommandTable commandTable =
        {
            { "vip", premiumCommandTable },
            { "coin", coinCommandTable },
        };

        return commandTable;
    }

    static bool CanUseCommand(Player* player, ChatHandler* handler)
    {
        if (player->IsInFlight())
        {
            handler->PSendSysMessage("Failure! You are flying.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInCombat())
        {
            handler->PSendSysMessage("Failure! You are in combat.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->isDead() || player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        {
            handler->PSendSysMessage("Failure! You are dead.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }

    static bool AccountExists(std::string accountName)
    {
        QueryResult result = LoginDatabase.PQuery("SELECT id FROM account WHERE username = '{}'", accountName.c_str());

        if (!result)
            return false;

        return true;
    }

    static bool CharacterExists(std::string characterName)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM characters WHERE name = '{}'", characterName.c_str());

        if (!result)
            return false;

        return true;
    }

    static bool AccountHasCharacter(uint32 accountId, std::string characterName)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM characters WHERE account = {} AND name = '{}'", accountId, characterName.c_str());

        if (!result)
            return false;

        return true;
    }

    static bool HasRealmCharacter(uint32 accountId)
    {
        QueryResult result = LoginDatabase.PQuery("SELECT acctid FROM realmcharacters WHERE acctid = {}", accountId);

        if (!result)
            return false;

        return true;
    }

    static uint32 GetCharacterAccountId(std::string characterName)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT account FROM characters WHERE name = '{}'", characterName.c_str());

        if (!result)
            return NULL;

        return result->Fetch()[0].GetUInt32();
    }

    static std::string GetAccountNameById(uint32 accountId)
    {
        QueryResult result = LoginDatabase.PQuery("SELECT username FROM account WHERE id = {}", accountId);

        if (!result)
            return NULL;

        return result->Fetch()[0].GetString();
    }
    static bool HandleTransferCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CanUseCommand(player, handler))
            return false;

        if (!*args)
        {
            handler->PSendSysMessage("Syntax: .transfer #characterName #newAccountName");
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* c_CharName = strtok((char*)args, " ");
        char* c_NewAccountName = strtok(NULL, " ");
        if (!c_CharName || !c_NewAccountName)
            return false;

        std::string CharName = c_CharName;
        CharName[0] = toupper(CharName[0]);

        if (!CharacterExists(CharName))
        {
            handler->PSendSysMessage("Failure! The character %s not exists.", CharName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string NewAccountName = c_NewAccountName;
        NewAccountName[0] = toupper(NewAccountName[0]);

        if (!AccountExists(NewAccountName))
        {
            handler->PSendSysMessage("Failure! The account %s not exists.", NewAccountName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 OldAccountId = GetCharacterAccountId(CharName);
        uint32 NewAccountId = AccountMgr::GetId(NewAccountName);

        if (!AccountHasCharacter(OldAccountId, CharName))
        {
            handler->PSendSysMessage("Failure! The account %s not has the character %s.", GetAccountNameById(OldAccountId).c_str(), CharName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (AccountMgr::GetCharactersCount(NewAccountId) >= sWorld->getIntConfig(CONFIG_CHARACTERS_PER_REALM))
        {
            handler->PSendSysMessage("Failure! The account %s is full.", NewAccountName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (Player* player = ObjectAccessor::FindConnectedPlayerByName(CharName.c_str()))
        {
            player->SaveToDB();
            player->GetSession()->KickPlayer("Bonk");
        }

        if (!HasRealmCharacter(NewAccountId))
            LoginDatabase.PQuery("INSERT INTO realmcharacters (realmid, acctid, numchars) VALUES ('1', '%u', '0')", NewAccountId);

        LoginDatabase.PQuery("UPDATE realmcharacters SET numchars = numchars - 1 WHERE acctid = {}", OldAccountId);
        LoginDatabase.PQuery("UPDATE realmcharacters SET numchars = numchars + 1 WHERE acctid = {}", NewAccountId);
        CharacterDatabase.PQuery("UPDATE characters SET account = {} WHERE name = '{}'", NewAccountId, CharName.c_str());

        handler->PSendSysMessage("Success! The character %s was transferred to the account %s.", CharName.c_str(), NewAccountName.c_str());
        return true;
    }


    static bool HandleActivateCommand(ChatHandler* handler)
    {
        Player* player = handler->GetSession()->GetPlayer();
        time_t unsetdate;
        /*time_t setdate;*/

        if (player->HasItemCount(37742, 3000, false))
        {
            if (player->GetAditionalData()->isPremium())
            {
                unsetdate = player->GetAditionalData()->getPremiumUnsetdate() + 604800; // 7 day
                AccountMgr::UpdateVipStatus(player->GetSession()->GetAccountId(), unsetdate);
            }
            else
            {
                unsetdate = GameTime::GetGameTime() + 604800; // 7 day
                AccountMgr::SetVipStatus(player->GetSession()->GetAccountId(), unsetdate);
            }
            player->GetAditionalData()->setPremiumUnsetdate(unsetdate);
            player->GetAditionalData()->setPremiumStatus(true);
            //player->AddItem(184, 1);
            player->DestroyItemCount(37742, 3000, true, false);
            handler->PSendSysMessage("Your VIP rank has been updated.Login to get it active");
            return true;
        }
        return true;
    }

    static bool HandleSetVipCommand(ChatHandler* handler, uint32 days_bonus, uint32 accountID)
    {
        Player* target = handler->getSelectedPlayerOrSelf();

        if (!accountID)
            accountID = target->GetSession()->GetAccountId();

        time_t current_time = GameTime::GetGameTime();
        time_t unsetdate = current_time + 24 * 60 * 60 * days_bonus;

        bool vip = AccountMgr::GetVipStatus(accountID);

        if (vip)
            AccountMgr::UpdateVipStatus(accountID, unsetdate);
        else
            AccountMgr::SetVipStatus(accountID, unsetdate);

        // check on online
        ObjectGuid::LowType guid = AccountMgr::GetGuidOfOnlineCharacter(accountID);
        if (guid)
        {
            if (Player* player = ObjectAccessor::FindPlayerByLowGUID(guid))
            {
                player->GetAditionalData()->setPremiumStatus(true);
                player->GetAditionalData()->setPremiumUnsetdate(unsetdate);
                handler->PSendSysMessage("VIP privileges has been set for Account: %u Character:[%s] (online) (GUID: %u), for %u days", accountID, player->GetName(), player->GetGUID().GetCounter(), days_bonus);
            }
        }
        else
            handler->PSendSysMessage("VIP privileges has been set for Account: %u, for %u days, no characters online", accountID, days_bonus);
        return true;
    }

    static bool HandleDelVipCommand(ChatHandler* handler, uint32 accountID)
    {
        Player* target = handler->getSelectedPlayerOrSelf();

        if (!accountID)
            accountID = target->GetSession()->GetAccountId();

        bool vip = AccountMgr::GetVipStatus(accountID);
        if (!vip)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        AccountMgr::RemoveVipStatus(accountID);
        ObjectGuid::LowType guid = AccountMgr::GetGuidOfOnlineCharacter(accountID);
        if (guid)
        {
            if (Player* p = ObjectAccessor::FindPlayerByLowGUID(guid))
            {
                p->GetAditionalData()->setPremiumStatus(false);
                p->GetAditionalData()->setPremiumUnsetdate(0);
                handler->PSendSysMessage("VIP privileges were removed for Account: %u Character:[%s] (online) (GUID: %u)", accountID, p->GetName(), p->GetGUID().GetCounter());
            }
        }
        else
            handler->PSendSysMessage("VIP privileges were removed for Account: %u, no characters online", accountID);
        return true;
    }

    static bool HandleGuildBuffCommand(ChatHandler* handler)
    {
        Player* me = handler->GetSession()->GetPlayer();
        Guild* guild = me->GetGuild();
        if (!guild)
        {
            handler->SendSysMessage("Sie sind in keiner Gilde.");
            return false;
        }
        Group* group = me->GetGroup();
        if (!group)
        {
            handler->SendSysMessage("Ihr seid in keiner Gruppe.");
            return false;
        }
        if (me->GetGUID() != group->GetLeaderGUID()) {
            handler->SendSysMessage("Ihr seid nicht der Gruppenleiter.");
            return false;
        }
        GroupReference* target = group->GetFirstMember();
        while (target)
        {
            me = target->GetSource();
            //INSERT BUFFLIST HERE:
            me->CastSpell(me, 48073, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 48161, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 48469, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 58054, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 42995, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 48102, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 48104, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 58451, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 58449, TRIGGERED_FULL_MASK);
            me->CastSpell(me, 48100, TRIGGERED_FULL_MASK);
            //END BUFFLIST
            target = target->next();
        }
        return true;
    }

    static bool HandleVipStatusCommand(ChatHandler* handler, const char* args)
    {
        uint32 accountId;

        AccountTypes level = handler->GetSession()->GetSecurity();
        if (uint32(level) > 0)
        {
            if (!*args)
                return false;

            std::string accountName = strtok((char*)args, " ");
            // Fix?!
            uint32 accountid = AccountMgr::GetId(accountName);
            if (!AccountMgr::GetName(accountid, accountName))
            {
                handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            accountId = AccountMgr::GetId(accountName);
            if (!accountId)
            {
                handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
        else
            accountId = handler->GetSession()->GetAccountId();

        uint32 days = AccountMgr::VipDaysLeft(accountId);

        if (days > 0)
            handler->PSendSysMessage("Du hast keinen Elite Account.", days);
        else
            handler->PSendSysMessage("Dein Elite Account endet in Tagen.");

        return true;
    }

    static bool HandleTelesNameCommand(ChatHandler* handler, Optional<PlayerIdentifier> player, Variant<GameTele const*, EXACT_SEQUENCE("$home")> where)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!player)
            player = PlayerIdentifier::FromTargetOrSelf(handler);
        if (!player)
            return false;

        if (where.index() == 1)    // References target's homebind
        {
            if (Player* target = player->GetConnectedPlayer())
                target->TeleportTo(target->m_homebindMapId, target->m_homebindX, target->m_homebindY, target->m_homebindZ, target->GetOrientation());
            else
            {
                CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_HOMEBIND);
                stmt->setUInt32(0, player->GetGUID().GetCounter());
                PreparedQueryResult resultDB = CharacterDatabase.Query(stmt);

                if (resultDB)
                {
                    Field* fieldsDB = resultDB->Fetch();
                    WorldLocation loc(fieldsDB[0].GetUInt16(), fieldsDB[2].GetFloat(), fieldsDB[3].GetFloat(), fieldsDB[4].GetFloat(), 0.0f);
                    uint32 zoneId = fieldsDB[1].GetUInt16();

                    Player::SavePositionInDB(loc, zoneId, player->GetGUID(), nullptr);
                }
            }

            return true;
        }

        GameTele const* tele = where.get<GameTele const*>();
        if (Player* target = player->GetConnectedPlayer())

        {
            // check online security
            if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
                return false;

            std::string chrNameLink = handler->playerLink(target->GetName());

            if (target->IsBeingTeleported() == true)
            {
                handler->PSendSysMessage(LANG_IS_TELEPORTED, chrNameLink.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->PSendSysMessage(LANG_TELEPORTING_TO, chrNameLink.c_str(), "", tele->name.c_str());
            if (handler->needReportToTarget(target))
                ChatHandler(target->GetSession()).PSendSysMessage(LANG_TELEPORTED_TO_BY, handler->GetNameLink().c_str());

            // stop flight if need
            if (target->IsInFlight())
            {
                target->GetMotionMaster()->Clear();
                target->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                target->SaveRecallPosition();

            target->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
        }
        else
        {
            // check offline security
            if (handler->HasLowerSecurity(nullptr, player->GetGUID()))
                return false;

            std::string nameLink = handler->playerLink(player->GetName());

            handler->PSendSysMessage(LANG_TELEPORTING_TO, nameLink.c_str(), handler->GetTrinityString(LANG_OFFLINE), tele->name.c_str());

            Player::SavePositionInDB(WorldLocation(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation),
                sMapMgr->GetZoneId(PHASEMASK_NORMAL, tele->mapId, tele->position_x, tele->position_y, tele->position_z), player->GetGUID(), nullptr);
        }

        return true;
    }

    static bool HandlePremiumBankCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Command.Bank.Premium"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());
        return true;
    }

    static bool HandleVipResetTalentsCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Reset.Talents.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Reset Talents
        handler->GetSession()->GetPlayer()->ResetTalents(true);
        handler->GetSession()->GetPlayer()->SendTalentsInfoData(false);

        handler->PSendSysMessage(LANG_RESET_TALENTS_ONLINE, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
        return true;
    }
    //bag
    static bool HandlePremiumMailCommand(ChatHandler* handler)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Command.Mail.Premium"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }
        handler->GetSession()->SendShowMailBox(_player->GetGUID());
        return true;
    }

    static bool HandleVipjoinArathiCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BattlegroundTypeId bgTypeId = BATTLEGROUND_AB;
        handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
        //handler->PSendSysMessage(LANG_QUEUE_ARATHI);
        return true;
    }

    static bool HandleVipjoinEyeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BattlegroundTypeId bgTypeId = BATTLEGROUND_EY;
        handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
        return true;
    }

    static bool HandleVipjoinWarsongCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BattlegroundTypeId bgTypeId = BATTLEGROUND_WS;
        handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
        //handler->PSendSysMessage(LANG_QUEUE_WARSONG);
        return true;
    }

    static bool HandleVipjoinAlteracCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BattlegroundTypeId bgTypeId = BATTLEGROUND_AV;
        handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
        return true;
    }

    static bool HandleVipjoinArenaCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BattlegroundTypeId bgTypeId = BATTLEGROUND_AA;
        handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
        return true;
    }

    static bool HandleVipbuffsCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        {
            _player->Dismount();
            _player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            _player->AddAura(48161, _player);              // Power Word: Fortitude
            _player->AddAura(48073, _player);              // Divine Spirit
            _player->AddAura(20217, _player);              // Blessing of Kings
            _player->AddAura(48469, _player);              // Mark of the wild
            _player->AddAura(16609, _player);              // Spirit of Zandalar
            _player->AddAura(15366, _player);              // Songflower Serenade
            _player->AddAura(22888, _player);              // Rallying Cry of the Dragonslayer
            _player->AddAura(57399, _player);              // Well Fed
            _player->AddAura(17013, _player);              // Agamaggan's Agility
            _player->AddAura(16612, _player);              // Agamaggan's Strength
            _player->AddAura(24705, _player);
            _player->AddAura(26035, _player);
            _player->AddAura(31305, _player);
            _player->AddAura(36001, _player);
            _player->AddAura(70235, _player);
            _player->AddAura(70242, _player);
            _player->AddAura(70244, _player);
            _player->AddAura(30090, _player);
            _player->AddAura(30088, _player);
            _player->AddAura(30089, _player);
            _player->AddAura(131, _player);
            _player->AddAura(34906, _player);
            //_player->AddAura(53642, _player);
        }
        return true;
    }

    static bool HandleAppearCommand(ChatHandler* handler, const char* args)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Appear.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;


        if (target == _player || targetGuid == _player->GetGUID())
        {
            handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetGroup())
        {

            // check online security
            if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
                return false;

            std::string chrNameLink = handler->playerLink(targetName);

            Map* map = target->GetMap();
            if (target->IsInCombat())
            {
                handler->SendSysMessage(LANG_YOU_IN_COMBAT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->IsInFlight())
            {
                handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->GetMap()->IsBattlegroundOrArena())
            {
                handler->SendSysMessage(LANG_VIP_BG);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->HasStealthAura())
            {
                handler->SendSysMessage(LANG_VIP_STEALTH);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->isDead() || target->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
            {
                handler->SendSysMessage(LANG_VIP_DEAD);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (_player->GetGroup())
            {

                // we are in group, we can go only if we are in the player group
                if (_player->GetGroup() != target->GetGroup())
                {
                    handler->SendSysMessage(LANG_VIP_GROUP);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            // if the player or the player's group is bound to another instance
            // the player will not be bound to another one
            InstancePlayerBind* bind = _player->GetBoundInstance(target->GetMapId(), target->GetDifficulty(map->IsRaid()));
            if (!bind)
            {
                Group* group = _player->GetGroup();
                // if no bind exists, create a solo bind
                InstanceGroupBind* gBind = group ? group->GetBoundInstance(target) : nullptr;                // if no bind exists, create a solo bind
                if (!gBind)
                    if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(target->GetInstanceId()))
                        _player->BindToInstance(save, !save->CanReset());
            }

            if (map->IsRaid())
                _player->SetRaidDifficulty(target->GetRaidDifficulty());
            else
                _player->SetDungeonDifficulty(target->GetDungeonDifficulty());


            handler->PSendSysMessage(LANG_APPEARING_AT, chrNameLink.c_str());

            // stop flight if need
            if (_player->IsInFlight())
            {
                _player->GetMotionMaster()->Clear();
                _player->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                _player->SaveRecallPosition();
            // to point to see at target with same orientation
            float x, y, z;
            target->GetContactPoint(_player, x, y, z);
            _player->TeleportTo(target->GetMapId(), x, y, z, _player->GetAbsoluteAngle(target), TELE_TO_GM_MODE);
            _player->SetPhaseMask(target->GetPhaseMask(), true);
        }
        else
        {
            // check offline security
            if (handler->HasLowerSecurity(nullptr, targetGuid))
                return false;

            std::string nameLink = handler->playerLink(targetName);

            handler->SendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE);
            handler->SetSentErrorMessage(true);
            return false;

        }

        return true;
    }

    static bool HandleChangeRaceCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Changerace.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        _player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
        handler->SendSysMessage(LANG_VIP_CHANGE_RACE);
        return true;
    }

    static bool HandleCustomizeCommand(ChatHandler* handler, const char* /*args*/)
    {

        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Customize.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        _player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
        handler->SendSysMessage(LANG_VIP_CHANGE_CUSTOMIZE);
        return true;
    }

    static bool HandleVipTaxiCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Taxi.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        _player->SetTaxiCheater(true);
        handler->PSendSysMessage(LANG_YOU_GIVE_TAXIS, handler->GetNameLink(_player).c_str());
        if (handler->needReportToTarget(_player))
            ChatHandler(_player->GetSession()).PSendSysMessage(LANG_YOURS_TAXIS_ADDED, handler->GetNameLink().c_str());
        return true;
    }

    static bool HandleVipHomeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Home.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        _player->GetSpellHistory()->ResetCooldown(8690, true);
        _player->CastSpell(_player, 8690, false);
        return true;
    }

    static bool HandleVipDebuffCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Debuff.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->GetPlayer()->RemoveAurasDueToSpell(15007);
        handler->GetSession()->GetPlayer()->RemoveAurasDueToSpell(26013);

        return true;
    }

    static bool HandleVipMapCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_YOU_SET_EXPLORE_ALL, handler->GetNameLink(_player).c_str());
        for (uint8 i = 0; i < PLAYER_EXPLORED_ZONES_SIZE; ++i)
        {
            handler->GetSession()->GetPlayer()->SetFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);
        }

        return true;
    }

    static bool HandleVipRepairCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Repair.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->GetPlayer()->DurabilityRepairAll(false, 0, false);

        handler->PSendSysMessage(LANG_YOUR_ITEMS_REPAIRED, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
        return true;
    }

    static bool HandleVipCapitalCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sGameConfig->GetBoolConfig("Vip.Capital.Command"))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->isDead() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }
        //chr->SetPhaseMask(2, true);
        if (_player->GetTeam() == HORDE)
            _player->CastSpell(_player, 3567, true);
        else
            _player->CastSpell(_player, 3561, true);

        return true;
    }

    static bool HandleCoinCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();
        if (!_player)
            return false;

        Player* target = handler->getSelectedPlayerOrSelf();
        uint32 coins;

        if (handler->HasPermission(rbac::RBAC_PERM_COMMAND_ADDCOIN) && _player != target)
        {
            // GM should can to see the coins of target-player by .coin too
            coins = target->GetCoins();
            handler->PSendSysMessage("The AccountID %u [player : %s ] have %u coins", target->GetSession()->GetAccountId(), target->GetName(), coins);
        }
        else
        {
            coins = _player->GetCoins();
            handler->PSendSysMessage("You have the %u coins", coins);
        }

        return true;
    }

    static bool HandleCoinAddCommand(ChatHandler* handler, uint32 accountID, int32 coinAdded)
    {
        Player* target = handler->getSelectedPlayerOrSelf();

        if (!accountID)
            accountID = target->GetSession()->GetAccountId();

        if (coinAdded < 0)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 coinCount = coinAdded;
        coinCount += AccountMgr::GetCoins(accountID);
        ObjectGuid::LowType guid = AccountMgr::GetGuidOfOnlineCharacter(accountID);
        if (guid)
        {
            if (Player* p = ObjectAccessor::FindPlayerByLowGUID(guid))
                p->SetCoins(coinCount);
        }

        AccountMgr::SetCoins(accountID, coinCount);
        handler->PSendSysMessage("The AccountID %u has received %u coins, and now have a %u coins", accountID, coinAdded, coinCount);
        return true;
    }

    static bool HandleCoinDelCommand(ChatHandler* handler, uint32 accountID, int32 coinRemoved)
    {
        Player* target = handler->getSelectedPlayerOrSelf();
        if (!accountID)
            accountID = target->GetSession()->GetAccountId();

        if (coinRemoved < 0)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }
        uint32 coinCount = AccountMgr::GetCoins(accountID);

        if (coinCount < uint32(coinRemoved))
            coinCount = 0;
        else
            coinCount -= uint32(coinRemoved);

        ObjectGuid::LowType guid = AccountMgr::GetGuidOfOnlineCharacter(accountID);
        if (guid)
        {
            if (Player* p = ObjectAccessor::FindPlayerByLowGUID(guid))
                p->SetCoins(coinCount);
        }

        AccountMgr::SetCoins(accountID, coinCount);
        handler->PSendSysMessage("The AccountID %u has removed %u coins, and now have a %u coins", accountID, coinRemoved, coinCount);
        return true;
    }

};

void AddSC_premium_commandscript()
{
    new premium_commandscript();
}
