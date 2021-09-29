#include "Config.h"
//#include "GuildMgr.h"
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
#include "DatabaseEnv.h"
#include "Log.h"
#include "SpellMgr.h"
#include "StringConvert.h"
#include "StringFormat.h"
#include <fstream>
#include <unordered_map>

#define SPELL_MASK_CLASS        1
#define SPELL_MASK_RIDING       2
#define SPELL_MASK_MOUNT        4
#define SPELL_MASK_WEAPON       8
#define SPELL_MASK_PROFESSION   16
#define SPELL_MASK_DUAL_SPEC    32


struct LearnSpellForClassInfo
{
    uint32  spellid;
    uint8   SpellMask;
    uint32  RequiredClassMask;
    uint32  RequiredRaceMask;
    uint8   RequiredLevel;
    uint32  RequiredSpellId;
    uint16  RequiredSkillId;
    uint16  RequiredSkillValue;
};

bool AutoLearnEnable = true;

uint8 OnLevelSpellMask = 0;
uint8 OnSkillSpellMask = 0;
uint8 OnLoginSpellMask = 0;
uint8 OnCreateSpellMask = 0;

std::vector<LearnSpellForClassInfo> LearnSpellForClass;


class Mod_AutoLearn_WorldScript : public WorldScript
{
public:
    Mod_AutoLearn_WorldScript() : WorldScript("Mod_AutoLearn_WorldScript") { }

    // Called after the world configuration is (re)loaded.
    //OnStartup
    //void OnAfterConfigLoad(bool /*reload*/)
    void OnStartup()
    {
        AutoLearnEnable = sConfigMgr->GetBoolDefault("AutoLearn.Enable", true);
        if (!AutoLearnEnable)
            return;

        uint8 loadSpellMask = OnLevelSpellMask | OnSkillSpellMask;
        OnLevelSpellMask = 0;
        OnSkillSpellMask = 0;
        OnLoginSpellMask = 0;
        OnCreateSpellMask = 0;

        if (sConfigMgr->GetBoolDefault("AutoLearn.Check.Level", true))
        {
            if (sConfigMgr->GetBoolDefault("AutoLearn.SpellClass", true))
                OnLevelSpellMask += SPELL_MASK_CLASS;
            if (sConfigMgr->GetBoolDefault("AutoLearn.SpellRiding", true))
                OnLevelSpellMask += SPELL_MASK_RIDING;
            if (sConfigMgr->GetBoolDefault("AutoLearn.SpellMount", true))
                OnLevelSpellMask += SPELL_MASK_MOUNT;
            if (sConfigMgr->GetBoolDefault("AutoLearn.SpellWeapon", true))
                OnLevelSpellMask += SPELL_MASK_WEAPON;
            if (sConfigMgr->GetBoolDefault("AutoLearn.DualSpec", true))
                OnLevelSpellMask += SPELL_MASK_DUAL_SPEC;

            if (sConfigMgr->GetBoolDefault("AutoLearn.Login.Spell", true))
                OnLoginSpellMask += OnLevelSpellMask;

            if (sConfigMgr->GetBoolDefault("AutoLearn.Create.Spell", true))
                OnCreateSpellMask += OnLevelSpellMask;
        }

        if (sConfigMgr->GetBoolDefault("AutoLearn.SpellProfession", true))
            OnSkillSpellMask += SPELL_MASK_PROFESSION;

        if (sConfigMgr->GetBoolDefault("AutoLearn.Login.Skill", true))
            OnLoginSpellMask += OnSkillSpellMask;

        if (sConfigMgr->GetBoolDefault("AutoLearn.Create.Skill", true))
            OnCreateSpellMask += OnSkillSpellMask;

        if (loadSpellMask != (OnLevelSpellMask | OnSkillSpellMask))
            LoadDataFromDataBase();
    }

