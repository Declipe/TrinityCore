/*
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('100005', '0', '0', '0', '0', '0', '18', '0', '18', '0', 'npc title', 'title master', '', '0', '59', '61', '0', '35', '35', '1', '1.48', '1.14286', '0.0', '0', '655.0', '663.0', '0', '158', '1.0', '1500', '1900', '1', '0', '0', '0', '0', '0', '0', '0', '0.0', '0.0', '100', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '1', '3', '1.0', '1.0', '1.0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', 'npc_title', '1');
*/

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

#define MSG_GOSSIP_TEXT_GETTING_STARTED	"Приветствуем вас на сервере!"

#define MSG_ERR_ARENA_POINT "У вас не достаточно Апа!"
#define MSG_ERR_TITLE "У данного персонажа уже есть такое звание!"
#define MSG_ERR_INCOMBAT "Вы находитесь в бою. Чтобы использовать данного Npc выйдите из него."

#define MSG_GOSSIP_TEXT_NEXT_2 "[Вторая страница] ->"
#define MSG_GOSSIP_TEXT_NEXT_3 "[Третья страница] ->"
#define MSG_GOSSIP_TEXT_NEXT_4 "[Четвертая страница] ->"
#define MSG_GOSSIP_TEXT_NEXT_5 "[Пятая страница] ->"

#define MSG_GOSSIP_TEXT_BACK_4 "<- [Четвертая страница] "
#define MSG_GOSSIP_TEXT_BACK_3 "<- [Третья страница] "
#define MSG_GOSSIP_TEXT_BACK_2 "<- [Вторая страница] "
#define MSG_GOSSIP_TEXT_BACK_1 "<- [Первая страница] "

#define MSG_GOSSIP_TEXT_144 "Получить звание Адмирал Кровавого Паруса"
#define MSG_GOSSIP_TEXT_62 "Получить звание Безжалостный гладиатор"
#define MSG_GOSSIP_TEXT_135 "Получить звание Безумно влюбленный"
#define MSG_GOSSIP_TEXT_141 "Получить звание Бессмертный"
#define MSG_GOSSIP_TEXT_138 "Получить звание Благодетель"
#define MSG_GOSSIP_TEXT_170 "Получить звание Великий крестоносец"
#define MSG_GOSSIP_TEXT_28 "Получить звание Верховный вождь"
#define MSG_GOSSIP_TEXT_48 "Получить звание Вершитель правосудия"
#define MSG_GOSSIP_TEXT_134 "Получить звание Весельчак"
#define MSG_GOSSIP_TEXT_72 "Получить звание Военачальник"
#define MSG_GOSSIP_TEXT_42 "Получить звание Гладиатор"
#define MSG_GOSSIP_TEXT_113 "Получить звание Гномреганский"
#define MSG_GOSSIP_TEXT_147 "Получить звание Дарнасский"
#define MSG_GOSSIP_TEXT_143 "Получить звание Дженкинс"
#define MSG_GOSSIP_TEXT_79 "Получить звание Дипломат"
#define MSG_GOSSIP_TEXT_64 "Получить звание Длань А'дала"
#define MSG_GOSSIP_TEXT_43 "Получить звание Дуэлянт"
#define MSG_GOSSIP_TEXT_80 "Получить звание Жестокий Гладиатор"
#define MSG_GOSSIP_TEXT_47 "Получить звание Завоеватель"
#define MSG_GOSSIP_TEXT_122 "Получить звание Завоеватель Наксрамаса"
#define MSG_GOSSIP_TEXT_81 "Получить звание Искатель"
#define MSG_GOSSIP_TEXT_78 "Получить звание Исследователь"
#define MSG_GOSSIP_TEXT_156 "Получить звание Крестоносец"
#define MSG_GOSSIP_TEXT_83 "Получить звание Морской дьявол"
#define MSG_GOSSIP_TEXT_71 "Получить звание Мстительный гладиатор"
#define MSG_GOSSIP_TEXT_142 "Получить звание Неумирающий"
#define MSG_GOSSIP_TEXT_146 "Получить звание Неумолимый гладиатор"
#define MSG_GOSSIP_TEXT_150 "Получить звание Оргриммарский"
#define MSG_GOSSIP_TEXT_46 "Получить звание Повелитель Скарабеев"
#define MSG_GOSSIP_TEXT_278 "Получить звание Повелитель Огня"
#define MSG_GOSSIP_TEXT_139 "Получить звание Покоритель Обсидианового святилища"
#define MSG_GOSSIP_TEXT_77 "Получить звание Превозносимый"
#define MSG_GOSSIP_TEXT_74 "Получить звание Премудрый"
#define MSG_GOSSIP_TEXT_45 "Получить звание Претендент"
#define MSG_GOSSIP_TEXT_177 "Получить звание Разгневанный гладиатор"
#define MSG_GOSSIP_TEXT_151 "Получить звание Сен'джинский"
#define MSG_GOSSIP_TEXT_171 "Получить звание Серебряный заступник"
#define MSG_GOSSIP_TEXT_131 "Получить звание Серебряный защитник"
#define MSG_GOSSIP_TEXT_173 "Получить звание Сияние Рассвета"
#define MSG_GOSSIP_TEXT_157 "Получить звание Смертоносный гладиатор"
#define MSG_GOSSIP_TEXT_75 "Получить звание Страж огня"
#define MSG_GOSSIP_TEXT_140 "Получить звание Сумеречный"
#define MSG_GOSSIP_TEXT_172 "Получить звание Терпеливый"
#define MSG_GOSSIP_TEXT_124 "Получить звание Тыквер"
#define MSG_GOSSIP_TEXT_133 "Получить звание Хмелевар"
#define MSG_GOSSIP_TEXT_125 "Получить звание Хранитель мудрости"
#define MSG_GOSSIP_TEXT_76 "Получить звание Хранитель огня"
#define MSG_GOSSIP_TEXT_145 "Получить звание Чокнутый"
#define MSG_GOSSIP_TEXT_155 "Получить звание Чудесный"
#define MSG_GOSSIP_TEXT_84 "Получить звание Шеф-повар"

