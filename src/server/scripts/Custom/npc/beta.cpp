#include "Config.h"
#include "GuildMgr.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "Group.h"
#include "AchievementMgr.h"
#include "ObjectAccessor.h"
#include "Unit.h"
#include "SharedDefines.h"
#include "Creature.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Language.h"
#include "Chat.h"
#include <sstream>
#include "SpellInfo.h"
#include "Channel.h"
#include "SpellMgr.h"
#include "Implementation/ZynDatabase.h"
#include "DatabaseEnvFwd.h"
#include "Log.h"

#define MSG_GOSSIP_TEXT_GETTING_STARTED	"Приветствуем вас на сервере"// OneGo - World of Warcraft !"

#define MSG_ERR_HONOR "У вас не достаточно хонора для совершения покупки!"
#define MSG_ERR_ARENA_POINT_1 "У вас не достаточно Апа!"
#define MSG_ERR_TITLE "У данного персонажа уже есть такое звание!"
#define MSG_ERR_INCOMBAT "Вы находитесь в бою. Чтобы использовать данного Npc выйдите из него."
#define MSG_COMPLETE_RENAME "Оплата Успешно Произведена Сделайте Логаут и Введите Новое Имя Персонажа.Не Забудьте После Смены Имени,Выйти Из Игры и Удалить Из Клиента Папку Cache!"
#define MSG_CUSTOMIZE_COMPLETE "Оплата Успешно Произведена Сделайте Логаут и Измените Внешность Персонажа.Не Забудьте После Смены Внешности,Выйти Из Игры и Удалить Из Клиента Папку Cache!"
#define MSG_CHANGE_FACTION_COMPLETE "Оплата Успешно Произведена Сделайте Логаут и Измените Фракцию Персонажа.Не Забудьте После Смены Внешности,Выйти Из Игры и Удалить Из Клиента Папку Cache!"

#define MSG_GOSSIP_TEXT_MAX_HEALTH "Вылечи меня!"
#define MSG_GOSSIP_TEXT_MAX_SKILL "Выучить все навыки оружия и защиты."

#define MSG_GOSSIP_TEXT_BUFF_POWER_WORD "[Бафнуть] Слово силы: Стойкость"
#define MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_KINGS "[Бафнуть] Благословение Королей"
#define MSG_GOSSIP_TEXT_BUFF_MARK_OF_THE_WILD "[Бафнуть] Знак дикой природы"
#define MSG_GOSSIP_TEXT_BUFF_ARCANE_BRILLIANCE "[Бафнуть] Чародейская гениальность Даларана"
#define MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_MIGHT "[Бафнуть] Великое Благословение могущества"
#define MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_WISDOM "[Бафнуть] Благословение мудрости"
#define MSG_GOSSIP_TEXT_BUFF_THORNS "[Бафнуть] Шипы"
#define MSG_GOSSIP_TEXT_BUFF_DIVINE_SPIRIT "[Бафнуть] Божественный дух"
#define MSG_GOSSIP_TEXT_BUFF_SHADOW_PROTECTION "[Бафнуть] Защита от темной магии"
#define MSG_GOSSIP_TEXT_BUFF_STAMINA "[Бафнуть] Выносливость"

#define MSG_GOSSIP_TEXT_BUFF_MENU "[Меню бафов] ->"
#define MSG_GOSSIP_TEXT_BUFF_MENUvip1 "[Premium buff] ->"
#define MSG_GOSSIP_TEXT_BUFF_MENUvip2 "[Title] ->"
#define MSG_GOSSIP_TEXT_BUFF_MENUvip3 "[test] ->"
#define MSG_GOSSIP_TEXT_BUFF_MENUvip33 "[titlestest] ->"
#define MSG_GOSSIP_TEXT_BUFF_MENUvip333 "[sumki] ->"
#define MSG_GOSSIP_TEXT_BUFF_MENUvip44 "[key] ->"
//#define MSG_GOSSIP_TEXT_BUFF_MENUvip55 "[kaput] ->"
#define MSG_GOSSIP_TEXT_MAIN_MENU "<- [Вернутся в Главное меню]"
#define MSG_GOSSIP_TEXT_PROFFESION_MENU "[Меню Профессий] ->"
#define MSG_GOSSIP_TEXT_PROFFESION_SECON_MENU "[Меню Второстепенных профессий] ->"

