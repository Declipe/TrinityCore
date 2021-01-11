#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "ArenaTeamMgr.h"
#include "CellImpl.h"
#include "CharacterCache.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DisableMgr.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "GroupMgr.h"
#include "InstanceSaveMgr.h"
#include "Item.h"
#include "Language.h"
#include "LFG.h"
#include "Log.h"
#include "MapManager.h"
#include "MMapFactory.h"
#include "MovementGenerator.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "Realm.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "ChaseMovementGenerator.h"
#include "FollowMovementGenerator.h"
#include "Transport.h"
#include "Weather.h"
#include "WeatherMgr.h"
#include "World.h"
#include "WorldSession.h"
#include <boost/asio/ip/address_v4.hpp>

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
			//{ "qcomplete", rbac::RBAC_PERM_COMMAND_VIP_qcomplete, false, &HandleQuestCompletes, "" },
		};

		static ChatCommandTable commandTable =
		{
			{ "vip", premiumCommandTable },
		};

		return commandTable;
	}

	static bool HandleVipStatusCommand(ChatHandler* handler, char const* args)
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
	/* static bool HandleQuestCompletes(ChatHandler* handler, char const* args)
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

		Player* player = handler->getSelectedPlayerOrSelf();
		if (!player)
		{
			handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
			handler->SetSentErrorMessage(true);
			return false;
		}

		// .quest complete #entry
		// number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
		char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
		if (!cId)
			return false;

		uint32 entry = atoul(cId);

		Quest const* quest = sObjectMgr->GetQuestTemplate(entry);

		// If player doesn't have the quest
		if (!quest || player->GetQuestStatus(entry) == QUEST_STATUS_NONE)
		{
			handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
			handler->SetSentErrorMessage(true);
			return false;
		}

		// Add quest items for quests that require items
		for (uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
		{
			uint32 id = quest->RequiredItemId[x];
			uint32 count = quest->RequiredItemCount[x];
			if (!id || !count)
				continue;

			uint32 curItemCount = player->GetItemCount(id, true);

			ItemPosCountVec dest;
			uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count - curItemCount);
			if (msg == EQUIP_ERR_OK)
			{
				Item* item = player->StoreNewItem(dest, id, true);
				player->SendNewItem(item, count - curItemCount, true, false);
			}
		}

		// All creature/GO slain/cast (not required, but otherwise it will display "Creature slain 0/10")
		for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
		{
			int32 creature = quest->RequiredNpcOrGo[i];
			uint32 creatureCount = quest->RequiredNpcOrGoCount[i];

			if (creature > 0)
			{
				if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creature))
					for (uint16 z = 0; z < creatureCount; ++z)
						player->KilledMonster(creatureInfo, ObjectGuid::Empty);
			}
			else if (creature < 0)
				for (uint16 z = 0; z < creatureCount; ++z)
					player->KillCreditGO(creature);
		}

		// If the quest requires reputation to complete
		if (uint32 repFaction = quest->GetRepObjectiveFaction())
		{
			uint32 repValue = quest->GetRepObjectiveValue();
			uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
			if (curRep < repValue)
				if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
					player->GetReputationMgr().SetReputation(factionEntry, repValue);
		}

		// If the quest requires a SECOND reputation to complete
		if (uint32 repFaction = quest->GetRepObjectiveFaction2())
		{
			uint32 repValue2 = quest->GetRepObjectiveValue2();
			uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
			if (curRep < repValue2)
				if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
					player->GetReputationMgr().SetReputation(factionEntry, repValue2);
		}

		// If the quest requires money
		int32 ReqOrRewMoney = quest->GetRewOrReqMoney();
		if (ReqOrRewMoney < 0)
			player->ModifyMoney(-ReqOrRewMoney);

		if (sWorld->getBoolConfig(CONFIG_QUEST_ENABLE_QUEST_TRACKER)) // check if Quest Tracker is enabled
		{
			// prepare Quest Tracker datas
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_QUEST_TRACK_GM_COMPLETE);
			stmt->setUInt32(0, quest->GetQuestId());
			stmt->setUInt32(1, player->GetGUID().GetCounter());

			// add to Quest Tracker
			CharacterDatabase.Execute(stmt);
		}

		player->CompleteQuest(entry);
		return true;
	}*/

	static bool HandleTelesNameCommand(ChatHandler* handler, Optional<PlayerIdentifier> player, Variant<GameTele const*, EXACT_SEQUENCE("$home")> where)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		//if (!sWorld->getBoolConfig(COMMAND_BANK_PREMIUM))
		//{
		//	handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
		//	handler->SetSentErrorMessage(true);
		//	return false;
		//}

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

	static bool HandlePremiumBankCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(COMMAND_BANK_PREMIUM))
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

	static bool HandleVipResetTalentsCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player *_player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_RESET_TALENTS_COMMAND))
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
	static bool HandlePremiumMailCommand(ChatHandler* handler, char const* args)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(COMMAND_MAIL_PREMIUM))
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

	static bool HandleVipjoinArathiCommand(ChatHandler* handler, char const* /*args*/)
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

	static bool HandleVipjoinEyeCommand(ChatHandler* handler, char const* /*args*/)
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
			//handler->PSendSysMessage(LANG_QUEUE_EYE);
		return true;
	}

	static bool HandleVipjoinWarsongCommand(ChatHandler* handler, char const* /*args*/)
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

	static bool HandleVipjoinAlteracCommand(ChatHandler* handler, char const* /*args*/)
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

	static bool HandleVipjoinArenaCommand(ChatHandler* handler, char const* /*args*/)
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

	static bool HandleVipbuffsCommand(ChatHandler* handler, char const* /*args*/)
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
			_player->AddAura(53642, _player);
		}
		return true;
	}

	static bool HandleAppearCommand(ChatHandler* handler, char const* args)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_APPEAR_COMMAND))
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

	static bool HandleChangeRaceCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_CHANGERACE_COMMAND))
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

	static bool HandleCustomizeCommand(ChatHandler* handler, char const* /*args*/)
	{

		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_CUSTOMIZE_COMMAND))
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

	static bool HandleVipTaxiCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_RESET_TALENTS_COMMAND))
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

	static bool HandleVipHomeCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_HOME_COMMAND))
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

	static bool HandleVipDebuffCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_DEBUFF_COMMAND))
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

	static bool HandleVipMapCommand(ChatHandler* handler, char const* /*args*/)
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
		for (uint8 i = 0; i<PLAYER_EXPLORED_ZONES_SIZE; ++i)
		{
			handler->GetSession()->GetPlayer()->SetFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);
		}

		return true;
	}

	static bool HandleVipRepairCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_REPAIR_COMMAND))
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

	static bool HandleVipCapitalCommand(ChatHandler* handler, char const* /*args*/)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (!handler->GetSession()->IsPremium())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!sWorld->getBoolConfig(CONFIG_VIP_CAPITAL_COMMAND))
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

};

void AddSC_premium_commandscript()
{
	new premium_commandscript();
}
