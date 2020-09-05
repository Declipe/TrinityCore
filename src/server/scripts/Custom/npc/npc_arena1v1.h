/*
 *
 * Copyright (C) 2013 Emu-Devstore <http://emu-devstore.com/>
 * Written by Teiby <http://www.teiby.de/>
 *
 */

#ifndef ARENA_1V1_H
#define ARENA_1V1_H

#include "Config.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
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
#include "Channel.h"
#include "MapManager.h"
#include "CreatureTextMgr.h"
#include "SmartScriptMgr.h"
#include "ZynDatabase.h"
#include "DatabaseEnvFwd.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "GameTime.h"
#include "DBCStructure.h"
#include "GameEventMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "MotionMaster.h"
#include "ScriptedGossip.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Item.h"
#include "ItemDefines.h"
#include "ItemEnchantmentMgr.h"
#include "ItemTemplate.h"

// TalentTab.dbc -> TalentTabID
const uint32 FORBIDDEN_TALENTS_IN_1V1_ARENA[] =
{
    // Healer
    201, // PriestDiscipline
    202, // PriestHoly
    382, // PaladinHoly
    262, // ShamanRestoration
    282, // DruidRestoration

    // Tanks
    //383, // PaladinProtection
    //163, // WarriorProtection

    0 // End
};


// Return false, if player have invested more than 35 talentpoints in a forbidden talenttree.
static bool Arena1v1CheckTalents(Player* player)
{
    if(!player)
        return false;

    if(sWorld->getBoolConfig(CONFIG_ARENA_1V1_BLOCK_FORBIDDEN_TALENTS) == false)
        return true;

    uint32 count = 0;
    for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId)
    {
        TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentId);

        if (!talentInfo)
            continue;

        for (int8 rank = MAX_TALENT_RANK-1; rank >= 0; --rank)
        {
            if (talentInfo->SpellRank[rank] == 0)
                continue;

            if (player->HasTalent(talentInfo->SpellRank[rank], player->GetActiveSpec()))
            {
                for(int8 i = 0; FORBIDDEN_TALENTS_IN_1V1_ARENA[i] != 0; i++)
                    if(FORBIDDEN_TALENTS_IN_1V1_ARENA[i] == talentInfo->TabID)
                        count += rank + 1;
            }
        }
    }

    if(count >= 36)
    {
        ChatHandler(player->GetSession()).SendSysMessage("You can't join, because you have invested too many points in a forbidden talent. Please edit your talents.");
        return false;
    }
    else
        return true;
}

#endif
