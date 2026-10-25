# Battleground and arena replays

The replay script records arena matches, battleground matches, and (when enabled) battlefield matches. Saved matches can be watched by replay ID or selected from the latest-match/favorites lists in an NPC gossip menu.

## Setup

1. Apply `sql/custom/characters/mod_arena_replay.sql` to the characters database.
2. Keep `ArenaReplay.Enable = 1` in `worldserver.conf`. Replay capture and retention options are documented in `worldserver.conf.dist`.
3. Attach `ReplayGossip` to a dedicated creature template and enable the gossip NPC flag. Replace `<creature_entry>` with the template entry you selected:

   ```sql
   UPDATE `creature_template`
   SET `ScriptName` = 'ReplayGossip', `npcflag` = `npcflag` | 1
   WHERE `entry` = <creature_entry>;
   ```

   Use a dedicated creature or verify that replacing its existing script is safe.
4. Restart `worldserver`. Only matches played after startup can be replayed.

The NPC lists the latest ten matches, accepts a replay ID, and lets each character save favorites. Retention follows `ArenaReplay.DeleteReplaysAfterDays`; saved replays are preserved unless `ArenaReplay.DeleteSavedReplays = 1`.