#define CONST_ARENA_POINT 200

class npc_title : public CreatureScript
{
public:
    npc_title() : CreatureScript("npc_title") { }

    struct npc_titleAI : public ScriptedAI
    {

        npc_titleAI(Creature* me) : ScriptedAI(me) { }

        bool AddTitle(Player* player, CharTitlesEntry const* titleInfo)
        {
            if (player->HasTitle(titleInfo)) {
                me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                return false;
            }

            if (player->GetArenaPoints() < CONST_ARENA_POINT) {
                //me->Whisper(MSG_ERR_ARENA_POINT, player->GetGUID()); 
                me->Whisper(MSG_ERR_ARENA_POINT, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);

            }
            else {

                player->SetTitle(titleInfo);
                player->ModifyArenaPoints(-CONST_ARENA_POINT);
                CloseGossipMenuFor(player);
            }
            return true;
        }

        bool OnGossipHello(Player* player) override
        {
            me->Whisper(MSG_GOSSIP_TEXT_GETTING_STARTED, LANG_UNIVERSAL, player);
            player->PlayerTalkClass->ClearMenus();
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_144, GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_62, GOSSIP_SENDER_MAIN, 3);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_135, GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_141, GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_138, GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_170, GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_28, GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_48, GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_134, GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_72, GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_2, GOSSIP_SENDER_MAIN, 12);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;

        }

        bool OnGossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            return GossipSelect(player, sender, action);
        }

        bool GossipSelect(Player* player, uint32 sender, uint32 action)
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
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(144);
            AddTitle(player, titleInfo);
            }
            break;
            case 3:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(62);
            AddTitle(player, titleInfo);
            }
            break;
            case 4:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(135);
            AddTitle(player, titleInfo);
            }
            break;
            case 5:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(141);
            AddTitle(player, titleInfo);
            }
            break;
            case 6:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(138);
            AddTitle(player, titleInfo);
            }
            break;
            case 7:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(170);
            AddTitle(player, titleInfo);
            }
            break;
            case 8:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(28);
            AddTitle(player, titleInfo);
            }
            break;
            case 9:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(48);
            AddTitle(player, titleInfo);
            }
            break;
            case 10:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(134);
            AddTitle(player, titleInfo);
            }
            break;
            case 11:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(72);
            AddTitle(player, titleInfo);
            }
            break;
            case 12:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_42, GOSSIP_SENDER_MAIN, 13);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_113, GOSSIP_SENDER_MAIN, 14);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_147, GOSSIP_SENDER_MAIN, 15);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_143, GOSSIP_SENDER_MAIN, 16);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_79, GOSSIP_SENDER_MAIN, 17);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_64, GOSSIP_SENDER_MAIN, 18);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_43, GOSSIP_SENDER_MAIN, 19);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_80, GOSSIP_SENDER_MAIN, 20);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_47, GOSSIP_SENDER_MAIN, 21);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_122, GOSSIP_SENDER_MAIN, 22);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BACK_1, GOSSIP_SENDER_MAIN, 56);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_3, GOSSIP_SENDER_MAIN, 23);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 13:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(42);
            AddTitle(player, titleInfo);
            }
            break;
            case 14:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(113);
            AddTitle(player, titleInfo);
            }
            break;
            case 15:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(147);
            AddTitle(player, titleInfo);
            }
            break;
            case 16:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(143);
            AddTitle(player, titleInfo);
            }
            break;
            case 17:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(79);
            AddTitle(player, titleInfo);
            }
            break;
            case 18:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(64);
            AddTitle(player, titleInfo);
            }
            break;
            case 19:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(43);
            AddTitle(player, titleInfo);
            }
            break;
            case 20:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(80);
            AddTitle(player, titleInfo);
            }
            break;
            case 21:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(47);
            AddTitle(player, titleInfo);
            }
            break;
            case 22:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(122);
            AddTitle(player, titleInfo);
            }
            break;
            case 23:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_81, GOSSIP_SENDER_MAIN, 24);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_78, GOSSIP_SENDER_MAIN, 25);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_156, GOSSIP_SENDER_MAIN, 26);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_83, GOSSIP_SENDER_MAIN, 27);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_71, GOSSIP_SENDER_MAIN, 28);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_142, GOSSIP_SENDER_MAIN, 29);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_146, GOSSIP_SENDER_MAIN, 30);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_150, GOSSIP_SENDER_MAIN, 31);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_46, GOSSIP_SENDER_MAIN, 32);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_278, GOSSIP_SENDER_MAIN, 33);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BACK_2, GOSSIP_SENDER_MAIN, 57);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_4, GOSSIP_SENDER_MAIN, 34);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 24:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(81);
            AddTitle(player, titleInfo);
            }
            break;
            case 25:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(78);
            AddTitle(player, titleInfo);
            }
            break;
            case 26:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(156);
            AddTitle(player, titleInfo);
            }
            break;
            case 27:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(83);
            AddTitle(player, titleInfo);
            }
            break;
            case 28:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(71);
            AddTitle(player, titleInfo);
            }
            break;
            case 29:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(142);
            AddTitle(player, titleInfo);
            }
            break;
            case 30:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(146);
            AddTitle(player, titleInfo);
            }
            break;
            case 31:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(150);
            AddTitle(player, titleInfo);
            }
            break;
            case 32:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(46);
            AddTitle(player, titleInfo);
            }
            break;
            case 33:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(278);
            AddTitle(player, titleInfo);
            }
            break;
            case 34:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_139, GOSSIP_SENDER_MAIN, 35);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_77, GOSSIP_SENDER_MAIN, 36);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_74, GOSSIP_SENDER_MAIN, 37);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_45, GOSSIP_SENDER_MAIN, 38);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_177, GOSSIP_SENDER_MAIN, 39);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_151, GOSSIP_SENDER_MAIN, 40);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_171, GOSSIP_SENDER_MAIN, 41);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_131, GOSSIP_SENDER_MAIN, 42);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_173, GOSSIP_SENDER_MAIN, 43);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_157, GOSSIP_SENDER_MAIN, 44);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BACK_3, GOSSIP_SENDER_MAIN, 58);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_5, GOSSIP_SENDER_MAIN, 45);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 35:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(139);
            AddTitle(player, titleInfo);
            }
            break;
            case 36:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(77);
            AddTitle(player, titleInfo);
            }
            break;
            case 37:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(74);
            AddTitle(player, titleInfo);
            }
            break;
            case 38:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(45);
            AddTitle(player, titleInfo);
            }
            break;
            case 39:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(177);
            AddTitle(player, titleInfo);
            }
            break;
            case 40:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(151);
            AddTitle(player, titleInfo);
            }
            break;
            case 41:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(171);
            AddTitle(player, titleInfo);
            }
            break;
            case 42:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(131);
            AddTitle(player, titleInfo);
            }
            break;
            case 43:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(173);
            AddTitle(player, titleInfo);
            }
            break;
            case 44:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(157);
            AddTitle(player, titleInfo);
            }
            break;
            case 45:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_75, GOSSIP_SENDER_MAIN, 46);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_140, GOSSIP_SENDER_MAIN, 47);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_172, GOSSIP_SENDER_MAIN, 48);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_124, GOSSIP_SENDER_MAIN, 49);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_133, GOSSIP_SENDER_MAIN, 50);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_76, GOSSIP_SENDER_MAIN, 51);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_125, GOSSIP_SENDER_MAIN, 52);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_145, GOSSIP_SENDER_MAIN, 53);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_155, GOSSIP_SENDER_MAIN, 54);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_84, GOSSIP_SENDER_MAIN, 55);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BACK_4, GOSSIP_SENDER_MAIN, 59);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 46:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(75);
            AddTitle(player, titleInfo);
            }
            break;
            case 47:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(140);
            AddTitle(player, titleInfo);
            }
            break;
            case 48:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(172);
            AddTitle(player, titleInfo);
            }
            break;
            case 49:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(124);
            AddTitle(player, titleInfo);
            }
            break;
            case 50:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(133);
            AddTitle(player, titleInfo);
            }
            break;
            case 51:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(125);
            AddTitle(player, titleInfo);
            }
            break;
            case 52:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(76);
            AddTitle(player, titleInfo);
            }
            break;
            case 53:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(145);
            AddTitle(player, titleInfo);
            }
            break;
            case 54:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(155);
            AddTitle(player, titleInfo);
            }
            break;
            case 55:
            {CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(84);
            AddTitle(player, titleInfo);
            }
            break;
            case 56:
                OnGossipHello(player);
                break;
            case 57:
                OnGossipSelect(player, sender, 12);
                break;
            case 58:
                OnGossipSelect(player, sender, 23);
                break;
            case 59:
                OnGossipSelect(player, sender, 34);
                break;
            }

            return true;
        }
    };

    CreatureAI* GetAI(Creature* me) const override
    {
        return new npc_titleAI(me);
    }
};
void AddSC_npc_title()
{
    new npc_title;
}
