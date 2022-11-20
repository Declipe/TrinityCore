SET @BOT_START = 70001;
SET @BOT_END   = 70574;

delete from `creature_equip_template` where CreatureID between @BOT_START and @BOT_END;