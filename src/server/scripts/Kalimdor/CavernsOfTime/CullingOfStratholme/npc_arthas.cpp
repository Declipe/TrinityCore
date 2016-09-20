/*
* Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "culling_of_stratholme.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "SplineChainMovementGenerator.h"

enum Entries
{
    NPC_MALGANIS_BUNNY = 20562,
    NPC_UTHER = 26528,
    NPC_JAINA = 26497,
    NPC_CITIZEN = 28167,
    NPC_RESIDENT = 28169,
    NPC_FOOTMAN = 27745,
    NPC_KNIGHT = 27746,
    NPC_PRIEST = 27747,
    NPC_SORCERESS = 27752,
    NPC_RISEN_ZOMBIE = 27737,
    NPC_CITIZEN_INFINITE = 28340,
    NPC_RESIDENT_INFINITE = 28341,
    NPC_INFINITE_HUNTER = 27743,
    NPC_INFINITE_AGENT = 27744,
    NPC_INFINITE_ADVERSARY = 27742,
    NPC_EPOCH = 26532,
    NPC_MALGANIS = 26533,
    NPC_CHROMIE_3 = 30997,

    SPELL_HOLY_LIGHT = 52444,
    SPELL_EXORCISM = 52445,
    SPELL_CRUSADER_STRIKE = 50773,
    SPELL_SHADOWSTEP_VISUAL = 51908,
    SPELL_TRANSFORM_VISUAL = 33133,
    SPELL_MALGANIS_QUEST_CREDIT = 58124,
    SPELL_MALGANIS_KILL_CREDIT = 58630,
    SPELL_CHROMIE_3_TRANSFORM = 58986,
    GO_CHEST_NORMAL = 190663,
    GO_CHEST_HEROIC = 193597
};

enum SplineChains
{
    // RP1: Arthas/Jaina/Uther, outside Stratholme gates
    RP1_CHAIN_ARTHAS1   = 1, // Initial movement
    RP1_CHAIN_UTHER1    = 1, // Initial movement
    RP1_CHAIN_JAINA1    = 1, // Initial movement
    RP1_CHAIN_ARTHAS2   = 2, // Arthas moves up the hill to look out over Stratholme
    RP1_CHAIN_UTHER2    = 2, // Uther follows
    RP1_CHAIN_JAINA2    = 2, // Jaina follows
    RP1_CHAIN_UTHER3    = 3, // Uther leaves
    RP1_CHAIN_JAINA3    = 3, // Jaina turns to leave, but is stopped by Arthas
    RP1_CHAIN_JAINA4    = 4, // Jaina leaves for real
    RP1_CHAIN_ARTHAS3   = 3, // Arthas moves down the hill to the bridge
    RP1_CHAIN_ARTHAS4   = 4, // Arthas moves to the Stratholme entrance

    // RP2: Arthas/Mal'ganis, at Stratholme gates
    RP2_CHAIN_ARTHAS1   = 5, // Arthas enters Stratholme proper
    RP2_CHAIN_CITIZEN1  = 1, // A Stratholme citizen greets Arthas
    RP2_CHAIN_ARTHAS2   = 6, // Arthas advances towards the approaching citizen
    RP2_CHAIN_ARTHAS3   = 7, // Arthas moves on to a nearby resident

    // RP3: Arthas/Infinites, in Stratholme Town Hall
    RP3_CHAIN_ARTHAS1   = 91, // Arthas enters Stratholme Town Hall
    RP3_CHAIN_ARTHAS2   = 92, // Arthas approaches one of the disguised Infinites
    RP3_CHAIN_ARTHAS3   = 95, // Arthas proceeds up the stairs
    RP3_CHAIN_ARTHAS3_2 = 96, // Arthas encounters more Infinite resistance
    RP3_CHAIN_ARTHAS4   = 97, // Arthas presses onward into a hallway
    RP3_CHAIN_ARTHAS5   = 98, // Arthas advances into the boss room
    RP3_CHAIN_EPOCH     = 1,  // Chrono-Lord Epoch advances out of the portal
};

enum PointIDs
{
    RP1_POINTID_UTHER1 = 1,
    RP1_POINTID_ARTHAS2,
    RP1_POINTID_UTHER2,
    RP1_POINTID_JAINA2,
    RP1_POINTID_UTHER3,
    RP1_POINTID_JAINA4,
    RP1_POINTID_ARTHAS3,
    RP1_POINTID_ARTHAS4,

    RP2_POINTID_ARTHAS1,
    RP2_POINTID_ARTHAS2,
    RP2_POINTID_ARTHAS3,

    RP3_POINTID_TOWN_HALL,
    RP3_POINTID_ARTHAS1,
    RP3_POINTID_ARTHAS2,
    RP3_POINTID_ARTHAS3,
    RP3_POINTID_ARTHAS4,
    RP3_POINTID_ARTHAS5
};

enum Actions
{
    ACTION_NONE = 0,
    RP3_ACTION_AFTER_INITIAL,
    RP3_ACTION_AFTER_SPAWN1,
    RP3_ACTION_AFTER_SPAWN2,
    RP3_ACTION_AFTER_SPAWN3,
    RP3_ACTION_AFTER_EPOCH,
    RP5_ACTION_AFTER_MALGANIS
};

enum Data
{
    DATA_RP_DUMMY_MOVED
};

enum RPEvents
{
    RP1_EVENT_START1 = 1,
    RP1_EVENT_START2,
    RP1_EVENT_ARTHAS1,
    RP1_EVENT_UTHER1,
    RP1_EVENT_ARTHAS2,
    RP1_EVENT_ARTHAS3,
    RP1_EVENT_UTHER_FACE,
    RP1_EVENT_JAINA_FACE,
    RP1_EVENT_UTHER2,
    RP1_EVENT_ARTHAS4,
    RP1_EVENT_UTHER3,
    RP1_EVENT_ARTHAS_TURN,
    RP1_EVENT_ARTHAS5,
    RP1_EVENT_UTHER4,
    RP1_EVENT_ARTHAS6,
    RP1_EVENT_UTHER5,
    RP1_EVENT_ARTHAS7,
    RP1_EVENT_JAINA1,
    RP1_EVENT_ARTHAS8,
    RP1_EVENT_ARTHAS8_2,
    RP1_EVENT_UTHER6,
    RP1_EVENT_UTHER_LEAVE,
    RP1_EVENT_JAINA_LEAVE,
    RP1_EVENT_ARTHAS9,
    RP1_EVENT_JAINA2,
    RP1_EVENT_JAINA_LEAVE2,
    RP1_EVENT_ARTHAS_LEAVE,
    RP1_EVENT_ARTHAS10,
    RP1_EVENT_ARTHAS_LEAVE2,
    RP1_EVENT_FINISHED,

    RP2_EVENT_ARTHAS_MOVE_1,
    RP2_EVENT_CITIZEN1,
    RP2_EVENT_ARTHAS2,
    RP2_EVENT_ARTHAS_MOVE_2,
    RP2_EVENT_CITIZEN2,
    RP2_EVENT_KILL1,
    RP2_EVENT_ARTHAS_MOVE_3,
    RP2_EVENT_KILL2,
    RP2_EVENT_REACT1,
    RP2_EVENT_REACT2,
    RP2_EVENT_REACT3,
    RP2_EVENT_REACT4,
    RP2_EVENT_REACT5,
    RP2_EVENT_ARTHAS_MOVE_4,
    RP2_EVENT_ARTHAS3,
    RP2_EVENT_MALGANIS1,
    RP2_EVENT_TROOPS_FACE,
    RP2_EVENT_ARTHAS_FACE,
    RP2_EVENT_MALGANIS2,
    RP2_EVENT_MALGANIS_LEAVE1,
    RP2_EVENT_MALGANIS_LEAVE2,
    RP2_EVENT_ARTHAS4,
    RP2_EVENT_ARTHAS4_2,
    RP2_EVENT_ARTHAS_MOVE_5,
    RP2_EVENT_ARTHAS5,
    RP2_EVENT_ARTHAS5_2,
    RP2_EVENT_ARTHAS5_3,
    RP2_EVENT_WAVE_START,

    EVENT_TOWN_HALL_REACHED,

    RP3_EVENT_RESIDENT_FACE,
    RP3_EVENT_ARTHAS_FACE,
    RP3_EVENT_CITIZEN1,
    RP3_EVENT_ARTHAS2,
    RP3_EVENT_ARTHAS_KILL,
    RP3_EVENT_INFINITE_LAUGH,
    RP3_EVENT_ARTHAS3,
    RP3_EVENT_CITIZEN2,
    RP3_EVENT_TRANSFORM1,
    RP3_EVENT_TRANSFORM2,
    RP3_EVENT_TRANSFORM3,
    RP3_EVENT_AGGRO,
    RP3_EVENT_ARTHAS4,
    RP3_EVENT_ARTHAS_MOVE_1,
    RP3_EVENT_ARTHAS_MOVE_1_2,
    RP3_EVENT_SPAWN1,
    RP3_EVENT_SPAWN1_FACE,
    RP3_EVENT_SPAWN1_AGGRO,
    RP3_EVENT_ARTHAS11,
    RP3_EVENT_ARTHAS_MOVE_2,
    RP3_EVENT_ARTHAS20,
    RP3_EVENT_SPAWN2,
    RP3_EVENT_ARTHAS_FACE2,
    RP3_EVENT_SPAWN2_FACE,
    RP3_EVENT_SPAWN2_AGGRO,
    RP3_EVENT_ARTHAS21,
    RP3_EVENT_ARTHAS_MOVE_3,
    RP3_EVENT_SPAWN3,
    RP3_EVENT_ARTHAS30,
    RP3_EVENT_SPAWN3_FACE,
    RP3_EVENT_SPAWN3_AGGRO,
    RP3_EVENT_EPOCH_SPAWN,
    RP3_EVENT_ARTHAS31,
    RP3_EVENT_EPOCH_FACE,
    RP3_EVENT_EPOCH1,
    RP3_EVENT_ARTHAS32,
    RP3_EVENT_ARTHAS32_2,
    RP3_EVENT_EPOCH_AGGRO,

    RP4_EVENT_ARTHAS2,
    RP4_EVENT_HIDDEN_PASSAGE,
    RP4_EVENT_ARTHAS3,
    RP4_EVENT_GAUNTLET_REACHED,

    RP4_EVENT_ARTHAS11,
    RP4_EVENT_ARTHAS_FACE,
    RP4_EVENT_ARTHAS12,
    RP4_EVENT_GAUNTLET_RESUME,
    RP4_EVENT_ARTHAS13,
    RP4_EVENT_GAUNTLET_DONE,

    RP5_EVENT_ARTHAS2,
    RP5_EVENT_MALGANIS1,
    RP5_EVENT_MALGANIS_DONE,
    RP5_EVENT_MALGANIS12,
    RP5_EVENT_MALGANIS_LEAVE,
    RP5_EVENT_ARTHAS10,
    RP5_EVENT_ARTHAS10_2,
    RP5_EVENT_ARTHAS11,
    RP5_EVENT_ARTHAS11_2,
    RP5_EVENT_CHROMIE_SPAWN,
    RP5_EVENT_CHROMIE_LAND,
    RP5_EVENT_CHROMIE_TRANSFORM
};

enum RPEventLines1
{
    RP1_LINE_ARTHAS1    =  0, // Glad you could make it, Uther.
    RP1_LINE_UTHER1     =  0, // Watch your tone with me, boy. You may be the prince, but I'm still your superior as a paladin!
    RP1_LINE_ARTHAS2    =  1, // As if I could forget. Listen, Uther, there's something about the plague you should know...
    RP1_LINE_ARTHAS3    =  2, // Oh, no. We're too late. These people have all been infected! They may look fine now, but it's just a matter of time before they turn into the undead!
    RP1_LINE_UTHER2     =  1, // What?
    RP1_LINE_ARTHAS4    =  3, // This entire city must be purged.
    RP1_LINE_UTHER3     =  2, // How can you even consider that? There's got to be some other way.
    RP1_LINE_ARTHAS5    =  4, // Damn it, Uther! As your future king, I order you to purge this city!
    RP1_LINE_UTHER4     =  3, // You are not my king yet, boy! Nor would I obey that command even if you were!
    RP1_LINE_ARTHAS6    =  5, // Then I must consider this an act of treason.
    RP1_LINE_UTHER5     =  4, // Treason? Have you lost your mind, Arthas?
    RP1_LINE_ARTHAS7    =  6, // Have I? Lord Uther, by my right of succession and the sovereignty of my crown, I hereby relieve you of your command and suspend your paladins from service.
    RP1_LINE_JAINA1     =  0, // Arthas! You can't just--
    RP1_LINE_ARTHAS8    =  7, // It's done! Those of you who have the will to save this land, follow me! The rest of you... get out of my sight!
    RP1_LINE_UTHER6     =  5, // You've just crossed a terrible threshold, Arthas.
    RP1_LINE_ARTHAS9    =  8, // Jaina?
    RP1_LINE_JAINA2     =  1, // I'm sorry, Arthas. I can't watch you do this.
    RP1_LINE_ARTHAS10   =  9, // Take position here, and I will lead a small force inside Stratholme to begin the culling. We must contain and purge the infected for the sake of all of Lordaeron!
    RP1_LINE_ARTHAS11   = 40  // All officers should check in with me when their squads are ready. We'll enter Stratholme on my order.
};

enum RPEventLines2
{
    RP2_LINE_ARTHAS1    = 10, // Everyone looks ready. Remember, these people are all infected with the plague and will die soon. We must purge Stratholme to protect the remainder of Lordaeron from the Scourge. Let's go.
    RP2_LINE_CITIZEN1   =  0, // Prince Arthas, may the light be praised! Many people in the town have begun to fall seriously ill, can you help us?
    RP2_LINE_ARTHAS2    = 11, // I can only help you with a clean death.
    RP2_LINE_CITIZEN2   =  1, // What? This can't be!
    RP2_LINE_RESIDENT1  =  0, // Oh, no...
    RP2_LINE_ARTHAS3    = 12, // That was just the beginning.
    RP2_LINE_MALGANIS1  =  0, // Yes, this is the beginning. I've been waiting for you, young prince. I am Mal'Ganis.
    RP2_LINE_MALGANIS2  =  1, // As you can see, your people are now mine. I will now turn this city household by household, until the flame of life has been snuffed out... forever.
    RP2_LINE_ARTHAS4    = 13, // I won't allow it, Mal'Ganis! Better that these people die by my hand than serve as your slaves in death!
    RP2_LINE_ARTHAS5    = 14  // Mal'Ganis will send out some of his Scourge minions to interfere with us. Those of you with the strongest steel and magic shall go forth and destroy them. I will lead the rest of my forces in purging Stratholme of the infected.
};

enum RPEventLines3
{
    RP3_LINE_ARTHAS1    = 16, // Follow me, I know the way through.
    RP3_LINE_CITIZEN1   =  0, // Ah, you've finally arrived Prince Arthas. You're here just in the nick of time.
    RP3_LINE_ARTHAS2    = 17, // Yes, I'm glad I could get to you before the plague.
    RP3_LINE_ARTHAS3    = 18, // What is this sorcery?
    RP3_LINE_CITIZEN2   =  1, // There's no need for you to understand, Arthas. All you need to do is die.
    RP3_LINE_ARTHAS4    = 19, // Mal'Ganis appears to have more than Scourge in his arsenal. We should make haste.

    RP3_LINE_ARTHAS10   = 20, // More vile sorcery! Be ready for anything!
    RP3_LINE_ARTHAS11   = 21, // Let's move on.

    RP3_LINE_ARTHAS20   = 22, // Watch your backs: they have us surrounded in this hall.
    RP3_LINE_ARTHAS21   = 24, // Mal'ganis is not making this easy.

    RP3_LINE_ARTHAS30   = 25, // They're very persistent.
    RP3_LINE_ARTHAS31   = 26, // What else can he put in my way?
    RP3_LINE_EPOCH1     =  0, // Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own.
    RP3_LINE_ARTHAS32   = 27, // I do what I must for Lordaeron, and neither your words nor your actions will stop me.
    RP3_LINE_EPOCH2     =  1  // We'll see about that, young prince.
};

enum RPEventLines4
{
    RP4_LINE_ARTHAS1    = 28, // The quickest path to Mal'Ganis lies behind that bookshelf ahead.
    RP4_LINE_ARTHAS2    = 29, // This will only take a moment.
    RP4_LINE_ARTHAS3    = 30, // I'm relieved this secret passage still works.
    
    RP4_LINE_ARTHAS10   = 31, // Let's move through here as quickly as possible. If the undead don't kill us, the fires might.
    RP4_LINE_ARTHAS11   = 32, // Rest a moment and clear your lungs, but we must move again soon.
    RP4_LINE_ARTHAS12   = 33, // That's enough; we must move again. Mal'Ganis awaits.
    RP4_LINE_ARTHAS13   = 34  // At last some good luck. Market Row has not caught fire yet. Mal'Ganis is supposed to be in Crusaders' Square, which is just ahead. Tell me when you're ready to move forward.
};

enum RPEventLines5
{
    RP5_LINE_ARTHAS1    = 35, // Justice will be done.
    RP5_LINE_ARTHAS2    = 36, // We're going to finish this right now, Mal'Ganis. Just you... and me.
    RP5_LINE_MALGANIS1  =  2, // This will be a fine test, Prince Arthas.
    RP5_LINE_MALGANIS12 = 10, // Your journey has just begun, young prince. Gather your forces and meet me in the arctic land of Northrend. It is there that we shall settle the score between us. It is there that your true destiny will unfold.
    RP5_LINE_ARTHAS10   = 37, // I'll hunt you to the ends of the earth if I have to! Do you hear me? To the ends of the earth!
    RP5_LINE_ARTHAS11   = 38, // You performed well this day. Anything that Mal'Ganis has left behind is yours. Take it as your reward. I must now begin plans for an expedition to Northrend.
    RP5_LINE_CHROMIE0   =  0  // Why, hello again!
};

enum PositionIndices : uint32
{
    // Arthas/Uther RP
    RP1_ARTHAS_INITIAL = 0,
    RP1_UTHER_SPAWN,
    RP1_JAINA_SPAWN,
    
    // Arthas/Mal'ganis RP
    ARTHAS_PURGE_PENDING_POS,
    ARTHAS_WAVES_POS,
    RP2_PRIEST1_POS,
    RP2_PRIEST2_POS,
    RP2_FOOT1_POS,
    RP2_FOOT2_POS,
    RP2_FOOT3_POS,
    RP2_FOOT4_POS,
    RP2_MALGANIS_POS,

    // Town Hall
    ARTHAS_TOWN_HALL_POS,
    RP3_SPAWN1_ADVERSARY1_SPAWN,
    RP3_SPAWN1_ADVERSARY1_WP1,
    RP3_SPAWN1_ADVERSARY1_WP2,
    RP3_SPAWN1_ADVERSARY1_WP3,
    RP3_SPAWN1_ADVERSARY1_WP4,
    RP3_SPAWN1_ADVERSARY2_SPAWN,
    RP3_SPAWN1_HUNTER1_SPAWN,
    RP3_SPAWN1_HUNTER1_WP1,
    RP3_SPAWN1_HUNTER1_WP2,
    RP3_SPAWN1_HUNTER1_WP3,
    RP3_SPAWN1_HUNTER1_WP4,
    RP3_SPAWN1_HUNTER1_WP5,
    RP3_SPAWN1_AGENT1_SPAWN,
    RP3_SPAWN2_ADVERSARY1_SPAWN,
    RP3_SPAWN2_HUNTER1_SPAWN,
    RP3_SPAWN2_HUNTER1_WP1,
    RP3_SPAWN2_HUNTER1_WP2,
    RP3_SPAWN2_HUNTER1_WP3,
    RP3_SPAWN2_HUNTER1_WP4,
    RP3_SPAWN2_HUNTER2_SPAWN,
    RP3_SPAWN2_AGENT1_SPAWN,
    RP3_SPAWN2_AGENT1_WP1,
    RP3_SPAWN2_AGENT1_WP2,
    RP3_SPAWN2_AGENT1_WP3,
    RP3_SPAWN2_AGENT1_WP4,
    ARTHAS_TOWN_HALL_END_POS,
    RP3_SPAWN3_ADVERSARY1_SPAWN,
    RP3_SPAWN3_ADVERSARY1_WP1,
    RP3_SPAWN3_ADVERSARY1_WP2,
    RP3_SPAWN3_HUNTER1_SPAWN,
    RP3_SPAWN3_HUNTER1_WP1,
    RP3_SPAWN3_HUNTER1_WP2,
    RP3_SPAWN3_HUNTER1_WP3,
    RP3_SPAWN3_HUNTER1_WP4,
    RP3_SPAWN3_HUNTER1_WP5,
    RP3_SPAWN3_HUNTER1_WP6,
    RP3_SPAWN3_HUNTER2_SPAWN,
    RP3_SPAWN3_HUNTER2_WP1,
    RP3_SPAWN3_HUNTER2_WP2,
    RP3_SPAWN3_HUNTER2_WP3,
    RP3_SPAWN3_HUNTER2_WP4,
    RP3_SPAWN3_AGENT1_SPAWN,
    RP3_SPAWN3_AGENT1_WP1,
    RP3_SPAWN3_AGENT1_WP2,
    RP3_SPAWN3_AGENT1_WP3,
    RP3_EPOCH_SPAWN,

    // Gauntlet
    RP4_ARTHAS_WP1,
    RP4_ARTHAS_WP2,
    RP4_ARTHAS_WP3,
    RP4_ARTHAS_WP4,
    RP4_ARTHAS_WP5,
    RP4_ARTHAS_WP6,
    RP4_ARTHAS_WP7,
    RP4_ARTHAS_WP8,
    RP4_ARTHAS_WP9,
    RP4_ARTHAS_WP10,
    RP4_ARTHAS_WP11,
    RP4_ARTHAS_WP12,
    RP4_ARTHAS_WP13,
    RP4_ARTHAS_WP14,
    RP4_ARTHAS_WP15,
    RP4_ARTHAS_WP16,
    RP4_ARTHAS_WP17,
    RP4_ARTHAS_WP18,
    RP4_ARTHAS_WP19,
    RP4_ARTHAS_WP30,
    RP4_ARTHAS_WP31,
    RP4_ARTHAS_WP32,
    RP4_ARTHAS_WP33,
    RP4_ARTHAS_WP34,
    RP4_ARTHAS_WP35,
    RP4_ARTHAS_WP36,
    RP4_ARTHAS_WP37,
    RP4_ARTHAS_WP38,
    RP4_ARTHAS_WP39,
    RP4_ARTHAS_WP40,
    RP4_ARTHAS_WP41,
    RP4_ARTHAS_WP42,
    RP4_ARTHAS_WP43,
    RP4_ARTHAS_WP44,
    RP4_ARTHAS_WP45,
    RP4_ARTHAS_WP46,
    RP4_ARTHAS_WP47,
    RP4_ARTHAS_WP48,
    RP4_ARTHAS_WP49,
    RP4_ARTHAS_WP50,
    RP4_ARTHAS_WP51,
    RP4_ARTHAS_WP52,
    RP4_ARTHAS_WP53,
    RP4_ARTHAS_WP54,
    RP4_ARTHAS_WP55,
    RP4_ARTHAS_WP56,
    RP4_ARTHAS_WP57,
    RP4_ARTHAS_WP58,
    RP4_ARTHAS_WP59,
    RP4_ARTHAS_WP60,
    RP4_ARTHAS_WP61,
    RP4_ARTHAS_WP62,
    RP4_ARTHAS_WP63,
    RP4_ARTHAS_WP64,
    RP4_ARTHAS_WP65,
    RP4_ARTHAS_WP66,
    RP4_ARTHAS_WP67,

    ARTHAS_GAUNTLET_POS,
    GAUNTLET_WP1,
    GAUNTLET_WP2,
    GAUNTLET_WP3,
    GAUNTLET_WP4,
    GAUNTLET_WP5,
    GAUNTLET_WP6,
    GAUNTLET_WP7,
    GAUNTLET_WP8,
    GAUNTLET_WP9,
    GAUNTLET_WP10,
    GAUNTLET_WP11,
    GAUNTLET_WP12,
    GAUNTLET_WP13,
    GAUNTLET_WP14,
    GAUNTLET_WP15,
    GAUNTLET_WP16,
    GAUNTLET_WP17,
    GAUNTLET_WP18,
    GAUNTLET_WP19,
    GAUNTLET_WP20,
    GAUNTLET_WP21,
    GAUNTLET_WP22,
    GAUNTLET_WP23,
    GAUNTLET_WP24,
    GAUNTLET_WP25,
    GAUNTLET_WP26,
    GAUNTLET_WP27,
    GAUNTLET_WP28,
    GAUNTLET_WP29,
    GAUNTLET_WP30,
    GAUNTLET_WP31,
    GAUNTLET_WP32,
    GAUNTLET_WP33,
    GAUNTLET_WP34,
    GAUNTLET_WP35,
    GAUNTLET_WP36,
    GAUNTLET_WP37,
    GAUNTLET_WP38,
    GAUNTLET_WP39,
    GAUNTLET_WP40,
    GAUNTLET_WP41,
    GAUNTLET_WP42,
    GAUNTLET_WP43,
    GAUNTLET_WP44,
    GAUNTLET_WP45,
    GAUNTLET_WP46,
    GAUNTLET_WP47,
    GAUNTLET_WP48,
    GAUNTLET_WP50,
    GAUNTLET_WP51,
    GAUNTLET_WP52,
    GAUNTLET_WP53,
    GAUNTLET_WP54,
    GAUNTLET_WP55,
    GAUNTLET_WP56,
    GAUNTLET_WP57,
    GAUNTLET_WP58,
    GAUNTLET_WP59,
    GAUNTLET_WP60,
    GAUNTLET_WP61,
    GAUNTLET_WP62,
    GAUNTLET_WP63,
    GAUNTLET_WP64,
    GAUNTLET_WP65,
    GAUNTLET_WP66,
    GAUNTLET_WP67,
    GAUNTLET_WP68,
    GAUNTLET_WP69,
    GAUNTLET_WP70,
    GAUNTLET_WP71,
    GAUNTLET_WP72,
    GAUNTLET_WP73,
    GAUNTLET_WP74,
    GAUNTLET_WP75,
    GAUNTLET_WP76,
    GAUNTLET_WP77,
    GAUNTLET_WP78,
    GAUNTLET_WP79,
    GAUNTLET_WP80,
    GAUNTLET_WP81,
    GAUNTLET_WP82,
    GAUNTLET_WP83,
    GAUNTLET_WP84,
    GAUNTLET_WP85,
    GAUNTLET_WP86,
    GAUNTLET_WP87,
    GAUNTLET_WP88,
    GAUNTLET_WP89,
    GAUNTLET_WP90,
    GAUNTLET_WP91,
    GAUNTLET_WP92,
    GAUNTLET_WP93,
    GAUNTLET_WP94,
    GAUNTLET_WP95,
    GAUNTLET_WP96,
    GAUNTLET_WP97,
    GAUNTLET_WP98,
    GAUNTLET_WP99,
    GAUNTLET_WP100,
    GAUNTLET_WP101,
    GAUNTLET_WP102,
    GAUNTLET_WP103,
    GAUNTLET_WP104,

    ARTHAS_GAUNTLET_END_POS,
    RP5_MALGANIS_POS,
    RP5_ARTHAS_WP1,
    RP5_ARTHAS_WP2,
    RP5_ARTHAS_WP3,
    RP5_ARTHAS_WP4,
    RP5_ARTHAS_WP5,
    RP5_ARTHAS_WP6,
    RP5_ARTHAS_WP7,
    RP5_ARTHAS_WP8,
    RP5_ARTHAS_WP9,
    RP5_ARTHAS_WP10,
    RP5_ARTHAS_WP11,
    RP5_ARTHAS_WP12,
    RP5_ARTHAS_WP13,
    RP5_ARTHAS_WP14,
    RP5_ARTHAS_WP15,
    RP5_ARTHAS_WP16,
    RP5_ARTHAS_WP17,
    RP5_ARTHAS_WP18,
    RP5_ARTHAS_WP19,
    RP5_ARTHAS_WP20,
    RP5_ARTHAS_WP21,
    RP5_ARTHAS_WP22,
    RP5_CHROMIE_SPAWN,
    RP5_CHROMIE_WP1,
    RP5_CHROMIE_WP2,
    RP5_CHROMIE_WP3,

    // Array element count
    NUM_POSITIONS
};

enum OtherLines
{
    LINE_TOWN_HALL_PENDING  = 15,
    LINE_SLAY_ZOMBIE        = 39
};

class npc_arthas_stratholme : public CreatureScript
{
    public:
    npc_arthas_stratholme() : CreatureScript("npc_arthas_stratholme") { }

    struct npc_arthas_stratholmeAI : public ScriptedAI
    {
        npc_arthas_stratholmeAI(Creature* creature) : ScriptedAI(creature), _hadSetup(false), instance(creature->GetInstanceScript()), _exorcismCooldown(urandms(7,14)), _progressRP(true), _afterCombat(ACTION_NONE) { }

        static const std::array<Position, NUM_POSITIONS> _positions; // all kinds of positions we'll need for RP events (there's a lot of these)
        static const float _snapbackDistanceThreshold; // how far we can be from where we're supposed at start of phase to be before we snap back
        struct SnapbackInfo
        {
            ReactStates const reactState;
            bool const haveGossip;
            Position const* const snapbackPos;
        };
        static const std::map<ProgressStates, SnapbackInfo> _snapbackPositions; // positions we should be at when starting a given phase

        inline ProgressStates GetCurrentProgress() { return ProgressStates(instance->GetData(DATA_INSTANCE_PROGRESS)); }
        void AdvanceToState(ProgressStates newState)
        {
            std::cout << "Arthas AI: Advancing to " << newState << std::endl;
            if (!_progressRP)
            {
                TC_LOG_WARN("scripts.scripts", "CoT4 Arthas AI: Advancing to instance state 0x%X, but RP is paused. Overriding!", newState);
                _progressRP = true;
            }
            std::map<ProgressStates, SnapbackInfo>::const_iterator it = _snapbackPositions.find(newState);
            if (it != _snapbackPositions.end())
            {
                SnapbackInfo const& target = it->second;

                // Adjust react state and npc flags based on current state
                me->SetReactState(target.reactState);
                if (target.reactState == REACT_PASSIVE)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                else
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

                // Adjust gossip flag based on whether we have a gossip menu or not
                if (target.haveGossip)
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                else
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                std::cout << "Have snapback for this state, distance from it is " << target.snapbackPos->GetExactDist(me) << " yd" << std::endl;
                // Snapback handling - if we're too far from where we're supposed to be, teleport there
                if (target.snapbackPos->GetExactDist(me) > _snapbackDistanceThreshold)
                    me->NearTeleportTo(*target.snapbackPos);
            }

            switch (newState)
            {
                case WAVES_DONE:
                    // @todo proper movement
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->GetMotionMaster()->MovePoint(RP3_POINTID_TOWN_HALL, _positions[ARTHAS_TOWN_HALL_POS], true);
                    break;
                case TOWN_HALL_PENDING:
                    Talk(LINE_TOWN_HALL_PENDING);
                    break;
                case COMPLETE:
                    if (events.Empty())
                    {
                        // This must be instance loading into COMPLETE state, spawn chromie
                        events.ScheduleEvent(RP5_EVENT_CHROMIE_SPAWN, Seconds(1));
                        events.ScheduleEvent(RP5_EVENT_CHROMIE_LAND, Seconds(12)+Milliseconds(668));
                        events.ScheduleEvent(RP5_EVENT_CHROMIE_TRANSFORM, Seconds(15)+Milliseconds(491));
                    }
                    break;
                default:
                    break;
            }
        }

        void MoveAlongPath(Creature* creature, PositionIndices start, PositionIndices end, bool walk = false)
        {
            if (start == end)
            {
                creature->SetWalk(walk);
                creature->GetMotionMaster()->MovePoint(end, _positions[end], false);
            }
            else
            {
                Movement::PointsArray const path(_positions.begin() + start, _positions.begin() + end + 1);
                creature->GetMotionMaster()->MoveSmoothPath(end, path, walk);
            }
        }

        // Will happen immediately (if OOC) or on reached home (if IC)
        void ScheduleActionOOC(Actions action)
        {
            if (_progressRP)
                DoAction(action);
            else
                _afterCombat = action;
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case -ACTION_PROGRESS_UPDATE:
                    AdvanceToState(GetCurrentProgress());
                    break;
                case -ACTION_PROGRESS_UPDATE_FORCE:
                {
                    // turn RP off so we don't process movementinforms
                    _progressRP = false;

                    // Reset waypoint and scheduling info
                    _afterCombat = ACTION_NONE;
                    _resumeMovement.Chain = nullptr;
                    me->StopMoving();
                    me->GetMotionMaster()->Clear();
                    events.Reset();

                    std::map<ProgressStates, SnapbackInfo>::const_iterator it = _snapbackPositions.find(GetCurrentProgress());
                    if (it != _snapbackPositions.end())
                        me->SetHomePosition(*it->second.snapbackPos);

                    // Re-initialize on next tick
                    _hadSetup = false;
                    break;
                }
                case RP3_ACTION_AFTER_INITIAL:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS4, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_MOVE_1, Seconds(7));
                    break;
                case RP3_ACTION_AFTER_SPAWN1:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS11, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_MOVE_2, Seconds(3));
                    break;
                case RP3_ACTION_AFTER_SPAWN2:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS21, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_MOVE_3, Seconds(4));
                    break;
                case RP3_ACTION_AFTER_SPAWN3:
                    events.ScheduleEvent(RP3_EVENT_EPOCH_SPAWN, Seconds(3));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS31, Seconds(4));
                    events.ScheduleEvent(RP3_EVENT_EPOCH_FACE, Seconds(6));
                    events.ScheduleEvent(RP3_EVENT_EPOCH1, Seconds(9));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS32, Seconds(22));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS32_2, Seconds(27));
                    events.ScheduleEvent(RP3_EVENT_EPOCH_AGGRO, Seconds(30));
                    break;
                case RP3_ACTION_AFTER_EPOCH:
                    instance->SetData(DATA_TOWN_HALL_DONE, 1);
                    break;
                case RP5_ACTION_AFTER_MALGANIS:
                    events.ScheduleEvent(RP5_EVENT_MALGANIS_DONE, Seconds(3));
                    break;
            }
        }

        void SetGUID(ObjectGuid guid, int32 type) override
        {
            _eventStarterGuid = guid;
            switch (type)
            {
                case -ACTION_START_RP_EVENT1:
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, _positions[RP1_UTHER_SPAWN], TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->setActive(true);
                        uther->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_UTHER1, RP1_CHAIN_UTHER1, false);
                    }
                    if (Creature* jaina = me->SummonCreature(NPC_JAINA, _positions[RP1_JAINA_SPAWN], TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        jaina->setActive(true);
                        jaina->GetMotionMaster()->MoveAlongSplineChain(0, RP1_CHAIN_JAINA1, true);
                    }
                    me->GetMotionMaster()->MoveAlongSplineChain(0, RP1_CHAIN_ARTHAS1, false);
                    break;
                case -ACTION_START_RP_EVENT2:
                    Talk(RP2_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_1, Seconds(9));
                    break;
                case -ACTION_START_RP_EVENT3:
                    Talk(RP3_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    me->GetMotionMaster()->MoveAlongSplineChain(RP3_POINTID_ARTHAS1, RP3_CHAIN_ARTHAS1, false);
                    break;
                case -ACTION_START_RP_EVENT4_1:
                    Talk(RP4_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    MoveAlongPath(me, RP4_ARTHAS_WP1, RP4_ARTHAS_WP19);
                    break;
                case -ACTION_START_RP_EVENT4_2:
                    Talk(RP4_LINE_ARTHAS10, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    MoveAlongPath(me, GAUNTLET_WP1, GAUNTLET_WP48);
                    break;
                case -ACTION_START_RP_EVENT5:
                    Talk(RP5_LINE_ARTHAS1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                    instance->instance->SummonCreature(NPC_MALGANIS, _positions[RP5_MALGANIS_POS]);
                    MoveAlongPath(me, RP5_ARTHAS_WP1, RP5_ARTHAS_WP22);
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (!_progressRP)
                return;
            if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE && type != SPLINE_CHAIN_MOTION_TYPE)
                return;
            switch (id)
            {
                case RP1_POINTID_UTHER1:
                    events.ScheduleEvent(RP1_EVENT_START1, Seconds(0));
                    events.ScheduleEvent(RP1_EVENT_START2, Seconds(1));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS1, Seconds(4));
                    events.ScheduleEvent(RP1_EVENT_UTHER1, Seconds(8));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS2, Seconds(15));
                    break;
                case RP1_POINTID_UTHER2:
                    events.ScheduleEvent(RP1_EVENT_UTHER_FACE, Seconds(0));
                    break;
                case RP1_POINTID_JAINA2:
                    events.ScheduleEvent(RP1_EVENT_JAINA_FACE, Seconds(0));
                    break;
                case RP1_POINTID_ARTHAS2:
                    events.ScheduleEvent(RP1_EVENT_ARTHAS3, Seconds(1));
                    events.ScheduleEvent(RP1_EVENT_UTHER2, Seconds(12));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_TURN, Seconds(13));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS4, Seconds(14));
                    events.ScheduleEvent(RP1_EVENT_UTHER3, Seconds(18));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS5, Seconds(24));
                    events.ScheduleEvent(RP1_EVENT_UTHER4, Seconds(30));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS6, Seconds(36));
                    events.ScheduleEvent(RP1_EVENT_UTHER5, Seconds(39));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS7, Seconds(44));
                    events.ScheduleEvent(RP1_EVENT_JAINA1, Seconds(57));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS8, Seconds(59));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS8_2, Seconds(65));
                    events.ScheduleEvent(RP1_EVENT_UTHER6, Seconds(71));
                    events.ScheduleEvent(RP1_EVENT_UTHER_LEAVE, Seconds(74));
                    events.ScheduleEvent(RP1_EVENT_JAINA_LEAVE, Seconds(75));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS9, Seconds(76));
                    events.ScheduleEvent(RP1_EVENT_JAINA2, Seconds(78));
                    events.ScheduleEvent(RP1_EVENT_JAINA_LEAVE2, Seconds(82));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_LEAVE, Seconds(88));
                    break;
                case RP1_POINTID_UTHER3:
                    if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 500.0f, true))
                        uther->DespawnOrUnsummon();
                    break;
                case RP1_POINTID_JAINA4:
                    if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 500.0f, true))
                        jaina->DespawnOrUnsummon();
                    break;
                case RP1_POINTID_ARTHAS3:
                    events.ScheduleEvent(RP1_EVENT_ARTHAS10, Seconds(0));
                    events.ScheduleEvent(RP1_EVENT_ARTHAS_LEAVE2, Seconds(12));
                    break;
                case RP1_POINTID_ARTHAS4:
                    events.ScheduleEvent(RP1_EVENT_FINISHED, Seconds(0));
                    break;
                case RP2_POINTID_ARTHAS1:
                    if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                        citizen->SetFacingToObject(me);
                    if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT, 100.0f, true))
                        resident->SetFacingToObject(me);
                    events.ScheduleEvent(RP2_EVENT_CITIZEN1, Seconds(2));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS2, Seconds(10));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_2, Seconds(11));
                    break;
                case RP2_POINTID_ARTHAS2:
                    events.ScheduleEvent(RP2_EVENT_CITIZEN2, Seconds(0));
                    events.ScheduleEvent(RP2_EVENT_KILL1, Seconds(1));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_3, Seconds(3));
                    break;
                case RP2_POINTID_ARTHAS3:
                    events.ScheduleEvent(RP2_EVENT_KILL2, Seconds(1));
                    events.ScheduleEvent(RP2_EVENT_REACT1, Seconds(2));
                    events.ScheduleEvent(RP2_EVENT_REACT2, Seconds(3));
                    events.ScheduleEvent(RP2_EVENT_REACT3, Seconds(4));
                    events.ScheduleEvent(RP2_EVENT_REACT4, Seconds(6));
                    events.ScheduleEvent(RP2_EVENT_REACT5, Seconds(6));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_4, Seconds(4));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS3, Seconds(6));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS1, Seconds(10));
                    events.ScheduleEvent(RP2_EVENT_TROOPS_FACE, Seconds(11));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_FACE, Seconds(13));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS2, Seconds(22));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS_LEAVE1, Seconds(34));
                    events.ScheduleEvent(RP2_EVENT_MALGANIS_LEAVE2, Seconds(35));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS4, Seconds(35));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS4_2, Seconds(39));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS_MOVE_5, Seconds(45));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5, Seconds(45));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5_2, Seconds(51));
                    events.ScheduleEvent(RP2_EVENT_ARTHAS5_3, Seconds(57));
                    events.ScheduleEvent(RP2_EVENT_WAVE_START, Seconds(64));
                    break;
                case RP3_POINTID_TOWN_HALL:
                    events.ScheduleEvent(EVENT_TOWN_HALL_REACHED, Seconds(1));
                    break;
                case RP3_POINTID_ARTHAS1:
                {
                    std::list<Creature*> infinites;
                    me->GetCreatureListWithEntryInGrid(infinites, NPC_CITIZEN_INFINITE, 100.0f);
                    for (Creature* infinite : infinites)
                        infinite->SetFacingToObject(me);
                    events.ScheduleEvent(RP3_EVENT_RESIDENT_FACE, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_FACE, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_CITIZEN1, Seconds(3));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS2, Seconds(12));
                    break;
                }
                case RP3_POINTID_ARTHAS2:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_KILL, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_INFINITE_LAUGH, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS3, Seconds(6));
                    events.ScheduleEvent(RP3_EVENT_CITIZEN2, Seconds(8));
                    events.ScheduleEvent(RP3_EVENT_TRANSFORM1, Seconds(10));
                    events.ScheduleEvent(RP3_EVENT_TRANSFORM2, Seconds(12));
                    events.ScheduleEvent(RP3_EVENT_TRANSFORM3, Seconds(14));
                    events.ScheduleEvent(RP3_EVENT_AGGRO, Seconds(15));
                    break;
                case RP3_POINTID_ARTHAS3:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_MOVE_1_2, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_SPAWN1, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_SPAWN1_FACE, Seconds(5));
                    events.ScheduleEvent(RP3_EVENT_SPAWN1_AGGRO, Seconds(7));
                    break;
                case RP3_POINTID_ARTHAS4:
                    events.ScheduleEvent(RP3_EVENT_ARTHAS20, Seconds(1));
                    events.ScheduleEvent(RP3_EVENT_SPAWN2, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS_FACE2, Seconds(3));
                    events.ScheduleEvent(RP3_EVENT_SPAWN2_FACE, Seconds(4));
                    events.ScheduleEvent(RP3_EVENT_SPAWN2_AGGRO, Seconds(6));
                    break;
                case RP3_POINTID_ARTHAS5:
                    events.ScheduleEvent(RP3_EVENT_SPAWN3, Seconds(2));
                    events.ScheduleEvent(RP3_EVENT_ARTHAS30, Seconds(4));
                    events.ScheduleEvent(RP3_EVENT_SPAWN3_FACE, Seconds(5));
                    events.ScheduleEvent(RP3_EVENT_SPAWN3_AGGRO, Seconds(6));
                    break;
                case RP4_ARTHAS_WP19:
                    events.ScheduleEvent(RP4_EVENT_ARTHAS2, Seconds(1));
                    events.ScheduleEvent(RP4_EVENT_HIDDEN_PASSAGE, Seconds(4)); // @todo sniff timer
                    events.ScheduleEvent(RP4_EVENT_ARTHAS3, Seconds(5));
                    break;
                case ARTHAS_GAUNTLET_POS:
                    events.ScheduleEvent(RP4_EVENT_GAUNTLET_REACHED, Seconds(1));
                    break;
                case GAUNTLET_WP48:
                    events.ScheduleEvent(RP4_EVENT_ARTHAS11, Seconds(1));
                    events.ScheduleEvent(RP4_EVENT_ARTHAS_FACE, Seconds(5));
                    events.ScheduleEvent(RP4_EVENT_ARTHAS12, Seconds(21));
                    events.ScheduleEvent(RP4_EVENT_GAUNTLET_RESUME, Seconds(25));
                    break;
                case ARTHAS_GAUNTLET_END_POS:
                    events.ScheduleEvent(RP4_EVENT_ARTHAS13, Seconds(1));
                    events.ScheduleEvent(RP4_EVENT_GAUNTLET_DONE, Seconds(7));
                    break;
                case RP5_ARTHAS_WP22:
                    events.ScheduleEvent(RP5_EVENT_ARTHAS2, Seconds(1));
                    events.ScheduleEvent(RP5_EVENT_MALGANIS1, Seconds(6));
                    break;
                default:
                    break;
            }
        }

        bool CanAIAttack(Unit const* who) const override
        {
            if (me->HasReactState(REACT_AGGRESSIVE))
            {
                Position const& relativePos = me->IsInCombat() ? me->GetHomePosition() : me->GetPosition();
                // Don't let us chase too far from home
                if (relativePos.GetExactDist2d(who) > 30.0f)
                    return false;
            }
            return ScriptedAI::CanAIAttack(who);
        }

        void UpdateAICombat(uint32 diff)
        {
            if (me->HasReactState(REACT_PASSIVE))
                return;

            if (!UpdateVictim())
                return;

            if (HealthBelowPct(40))
                DoCastSelf(SPELL_HOLY_LIGHT);
            if (_exorcismCooldown <= diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    _exorcismCooldown = 0;
                else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_EXORCISM);
                    _exorcismCooldown = urandms(7, 14);
                }
            }
            else
                _exorcismCooldown -= diff;

            DoMeleeAttackIfReady();
        }

        void UpdateAI(uint32 diff) override
        {
            if (!_hadSetup && me->IsAlive())
            {
                _hadSetup = true;
                _progressRP = true;
                AdvanceToState(GetCurrentProgress());
            }

            if (me->IsInCombat())
            {
                UpdateAICombat(diff);
                return;
            }

            // EventMap is only used for RP handling. RP events are paused while in combat or leashing back to resume pos
            if (!_progressRP)
                return;

            events.Update(diff);
            while (uint32 event = events.ExecuteEvent())
            {
                uint32 talkerEntry = UINT_MAX, talkerLine = 0;
                switch (event)
                {
                    case RP1_EVENT_START1:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            jaina->SetFacingToObject(me);
                        break;
                    case RP1_EVENT_START2:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                        {
                            uther->SetFacingToObject(me);
                            me->SetFacingToObject(uther);
                        }
                        break;
                    case RP1_EVENT_ARTHAS1:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS1;
                        break;
                    case RP1_EVENT_UTHER1:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER1;
                        break;
                    case RP1_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS2;
                        me->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_ARTHAS2, RP1_CHAIN_ARTHAS2, true);
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            uther->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_UTHER2, RP1_CHAIN_UTHER2, true);
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            jaina->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_JAINA2, RP1_CHAIN_JAINA2, true);
                        break;
                    case RP1_EVENT_UTHER_FACE:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            uther->SetFacingToObject(me);
                        break;
                    case RP1_EVENT_JAINA_FACE:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            jaina->SetFacingToObject(me);
                        break;
                    case RP1_EVENT_ARTHAS3:
                    {
                        me->SetFacingTo(6.248279f); // @todo migrate
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS3;
                        std::list<Creature*> troops;
                        me->GetCreatureListWithEntryInGrid(troops, NPC_FOOTMAN, 100.0f);
                        me->GetCreatureListWithEntryInGrid(troops, NPC_SORCERESS, 100.0f);
                        me->GetCreatureListWithEntryInGrid(troops, NPC_KNIGHT, 100.0f);
                        me->GetCreatureListWithEntryInGrid(troops, NPC_PRIEST, 100.0f);
                        for (Creature* unit : troops)
                            if (unit->IsAlive())
                                unit->SetFacingToObject(me);
                        break;
                    }
                    case RP1_EVENT_UTHER2:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER2;
                        break;
                    case RP1_EVENT_ARTHAS4:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS4;
                        break;
                    case RP1_EVENT_UTHER3:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER3;
                        break;
                    case RP1_EVENT_ARTHAS_TURN:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            me->SetFacingToObject(uther);
                        break;
                    case RP1_EVENT_ARTHAS5:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS5;
                        break;
                    case RP1_EVENT_UTHER4:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER4;
                        break;
                    case RP1_EVENT_ARTHAS6:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS6;
                        break;
                    case RP1_EVENT_UTHER5:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER5;
                        break;
                    case RP1_EVENT_ARTHAS7:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS7;
                        break;
                    case RP1_EVENT_JAINA1:
                        talkerEntry = NPC_JAINA, talkerLine = RP1_LINE_JAINA1;
                        break;
                    case RP1_EVENT_ARTHAS8:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS8;
                        break;
                    case RP1_EVENT_ARTHAS8_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        break;
                    case RP1_EVENT_UTHER6:
                        talkerEntry = NPC_UTHER, talkerLine = RP1_LINE_UTHER6;
                        break;
                    case RP1_EVENT_UTHER_LEAVE:
                        if (Creature* uther = me->FindNearestCreature(NPC_UTHER, 100.0f, true))
                            uther->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_UTHER3, RP1_CHAIN_UTHER3, true);
                        break;
                    case RP1_EVENT_JAINA_LEAVE:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            jaina->GetMotionMaster()->MoveAlongSplineChain(0, RP1_CHAIN_JAINA3, true);
                        break;
                    case RP1_EVENT_ARTHAS9:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS9;
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            me->SetFacingToObject(jaina);
                        break;
                    case RP1_EVENT_JAINA2:
                        talkerEntry = NPC_JAINA, talkerLine = RP1_LINE_JAINA2;
                        break;
                    case RP1_EVENT_JAINA_LEAVE2:
                        if (Creature* jaina = me->FindNearestCreature(NPC_JAINA, 100.0f, true))
                            jaina->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_JAINA4, RP1_CHAIN_JAINA4, true);
                        break;
                    case RP1_EVENT_ARTHAS_LEAVE:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_ARTHAS3, RP1_CHAIN_ARTHAS3, true);
                        break;
                    case RP1_EVENT_ARTHAS10:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS10;
                        me->SetFacingTo(3.141593f); // @todo migrate
                        break;
                    case RP1_EVENT_ARTHAS_LEAVE2:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP1_POINTID_ARTHAS4, RP1_CHAIN_ARTHAS4, false);
                        break;
                    case RP1_EVENT_FINISHED:
                        talkerEntry = 0, talkerLine = RP1_LINE_ARTHAS11;
                        me->SetFacingTo(_positions[ARTHAS_PURGE_PENDING_POS].GetOrientation());
                        instance->SetData(DATA_UTHER_FINISHED, 1);
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_1:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP2_POINTID_ARTHAS1, RP2_CHAIN_ARTHAS1, true);
                        break;
                    case RP2_EVENT_CITIZEN1:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                        {
                            citizen->GetMotionMaster()->MoveAlongSplineChain(0, RP2_CHAIN_CITIZEN1, true);
                            citizen->AI()->Talk(RP2_LINE_CITIZEN1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                        }
                        break;
                    case RP2_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS2;
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_2:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP2_POINTID_ARTHAS2, RP2_CHAIN_ARTHAS2, true);
                        break;
                    case RP2_EVENT_CITIZEN2:
                        talkerEntry = NPC_CITIZEN, talkerLine = RP2_LINE_CITIZEN2;
                        break;
                    case RP2_EVENT_KILL1:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN, 100.0f, true))
                            DoCast(citizen, SPELL_CRUSADER_STRIKE);
                        if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT, 100.0f, true))
                        {
                            resident->SetFlag(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                            resident->AI()->Talk(RP2_LINE_RESIDENT1, ObjectAccessor::GetPlayer(*me, _eventStarterGuid));
                        }
                        break;
                    case RP2_EVENT_ARTHAS_MOVE_3:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP2_POINTID_ARTHAS3, RP2_CHAIN_ARTHAS3, true);
                        break;
                    case RP2_EVENT_KILL2:
                        if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT, 100.0f, true))
                            DoCast(resident, SPELL_CRUSADER_STRIKE);
                        break;
                    case RP2_EVENT_REACT1:
                    case RP2_EVENT_REACT2:
                    case RP2_EVENT_REACT3:
                    case RP2_EVENT_REACT4:
                    case RP2_EVENT_REACT5:
                    {
                        std::list<Creature*> nearbyVictims;
                        me->GetCreatureListWithEntryInGrid(nearbyVictims, urand(0, 1) ? NPC_CITIZEN : NPC_RESIDENT, 60.0f);
                        if (!nearbyVictims.empty())
                        {
                            std::list<Creature*>::iterator it = nearbyVictims.begin();
                            std::advance(it, urand(0, nearbyVictims.size()-1));
                            Emote emote;
                            switch (urand(0, 3))
                            {
                                case 0:
                                    emote = EMOTE_ONESHOT_TALK;
                                    break;
                                case 1:
                                    emote = EMOTE_ONESHOT_EXCLAMATION;
                                    break;
                                case 2:
                                    emote = EMOTE_ONESHOT_RUDE;
                                    break;
                                case 3:
                                    emote = EMOTE_ONESHOT_ROAR;
                                    break;
                            }
                            if ((*it)->IsAlive())
                                (*it)->HandleEmoteCommand(emote);
                        }
                        break;
                    }
                    case RP2_EVENT_ARTHAS_MOVE_4:
                        me->SetFacingTo(2.234021f); // @todo
                        break;
                    case RP2_EVENT_ARTHAS3:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS3;
                        break;
                    case RP2_EVENT_MALGANIS1:
                        if (Creature* bunny = me->FindNearestCreature(NPC_MALGANIS_BUNNY, 80.0f, true))
                            bunny->CastSpell(bunny, SPELL_SHADOWSTEP_VISUAL);
                        if (Creature* malganis = instance->instance->SummonCreature(NPC_MALGANIS, _positions[RP2_MALGANIS_POS]))
                        {
                            malganis->CastSpell(malganis, SPELL_SHADOWSTEP_VISUAL);
                            malganis->AI()->Talk(RP2_LINE_MALGANIS1);
                        }
                        break;
                    case RP2_EVENT_TROOPS_FACE:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                        {
                            std::list<Creature*> troops;
                            me->GetCreatureListWithEntryInGrid(troops, NPC_FOOTMAN, 50.0f);
                            me->GetCreatureListWithEntryInGrid(troops, NPC_PRIEST, 50.0f);
                            for (Creature* unit : troops)
                                if (unit->IsAlive())
                                    unit->SetFacingToObject(malganis);
                        }
                        break;
                    case RP2_EVENT_ARTHAS_FACE:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                            me->SetFacingToObject(malganis);
                        break;
                    case RP2_EVENT_MALGANIS2:
                        talkerEntry = NPC_MALGANIS, talkerLine = RP2_LINE_MALGANIS2;
                        break;
                    case RP2_EVENT_MALGANIS_LEAVE1:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                            malganis->CastSpell(malganis, SPELL_SHADOWSTEP_VISUAL);
                        break;
                    case RP2_EVENT_MALGANIS_LEAVE2:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 80.0f, true))
                            malganis->DespawnOrUnsummon(0);
                        if (Creature* bunny = me->FindNearestCreature(NPC_MALGANIS_BUNNY, 80.0f, true))
                            bunny->CastSpell(bunny, SPELL_SHADOWSTEP_VISUAL);
                        break;
                    case RP2_EVENT_ARTHAS4:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS4;
                        break;
                    case RP2_EVENT_ARTHAS4_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        break;
                    case RP2_EVENT_ARTHAS5:
                        talkerEntry = 0, talkerLine = RP2_LINE_ARTHAS5;
                        break;
                    case RP2_EVENT_ARTHAS5_2:
                    case RP2_EVENT_ARTHAS5_3:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        break;
                    case RP2_EVENT_WAVE_START:
                        instance->SetData(DATA_START_WAVES, 1);
                        break;
                    case EVENT_TOWN_HALL_REACHED:
                        me->SetFacingTo(_positions[ARTHAS_TOWN_HALL_POS].GetOrientation());
                        instance->SetData(DATA_REACH_TOWN_HALL, 1);
                        break;
                    case RP3_EVENT_RESIDENT_FACE:
                        if (Creature* infinite = me->FindNearestCreature(NPC_RESIDENT_INFINITE, 100.0f, true))
                            infinite->SetFacingToObject(me);
                        break;
                    case RP3_EVENT_ARTHAS_FACE:
                        me->SetFacingTo(0.541052f); // @todo
                        break;
                    case RP3_EVENT_CITIZEN1:
                        talkerEntry = NPC_CITIZEN_INFINITE, talkerLine = RP3_LINE_CITIZEN1;
                        break;
                    case RP3_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS2;
                        me->GetMotionMaster()->MoveAlongSplineChain(RP3_POINTID_ARTHAS2, RP3_CHAIN_ARTHAS2, true);
                        break;
                    case RP3_EVENT_ARTHAS_KILL:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN_INFINITE, 100.0f, true))
                            DoCast(citizen, SPELL_CRUSADER_STRIKE);
                        break;
                    case RP3_EVENT_INFINITE_LAUGH:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN_INFINITE, 100.0f, true))
                            citizen->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                        break;
                    case RP3_EVENT_ARTHAS3:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS3;
                        break;
                    case RP3_EVENT_CITIZEN2:
                        talkerEntry = NPC_CITIZEN_INFINITE, talkerLine = RP3_LINE_CITIZEN2;
                        break;
                    case RP3_EVENT_TRANSFORM1:
                    case RP3_EVENT_TRANSFORM3:
                        if (Creature* citizen = me->FindNearestCreature(NPC_CITIZEN_INFINITE, 100.0f, true))
                        {
                            citizen->CastSpell(citizen, SPELL_TRANSFORM_VISUAL);
                            citizen->UpdateEntry(NPC_INFINITE_HUNTER);
                        }
                        break;
                    case RP3_EVENT_TRANSFORM2:
                        if (Creature* resident = me->FindNearestCreature(NPC_RESIDENT_INFINITE, 100.0f, true))
                        {
                            resident->CastSpell(resident, SPELL_TRANSFORM_VISUAL);
                            resident->UpdateEntry(NPC_INFINITE_AGENT);
                        }
                        break;
                    case RP3_EVENT_AGGRO:
                        EngageInfinites();
                        ScheduleActionOOC(RP3_ACTION_AFTER_INITIAL);
                        break;
                    case RP3_EVENT_ARTHAS4:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS4;
                        break;
                    case RP3_EVENT_ARTHAS_MOVE_1:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP3_POINTID_ARTHAS3, RP3_CHAIN_ARTHAS3, false);
                        break;
                    case RP3_EVENT_ARTHAS_MOVE_1_2:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS10;
                        me->GetMotionMaster()->MoveAlongSplineChain(0, RP3_CHAIN_ARTHAS3_2, true);
                        break;
                    case RP3_EVENT_SPAWN1:
                        // creatures without path movement don't have create in 4.x sniff, todo check other versions
                        if (Creature* adversary = instance->instance->SummonCreature(NPC_INFINITE_ADVERSARY, _positions[RP3_SPAWN1_ADVERSARY1_SPAWN]))
                            MoveAlongPath(adversary, RP3_SPAWN1_ADVERSARY1_WP1, RP3_SPAWN1_ADVERSARY1_WP4, true);
                        instance->instance->SummonCreature(NPC_INFINITE_ADVERSARY, _positions[RP3_SPAWN1_ADVERSARY2_SPAWN]);
                        if (Creature* hunter = instance->instance->SummonCreature(NPC_INFINITE_HUNTER, _positions[RP3_SPAWN1_HUNTER1_SPAWN]))
                            MoveAlongPath(hunter, RP3_SPAWN1_HUNTER1_WP1, RP3_SPAWN1_HUNTER1_WP5, true);
                        instance->instance->SummonCreature(NPC_INFINITE_AGENT, _positions[RP3_SPAWN1_AGENT1_SPAWN]);
                        break;
                    case RP3_EVENT_SPAWN1_FACE:
                    case RP3_EVENT_SPAWN2_FACE:
                    case RP3_EVENT_SPAWN3_FACE:
                    {
                        std::list<Creature*> infinites;
                        me->GetCreatureListWithEntryInGrid(infinites, NPC_INFINITE_ADVERSARY, 100.0f);
                        me->GetCreatureListWithEntryInGrid(infinites, NPC_INFINITE_AGENT, 100.0f);
                        me->GetCreatureListWithEntryInGrid(infinites, NPC_INFINITE_HUNTER, 100.0f);
                        for (Creature* target : infinites)
                            if (target->IsAlive())
                                target->SetFacingToObject(me);
                        break;
                    }
                    case RP3_EVENT_SPAWN1_AGGRO:
                        EngageInfinites();
                        ScheduleActionOOC(RP3_ACTION_AFTER_SPAWN1);
                        break;
                    case RP3_EVENT_ARTHAS11:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS11;
                        break;
                    case RP3_EVENT_ARTHAS_MOVE_2:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP3_POINTID_ARTHAS4, RP3_CHAIN_ARTHAS4, false);
                        break;
                    case RP3_EVENT_ARTHAS20:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS20;
                        break;
                    case RP3_EVENT_SPAWN2:
                        // same as above, no path movement -> no sniff data in 4.x era sniff
                        instance->instance->SummonCreature(NPC_INFINITE_ADVERSARY, _positions[RP3_SPAWN2_ADVERSARY1_SPAWN]);
                        if (Creature* hunter = instance->instance->SummonCreature(NPC_INFINITE_HUNTER, _positions[RP3_SPAWN2_HUNTER1_SPAWN]))
                            MoveAlongPath(hunter, RP3_SPAWN2_HUNTER1_WP1, RP3_SPAWN2_HUNTER1_WP4, true);
                        instance->instance->SummonCreature(NPC_INFINITE_HUNTER, _positions[RP3_SPAWN2_HUNTER2_SPAWN]);
                        if (Creature* agent = instance->instance->SummonCreature(NPC_INFINITE_AGENT, _positions[RP3_SPAWN2_AGENT1_SPAWN]))
                            MoveAlongPath(agent, RP3_SPAWN2_AGENT1_WP1, RP3_SPAWN2_AGENT1_WP4, true);
                        break;
                    case RP3_EVENT_ARTHAS_FACE2:
                        me->SetFacingTo(1.762783f); // @todo
                        break;
                    case RP3_EVENT_SPAWN2_AGGRO:
                        EngageInfinites();
                        ScheduleActionOOC(RP3_ACTION_AFTER_SPAWN2);
                        break;
                    case RP3_EVENT_ARTHAS21:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS21;
                        break;
                    case RP3_EVENT_ARTHAS_MOVE_3:
                        me->GetMotionMaster()->MoveAlongSplineChain(RP3_POINTID_ARTHAS5, RP3_CHAIN_ARTHAS5, false);
                        break;
                    case RP3_EVENT_SPAWN3:
                        if (Creature* adversary = instance->instance->SummonCreature(NPC_INFINITE_ADVERSARY, _positions[RP3_SPAWN3_ADVERSARY1_SPAWN]))
                            MoveAlongPath(adversary, RP3_SPAWN3_ADVERSARY1_WP1, RP3_SPAWN3_ADVERSARY1_WP2, true);
                        if (Creature* hunter = instance->instance->SummonCreature(NPC_INFINITE_HUNTER, _positions[RP3_SPAWN3_HUNTER1_SPAWN]))
                            MoveAlongPath(hunter, RP3_SPAWN3_HUNTER1_WP1, RP3_SPAWN3_HUNTER1_WP6, true);
                        if (Creature* hunter = instance->instance->SummonCreature(NPC_INFINITE_HUNTER, _positions[RP3_SPAWN3_HUNTER2_SPAWN]))
                            MoveAlongPath(hunter, RP3_SPAWN3_HUNTER2_WP1, RP3_SPAWN3_HUNTER2_WP4, true);
                        if (Creature* agent = instance->instance->SummonCreature(NPC_INFINITE_AGENT, _positions[RP3_SPAWN3_AGENT1_SPAWN]))
                            MoveAlongPath(agent, RP3_SPAWN3_AGENT1_WP1, RP3_SPAWN3_AGENT1_WP3, true);
                        break;
                    case RP3_EVENT_ARTHAS30:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS30;
                        break;
                    case RP3_EVENT_SPAWN3_AGGRO:
                        EngageInfinites();
                        ScheduleActionOOC(RP3_ACTION_AFTER_SPAWN3);
                        break;
                    case RP3_EVENT_EPOCH_SPAWN:
                        if (Creature* epoch = instance->instance->SummonCreature(NPC_EPOCH, _positions[RP3_EPOCH_SPAWN]))
                            epoch->GetMotionMaster()->MoveAlongSplineChain(0, RP3_CHAIN_EPOCH, false);
                        break;
                    case RP3_EVENT_ARTHAS31:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS31;
                        me->SetFacingTo(6.073746f); // @todo
                        break;
                    case RP3_EVENT_EPOCH_FACE:
                        if (Creature* epoch = me->FindNearestCreature(NPC_EPOCH, 100.0f, true))
                            epoch->SetFacingToObject(me);
                        break;
                    case RP3_EVENT_EPOCH1:
                        talkerEntry = NPC_EPOCH, talkerLine = RP3_LINE_EPOCH1;
                        break;
                    case RP3_EVENT_ARTHAS32:
                        talkerEntry = 0, talkerLine = RP3_LINE_ARTHAS32;
                        break;
                    case RP3_EVENT_ARTHAS32_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NO_SHEATHE);
                        break;
                    case RP3_EVENT_EPOCH_AGGRO:
                        talkerEntry = NPC_EPOCH, talkerLine = RP3_LINE_EPOCH2;
                        if (Creature* epoch = me->FindNearestCreature(NPC_EPOCH, 100.0f, true))
                        {
                            epoch->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                            me->SetInCombatWith(epoch);
                            epoch->SetInCombatWith(me);
                            me->AddThreat(epoch, 0.0f);
                            epoch->AddThreat(me, 0.0f);
                        }
                        ScheduleActionOOC(RP3_ACTION_AFTER_EPOCH);
                        break;
                    case RP4_EVENT_ARTHAS2:
                        me->SetFacingTo(_positions[RP4_ARTHAS_WP19].GetOrientation());
                        talkerEntry = 0, talkerLine = RP4_LINE_ARTHAS2;
                        break;
                    case RP4_EVENT_HIDDEN_PASSAGE:
                        if (GameObject* passage = me->FindNearestGameObject(GO_HIDDEN_PASSAGE, 100.0f))
                            passage->SetGoState(GO_STATE_ACTIVE);
                        break;
                    case RP4_EVENT_ARTHAS3:
                        talkerEntry = 0, talkerLine = RP4_LINE_ARTHAS3;
                        MoveAlongPath(me, RP4_ARTHAS_WP30, ARTHAS_GAUNTLET_POS, true);
                        break;
                    case RP4_EVENT_GAUNTLET_REACHED:
                        me->SetFacingTo(_positions[ARTHAS_GAUNTLET_POS].GetOrientation());
                        instance->SetData(DATA_GAUNTLET_REACHED, 1);
                        break;
                    case RP4_EVENT_ARTHAS11:
                        talkerEntry = 0, talkerLine = RP4_LINE_ARTHAS11;
                        break;
                    case RP4_EVENT_ARTHAS_FACE:
                        me->SetFacingTo(_positions[GAUNTLET_WP48].GetOrientation());
                        break;
                    case RP4_EVENT_ARTHAS12:
                        talkerEntry = 0, talkerLine = RP4_LINE_ARTHAS12;
                        break;
                    case RP4_EVENT_GAUNTLET_RESUME:
                        MoveAlongPath(me, GAUNTLET_WP50, ARTHAS_GAUNTLET_END_POS);
                        break;
                    case RP4_EVENT_ARTHAS13:
                        talkerEntry = 0, talkerLine = RP4_LINE_ARTHAS13;
                        me->SetFacingTo(_positions[ARTHAS_GAUNTLET_END_POS].GetOrientation());
                        break;
                    case RP4_EVENT_GAUNTLET_DONE:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NO_SHEATHE);
                        instance->SetData(DATA_GAUNTLET_DONE, 1);
                        break;
                    case RP5_EVENT_ARTHAS2:
                        talkerEntry = 0, talkerLine = RP5_LINE_ARTHAS2;
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 100.0f, true))
                            me->SetFacingToObject(malganis);
                        break;
                    case RP5_EVENT_MALGANIS1:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 100.0f, true))
                        {
                            malganis->AI()->Talk(RP5_LINE_MALGANIS1, ObjectAccessor::GetPlayer(*malganis, _eventStarterGuid));
                            malganis->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                            me->SetInCombatWith(malganis);
                            malganis->SetInCombatWith(me);
                            me->AddThreat(malganis, 0.0f);
                            malganis->AddThreat(me, 0.0f);
                        }
                        ScheduleActionOOC(RP5_ACTION_AFTER_MALGANIS);
                        break;
                    case RP5_EVENT_MALGANIS_DONE:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 100.0f, true))
                        {
                            malganis->SetFacingToObject(me);
                            malganis->CastSpell(malganis, SPELL_MALGANIS_QUEST_CREDIT, true);
                            malganis->CastSpell(malganis, SPELL_MALGANIS_KILL_CREDIT, true);
                            if (GameObject* chest = malganis->FindNearestGameObject(RAID_MODE(GO_CHEST_NORMAL, GO_CHEST_HEROIC), 100.0f))
                                chest->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        }
                        instance->SetBossState(DATA_MAL_GANIS, DONE);
                        instance->SetData(DATA_MALGANIS_DONE, 1);
                        events.ScheduleEvent(RP5_EVENT_MALGANIS12, Seconds(3));
                        events.ScheduleEvent(RP5_EVENT_MALGANIS_LEAVE, Seconds(19));
                        events.ScheduleEvent(RP5_EVENT_ARTHAS10, Seconds(20));
                        events.ScheduleEvent(RP5_EVENT_ARTHAS10_2, Seconds(27));
                        events.ScheduleEvent(RP5_EVENT_ARTHAS11, Seconds(30));
                        events.ScheduleEvent(RP5_EVENT_ARTHAS11_2, Seconds(35));
                        events.ScheduleEvent(RP5_EVENT_CHROMIE_SPAWN, Seconds(65));
                        events.ScheduleEvent(RP5_EVENT_CHROMIE_LAND, Seconds(76)+Milliseconds(668));
                        events.ScheduleEvent(RP5_EVENT_CHROMIE_TRANSFORM, Seconds(79)+Milliseconds(491));
                        break;
                    case RP5_EVENT_MALGANIS12:
                        talkerEntry = NPC_MALGANIS, talkerLine = RP5_LINE_MALGANIS12;
                        break;
                    case RP5_EVENT_MALGANIS_LEAVE:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 100.0f, true))
                            malganis->CastSpell(malganis, SPELL_SHADOWSTEP_VISUAL);
                        break;
                    case RP5_EVENT_ARTHAS10:
                        if (Creature* malganis = me->FindNearestCreature(NPC_MALGANIS, 100.0f, true))
                            malganis->DespawnOrUnsummon();
                        talkerEntry = 0, talkerLine = RP5_LINE_ARTHAS10;
                        break;
                    case RP5_EVENT_ARTHAS10_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        break;
                    case RP5_EVENT_ARTHAS11:
                        talkerEntry = 0, talkerLine = RP5_LINE_ARTHAS11;
                        break;
                    case RP5_EVENT_ARTHAS11_2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        break;
                    case RP5_EVENT_CHROMIE_SPAWN:
                        if (Creature* chromie = instance->instance->SummonCreature(NPC_CHROMIE_3, _positions[RP5_CHROMIE_SPAWN]))
                        {
                            // SMOOOOOOOOOOOOOOOOTH AF custom movespline
                            Movement::MoveSplineInit init(chromie);
                            init.Path().push_back(_positions[RP5_CHROMIE_WP1]);
                            init.Path().push_back(_positions[RP5_CHROMIE_WP2]);
                            init.Path().push_back(_positions[RP5_CHROMIE_WP3]);
                            init.Path().push_back(_positions[RP5_CHROMIE_WP3]);
                            init.SetFly();
                            init.SetWalk(true);
                            init.Launch();
                        }
                        break;
                    case RP5_EVENT_CHROMIE_LAND:
                        if (Creature* chromie = me->FindNearestCreature(NPC_CHROMIE_3, 100.0f, true))
                            chromie->SetByteValue(UNIT_FIELD_BYTES_1, 3, 0);
                        break;
                    case RP5_EVENT_CHROMIE_TRANSFORM:
                        if (Creature* chromie = me->FindNearestCreature(NPC_CHROMIE_3, 100.0f, true))
                        {
                            chromie->CastSpell(chromie, SPELL_CHROMIE_3_TRANSFORM);
                            chromie->AI()->Talk(RP5_LINE_CHROMIE0);
                            chromie->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                        }
                    default:
                        break;
                }
                
                if (talkerEntry != UINT_MAX)
                {
                    Creature* talker;
                    if (talkerEntry)
                        talker = me->FindNearestCreature(talkerEntry, 100.0f, true);
                    else
                        talker = me;
                    if (talker)
                        talker->AI()->Talk(talkerLine, ObjectAccessor::GetPlayer(*talker, _eventStarterGuid));
                }
            }
        }

        void EngageInfinites()
        {
            std::list<Creature*> infinites;
            me->GetCreatureListWithEntryInGrid(infinites, NPC_INFINITE_ADVERSARY, 100.0f);
            me->GetCreatureListWithEntryInGrid(infinites, NPC_INFINITE_AGENT, 100.0f);
            me->GetCreatureListWithEntryInGrid(infinites, NPC_INFINITE_HUNTER, 100.0f);
            for (Creature* target : infinites)
            {
                target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetInCombatWith(target);
                target->SetInCombatWith(me);
                me->AddThreat(target, 0.0f);
                target->AddThreat(me, 0.0f);
            }
        }

        void KilledUnit(Unit* who) override
        {
            if (who && who->GetEntry() == NPC_RISEN_ZOMBIE)
                Talk(LINE_SLAY_ZOMBIE, who);
        }

        void EnterCombat(Unit* who) override
        {
            std::cout << "EnterCombat " << _progressRP << std::endl;
            if (_progressRP)
            {
                _progressRP = false;
                me->SetHomePosition(me->GetPosition());
                MotionMaster const* mm = me->GetMotionMaster();
                if (MovementGenerator const* activeGen = mm->GetMotionSlot(MOTION_SLOT_ACTIVE))
                    if (activeGen->GetMovementGeneratorType() == SPLINE_CHAIN_MOTION_TYPE)
                    {
                        std::cout << "Arthas AI: spline chain motion paused" << std::endl;
                        _resumeMovement = reinterpret_cast<SplineChainMovementGenerator const*>(activeGen)->GetResumeInfo(me);
                    }
                    else
                        std::cout << "Arthas AI: entered combat, but active movement gen is of type " << activeGen->GetMovementGeneratorType() << " != spline chain." << std::endl;
                else
                    std::cout << "Arthas AI: entered combat without pathing, pausing RP regardless" << std::endl;
            }
            ScriptedAI::EnterCombat(who);
        }

        void EnterEvadeMode(EvadeReason why) override
        {
            std::cout << "EnterEvadeMode " << why << std::endl;
            ScriptedAI::EnterEvadeMode(why);
        }

        void JustReachedHome() override
        {
            _progressRP = true;
            if (_resumeMovement.Chain) // WP motion was interrupted, resume
            {
                std::cout << "Arthas AI: Resuming motion" << std::endl;
                me->GetMotionMaster()->ResumeSplineChain(_resumeMovement);
                _resumeMovement.Chain = nullptr;
            }
            else
                std::cout << "Arthas AI: Back at leash pos, resuming RP" << std::endl;

            if (_afterCombat)
            {
                DoAction(_afterCombat);
                _afterCombat = ACTION_NONE;
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            // Instance failure: regress back to last stable state
            instance->SetData(DATA_ARTHAS_DIED, 1);
        }

        void JustRespawned() override
        {
            // Reset waypoint and scheduling info
            _afterCombat = ACTION_NONE;
            _resumeMovement.Chain = nullptr;

            // Re-initialize on next tick
            _hadSetup = false;
        }

        private:
            bool _hadSetup; // on first update tick, adjust to current progress state
            InstanceScript* const instance;
            EventMap events;
            ObjectGuid _eventStarterGuid;
            uint32 _exorcismCooldown; // no EventMap entry for this, it's reserved for RP handling

            bool _progressRP;
            Actions _afterCombat;
            SplineChainResumeInfo _resumeMovement;
    };

    void AdvanceDungeon(Creature* creature, Player* cause, ProgressStates from, InstanceData command)
    {
        if (InstanceScript* instance = creature->GetInstanceScript())
            if (instance->GetData(DATA_INSTANCE_PROGRESS) == from)
                instance->SetGuidData(command, cause->GetGUID());
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        std::cout << (player ? player->GetName() : "nullptr") << " select " << action << " on " << (creature ? creature->GetName() : "nullptr") << std::endl;
        AdvanceDungeon(creature, player, PURGE_PENDING, DATA_START_PURGE);
        AdvanceDungeon(creature, player, TOWN_HALL_PENDING, DATA_START_TOWN_HALL);
        AdvanceDungeon(creature, player, TOWN_HALL_COMPLETE, DATA_TO_GAUNTLET);
        AdvanceDungeon(creature, player, GAUNTLET_PENDING, DATA_START_GAUNTLET);
        AdvanceDungeon(creature, player, GAUNTLET_COMPLETE, DATA_START_MALGANIS);
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        std::cout << (player ? player->GetName() : "nullptr") << " hello " << (creature ? creature->GetName() : "nullptr") << std::endl;
        return false;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetInstanceAI<npc_arthas_stratholmeAI>(creature);
    }
};

// @todo sniff
const std::array<Position, NUM_POSITIONS> npc_arthas_stratholme::npc_arthas_stratholmeAI::_positions = { {
    { 1983.857f, 1287.043f, 145.5596f, 3.0892330f }, // RP1_ARTHAS_INITIAL
    { 1783.843f, 1267.481f, 139.7800f, 0.2698664f }, // RP1_UTHER_SPAWN
    { 1876.788f, 1305.723f, 146.2474f, 6.0737460f }, // RP1_JAINA_SPAWN

    { 2047.948f, 1287.598f, 142.8568f, 3.176499f }, // ARTHAS_PURGE_PENDING_POS
    { 2091.994f, 1277.257f, 140.4707f, 1.134464f }, // ARTHAS_WAVES_POS // @todo
    { 2074.624f, 1282.958f, 141.6344f }, // RP2_PRIEST1_POS
    { 2074.805f, 1292.172f, 141.6728f }, // RP2_PRIEST2_POS
    { 2077.590f, 1284.609f, 141.5710f }, // RP2_FOOT1_POS
    { 2078.365f, 1281.254f, 141.5182f }, // RP2_FOOT2_POS
    { 2077.737f, 1290.441f, 141.5698f }, // RP2_FOOT3_POS
    { 2078.055f, 1293.624f, 141.5544f }, // RP2_FOOT4_POS
    { 2113.454f, 1287.986f, 136.3829f, 3.071779f }, // RP2_MALGANIS_POS

    { 2366.240f, 1195.253f, 132.0441f, 3.159046f }, // ARTHAS_TOWN_HALL_POS
    { 2433.040f, 1191.160f, 148.1285f, 4.998989f }, // RP3_SPAWN1_ADVERSARY1_SPAWN
    { 2432.566f, 1193.102f, 148.1593f }, // RP3_SPAWN1_ADVERSARY1_WP1
    { 2432.757f, 1192.121f, 148.1593f }, // RP3_SPAWN1_ADVERSARY1_WP2
    { 2433.525f, 1189.516f, 148.0759f }, // RP3_SPAWN1_ADVERSARY1_WP3
    { 2434.290f, 1185.112f, 148.0759f }, // RP3_SPAWN1_ADVERSARY1_WP4
    { 2437.091f, 1188.969f, 148.0759f }, // RP3_SPAWN1_ADVERSARY2_SPAWN
    { 2433.176f, 1193.429f, 148.1162f, 1.215144f }, // RP3_SPAWN1_HUNTER1_SPAWN
    { 2432.270f, 1191.664f, 148.1593f }, // RP3_SPAWN1_HUNTER1_WP1
    { 2432.829f, 1192.493f, 148.1593f }, // RP3_SPAWN1_HUNTER1_WP2
    { 2433.502f, 1194.306f, 148.0759f }, // RP3_SPAWN1_HUNTER1_WP3
    { 2433.700f, 1194.840f, 148.0759f }, // RP3_SPAWN1_HUNTER1_WP4
    { 2436.273f, 1198.183f, 148.0759f }, // RP3_SPAWN1_HUNTER1_WP5
    { 2438.529f, 1192.707f, 148.0579f }, // RP3_SPAWN1_AGENT_SPAWN
    { 2415.398f, 1140.661f, 148.0759f }, // RP3_SPAWN2_ADVERSARY1_SPAWN
    { 2404.376f, 1179.393f, 148.1379f, 5.141369f }, // RP3_SPAWN2_HUNTER1_SPAWN
    { 2404.736f, 1179.674f, 148.1587f }, // RP3_SPAWN2_HUNTER1_WP1
    { 2403.959f, 1180.304f, 148.1587f }, // RP3_SPAWN2_HUNTER1_WP2
    { 2405.620f, 1176.671f, 148.0759f }, // RP3_SPAWN2_HUNTER1_WP3
    { 2407.341f, 1172.870f, 148.0759f }, // RP3_SPAWN2_HUNTER1_WP4
    { 2411.154f, 1140.153f, 148.0759f }, // RP3_SPAWN2_HUNTER2_SPAWN
    { 2403.785f, 1179.004f, 148.1381f, 4.588950f }, // RP3_SPAWN2_AGENT1_SPAWN
    { 2403.891f, 1180.996f, 148.1587f }, // RP3_SPAWN2_AGENT1_WP1
    { 2403.908f, 1179.996f, 148.1587f }, // RP3_SPAWN2_AGENT1_WP2
    { 2403.413f, 1176.008f, 148.0759f }, // RP3_SPAWN2_AGENT1_WP3
    { 2403.444f, 1172.043f, 148.0759f }, // RP3_SPAWN2_AGENT1_WP4
    { 2425.898f, 1118.842f, 148.0759f, 6.073746f }, // ARTHAS_TOWN_HALL_END_POS
    { 2429.303f, 1102.002f, 148.1593f }, // RP3_SPAWN3_ADVERSARY1_SPAWN
    { 2428.320f, 1104.574f, 148.1176f }, // RP3_SPAWN3_ADVERSARY1_WP1
    { 2429.303f, 1102.002f, 148.1593f }, // RP3_SPAWN3_ADVERSARY1_WP2
    { 2442.348f, 1114.049f, 148.1413f, 2.183568f }, // RP3_SPAWN3_HUNTER1_SPAWN
    { 2442.718f, 1112.450f, 148.1593f }, // RP3_SPAWN3_HUNTER1_WP1
    { 2442.771f, 1113.448f, 148.1593f }, // RP3_SPAWN3_HUNTER1_WP2
    { 2440.811f, 1116.235f, 148.0759f }, // RP3_SPAWN3_HUNTER1_WP3
    { 2439.670f, 1117.256f, 148.0759f }, // RP3_SPAWN3_HUNTER1_WP4
    { 2438.849f, 1117.990f, 148.0759f }, // RP3_SPAWN3_HUNTER1_WP5
    { 2438.027f, 1118.726f, 148.0759f }, // RP3_SPAWN3_HUNTER1_WP6
    { 2430.089f, 1103.095f, 148.1353f, 0.905703f }, // RP3_SPAWN3_HUNTER2_SPAWN
    { 2429.549f, 1101.521f, 148.1593f }, // RP3_SPAWN3_HUNTER2_WP1
    { 2429.636f, 1102.517f, 148.1593f }, // RP3_SPAWN3_HUNTER2_WP2
    { 2431.217f, 1104.532f, 148.0759f }, // RP3_SPAWN3_HUNTER2_WP3
    { 2432.575f, 1108.833f, 148.0759f }, // RP3_SPAWN3_HUNTER2_WP4
    { 2442.031f, 1113.648f, 148.1593f }, // RP3_SPAWN3_AGENT1_SPAWN
    { 2440.408f, 1113.748f, 148.1176f }, // RP3_SPAWN3_AGENT1_WP1
    { 2436.158f, 1113.748f, 148.1176f }, // RP3_SPAWN3_AGENT1_WP2
    { 2442.031f, 1113.648f, 148.1593f }, // RP3_SPAWN3_AGENT1_WP3
    { 2457.008f, 1113.929f, 150.0776f, 3.272437f }, // RP3_EPOCH_SPAWN

    { 2431.960f, 1116.705f, 148.0759f }, // RP4_ARTHAS_WP1
    { 2433.980f, 1116.161f, 148.5607f }, // RP4_ARTHAS_WP2
    { 2439.980f, 1114.161f, 148.5607f }, // RP4_ARTHAS_WP3
    { 2441.157f, 1113.872f, 148.0759f }, // RP4_ARTHAS_WP4
    { 2448.360f, 1113.663f, 148.5421f }, // RP4_ARTHAS_WP5
    { 2450.621f, 1113.146f, 148.9136f }, // RP4_ARTHAS_WP6
    { 2451.243f, 1113.355f, 149.4610f }, // RP4_ARTHAS_WP7
    { 2451.993f, 1114.105f, 149.4610f }, // RP4_ARTHAS_WP8
    { 2454.493f, 1117.355f, 150.2110f }, // RP4_ARTHAS_WP9
    { 2455.493f, 1118.605f, 150.2110f }, // RP4_ARTHAS_WP10
    { 2455.993f, 1119.105f, 150.2110f }, // RP4_ARTHAS_WP11
    { 2456.407f, 1120.309f, 150.0083f }, // RP4_ARTHAS_WP12
    { 2457.265f, 1122.334f, 150.2657f }, // RP4_ARTHAS_WP13
    { 2458.015f, 1126.084f, 150.2657f }, // RP4_ARTHAS_WP14
    { 2459.765f, 1126.084f, 150.2657f }, // RP4_ARTHAS_WP15
    { 2462.765f, 1126.084f, 150.2657f }, // RP4_ARTHAS_WP16
    { 2464.515f, 1125.334f, 150.2657f }, // RP4_ARTHAS_WP17
    { 2466.265f, 1124.334f, 150.2657f }, // RP4_ARTHAS_WP18
    { 2468.624f, 1123.360f, 150.0230f, 0.226893f }, // RP4_ARTHAS_WP19
    { 2474.084f, 1123.945f, 150.2138f }, // RP4_ARTHAS_WP30
    { 2476.334f, 1124.195f, 150.2138f }, // RP4_ARTHAS_WP31
    { 2478.181f, 1123.791f, 149.9747f }, // RP4_ARTHAS_WP32
    { 2482.852f, 1122.718f, 150.1898f }, // RP4_ARTHAS_WP33
    { 2484.102f, 1122.468f, 150.1898f }, // RP4_ARTHAS_WP34
    { 2484.602f, 1119.718f, 150.1898f }, // RP4_ARTHAS_WP35
    { 2484.645f, 1118.929f, 149.9047f }, // RP4_ARTHAS_WP36
    { 2485.715f, 1113.623f, 150.1306f }, // RP4_ARTHAS_WP37
    { 2486.174f, 1109.669f, 148.9116f }, // RP4_ARTHAS_WP38
    { 2486.680f, 1107.990f, 148.2771f }, // RP4_ARTHAS_WP39
    { 2487.430f, 1103.740f, 145.7771f }, // RP4_ARTHAS_WP40
    { 2487.570f, 1101.130f, 144.9768f }, // RP4_ARTHAS_WP41
    { 2488.088f, 1100.423f, 144.9704f }, // RP4_ARTHAS_WP42
    { 2488.088f, 1100.173f, 144.9704f }, // RP4_ARTHAS_WP43
    { 2491.088f, 1100.673f, 144.9704f }, // RP4_ARTHAS_WP44
    { 2492.838f, 1101.173f, 144.7204f }, // RP4_ARTHAS_WP45
    { 2498.088f, 1102.173f, 144.7204f }, // RP4_ARTHAS_WP46
    { 2497.338f, 1105.923f, 144.2204f }, // RP4_ARTHAS_WP47
    { 2496.338f, 1110.923f, 143.9704f }, // RP4_ARTHAS_WP48
    { 2495.088f, 1115.923f, 143.9704f }, // RP4_ARTHAS_WP49
    { 2494.613f, 1118.074f, 142.8223f }, // RP4_ARTHAS_WP50
    { 2494.454f, 1120.003f, 142.1522f }, // RP4_ARTHAS_WP51
    { 2493.454f, 1124.533f, 140.4022f }, // RP4_ARTHAS_WP52
    { 2492.678f, 1126.886f, 139.9672f }, // RP4_ARTHAS_WP53
    { 2492.797f, 1127.180f, 140.2227f }, // RP4_ARTHAS_WP54
    { 2496.797f, 1128.180f, 140.2227f }, // RP4_ARTHAS_WP55
    { 2498.297f, 1128.430f, 140.2227f }, // RP4_ARTHAS_WP56
    { 2500.797f, 1128.930f, 140.2227f }, // RP4_ARTHAS_WP57
    { 2501.040f, 1128.099f, 139.9817f }, // RP4_ARTHAS_WP58
    { 2503.642f, 1119.790f, 140.0229f }, // RP4_ARTHAS_WP59
    { 2505.142f, 1120.040f, 140.0229f }, // RP4_ARTHAS_WP60
    { 2506.892f, 1120.290f, 140.0229f }, // RP4_ARTHAS_WP61
    { 2510.642f, 1121.040f, 137.0229f }, // RP4_ARTHAS_WP62
    { 2512.219f, 1121.014f, 135.7001f }, // RP4_ARTHAS_WP63
    { 2516.774f, 1121.972f, 132.8931f }, // RP4_ARTHAS_WP64
    { 2518.024f, 1122.222f, 132.3931f }, // RP4_ARTHAS_WP65
    { 2521.524f, 1123.722f, 132.3931f }, // RP4_ARTHAS_WP66
    { 2523.830f, 1124.929f, 132.0862f }, // RP4_ARTHAS_WP67

    { 2534.988f, 1126.163f, 130.8621f, 0.541850f }, // ARTHAS_GAUNTLET_POS (orientation not sniffed)
    { 2537.521f, 1127.299f, 130.8223f }, // GAUNTLET_WP1
    { 2539.021f, 1128.049f, 130.8223f }, // GAUNTLET_WP2
    { 2540.271f, 1128.549f, 131.0723f }, // GAUNTLET_WP3
    { 2540.771f, 1128.799f, 131.0723f }, // GAUNTLET_WP4
    { 2543.521f, 1130.299f, 130.8223f }, // GAUNTLET_WP5
    { 2546.771f, 1132.299f, 130.3223f }, // GAUNTLET_WP6
    { 2549.521f, 1134.049f, 130.0723f }, // GAUNTLET_WP7
    { 2551.760f, 1135.288f, 129.7018f }, // GAUNTLET_WP8
    { 2552.974f, 1136.249f, 129.6362f }, // GAUNTLET_WP9
    { 2557.724f, 1139.249f, 128.8862f }, // GAUNTLET_WP10
    { 2560.974f, 1146.999f, 128.3862f }, // GAUNTLET_WP11
    { 2562.618f, 1150.176f, 127.8944f }, // GAUNTLET_WP12
    { 2564.146f, 1153.992f, 127.5698f }, // GAUNTLET_WP13
    { 2566.646f, 1159.492f, 127.3198f }, // GAUNTLET_WP14
    { 2565.646f, 1169.742f, 127.0698f }, // GAUNTLET_WP15
    { 2564.724f, 1178.042f, 126.4597f }, // GAUNTLET_WP16
    { 2564.493f, 1180.685f, 126.6174f }, // GAUNTLET_WP17
    { 2564.243f, 1183.435f, 126.3674f }, // GAUNTLET_WP18
    { 2562.993f, 1197.185f, 126.1174f }, // GAUNTLET_WP19
    { 2562.821f, 1197.190f, 125.8900f }, // GAUNTLET_WP20
    { 2562.558f, 1199.721f, 126.0331f }, // GAUNTLET_WP21
    { 2562.308f, 1202.971f, 126.0331f }, // GAUNTLET_WP22
    { 2560.308f, 1207.721f, 125.7831f }, // GAUNTLET_WP23
    { 2557.124f, 1215.418f, 125.8095f }, // GAUNTLET_WP24
    { 2556.610f, 1217.303f, 126.0669f }, // GAUNTLET_WP25
    { 2555.610f, 1219.553f, 126.0669f }, // GAUNTLET_WP26
    { 2553.488f, 1224.329f, 125.7098f }, // GAUNTLET_WP27
    { 2549.292f, 1234.258f, 126.0170f }, // GAUNTLET_WP28
    { 2545.627f, 1241.927f, 125.9981f }, // GAUNTLET_WP29
    { 2543.625f, 1246.555f, 126.5570f }, // GAUNTLET_WP30
    { 2543.125f, 1247.805f, 126.5570f }, // GAUNTLET_WP31
    { 2540.625f, 1253.555f, 126.8070f }, // GAUNTLET_WP32
    { 2538.875f, 1257.305f, 126.8070f }, // GAUNTLET_WP33
    { 2536.125f, 1263.555f, 127.0570f }, // GAUNTLET_WP34
    { 2535.375f, 1265.055f, 127.0570f }, // GAUNTLET_WP35
    { 2534.375f, 1267.055f, 127.0570f }, // GAUNTLET_WP36
    { 2533.672f, 1268.224f, 127.0429f }, // GAUNTLET_WP37
    { 2531.305f, 1273.985f, 127.6081f }, // GAUNTLET_WP38
    { 2527.805f, 1282.735f, 128.8581f }, // GAUNTLET_WP39
    { 2526.497f, 1285.972f, 129.2153f }, // GAUNTLET_WP40
    { 2524.543f, 1291.004f, 130.4992f }, // GAUNTLET_WP41
    { 2524.043f, 1292.504f, 130.7492f }, // GAUNTLET_WP42
    { 2522.043f, 1297.504f, 130.7492f }, // GAUNTLET_WP43
    { 2521.293f, 1299.254f, 130.9992f }, // GAUNTLET_WP44
    { 2518.793f, 1307.254f, 130.9992f }, // GAUNTLET_WP45
    { 2517.043f, 1312.504f, 130.4992f }, // GAUNTLET_WP46
    { 2514.793f, 1319.504f, 131.7492f }, // GAUNTLET_WP47
    { 2513.089f, 1323.535f, 131.7832f, 1.780236f }, // GAUNTLET_WP48
    { 2511.267f, 1328.168f, 132.3065f }, // GAUNTLET_WP50
    { 2510.267f, 1330.668f, 132.3065f }, // GAUNTLET_WP51
    { 2508.517f, 1334.418f, 132.5565f }, // GAUNTLET_WP52
    { 2506.017f, 1339.918f, 133.3065f }, // GAUNTLET_WP53
    { 2505.187f, 1341.026f, 133.0155f }, // GAUNTLET_WP54
    { 2502.316f, 1347.414f, 133.1726f }, // GAUNTLET_WP55
    { 2501.094f, 1349.733f, 132.8003f }, // GAUNTLET_WP56
    { 2499.141f, 1354.359f, 132.7238f }, // GAUNTLET_WP57
    { 2498.891f, 1354.609f, 132.4738f }, // GAUNTLET_WP58
    { 2495.641f, 1361.609f, 131.4738f }, // GAUNTLET_WP59
    { 2492.141f, 1369.609f, 130.9783f }, // GAUNTLET_WP60
    { 2491.641f, 1370.609f, 130.6738f }, // GAUNTLET_WP61
    { 2489.043f, 1375.858f, 130.3458f }, // GAUNTLET_WP62
    { 2486.644f, 1381.297f, 130.5764f }, // GAUNTLET_WP63
    { 2485.894f, 1382.297f, 130.5764f }, // GAUNTLET_WP64
    { 2485.394f, 1383.297f, 130.5764f }, // GAUNTLET_WP65
    { 2479.644f, 1392.547f, 130.3264f }, // GAUNTLET_WP66
    { 2474.199f, 1400.597f, 130.4221f }, // GAUNTLET_WP67
    { 2473.844f, 1401.471f, 130.6458f }, // GAUNTLET_WP68
    { 2472.844f, 1402.971f, 130.8958f }, // GAUNTLET_WP69
    { 2472.344f, 1403.971f, 130.8958f }, // GAUNTLET_WP70
    { 2471.344f, 1405.471f, 130.8958f }, // GAUNTLET_WP71
    { 2467.344f, 1409.471f, 130.8958f }, // GAUNTLET_WP72
    { 2466.594f, 1410.221f, 130.8958f }, // GAUNTLET_WP73
    { 2460.344f, 1416.471f, 130.6458f }, // GAUNTLET_WP74
    { 2459.344f, 1417.471f, 130.6458f }, // GAUNTLET_WP75
    { 2455.844f, 1420.971f, 130.8958f }, // GAUNTLET_WP76
    { 2454.843f, 1421.910f, 130.6327f }, // GAUNTLET_WP77
    { 2453.803f, 1421.912f, 130.6348f }, // GAUNTLET_WP78
    { 2452.553f, 1421.662f, 130.6348f }, // GAUNTLET_WP79
    { 2451.053f, 1421.662f, 130.6348f }, // GAUNTLET_WP80
    { 2442.303f, 1421.162f, 130.6348f }, // GAUNTLET_WP81
    { 2434.803f, 1420.912f, 130.6348f }, // GAUNTLET_WP82
    { 2432.803f, 1420.912f, 130.8848f }, // GAUNTLET_WP83
    { 2426.013f, 1420.246f, 130.7083f }, // GAUNTLET_WP84
    { 2424.032f, 1420.176f, 130.8369f }, // GAUNTLET_WP85
    { 2422.282f, 1419.926f, 130.8369f }, // GAUNTLET_WP86
    { 2420.282f, 1419.926f, 130.8369f }, // GAUNTLET_WP87
    { 2417.532f, 1419.176f, 130.8369f }, // GAUNTLET_WP88
    { 2414.782f, 1418.426f, 130.5869f }, // GAUNTLET_WP89
    { 2413.282f, 1418.176f, 130.8369f }, // GAUNTLET_WP90
    { 2410.782f, 1417.426f, 130.8369f }, // GAUNTLET_WP91
    { 2407.282f, 1416.426f, 130.8369f }, // GAUNTLET_WP92
    { 2404.532f, 1415.926f, 130.8369f }, // GAUNTLET_WP93
    { 2402.032f, 1415.426f, 131.0869f }, // GAUNTLET_WP94
    { 2397.938f, 1414.144f, 130.2505f }, // GAUNTLET_WP95
    { 2392.439f, 1412.775f, 129.5187f }, // GAUNTLET_WP96
    { 2389.939f, 1412.025f, 129.2687f }, // GAUNTLET_WP97
    { 2384.689f, 1410.775f, 128.7687f }, // GAUNTLET_WP98
    { 2383.189f, 1410.275f, 128.5187f }, // GAUNTLET_WP99
    { 2377.189f, 1408.775f, 128.2687f }, // GAUNTLET_WP100
    { 2373.689f, 1407.775f, 128.0187f }, // GAUNTLET_WP101
    { 2372.439f, 1407.525f, 128.2687f }, // GAUNTLET_WP102
    { 2370.189f, 1407.025f, 128.5187f }, // GAUNTLET_WP103
    { 2367.439f, 1406.275f, 129.0187f }, // GAUNTLET_WP104
    
    { 2363.440f, 1404.906f, 128.7869f, 2.775074f }, // ARTHAS_GAUNTLET_END_POS
    { 2296.862f, 1501.015f, 128.4456f, 5.131268f }, // RP5_MALGANIS_POS
    { 2358.740f, 1404.739f, 128.9785f }, // RP5_ARTHAS_WP1
    { 2355.490f, 1404.489f, 128.7285f }, // RP5_ARTHAS_WP2
    { 2349.740f, 1405.489f, 128.7285f }, // RP5_ARTHAS_WP3
    { 2348.490f, 1405.739f, 128.4785f }, // RP5_ARTHAS_WP4
    { 2344.340f, 1406.146f, 128.3061f }, // RP5_ARTHAS_WP5
    { 2338.921f, 1407.289f, 128.3241f }, // RP5_ARTHAS_WP6
    { 2333.421f, 1410.039f, 128.0741f }, // RP5_ARTHAS_WP7
    { 2331.421f, 1411.039f, 128.0741f }, // RP5_ARTHAS_WP8
    { 2330.421f, 1412.789f, 128.0741f }, // RP5_ARTHAS_WP9
    { 2328.778f, 1415.333f, 127.6198f }, // RP5_ARTHAS_WP10
    { 2327.884f, 1417.254f, 127.7447f }, // RP5_ARTHAS_WP11
    { 2325.884f, 1421.504f, 127.9947f }, // RP5_ARTHAS_WP12
    { 2325.384f, 1422.254f, 127.9947f }, // RP5_ARTHAS_WP13
    { 2318.134f, 1438.504f, 127.9947f }, // RP5_ARTHAS_WP14
    { 2316.422f, 1441.448f, 127.8214f }, // RP5_ARTHAS_WP15
    { 2313.949f, 1447.049f, 128.1505f }, // RP5_ARTHAS_WP16
    { 2309.699f, 1456.049f, 128.1505f }, // RP5_ARTHAS_WP17
    { 2307.938f, 1458.750f, 127.8409f }, // RP5_ARTHAS_WP18
    { 2307.247f, 1460.363f, 127.9878f }, // RP5_ARTHAS_WP19
    { 2305.247f, 1464.363f, 127.9878f }, // RP5_ARTHAS_WP20
    { 2303.747f, 1467.863f, 127.9878f }, // RP5_ARTHAS_WP21
    { 2303.497f, 1469.113f, 127.7378f }, // RP5_ARTHAS_WP22
    { 2319.560f, 1506.408f, 152.0474f }, // RP5_CHROMIE_SPAWN
    { 2320.632f, 1507.193f, 152.5081f }, // RP5_CHROMIE_WP1
    { 2319.823f, 1506.605f, 152.5081f }, // RP5_CHROMIE_WP2
    { 2306.770f, 1496.780f, 128.3620f }  // RP5_CHROMIE_WP3
}};

const float npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackDistanceThreshold = 5.0f;
const std::map<ProgressStates, npc_arthas_stratholme::npc_arthas_stratholmeAI::SnapbackInfo> npc_arthas_stratholme::npc_arthas_stratholmeAI::_snapbackPositions = {
    { JUST_STARTED, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { CRATES_IN_PROGRESS, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { CRATES_DONE, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { UTHER_TALK, { REACT_PASSIVE, false, &_positions[RP1_ARTHAS_INITIAL] } },
    { PURGE_PENDING, { REACT_PASSIVE, true, &_positions[ARTHAS_PURGE_PENDING_POS] } },
    { PURGE_STARTING, { REACT_PASSIVE, false, &_positions[ARTHAS_PURGE_PENDING_POS] } },
    { WAVES_IN_PROGRESS, { REACT_AGGRESSIVE, false, &_positions[ARTHAS_WAVES_POS] } },
    { WAVES_DONE, { REACT_DEFENSIVE, false, &_positions[ARTHAS_WAVES_POS] } },
    { TOWN_HALL_PENDING, { REACT_DEFENSIVE, true, &_positions[ARTHAS_TOWN_HALL_POS] } },
    { TOWN_HALL, { REACT_DEFENSIVE, false, &_positions[ARTHAS_TOWN_HALL_POS] } },
    { TOWN_HALL_COMPLETE, { REACT_PASSIVE, true, &_positions[ARTHAS_TOWN_HALL_END_POS] } },
    { GAUNTLET_TRANSITION, { REACT_PASSIVE, false, &_positions[ARTHAS_TOWN_HALL_END_POS] } },
    { GAUNTLET_PENDING, { REACT_PASSIVE, true, &_positions[ARTHAS_GAUNTLET_POS] } },
    { GAUNTLET_IN_PROGRESS, { REACT_AGGRESSIVE, false, &_positions[ARTHAS_GAUNTLET_POS] } },
    { GAUNTLET_COMPLETE, { REACT_PASSIVE, true, &_positions[ARTHAS_GAUNTLET_END_POS] } },
    { MALGANIS_IN_PROGRESS, { REACT_DEFENSIVE, false, &_positions[ARTHAS_GAUNTLET_END_POS] } },
    { COMPLETE, { REACT_PASSIVE, true, &_positions[RP5_ARTHAS_WP22] } }
};

// Arthas' AI is the one controlling everything, all this AI does is report any movementinforms back to Arthas AI using SetData
struct npc_stratholme_rp_dummy : public StratholmeCreatureScript<NullCreatureAI>
{
    npc_stratholme_rp_dummy() : StratholmeCreatureScript<NullCreatureAI>("npc_stratholme_rp_dummy", ProgressStates(UTHER_TALK | PURGE_PENDING)) { }
    struct npc_stratholme_rp_dummyAI : public StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper
    {
        npc_stratholme_rp_dummyAI(Creature* creature) : StratholmeCreatureScript<NullCreatureAI>::StratholmeNPCAIWrapper(creature, ProgressStates(UTHER_TALK | PURGE_PENDING)) { }
        void MovementInform(uint32 type, uint32 id) override {
            if (type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE || type == SPLINE_CHAIN_MOTION_TYPE) if (TempSummon* self = me->ToTempSummon()) self->GetSummonerCreatureBase()->AI()->MovementInform(type, id);
        }
    };
    CreatureAI* GetAI(Creature* creature) const override { return GetInstanceAI<npc_stratholme_rp_dummyAI>(creature); }
};

class spell_stratholme_crusader_strike : public SpellScriptLoader
{
    public:
        spell_stratholme_crusader_strike() : SpellScriptLoader("spell_stratholme_crusader_strike") { }

        class spell_stratholme_crusader_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_stratholme_crusader_strike_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    if (target->GetEntry() == NPC_CITIZEN || target->GetEntry() == NPC_RESIDENT)
                        GetCaster()->Kill(target);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_stratholme_crusader_strike_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_stratholme_crusader_strike_SpellScript();
        }
};

void AddSC_npc_arthas_stratholme()
{
    new npc_arthas_stratholme();
    new npc_stratholme_rp_dummy();
    new spell_stratholme_crusader_strike();
}