#define MSG_GOSSIP_TEXT_RENAME "Сменить Имя персонажа."
#define MSG_GOSSIP_TEXT_CUSTOM "Сменить Внешность персонажа."
#define MSG_GOSSIP_TEXT_CHANGE_FACTION "Сменить Фракцию персонажа."

#define MSG_GOSSIP_TEXT_SUPPER_BUFF_BERSERK "[Бафнуть] Берсерк."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_AEGIS_OF_RAGNAROS "[Бафнуть] Эгида Рагнароса."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_AEGIS_OF_NELTHARION "[Бафнуть] Защита Нелтариона."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_ADALS "[Бафнуть] Благословение А'далла."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_CRIT_SPELLS "[Бафнуть] Критическое срабатывание."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_PINCHI "[Бафнуть] Благословение мистера Пинчи."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_TRANSPARENCY "[Бафнуть] Прозрачность."

#define MSG_GOSSIP_TEXT_TITLES_JENKINS "Получить звание Дженкинс."
#define MSG_GOSSIP_TEXT_TITLES_THE_LOVE_FOOL "Получить звание Безумно Влюбленный."
#define MSG_GOSSIP_TEXT_TITLES_MERRYMAKER "Получить звание Весельчак."
#define MSG_GOSSIP_TEXT_TITLES_SCARAB_LORD "Получить звание Повелитель Скоробеев."
#define MSG_GOSSIP_TEXT_TITLES_THE_NOBLE "Получить звание Чудесный."
#define MSG_GOSSIP_TEXT_TITLES_OBSIDIAN_SLAYER "Получить звание Покоритель Обсидианового святилища."

#define MSG_GOSSIP_TEXT_MORTH_GNOME_MALE "[Превратить] Гном, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_GNOME_FEMALE "[Превратить] Гном, женщина."
#define MSG_GOSSIP_TEXT_MORTH_HUMAN_FEMALE "[Превратить] Человек, женщина."
#define MSG_GOSSIP_TEXT_MORTH_HUMAN_MALE "[Превратить] Человек, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE "[Превратить] Эльф Крови, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_FEMALE "[Превратить] Эльф Крови, женщина."
#define MSG_GOSSIP_TEXT_MORTH_TAUREN_MALE "[Превратить] Таурен, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_TAUREN_FEMALE "[Превратить] Таурен, женщина."

#define MSG_GOSSIP_TEXT_ALCHEMY "|TInterface\\icons\\Trade_Alchemy:40:40:-14|t Изучить Алхимию."
#define MSG_GOSSIP_TEXT_BLACKSMITHING "|TInterface\\icons\\Trade_BlackSmithing:40:40:-14|t Изучить Кузнечное Дело."
#define MSG_GOSSIP_TEXT_ENCNANTING "|TInterface\\icons\\Trade_Engraving:40:40:-14|t Изучить Наложение Чар."
#define MSG_GOSSIP_TEXT_ENGINEERING "|TInterface\\icons\\Trade_Engineering:40:40:-14|t Изучить Инженерное дело."
#define MSG_GOSSIP_TEXT_HERBALISM "|TInterface\\icons\\Trade_Herbalism:40:40:-14|t Изучить Травничество."
#define MSG_GOSSIP_TEXT_INSCRIPTION "|TInterface\\icons\\INV_Inscription_Tradeskill01:40:40:-14|t Изучить Начертание."
#define MSG_GOSSIP_TEXT_JEWELCRAFTING "|TInterface\\icons\\INV_Misc_Gem_02:40:40:-14|t Изучить Ювелирное дело."
#define MSG_GOSSIP_TEXT_LEATHERWORKING "|TInterface\\icons\\Trade_LeatherWorking:40:40:-14|t Изучить Кожевничество."
#define MSG_GOSSIP_TEXT_MINING "|TInterface\\icons\\Trade_Mining:40:40:-14|t Изучить Горное дело."
#define MSG_GOSSIP_TEXT_SKINNING "|TInterface\\icons\\INV_Misc_Pelt_Wolf_01:40:40:-14|t Изучить Снятие шкур."
#define MSG_GOSSIP_TEXT_TAILORING "|TInterface\\icons\\Trade_Tailoring:40:40:-14|t Изучить Портное дело."