    void LoadDataFromDataBase(void)
    {
        LearnSpellForClass.clear();
        uint8 spellMask = OnLevelSpellMask | OnSkillSpellMask;

        if (spellMask == 0)
            return;

        sLog->outError("Loading AutoLearn...");
        uint32 oldMSTime = getMSTime();

        QueryResult result;

        result = WorldDatabase.PQuery("SELECT spellid, SpellMask, RequiredClassMask, RequiredRaceMask, RequiredLevel, RequiredSpellId, RequiredSkillId, RequiredSkillValue FROM `world_autolearn`");

        if (!result)
            return;

        uint16 count = 0;

        do
        {
            Field* fields = result->Fetch();

            LearnSpellForClassInfo Spell;

            Spell.spellid = fields[0].GetUInt32();
            Spell.SpellMask = fields[1].GetUInt16();
            Spell.RequiredClassMask = fields[2].GetUInt32();
            Spell.RequiredRaceMask = fields[3].GetUInt32();
            Spell.RequiredLevel = fields[4].GetUInt8();
            Spell.RequiredSpellId = fields[5].GetUInt32();
            Spell.RequiredSkillId = fields[6].GetUInt16();
            Spell.RequiredSkillValue = fields[7].GetUInt16();

            if (!sSpellMgr->GetSpellInfo(Spell.spellid))
            {
                sLog->outError("AutoLearn: Spell (ID: %u) non-existing", Spell.spellid);
                continue;
            }

            // Skip spell
            if (!(Spell.SpellMask & spellMask))
                continue;

            if (Spell.RequiredClassMask != 0 && !(Spell.RequiredClassMask & CLASSMASK_ALL_PLAYABLE))
            {
                sLog->outDetail("AutoLearn: Spell (ID: %u) RequiredClassMask (Mask: %u) non-existing", Spell.spellid, Spell.RequiredClassMask);
                continue;
            }

            if (Spell.RequiredRaceMask != 0 && !(Spell.RequiredRaceMask & RACEMASK_ALL_PLAYABLE))
            {
                sLog->outDetail("AutoLearn: Spell (ID: %u) RequiredRaceMask (Mask: %u) non-existing", Spell.spellid, Spell.RequiredRaceMask);
                continue;
            }

            if (Spell.RequiredSpellId != 0 && !sSpellMgr->GetSpellInfo(Spell.RequiredSpellId))
            {
                sLog->outDetail("AutoLearn: Spell (ID: %u) RequiredSpellId (ID: %u) non-existing", Spell.spellid, Spell.RequiredSpellId);
                continue;
            }

            LearnSpellForClass.push_back(Spell);
            ++count;
        } while (result->NextRow());

        sLog->outString(">> Loaded %u spells for AutoLearn in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
};

class Mod_AutoLearn_PlayerScript : public PlayerScript
{
public:
    Mod_AutoLearn_PlayerScript() : PlayerScript("Mod_AutoLearn_PlayerScript") { }

    // Called when a player's level changes (right before the level is applied)
    void OnLevelChanged(Player* player, uint8 oldLevel)
    {
        if (!AutoLearnEnable)
            return;

        AutoLearnSpell(OnLevelSpellMask, player);
    }

    // Called when a player logs in.
    void OnLogin(Player* player) override
    {
        if (!AutoLearnEnable || !OnLoginSpellMask)
            return;

        AutoLearnSpell(OnLoginSpellMask, player);
    }

    // Called when a player is created.
    void OnCreate(Player* player)
    {
        if (!AutoLearnEnable || !OnCreateSpellMask)
            return;

        AutoLearnSpell(OnCreateSpellMask, player);
        player->SaveToDB(true, false);
        //player->SaveToDB(false, false);
    }

    // Called when a player skill update
    void OnPlayerSkillUpdate(Player* Player, uint16 SkillId, uint16 /*SkillValue*/, uint16 SkillNewValue)
    {
        if (!AutoLearnEnable)
            return;

        AutoLearnSpell(OnSkillSpellMask, Player, SkillId, SkillNewValue);
    }

    void AutoLearnSpell(uint8 SpellMask, Player* Player, uint16 SkillId = 0, uint16 SkillValue = 0)
    {
        if (SpellMask & SPELL_MASK_DUAL_SPEC)
        {
            learnDualSpec(Player);
            SpellMask -= SPELL_MASK_DUAL_SPEC;
        }

        if (SpellMask == 0) return;

        uint32  PlayerClassMask = Player->getClassMask();
        uint32  PlayerRaceMask = Player->getRaceMask();
        uint8   PlayerLevel = Player->getLevel();

        for (uint16 i = 0; i < LearnSpellForClass.size(); ++i)
        {
            LearnSpellForClassInfo& Spell = LearnSpellForClass[i];
            if (!(Spell.SpellMask & SpellMask)) continue;
            if (Spell.RequiredClassMask != 0 && !(Spell.RequiredClassMask & PlayerClassMask)) continue;
            if (Spell.RequiredRaceMask != 0 && !(Spell.RequiredRaceMask & PlayerRaceMask)) continue;
            if (Spell.RequiredLevel > PlayerLevel) continue;
            if (Spell.RequiredSkillId != SkillId) continue;
            if (Spell.RequiredSkillValue > SkillValue) continue;
            if (Player->HasSpell(Spell.spellid)) continue;
            if (Spell.RequiredSpellId != 0 && !Player->HasSpell(Spell.RequiredSpellId)) continue;

            Player->learnSpell(Spell.spellid);
        }
    }

    void learnDualSpec(Player* Player)
    {
        if (Player->getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)) return;

        if (Player->GetSpecsCount() != 1) return;

        Player->CastSpell(Player, 63680, true, NULL, NULL, Player->GetGUID());
        Player->CastSpell(Player, 63624, true, NULL, NULL, Player->GetGUID());
    }
};

void AddModAutoLearnScripts()
{
    new Mod_AutoLearn_PlayerScript;
    new Mod_AutoLearn_WorldScript;
}


