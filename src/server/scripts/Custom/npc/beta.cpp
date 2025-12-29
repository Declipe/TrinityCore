#include "Custom/Dcl.h"

int32 CONST_HONOR_1 = 0;//1000000
int32 CONST_HONOR_2 = 0; //100000
int32 CONST_ARENA_POINT_1 = 0;//500
int32 CONST_ARENA_POINT_2 = 0;//1000
int32 CONST_ARENA_POINT_3 = 0;//3000
int32 CONST_ARENA_POINT_4 = 0;//5000
int32 CONST_HONOR_23 = 29434;
int32 CONST_HONOR_233 = 0;
uint32 proff = 12;

    struct npc_buffer : public ScriptedAI
    {
        npc_buffer(Creature* me) : ScriptedAI(me) { }

        void CompleteLearnProfession(Player* player, Creature* creature, SkillType skill)
        {
            if (PlayerAlreadyHasNineProfessions(player) && !IsSecondarySkill(skill))
                creature->Whisper("12proff!", LANG_UNIVERSAL, player);
            else
            {
                if (!LearnAllRecipesInProfession(player, skill))
                    creature->Whisper("Error!", LANG_UNIVERSAL, player);
            }
        }

        bool IsSecondarySkill(SkillType skill) const
        {
            return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
        }

        bool PlayerAlreadyHasNineProfessions(const Player* pPlayer) const
        {
            uint32 skillCount = 0;

            if (pPlayer->HasSkill(SKILL_MINING))
                skillCount++;
            if (pPlayer->HasSkill(SKILL_SKINNING))
                skillCount++;
            if (pPlayer->HasSkill(SKILL_HERBALISM))
                skillCount++;

            if (skillCount >= proff)
                return true;

            for (uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
            {
                SkillLineEntry const* SkillInfo = sSkillLineStore.LookupEntry(i);
                if (!SkillInfo)
                    continue;

                if (SkillInfo->CategoryID == SKILL_CATEGORY_SECONDARY)
                    continue;

                if ((SkillInfo->CategoryID != SKILL_CATEGORY_PROFESSION) || !SkillInfo->CanLink)
                    continue;

                const uint32 skillID = SkillInfo->ID;
                if (pPlayer->HasSkill(skillID))
                    skillCount++;

                if (skillCount >= proff)
                    return true;
            }

            return false;
        }

        bool LearnAllRecipesInProfession(Player* player, SkillType skill)
        {
            ChatHandler handler(player->GetSession());
            //char* skill_name;
            char const* skill_name = nullptr;

            SkillLineEntry const* SkillInfo = sSkillLineStore.LookupEntry(skill);
            skill_name = SkillInfo->DisplayName[handler.GetSessionDbcLocale()];

            if (!SkillInfo)
            {
                //  TC_LOG_ERROR("scripts", "Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
                return false;
            }

            LearnSkillRecipesHelper(player, SkillInfo->ID);

            uint16 maxLevel = player->GetPureMaxSkillValue(SkillInfo->ID);
            player->SetSkill(SkillInfo->ID, player->GetSkillStep(SkillInfo->ID), maxLevel, maxLevel);
            handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);

            return true;
        }
        // See "static void HandleLearnSkillRecipesHelper(Player* player,uint32 skill_id)" from cs_learn.cpp
        void LearnSkillRecipesHelper(Player* player, uint32 skill_id)
        {
            uint32 classmask = player->GetClassMask();

            for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
            {
                SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
                if (!skillLine)
                    continue;

                // wrong skill
                if (skillLine->SkillLine != skill_id)
                    continue;

                // not high rank
                if (skillLine->SupercededBySpell)
                    continue;

                // skip racial skills
                if (skillLine->RaceMask != 0)
                    continue;

                // skip wrong class skills
                if (skillLine->ClassMask && (skillLine->ClassMask & classmask) == 0)
                    continue;

                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->Spell);
                if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
                    continue;

                player->LearnSpell(skillLine->Spell, false);
            }
        }

        bool OnGossipHello(Player* player) override
        {
            LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

            me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_1, loc_idx), LANG_UNIVERSAL, player);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_68, loc_idx), GOSSIP_SENDER_MAIN, 35);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_9, loc_idx), GOSSIP_SENDER_MAIN, 33);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_10, loc_idx), GOSSIP_SENDER_MAIN, 34);
            //AddGossipItemFor(player, NPC_buba, 0, loc_idx), GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_21, loc_idx), GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_23, loc_idx), GOSSIP_SENDER_MAIN, 22);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_24, loc_idx), GOSSIP_SENDER_MAIN, 2233);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_26, loc_idx), GOSSIP_SENDER_MAIN, 32333);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_27, loc_idx), GOSSIP_SENDER_MAIN, 32344);
            if (sGameConfig->GetBoolConfig("CONFIG.Test"))
            {
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_22, loc_idx), GOSSIP_SENDER_MAIN, 13);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_25, loc_idx), GOSSIP_SENDER_MAIN, 22333);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_28, loc_idx), GOSSIP_SENDER_MAIN, 32355);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_30, loc_idx), GOSSIP_SENDER_MAIN, 36);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
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
            LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

            if (!player->getAttackers().empty())
            {
                me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_5, loc_idx), LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                return false;
            }

            switch (action)
            {
            case 2:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_11, loc_idx), GOSSIP_SENDER_MAIN, 3);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_12, loc_idx), GOSSIP_SENDER_MAIN, 4);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_13, loc_idx), GOSSIP_SENDER_MAIN, 5);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_14, loc_idx), GOSSIP_SENDER_MAIN, 6);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_15, loc_idx), GOSSIP_SENDER_MAIN, 7);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_16, loc_idx), GOSSIP_SENDER_MAIN, 8);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_17, loc_idx), GOSSIP_SENDER_MAIN, 9);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_18, loc_idx), GOSSIP_SENDER_MAIN, 10);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_19, loc_idx), GOSSIP_SENDER_MAIN, 11);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_20, loc_idx), GOSSIP_SENDER_MAIN, 12);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 3:
                // ????????? ????????? (??????????)
                player->CastSpell(player, 69377, true);
                CloseGossipMenuFor(player);
                break;
            case 4:
                // ????????? ????????????? ??????? (??????????)
                player->CastSpell(player, 56525, true);
                CloseGossipMenuFor(player);
                break;
            case 5:
                // ????????? ???? ????? ???????
                player->CastSpell(player, 1126, true);
                CloseGossipMenuFor(player);
                break;
            case 6:
                // ????????? ??????????? ???????????? ???????? (??????????)
                player->CastSpell(player, 61316, true);
                CloseGossipMenuFor(player);
                break;
            case 7:
                // ????????? ????????????? ?????????? (??????????)
                player->CastSpell(player, 29381, true);
                CloseGossipMenuFor(player);
                break;
            case 8:
                // ????????? ????????????? ???????? (??????????)
                player->CastSpell(player, 56521, true);
                CloseGossipMenuFor(player);
                break;
            case 9:
                // ????????? ????? (??????????)
                player->CastSpell(player, 53307, true);
                CloseGossipMenuFor(player);
                break;
            case 10:
                // ????????? ???????????? ???? (??????????)
                player->CastSpell(player, 48074, true);
                CloseGossipMenuFor(player);
                break;
            case 11:
                // ????????? ?????? ?? ?????? ????? (??????????)
                player->CastSpell(player, 48170, true);
                CloseGossipMenuFor(player);
                break;
            case 12:
                player->CastSpell(player, 48102, true);
                CloseGossipMenuFor(player);
                break;
            case 13:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_36, loc_idx), GOSSIP_SENDER_MAIN, 14);
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_37, loc_idx), GOSSIP_SENDER_MAIN, 15);
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_38, loc_idx), GOSSIP_SENDER_MAIN, 16);
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_39, loc_idx), GOSSIP_SENDER_MAIN, 17);
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_40, loc_idx), GOSSIP_SENDER_MAIN, 18);
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_41, loc_idx), GOSSIP_SENDER_MAIN, 19);
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_42, loc_idx), GOSSIP_SENDER_MAIN, 20);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 14:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_1))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ????????? ???????? ? ??????? 1 ???? ?????? (??????????)
                    player->CastSpell(player, 41924, true);
                    player->ModifyHonorPoints(-CONST_HONOR_1);
                    CloseGossipMenuFor(player);
                }
                break;
            case 15:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_2))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ????????? ????? ?????????(??????????)
                    player->CastSpell(player, 20620, true);
                    player->ModifyHonorPoints(-CONST_HONOR_2);
                    CloseGossipMenuFor(player);
                }
                break;
            case 16:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_2))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ????????? ?????? ??????????(??????????)
                    player->CastSpell(player, 51512, true);
                    player->ModifyHonorPoints(-CONST_HONOR_2);
                    CloseGossipMenuFor(player);
                }
                break;
            case 17:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_2))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ????????? ????????????? ?'????
                    player->CastSpell(player, 35076, true);
                    player->ModifyHonorPoints(-CONST_HONOR_2);
                    CloseGossipMenuFor(player);
                }
                break;
            case 18:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_2))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ????????? ???????????? ???????????? ????????????? ????????
                    player->CastSpell(player, 31305, true);
                    player->ModifyHonorPoints(-CONST_HONOR_2);
                    CloseGossipMenuFor(player);
                }
                break;
            case 19:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_2))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ????????? ????????????? ??????? ?????
                    player->CastSpell(player, 33053, true);
                    player->ModifyHonorPoints(-CONST_HONOR_2);
                    CloseGossipMenuFor(player);
                }
                break;
            case 20:
                if (player->GetHonorPoints() < unsigned(CONST_HONOR_2))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_2, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ????????? ????????????
                    player->CastSpell(player, 37802, true);
                    player->ModifyHonorPoints(-CONST_HONOR_2);
                    CloseGossipMenuFor(player);
                }
                break;
            case 21:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_68, loc_idx), GOSSIP_SENDER_MAIN, 35);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_9, loc_idx), GOSSIP_SENDER_MAIN, 33);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_10, loc_idx), GOSSIP_SENDER_MAIN, 34);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_21, loc_idx), GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_23, loc_idx), GOSSIP_SENDER_MAIN, 22);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_24, loc_idx), GOSSIP_SENDER_MAIN, 2233);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_26, loc_idx), GOSSIP_SENDER_MAIN, 32333);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_27, loc_idx), GOSSIP_SENDER_MAIN, 32344);
                if (sGameConfig->GetBoolConfig("CONFIG.Test"))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_22, loc_idx), GOSSIP_SENDER_MAIN, 13);
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_25, loc_idx), GOSSIP_SENDER_MAIN, 22333);
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_28, loc_idx), GOSSIP_SENDER_MAIN, 32355);
                }
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_30, loc_idx), GOSSIP_SENDER_MAIN, 36);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 32355:
                player->CastSpell(player, 30110, true);
                player->CastSpell(player, 30113, true);
                player->CastSpell(player, 30127, true);
                player->CastSpell(player, 53094, true);
                player->CastSpell(player, 53317, true);
                CloseGossipMenuFor(player);
                break;
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
                CloseGossipMenuFor(player);
                break;
            case 32333:
                if ((player->GetClass() == CLASS_HUNTER))
                {
                    player->AddItem(38082, 3);
                    player->AddItem(44448, 1);
                    player->AddItem(44447, 1);
                    CloseGossipMenuFor(player);
                    return true;
                }
                else
                {
                    player->AddItem(38082, 4);
                    CloseGossipMenuFor(player);
                    return true;
                }
                break;
            case 22333:
                player->PlayerTalkClass->ClearMenus();
                //AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, LANG_GOSSIP_OPTION_43, loc_idx), GOSSIP_SENDER_MAIN, 23);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_72, loc_idx), GOSSIP_SENDER_MAIN, 22334);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_73, loc_idx), GOSSIP_SENDER_MAIN, 22335);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_74, loc_idx), GOSSIP_SENDER_MAIN, 22336);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_75, loc_idx), GOSSIP_SENDER_MAIN, 22337);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_76, loc_idx), GOSSIP_SENDER_MAIN, 22338);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_77, loc_idx), GOSSIP_SENDER_MAIN, 22339);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_78, loc_idx), GOSSIP_SENDER_MAIN, 22340);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_79, loc_idx), GOSSIP_SENDER_MAIN, 22341);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_80, loc_idx), GOSSIP_SENDER_MAIN, 22342);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 22334:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(178);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22335:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(179);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22336:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(180);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22337:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(181);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22338:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(182);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22339:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(183);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22340:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(184);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22341:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(185);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 22342:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(186);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;

            case 2233:
                player->PlayerTalkClass->ClearMenus();
                //AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, LANG_GOSSIP_OPTION_43, loc_idx), GOSSIP_SENDER_MAIN, 23);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_81, loc_idx), GOSSIP_SENDER_MAIN, 711);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_82, loc_idx), GOSSIP_SENDER_MAIN, 712);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_83, loc_idx), GOSSIP_SENDER_MAIN, 713);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_84, loc_idx), GOSSIP_SENDER_MAIN, 714);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_85, loc_idx), GOSSIP_SENDER_MAIN, 715);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_86, loc_idx), GOSSIP_SENDER_MAIN, 716);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_87, loc_idx), GOSSIP_SENDER_MAIN, 717);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_88, loc_idx), GOSSIP_SENDER_MAIN, 718);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_89, loc_idx), GOSSIP_SENDER_MAIN, 719);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_90, loc_idx), GOSSIP_SENDER_MAIN, 720);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_91, loc_idx), GOSSIP_SENDER_MAIN, 721);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_92, loc_idx), GOSSIP_SENDER_MAIN, 722);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_93, loc_idx), GOSSIP_SENDER_MAIN, 723);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_94, loc_idx), GOSSIP_SENDER_MAIN, 724);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_95, loc_idx), GOSSIP_SENDER_MAIN, 725);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_96, loc_idx), GOSSIP_SENDER_MAIN, 726);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_97, loc_idx), GOSSIP_SENDER_MAIN, 727);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_98, loc_idx), GOSSIP_SENDER_MAIN, 728);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_99, loc_idx), GOSSIP_SENDER_MAIN, 729);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_32, loc_idx), GOSSIP_SENDER_MAIN, 748);
                //AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 22:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_43, loc_idx), GOSSIP_SENDER_MAIN, 23);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_44, loc_idx), GOSSIP_SENDER_MAIN, 24);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_45, loc_idx), GOSSIP_SENDER_MAIN, 25);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_46, loc_idx), GOSSIP_SENDER_MAIN, 26);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_47, loc_idx), GOSSIP_SENDER_MAIN, 27);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_48, loc_idx), GOSSIP_SENDER_MAIN, 28);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 711:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(30721);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.8f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 712:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(12432);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 713:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(17887);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.10f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 714:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(20964);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 715:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(16137);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 716:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(16064);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 717:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(15928);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.15f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 718:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(16033);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 719:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(16590);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 720:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(15945);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.3f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 721:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(20023);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 722:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(15786);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 723:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(4527);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 724:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(28213);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 725:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(28227);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.9f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 726:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(28127);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 727:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(28220);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.6f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 728:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(27569);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 729:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(23177);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 730:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(26623);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 731:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(25286);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 732:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(27511);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 733:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(27573);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.8f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 734:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(27537);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 735:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(12818);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 736:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(25241);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.6f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 737:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(775);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 738:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(29099);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 739:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(29114);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.3f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 740:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(11650);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 741:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(17035);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 742:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(6351);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 743:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(18038);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 744:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(18058);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 745:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(18916);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.3f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 746:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(17715);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 747:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_81, loc_idx), GOSSIP_SENDER_MAIN, 711);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_82, loc_idx), GOSSIP_SENDER_MAIN, 712);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_83, loc_idx), GOSSIP_SENDER_MAIN, 713);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_84, loc_idx), GOSSIP_SENDER_MAIN, 714);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_85, loc_idx), GOSSIP_SENDER_MAIN, 715);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_86, loc_idx), GOSSIP_SENDER_MAIN, 716);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_87, loc_idx), GOSSIP_SENDER_MAIN, 717);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_88, loc_idx), GOSSIP_SENDER_MAIN, 718);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_89, loc_idx), GOSSIP_SENDER_MAIN, 719);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_90, loc_idx), GOSSIP_SENDER_MAIN, 720);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_91, loc_idx), GOSSIP_SENDER_MAIN, 721);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_92, loc_idx), GOSSIP_SENDER_MAIN, 722);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_93, loc_idx), GOSSIP_SENDER_MAIN, 723);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_94, loc_idx), GOSSIP_SENDER_MAIN, 724);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_95, loc_idx), GOSSIP_SENDER_MAIN, 725);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_96, loc_idx), GOSSIP_SENDER_MAIN, 726);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_97, loc_idx), GOSSIP_SENDER_MAIN, 727);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_98, loc_idx), GOSSIP_SENDER_MAIN, 728);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_99, loc_idx), GOSSIP_SENDER_MAIN, 729);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_32, loc_idx), GOSSIP_SENDER_MAIN, 748);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 748:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_100, loc_idx), GOSSIP_SENDER_MAIN, 730);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_101, loc_idx), GOSSIP_SENDER_MAIN, 731);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_102, loc_idx), GOSSIP_SENDER_MAIN, 732);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_103, loc_idx), GOSSIP_SENDER_MAIN, 733);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_104, loc_idx), GOSSIP_SENDER_MAIN, 734);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_105, loc_idx), GOSSIP_SENDER_MAIN, 735);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_106, loc_idx), GOSSIP_SENDER_MAIN, 736);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_107, loc_idx), GOSSIP_SENDER_MAIN, 737);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_108, loc_idx), GOSSIP_SENDER_MAIN, 738);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_109, loc_idx), GOSSIP_SENDER_MAIN, 739);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_110, loc_idx), GOSSIP_SENDER_MAIN, 740);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_111, loc_idx), GOSSIP_SENDER_MAIN, 741);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_112, loc_idx), GOSSIP_SENDER_MAIN, 742);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_113, loc_idx), GOSSIP_SENDER_MAIN, 743);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_114, loc_idx), GOSSIP_SENDER_MAIN, 744);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_115, loc_idx), GOSSIP_SENDER_MAIN, 745);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_116, loc_idx), GOSSIP_SENDER_MAIN, 746);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_32, loc_idx), GOSSIP_SENDER_MAIN, 749);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 747);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 749:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_117, loc_idx), GOSSIP_SENDER_MAIN, 750);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_118, loc_idx), GOSSIP_SENDER_MAIN, 751);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_119, loc_idx), GOSSIP_SENDER_MAIN, 752);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_120, loc_idx), GOSSIP_SENDER_MAIN, 753);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_121, loc_idx), GOSSIP_SENDER_MAIN, 754);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_122, loc_idx), GOSSIP_SENDER_MAIN, 755);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_123, loc_idx), GOSSIP_SENDER_MAIN, 756);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_124, loc_idx), GOSSIP_SENDER_MAIN, 757);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_125, loc_idx), GOSSIP_SENDER_MAIN, 758);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_126, loc_idx), GOSSIP_SENDER_MAIN, 759);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_127, loc_idx), GOSSIP_SENDER_MAIN, 760);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_128, loc_idx), GOSSIP_SENDER_MAIN, 761);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_32, loc_idx), GOSSIP_SENDER_MAIN, 748);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 750:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(18314);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 751:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(19687);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 752:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(17695);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 753:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(17052);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 754:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(22124);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 755:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(20046);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.3f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 756:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(19586);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 757:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(22786);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 758:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(22976);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 759:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(16042);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 760:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(16540);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 761:
                if (player->GetItemCount(CONST_HONOR_23) < unsigned(CONST_HONOR_233))
                {
                    me->Yell(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_130, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    player->SetDisplayId(19329);
                    player->DestroyItemCount(CONST_HONOR_23, CONST_HONOR_233, true);
                    player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
                    CloseGossipMenuFor(player);
                }
                break;
            case 23:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(143);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ???? ?????? ????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 24:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(135);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ???? ?????? ??????? ??????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 25:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(134);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ???? ?????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 26:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(46);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ???? ?????? ?????????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 27:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(155);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1)) {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ???? ?????? ????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_1);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 28:
            {
                CharTitlesEntry const* titleInfo = sDBCMgr->GetCharTitlesEntry(139);
                if (player->HasTitle(titleInfo))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_4, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                    return false;
                }
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_1))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ???? ?????? ?????????? ????????????? ?????????
                    player->SetTitle(titleInfo);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_2);
                    CloseGossipMenuFor(player);
                }
            }
            break;
            case 29:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_33, loc_idx), GOSSIP_SENDER_MAIN, 30);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_34, loc_idx), GOSSIP_SENDER_MAIN, 31);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_35, loc_idx), GOSSIP_SENDER_MAIN, 32);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 30:
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_3))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ?????? ??????
                    player->SetAtLoginFlag(AT_LOGIN_RENAME);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_3);
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_6, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                break;
            case 31:
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_3))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ?????? ????? ?????????
                    player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_3);
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_7, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                break;
            case 32:
                if (player->GetArenaPoints() < unsigned(CONST_ARENA_POINT_4))
                {
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_3, loc_idx), LANG_UNIVERSAL, player);
                    CloseGossipMenuFor(player);
                }
                else
                {
                    // ?????? ????? ?????????
                    player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                    player->ModifyArenaPoints(-CONST_ARENA_POINT_4);
                    me->Whisper(sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_8, loc_idx), LANG_UNIVERSAL, player);
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
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_57, loc_idx), GOSSIP_SENDER_MAIN, 37);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_58, loc_idx), GOSSIP_SENDER_MAIN, 38);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_59, loc_idx), GOSSIP_SENDER_MAIN, 39);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_60, loc_idx), GOSSIP_SENDER_MAIN, 40);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_61, loc_idx), GOSSIP_SENDER_MAIN, 41);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_62, loc_idx), GOSSIP_SENDER_MAIN, 42);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_63, loc_idx), GOSSIP_SENDER_MAIN, 43);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_64, loc_idx), GOSSIP_SENDER_MAIN, 44);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_65, loc_idx), GOSSIP_SENDER_MAIN, 45);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_66, loc_idx), GOSSIP_SENDER_MAIN, 46);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_67, loc_idx), GOSSIP_SENDER_MAIN, 47);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_31, loc_idx), GOSSIP_SENDER_MAIN, 48);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 37:
                CompleteLearnProfession(player, me, SKILL_ALCHEMY);
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 38:
                CompleteLearnProfession(player, me, SKILL_BLACKSMITHING);
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 39:
                CompleteLearnProfession(player, me, SKILL_ENCHANTING);
                CloseGossipMenuFor(player);
                break;
            case 40:
                CompleteLearnProfession(player, me, SKILL_ENGINEERING);
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 41:
                CompleteLearnProfession(player, me, SKILL_HERBALISM);
                player->AddItem(40772, 1);
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
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 45:
                CompleteLearnProfession(player, me, SKILL_MINING);
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 46:
                CompleteLearnProfession(player, me, SKILL_SKINNING);
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 47:
                CompleteLearnProfession(player, me, SKILL_TAILORING);
                player->AddItem(40772, 1);
                CloseGossipMenuFor(player);
                break;
            case 48:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_69, loc_idx), GOSSIP_SENDER_MAIN, 49);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_70, loc_idx), GOSSIP_SENDER_MAIN, 50);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_71, loc_idx), GOSSIP_SENDER_MAIN, 51);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
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
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_49, loc_idx), GOSSIP_SENDER_MAIN, 53);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_50, loc_idx), GOSSIP_SENDER_MAIN, 54);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_52, loc_idx), GOSSIP_SENDER_MAIN, 55);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_51, loc_idx), GOSSIP_SENDER_MAIN, 56);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_53, loc_idx), GOSSIP_SENDER_MAIN, 57);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_53, loc_idx), GOSSIP_SENDER_MAIN, 58);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_55, loc_idx), GOSSIP_SENDER_MAIN, 59);
                AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_56, loc_idx), GOSSIP_SENDER_MAIN, 50);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, sObjectMgr->GetTrinityString(LANG_GOSSIP_OPTION_29, loc_idx), GOSSIP_SENDER_MAIN, 21);
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
                break;
                // }
            }
            return true;
        }
    };

class channel_factions : public PlayerScript
{
public:
    channel_factions() : PlayerScript("channel_factions") {}

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel)
    {
        // ChatHandler handler(player->GetSession());

        if (!sGameConfig->GetBoolConfig("chat.system.icon"))
        {
            //    handler.PSendSysMessage("error #24");
             //   handler.SetSentErrorMessage(true);
            return;
        }

        if (!player || !channel)
            return;

        std::stringstream ssMsg;
        ssMsg << ((player->GetTeam() == HORDE) ? "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18:0:-1|t" : "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18:0:-1|t") << msg;
        msg = ssMsg.str();
    }
};

void AddSC_Resets()
{
    RegisterCreatureAI(npc_buffer);
    new channel_factions();
}