#define MSG_GOSSIP_TEXT_RIDING "Изучить Верховую Езду."
#define MSG_GOSSIP_TEXT_COOKING	"|TInterface\\icons\\INV_Misc_Food_15:40:40:-14|t Изучить Кулинарию."
#define MSG_GOSSIP_TEXT_FIRST_AID "|TInterface\\icons\\Spell_Holy_SealOfSacrifice:40:40:-14|t Изучить Первую помощь."
#define MSG_GOSSIP_TEXT_FISHING	"Изучить Рыбную ловлю."

#define CONST_HONOR_1  0//1000000
#define CONST_HONOR_2 0 //100000
#define CONST_ARENA_POINT_1  0//500
#define CONST_ARENA_POINT_2  0//1000
#define CONST_ARENA_POINT_3  0//3000
#define CONST_ARENA_POINT_4  0//5000

class npc_buffer : public CreatureScript
{
public:
	npc_buffer() : CreatureScript("npc_buffer") { }

	struct npc_bufferAI : public ScriptedAI
	{
		npc_bufferAI(Creature* me) : ScriptedAI(me) { }

	void CompleteLearnProfession(Player *player, Creature* /*_creature*/, SkillType skill)
	{
		if (PlayerAlreadyHasNineProfessions(player) && !IsSecondarySkill(skill))
			me->Whisper("Вы уже выучили 11 професии!", LANG_UNIVERSAL, player);
		else
		{
			if (!LearnAllRecipesInProfession(player, skill))
				me->Whisper("Ошибка!", LANG_UNIVERSAL, player);
		}
	}

	bool IsSecondarySkill(SkillType skill) const
	{
		return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
	}

	bool PlayerAlreadyHasNineProfessions(const Player *pPlayer) const
	{
		uint32 skillCount = 0;

		if (pPlayer->HasSkill(SKILL_MINING))
			skillCount++;
		if (pPlayer->HasSkill(SKILL_SKINNING))
			skillCount++;
		if (pPlayer->HasSkill(SKILL_HERBALISM))
			skillCount++;

		if (skillCount >= 11)
			return true;

		for (uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
		{
			SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
			if (!SkillInfo)
				continue;

			if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
				continue;

			if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
				continue;

			const uint32 skillID = SkillInfo->id;
			if (pPlayer->HasSkill(skillID))
				skillCount++;

			if (skillCount >= 11)
				return true;
		}

		return false;
	}

	bool LearnAllRecipesInProfession(Player *player, SkillType skill)
	{
		ChatHandler handler(player->GetSession());
		char* skill_name;

		SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
		skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];

		if (!SkillInfo)
		{
			TC_LOG_ERROR("scripts", "Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
			return false;
		}

		LearnSkillRecipesHelper(player, SkillInfo->id);

		uint16 maxLevel = player->GetPureMaxSkillValue(SkillInfo->id);
		player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), maxLevel, maxLevel);
		handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);

		return true;
	}
	// See "static void HandleLearnSkillRecipesHelper(Player* player,uint32 skill_id)" from cs_learn.cpp
	void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
	{
		uint32 classmask = player->getClassMask();

		for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
		{
			SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
			if (!skillLine)
				continue;

			// wrong skill
			if (skillLine->skillId != skill_id)
				continue;

			// not high rank
			if (skillLine->forward_spellid)
				continue;

			// skip racial skills
			if (skillLine->racemask != 0)
				continue;

			// skip wrong class skills
			if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
				continue;

			SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
			if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
				continue;

			player->LearnSpell(skillLine->spellId, false);
		}
	}

	bool GossipHello(Player* player) override
	{
		me->Whisper(MSG_GOSSIP_TEXT_GETTING_STARTED, LANG_UNIVERSAL, player);
		AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_RIDING, GOSSIP_SENDER_MAIN, 35);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MAX_HEALTH, GOSSIP_SENDER_MAIN, 33);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MAX_SKILL, GOSSIP_SENDER_MAIN, 34);
		AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENU, GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip1, GOSSIP_SENDER_MAIN, 13);
        AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip2, GOSSIP_SENDER_MAIN, 22);
		AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip3, GOSSIP_SENDER_MAIN, 2233);
		AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip33, GOSSIP_SENDER_MAIN, 22333);
		AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip333, GOSSIP_SENDER_MAIN, 32333);
		AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip44, GOSSIP_SENDER_MAIN, 32344);
		//AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip55, GOSSIP_SENDER_MAIN, 32355);
		AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_PROFFESION_MENU, GOSSIP_SENDER_MAIN, 36);
		SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
		return true;
	}

	bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
	{
		uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
		uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
		return OnGossipSelect(player, sender, action);
	}

	bool OnGossipSelect(Player* player, uint32 /*sender*/, uint32 action)
	{
		if (!player->getAttackers().empty())
		{
			me->Whisper(MSG_ERR_INCOMBAT, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		switch (action)
		{
		case 2:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_POWER_WORD, GOSSIP_SENDER_MAIN, 3);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_KINGS, GOSSIP_SENDER_MAIN, 4);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_MARK_OF_THE_WILD, GOSSIP_SENDER_MAIN, 5);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_ARCANE_BRILLIANCE, GOSSIP_SENDER_MAIN, 6);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_MIGHT, GOSSIP_SENDER_MAIN, 7);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_WISDOM, GOSSIP_SENDER_MAIN, 8);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_THORNS, GOSSIP_SENDER_MAIN, 9);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_DIVINE_SPIRIT, GOSSIP_SENDER_MAIN, 10);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_SHADOW_PROTECTION, GOSSIP_SENDER_MAIN, 11);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_STAMINA, GOSSIP_SENDER_MAIN, 12);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 3:
			// Наложение стойкости (Исправлено)
			player->CastSpell(player, 69377, true);
			CloseGossipMenuFor(player);
			break;
		case 4:
			// Наложение Благословение Королей (Исправлено)
			player->CastSpell(player, 56525, true);
			CloseGossipMenuFor(player);
			break;
		case 5:
			// Наложение Знак дикой природы
			player->CastSpell(player, 1126, true);
			CloseGossipMenuFor(player);
			break;
		case 6:
			// Наложение Чародейской гениальности Даларана (Исправлено)
			player->CastSpell(player, 61316, true);
			CloseGossipMenuFor(player);
			break;
		case 7:
			// Наложение Благословение могущества (Исправлено)
			player->CastSpell(player, 29381, true);
			CloseGossipMenuFor(player);
			break;
		case 8:
			// Наложение Благословение мудрости (Исправлено)
			player->CastSpell(player, 56521, true);
			CloseGossipMenuFor(player);
			break;
		case 9:
			// Наложение Шипов (Исправлено)
			player->CastSpell(player, 53307, true);
			CloseGossipMenuFor(player);
			break;
		case 10:
			// Наложение Божественный духа (Исправлено)
			player->CastSpell(player, 48074, true);
			CloseGossipMenuFor(player);
			break;
		case 11:
			// Наложение Защиты от темной магии (Исправлено)
			player->CastSpell(player, 48170, true);
			CloseGossipMenuFor(player);
			break;
		case 12:
			player->CastSpell(player, 48102, true);
			CloseGossipMenuFor(player);
			break;
		case 13:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_BERSERK, GOSSIP_SENDER_MAIN, 14);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_AEGIS_OF_RAGNAROS, GOSSIP_SENDER_MAIN, 15);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_AEGIS_OF_NELTHARION, GOSSIP_SENDER_MAIN, 16);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_ADALS, GOSSIP_SENDER_MAIN, 17);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_CRIT_SPELLS, GOSSIP_SENDER_MAIN, 18);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_PINCHI, GOSSIP_SENDER_MAIN, 19);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_TRANSPARENCY, GOSSIP_SENDER_MAIN, 20);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 14:
			if (player->GetHonorPoints() < CONST_HONOR_1)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение берсерка и отнятие 1 ляма хонора (Доработано)
				player->CastSpell(player, 41924, true);
				player->ModifyHonorPoints(-CONST_HONOR_1);
				CloseGossipMenuFor(player);
			}
			break;
		case 15:
			if (player->GetHonorPoints() < CONST_HONOR_2)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение Эгиды Рагнароса(Доработано)
				player->CastSpell(player, 20620, true);
				player->ModifyHonorPoints(-CONST_HONOR_2);
				CloseGossipMenuFor(player);
			}
			break;
		case 16:
			if (player->GetHonorPoints() < CONST_HONOR_2)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение Защита Нелтариона(Доработано)
				player->CastSpell(player, 51512, true);
				player->ModifyHonorPoints(-CONST_HONOR_2);
				CloseGossipMenuFor(player);
			}
			break;
		case 17:
			if (player->GetHonorPoints() < CONST_HONOR_2)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение Благословение А'дала
				player->CastSpell(player, 35076, true);
				player->ModifyHonorPoints(-CONST_HONOR_2);
				CloseGossipMenuFor(player);
			}
			break;
		case 18:
			if (player->GetHonorPoints() < CONST_HONOR_2)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение Критического срабатывания положительных эффектов
				player->CastSpell(player, 31305, true);
				player->ModifyHonorPoints(-CONST_HONOR_2);
				CloseGossipMenuFor(player);
			}
			break;
		case 19:
			if (player->GetHonorPoints() < CONST_HONOR_2)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение Благословения мистера Пинчи
				player->CastSpell(player, 33053, true);
				player->ModifyHonorPoints(-CONST_HONOR_2);
				CloseGossipMenuFor(player);
			}
			break;
		case 20:
			if (player->GetHonorPoints() < CONST_HONOR_2)
			{
				me->Whisper(MSG_ERR_HONOR, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Наложение Прозрачности
				player->CastSpell(player, 37802, true);
				player->ModifyHonorPoints(-CONST_HONOR_2);
				CloseGossipMenuFor(player);
		case 21:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_RIDING, GOSSIP_SENDER_MAIN, 35);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MAX_HEALTH, GOSSIP_SENDER_MAIN, 33);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MAX_SKILL, GOSSIP_SENDER_MAIN, 34);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENU, GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip1, GOSSIP_SENDER_MAIN, 13);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip2, GOSSIP_SENDER_MAIN, 22);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip3, GOSSIP_SENDER_MAIN, 2233);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip33, GOSSIP_SENDER_MAIN, 22333);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip333, GOSSIP_SENDER_MAIN, 32333);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip44, GOSSIP_SENDER_MAIN, 32344);
			//AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENUvip55, GOSSIP_SENDER_MAIN, 32355);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_PROFFESION_MENU, GOSSIP_SENDER_MAIN, 36);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
        //case 32355:
        //player->CastSpell(player, 30110, true);
        //player->CastSpell(player, 30113, true);
        //player->CastSpell(player, 30127, true);
        //player->CastSpell(player, 53094, true);
        //player->CastSpell(player, 53317, true);
			//CloseGossipMenuFor(player);
        //break;
        case 32344:
          player->AddItem(6893, 1);
          player->AddItem(7146, 1);
          player->AddItem(11000, 1);
          player->AddItem(12382, 1);
          player->AddItem(13704, 1);
          player->AddItem(18249, 1);
          player->AddItem(24490, 1);
          player->AddItem(27991, 1);
          player->AddItem(28395, 1);
          player->AddItem(30633, 1);
          player->AddItem(30635, 1);
          player->AddItem(31084, 1);
          player->AddItem(38555, 1);
          player->AddItem(42482, 1);
          player->AddItem(43650, 1);
          player->AddItem(44581, 1);
          player->AddItem(44582, 1);
          player->AddItem(45796, 1);
        break;
        case 32333:
          if ((player->getClass() == CLASS_HUNTER))
           {
             player->AddItem(38082, 3);
             player->AddItem(44448, 1);
             player->AddItem(44447, 1);
             return true;
           }
          else
           {
                player->AddItem(38082, 4);
                return true;
            }
            break;
		case 22333:
			player->PlayerTalkClass->ClearMenus();
			//AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_JENKINS, GOSSIP_SENDER_MAIN, 23);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "1", GOSSIP_SENDER_MAIN, 22334);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "2", GOSSIP_SENDER_MAIN, 22335);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "3", GOSSIP_SENDER_MAIN, 22336);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "4", GOSSIP_SENDER_MAIN, 22337);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "5", GOSSIP_SENDER_MAIN, 22338);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "6", GOSSIP_SENDER_MAIN, 22339);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "7", GOSSIP_SENDER_MAIN, 22340);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "8", GOSSIP_SENDER_MAIN, 22341);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "9", GOSSIP_SENDER_MAIN, 22342);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 22334:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(178);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22335:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(179);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22336:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(180);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22337:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(181);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22338:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(182);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22339:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(183);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22340:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(184);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22341:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(185);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 22342:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(186);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else {
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;

		case 2233:
			player->PlayerTalkClass->ClearMenus();
			//AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_JENKINS, GOSSIP_SENDER_MAIN, 23);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Algalon", GOSSIP_SENDER_MAIN, 711);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Doomwalker", GOSSIP_SENDER_MAIN, 712);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Doomlord kazzak", GOSSIP_SENDER_MAIN, 713);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Onyxia", GOSSIP_SENDER_MAIN, 714);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Thaddius", GOSSIP_SENDER_MAIN, 715);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Gluth", GOSSIP_SENDER_MAIN, 716);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Maexxna", GOSSIP_SENDER_MAIN, 717);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Sapphiron", GOSSIP_SENDER_MAIN, 718);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Noth The Plaguebringer", GOSSIP_SENDER_MAIN, 719);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Kel'Thuzad", GOSSIP_SENDER_MAIN, 720);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Kael'thas Sunstrider", GOSSIP_SENDER_MAIN, 721);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "C'thun", GOSSIP_SENDER_MAIN, 722);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Thrall", GOSSIP_SENDER_MAIN, 723);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Sylvanas Windrunner", GOSSIP_SENDER_MAIN, 724);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Alexstrasza (Blutelf)", GOSSIP_SENDER_MAIN, 725);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Varian Wrynn", GOSSIP_SENDER_MAIN, 726);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Varimathras", GOSSIP_SENDER_MAIN, 727);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Alexstrasza (Drache)", GOSSIP_SENDER_MAIN, 728);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, "Mal'ganis", GOSSIP_SENDER_MAIN, 729);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 22:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_JENKINS, GOSSIP_SENDER_MAIN, 23);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_THE_LOVE_FOOL, GOSSIP_SENDER_MAIN, 24);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_MERRYMAKER, GOSSIP_SENDER_MAIN, 25);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_SCARAB_LORD, GOSSIP_SENDER_MAIN, 26);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_THE_NOBLE, GOSSIP_SENDER_MAIN, 27);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_TITLES_OBSIDIAN_SLAYER, GOSSIP_SENDER_MAIN, 28);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
       case 711: // Algalon
            CloseGossipMenuFor(player);
            player->SetDisplayId(28641);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
            break;
       case 712: // Doomwalker
            CloseGossipMenuFor(player);
            player->SetDisplayId(16630);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
            break;
       case 713: // Doomlord kazzak
            CloseGossipMenuFor(player);
            player->SetDisplayId(17887);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.10f);
            break;
       case 714: // Onyxia
            CloseGossipMenuFor(player);
            player->SetDisplayId(8570);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
            break;
       case 715: // Thaddius
            CloseGossipMenuFor(player);
            player->SetDisplayId(16137);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
            break;
       case 716: // Gluth
            CloseGossipMenuFor(player);
            player->SetDisplayId(16064);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
            break;
       case 717: // Maexxna
            CloseGossipMenuFor(player);
            player->SetDisplayId(15928);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.15f);
            break;
       case 718: // Sapphiron
            CloseGossipMenuFor(player);
            player->SetDisplayId(16033);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
            break;
       case 719: // Noth The Plaguebringer
            CloseGossipMenuFor(player);
            player->SetDisplayId(16590);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
            break;
       case 720: // Kel'Thuzad
			CloseGossipMenuFor(player);
            player->SetDisplayId(15945);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
            break;
       case 721: // Kael'thas Sunstrider
            CloseGossipMenuFor(player);
            player->SetDisplayId(20023);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
            break;
	   case 722: // C'thun
            CloseGossipMenuFor(player);
            player->SetDisplayId(15786);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
            break;
       case 723: // Thrall
            CloseGossipMenuFor(player);
            player->SetDisplayId(4527);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
            break;
       case 724: // Sylvanas Windrunner
            CloseGossipMenuFor(player);
            player->SetDisplayId(28213);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
            break;
		case 725: // Alexstrasza, BElf
            CloseGossipMenuFor(player);
            player->SetDisplayId(28227);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.8f);
            break;
       case 726: // Varian Wrynn
            CloseGossipMenuFor(player);
            player->SetDisplayId(28127);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
            break;
       case 727: // Varimathras
            CloseGossipMenuFor(player);
            player->SetDisplayId(28220);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
            break;
       case 728: // Alexstrasza, dragon
            CloseGossipMenuFor(player);
            player->SetDisplayId(27569);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
            break;
       case 729: // Mal'ganis
            CloseGossipMenuFor(player);
            player->SetDisplayId(26582);
            player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
            break;
		case 23:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(143);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1)
		{
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else{
			// Даем звание Дженкинс
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 24:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(135);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1)
		{
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else{
			// Даем звание Безумно Влюбленный
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 25:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(134);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else{
			// Даем звание Весельчак
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 26:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(46);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else{
			// Даем звание Повелитель Скоробеев
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 27:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(155);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else{
			// Даем звание Чудесный
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 28:
		{CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(139);
		if (player->HasTitle(titleInfo))
		{
			me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}
		if (player->GetArenaPoints() < CONST_ARENA_POINT_1) {
			me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
		}
		else{
			// Даем звание Покоритель Обсидианового святилища
			player->SetTitle(titleInfo);
			player->ModifyArenaPoints(-CONST_ARENA_POINT_2);
			CloseGossipMenuFor(player);
		}
		}
		break;
		case 29:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_RENAME, GOSSIP_SENDER_MAIN, 30);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_CUSTOM, GOSSIP_SENDER_MAIN, 31);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_CHANGE_FACTION, GOSSIP_SENDER_MAIN, 32);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 30:
			if (player->GetArenaPoints() < CONST_ARENA_POINT_3)
			{
				me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Делаем Ренейм
				player->SetAtLoginFlag(AT_LOGIN_RENAME);
				player->ModifyArenaPoints(-CONST_ARENA_POINT_3);
				me->Whisper(MSG_COMPLETE_RENAME, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			break;
		case 31:
			if (player->GetArenaPoints() < CONST_ARENA_POINT_3)
			{
				me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Делаем Смену внешности
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
				player->ModifyArenaPoints(-CONST_ARENA_POINT_3);
				me->Whisper(MSG_CUSTOMIZE_COMPLETE, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			break;
		case 32:
			if (player->GetArenaPoints() < CONST_ARENA_POINT_4)
			{
				me->Whisper(MSG_ERR_ARENA_POINT_1, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else{
				// Делаем Смену внешности
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
				player->ModifyArenaPoints(-CONST_ARENA_POINT_4);
				me->Whisper(MSG_CHANGE_FACTION_COMPLETE, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			break;
		case 33:
			player->SetHealth(player->GetMaxHealth());
			CloseGossipMenuFor(player);

			break;
		case 34:
			player->UpdateWeaponsSkillsToMaxSkillsForLevel();
			CloseGossipMenuFor(player);
			break;
		case 35:
			player->LearnSpell(54197, true);
            player->LearnSpell(34093, true);
            player->LearnSpell(34092, true);
            player->LearnSpell(33392, true);
            player->LearnSpell(33389, true);
			CloseGossipMenuFor(player);
			break;
		case 36:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ALCHEMY, GOSSIP_SENDER_MAIN, 37);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BLACKSMITHING, GOSSIP_SENDER_MAIN, 38);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENCNANTING, GOSSIP_SENDER_MAIN, 39);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENGINEERING, GOSSIP_SENDER_MAIN, 40);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_HERBALISM, GOSSIP_SENDER_MAIN, 41);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_INSCRIPTION, GOSSIP_SENDER_MAIN, 42);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_JEWELCRAFTING, GOSSIP_SENDER_MAIN, 43);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_LEATHERWORKING, GOSSIP_SENDER_MAIN, 44);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_MINING, GOSSIP_SENDER_MAIN, 45);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_SKINNING, GOSSIP_SENDER_MAIN, 46);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_TAILORING, GOSSIP_SENDER_MAIN, 47);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_PROFFESION_SECON_MENU, GOSSIP_SENDER_MAIN, 48);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
		   SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 37:
			CompleteLearnProfession(player, me, SKILL_ALCHEMY);
			CloseGossipMenuFor(player);
			break;
		case 38:
			CompleteLearnProfession(player, me, SKILL_BLACKSMITHING);
			CloseGossipMenuFor(player);
			break;
		case 39:
			CompleteLearnProfession(player, me, SKILL_ENCHANTING);
			CloseGossipMenuFor(player);
			break;
		case 40:
			CompleteLearnProfession(player, me, SKILL_ENGINEERING);
			CloseGossipMenuFor(player);
			break;
		case 41:
			CompleteLearnProfession(player, me, SKILL_HERBALISM);
			CloseGossipMenuFor(player);
                        break;
		case 42:
			CompleteLearnProfession(player, me, SKILL_INSCRIPTION);
			CloseGossipMenuFor(player);
			break;
		case 43:
			CompleteLearnProfession(player, me, SKILL_JEWELCRAFTING);
			CloseGossipMenuFor(player);
			break;
		case 44:
			CompleteLearnProfession(player, me, SKILL_LEATHERWORKING);
			CloseGossipMenuFor(player);
			break;
		case 45:
			CompleteLearnProfession(player, me, SKILL_MINING);
			CloseGossipMenuFor(player);
			break;
		case 46:
			CompleteLearnProfession(player, me, SKILL_SKINNING);
			CloseGossipMenuFor(player);
			break;
		case 47:
			CompleteLearnProfession(player, me, SKILL_TAILORING);
			CloseGossipMenuFor(player);
			break;
		case 48:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_COOKING, GOSSIP_SENDER_MAIN, 49);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_FIRST_AID, GOSSIP_SENDER_MAIN, 50);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_FISHING, GOSSIP_SENDER_MAIN, 51);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 49:
			CompleteLearnProfession(player, me, SKILL_COOKING);
			CloseGossipMenuFor(player);
			break;
		case 50:
			CompleteLearnProfession(player, me, SKILL_FIRST_AID);
			CloseGossipMenuFor(player);
			break;
		case 51:
			CompleteLearnProfession(player, me, SKILL_FISHING);
			CloseGossipMenuFor(player);
			break;
		case 52:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_GNOME_MALE, GOSSIP_SENDER_MAIN, 53);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_GNOME_FEMALE, GOSSIP_SENDER_MAIN, 54);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_HUMAN_MALE, GOSSIP_SENDER_MAIN, 55);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_HUMAN_FEMALE, GOSSIP_SENDER_MAIN, 56);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE, GOSSIP_SENDER_MAIN, 57);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE, GOSSIP_SENDER_MAIN, 58);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_TAUREN_MALE, GOSSIP_SENDER_MAIN, 59);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_TAUREN_FEMALE, GOSSIP_SENDER_MAIN, 50);
			AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
			break;
		case 53:
			player->CastSpell(player, 37808, true);
			CloseGossipMenuFor(player);
			break;
		case 54:
			player->CastSpell(player, 37809, true);
			CloseGossipMenuFor(player);
			break;
		case 55:
			player->CastSpell(player, 35466, true);
			CloseGossipMenuFor(player);
			break;
		case 56:
			player->CastSpell(player, 37805, true);
			CloseGossipMenuFor(player);
			break;
		case 57:
			player->CastSpell(player, 37807, true);
			CloseGossipMenuFor(player);
			break;
		case 58:
			player->CastSpell(player, 37806, true);
			CloseGossipMenuFor(player);
			break;
		case 59:
			player->CastSpell(player, 37810, true);
			CloseGossipMenuFor(player);
			break;
		case 60:
			player->CastSpell(player, 37811, true);
			CloseGossipMenuFor(player);
			break;
		case 61:
			player->RemoveAllAuras();
			CloseGossipMenuFor(player);
			}
		}
		return true;
		}
	};

	CreatureAI* GetAI(Creature* me) const override
	{
		return new npc_bufferAI(me);
	}
};

class channel_factions : public PlayerScript
{
public:
	channel_factions() : PlayerScript("channel_factions") {}

	void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel)
	{
		if (!player || !channel)
			return;

		std::stringstream ssMsg;
		ssMsg << ((player->GetTeam() == HORDE) ? "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18:0:-1|t" : "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18:0:-1|t") << msg;
		msg = ssMsg.str();
	}
};

void AddSC_Resets()
{
	new npc_buffer();
	new channel_factions();
}
