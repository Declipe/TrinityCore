#include "WintergraspWorkshop.h"
#include "Battleground.h"
#include "MotionMaster.h"
#include "Log.h"
#include "WintergraspCapturePoint.h"
#include "WorldStatePackets.h"
#include "World.h"

WintergraspGobjectWithCreatureData const GobjectWithCreatureData[WG_MAX_WORKSHOP] =
{
    {
        BATTLEFIELD_WG_WORKSHOP_SE,
        {
            //                  Position                                QuaternionData           Horde   Alliance
            { { 4417.25f, 2301.14f, 377.214f, 0.026179f }, { 0.f, 0.f, 0.0130891f, 0.999914f }, 192453, 192418 },       // Flag near workshop
            { { 4417.94f, 2324.81f, 371.577f, 3.08051f }, { 0.f, 0.f, 0.999534f, 0.0305366f }, 192272, 192273 },        // Flagpole near workshop
            { { 4416.59f, 2414.08f, 377.196f, 0.0f }, { 0.f, 0.f, 0.0f, 1.0f }, 192452, 192417 },                       // Flag near workshop west
            { { 4408.57f, 2422.61f, 377.179f, 1.58825f }, { 0.f, 0.f, 0.713251f, 0.700909f }, 192451, 192416 }          // Flag near workshop west
        },
        {
            { { 4349.120117f, 2299.280029f, 374.743011f, 4.904380f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4391.67f, 2300.61f, 374.743f, 4.92183f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4417.92f, 2331.24f, 370.919f, 5.84685f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4418.61f, 2355.29f, 372.491f, 6.02139f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4413.43f, 2393.45f, 376.36f, 1.06465f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
            { { 4388.13f, 2411.98f, 374.743f, 1.64061f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4349.54f, 2411.26f, 374.743f, 2.05949f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4357.67f, 2357.99f, 382.007f, 1.67552f }, NPC_WINTERGRASP_GOBLIN_ENGINEER_ALLIANCE, NPC_WINTERGRASP_GNOMISH_ENGINEER_ALLIANCE }     // Engineer
        }
    },
    {
        BATTLEFIELD_WG_WORKSHOP_SW,
        {
            { { 4399.59f, 3231.43f, 369.216f, 1.67552f }, { 0.f, 0.f, 0.743146f, 0.669129f }, 192408, 192409 },         // Flagpole east
            { { 4448.17f, 3235.63f, 370.412f, -1.56207f }, { 0.f, 0.f, -0.704015f, 0.710185f }, 192441, 192407 },       // Flag east
            { { 4424.15f, 3286.54f, 371.546f, 3.12412f }, { 0.f, 0.f, 0.999962f, 0.00873622f }, 192275, 192274 },       // Flagpole middle
            { { 4438.3f, 3361.08f, 371.568f, -0.017451f }, { 0.f, 0.f, -0.00872539f, 0.999962f }, 192440, 192406 },     // Flag west
            { { 4401.63f, 3377.46f, 363.365f, 1.55334f }, { 0.f, 0.f, 0.700908f, 0.713252f }, 192432, 192433 }          // Flagpole west
        },
        {
            { { 4389.94f, 3230.75f, 369.18f, 4.66f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4373.67f, 3228.11f, 369.183f, 4.95f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },    // Standing Guard
            { { 4414.095f, 3288.15f, 372.363f, 0.376f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },  // Standing Guard
            { { 4411.95f, 3318.94f, 371.466f, 6.252f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },   // Standing Guard
            { { 4394.11f, 3377.31f, 364.23f, 1.5f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4380.58f, 3381.58f, 363.32f, 1.52f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4354.15f, 3312.82f, 378.046f, 1.67552f }, NPC_WINTERGRASP_GOBLIN_ENGINEER_ALLIANCE, NPC_WINTERGRASP_GNOMISH_ENGINEER_ALLIANCE }     // Engineer
        }
    },
    {
        BATTLEFIELD_WG_WORKSHOP_NE,
        {
            { { 4778.06f, 2438.02f, 345.706f, -2.93214f }, { 0.f, 0.f,  -0.994521f, 0.104535f }, 192289, 192288 },      //  Flagpole south
            { { 4811.44f, 2441.85f, 357.982f, -2.02458f }, { 0.f, 0.f,  -0.848048f, 0.52992f }, 192458, 192425 },       //  flag left south
            { { 4805.51f, 2407.84f, 357.941f, 1.77151f }, { 0.f, 0.f,  0.774393f, 0.632705f }, 192459, 192426 },        //  flag right south
            { { 4805.51f, 2407.84f, 357.941f, 1.77151f }, { 0.f, 0.f,  0.774393f, 0.632705f }, 192461, 192428 },        //  flag right north
            { { 5004.35f, 2486.36f, 358.449f, 2.17294f }, { 0.f, 0.f,  0.884989f, 0.465612f }, 192460, 192427 },        //  flag left north
            { { 4991.08f, 2525.76f, 340.918f, 4.04044f }, { 0.f, 0.f,  0.900698f, -0.434447f }, 192290, 192291 }        //  Flagpole north
        },
        {
            { { 5000.062f, 2524.42f, 338.82f, 1.377f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
            { { 5013.839f, 2515.873f, 339.39f, 1.141f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Standing Guard
            { { 4954.693f, 2446.817f, 320.996f, 0.976f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4935.209f, 2453.341f, 320.230f, 3.383f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4770.758f, 2434.176f, 348.379f, 3.249f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4769.083f, 2418.633f, 350.093f, 3.430f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4939.76f, 2389.06f, 326.153f, 3.26377f }, NPC_WINTERGRASP_GOBLIN_ENGINEER_ALLIANCE, NPC_WINTERGRASP_GNOMISH_ENGINEER_ALLIANCE }     // Engineer
        }
    },
    {
        BATTLEFIELD_WG_WORKSHOP_NW,
        {
            { { 4857.97f, 3335.42f, 369.292f, -2.88851f }, { 0.f, 0.f,  -0.992004f, 0.126204f }, 192280, 192281 },      // Flagpole south
            { { 4855.44f, 3297.6f, 376.496f, -3.11539f }, { 0.f, 0.f,  -0.999914f, 0.0131009f }, 192435, 192401 },      // Flag south
            { { 5006.32f, 3280.36f, 371.242f, 2.24275f }, { 0.f, 0.f,  0.900699f, 0.434444f }, 192283, 192282 },        // Flagpole north
            { { 5041.65f, 3294.32f, 381.92f, -1.6057f }, { 0.f, 0.f,  -0.719339f, 0.694659f }, 192434, 192400 }         // Flag north
        },
        {
            { { 4855.189f, 3312.502f, 368.395f, 3.529f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4848.059f, 3322.709f, 369.305f, 3.313f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4939.664f, 3342.562f, 376.864f, 4.621f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4960.131f, 3338.029f, 376.876f, 4.943f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 5008.679f, 3282.526f, 371.454f, 5.312f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 5021.111f, 3293.236f, 370.746f, 5.418f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4964.89f, 3383.06f, 382.911f, 6.12611f }, NPC_WINTERGRASP_GOBLIN_ENGINEER_ALLIANCE, NPC_WINTERGRASP_GNOMISH_ENGINEER_ALLIANCE }     // Engineer
        }
    },
    {
        BATTLEFIELD_WG_WORKSHOP_KEEP_WEST,
        {
            //{ { 4466.793f, 1960.418f, 459.1437f, 1.151916f }, { 0.f, 0.f,  0.5446386f, 0.8386708f }, 192488, 192501 },     // Flag on tower
        },
        {
            { { 5392.91f, 2975.26f, 415.223f, 4.55531f }, NPC_WINTERGRASP_GOBLIN_ENGINEER_ALLIANCE, NPC_WINTERGRASP_GNOMISH_ENGINEER_ALLIANCE }     // Engineer
        }
    },
    {
        BATTLEFIELD_WG_WORKSHOP_KEEP_EAST,
        {
            //{ { 4466.793f, 1960.418f, 459.1437f, 1.151916f }, { 0.f, 0.f,  0.5446386f, 0.8386708f }, 192488, 192501 },     // Flag on tower
        },
        {
            { { 5391.61f, 2707.72f, 415.051f, 4.55531f }, NPC_WINTERGRASP_GOBLIN_ENGINEER_ALLIANCE, NPC_WINTERGRASP_GNOMISH_ENGINEER_ALLIANCE }     // Engineer
        }
    }
};

WintergraspWorkshop::WintergraspWorkshop(BattlefieldWG* wg, uint8 type)
{
    ASSERT(wg && type < WG_MAX_WORKSHOP);
    _wg = wg;
    _state = BATTLEFIELD_WG_OBJECTSTATE_NONE;
    _teamControl = TEAM_NEUTRAL;
    _capturePoint = nullptr;
    _staticInfo = &WorkshopData[type];
    if (type < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
    {
        if (GameObject* flagGO = _wg->SpawnGameObject(_staticInfo->entry, _staticInfo->Pos, _staticInfo->Rot))
        {
            m_workshopGoGUID = flagGO->GetGUID();
        }
        else
        {
            TC_LOG_ERROR("bg.battlefield", "WintergraspWorkshop type {} can't be initialized", type);
        }
    }
}

WintergraspWorkshop::~WintergraspWorkshop()
{
    _wg = nullptr;
    _state = BATTLEFIELD_WG_OBJECTSTATE_NONE;
    _teamControl = TEAM_NEUTRAL;
    _staticInfo = nullptr;
    _capturePoint = nullptr;
}

uint8 WintergraspWorkshop::GetId() const
{
    return _staticInfo->WorkshopId;
}

void WintergraspWorkshop::GiveControlTo(TeamId teamId, bool init /*= false*/)
{
    switch (teamId)
    {
        case TEAM_NEUTRAL:
        {
            // Send warning message to all player for inform a faction attack a workshop
            _state = BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT;
            // alliance / horde attacking workshop
            _wg->SendWarning(_teamControl == TEAM_ALLIANCE ? _staticInfo->TextIds.HordeAttack : _staticInfo->TextIds.AllianceAttack);
            break;
        }
        case TEAM_ALLIANCE:
        {
            // Updating worldstate
            _state = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT;
            // Warning message
            if (!init)
                _wg->SendWarning(_staticInfo->TextIds.AllianceCapture); // workshop taken - alliance
            // Found associate graveyard and update it
            if (_staticInfo->WorkshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
                if (BfGraveyard* gy = _wg->GetGraveyardById(_staticInfo->WorkshopId))
                    gy->GiveControlTo(TEAM_ALLIANCE);
            _teamControl = teamId;
            break;
        }
        case TEAM_HORDE:
        {
            // Update worldstate
            _state = BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT;
            // Warning message
            if (!init)
                _wg->SendWarning(_staticInfo->TextIds.HordeCapture); // workshop taken - horde
            // Update graveyard control
            if (_staticInfo->WorkshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
                if (BfGraveyard* gy = _wg->GetGraveyardById(_staticInfo->WorkshopId))
                    gy->GiveControlTo(TEAM_HORDE);
            _teamControl = teamId;
            break;
        }
    }

    _wg->SendUpdateWorldState(_staticInfo->WorldStateId, _state);
    UpdateCreatureAndGo();

    if (!init)
        _wg->UpdateCounterVehicle(false);

    if (init && _capturePoint)
        _capturePoint->InitState(teamId);
}

void WintergraspWorkshop::UpdateGraveyardAndWorkshop()
{
    if (_staticInfo->WorkshopId < BATTLEFIELD_WG_WORKSHOP_NE)
        GiveControlTo(_wg->GetAttackerTeam(), true);
    else
        GiveControlTo(_wg->GetDefenderTeam(), true);
}

void WintergraspWorkshop::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(_staticInfo->WorldStateId, _state);
}

void WintergraspWorkshop::Save()
{
    sWorld->setWorldState(_staticInfo->WorldStateId, _state);
}

void WintergraspWorkshop::LinkCapturePoint(WintergraspCapturePoint* pointer)
{
    _capturePoint = pointer;
}

void WintergraspWorkshop::FillRelatedGoAndNpc()
{
    // Spawn associate gameobjects
    for (WintergraspGameObjectData const& gobData : GobjectWithCreatureData[_staticInfo->WorkshopId].GameObject)
    {
        if (GameObject* goHorde = _wg->SpawnGameObject(gobData.HordeEntry, gobData.Pos, gobData.Rot))
            m_gameObjects[TEAM_HORDE].push_back(goHorde->GetGUID());

        if (GameObject* goAlliance = _wg->SpawnGameObject(gobData.AllianceEntry, gobData.Pos, gobData.Rot))
            m_gameObjects[TEAM_ALLIANCE].push_back(goAlliance->GetGUID());
    }

    // Spawn associate npc bottom
    for (WintergraspObjectPositionData const& creatureData : GobjectWithCreatureData[_staticInfo->WorkshopId].CreatureBottom)
    {
        if (Creature* creature = _wg->SpawnCreature(creatureData.HordeEntry, creatureData.Pos))
        {
            m_creatures[TEAM_HORDE].push_back(creature->GetGUID());
            if (_wg->isCreatureGuard(creature))
                creature->GetMotionMaster()->MoveRandom(25.f);
        }

        if (Creature* creature = _wg->SpawnCreature(creatureData.AllianceEntry, creatureData.Pos))
        {
            m_creatures[TEAM_ALLIANCE].push_back(creature->GetGUID());
            if (_wg->isCreatureGuard(creature))
                creature->GetMotionMaster()->MoveRandom(25.f);
        }
    }
}

void WintergraspWorkshop::UpdateCreatureAndGo()
{
    for (ObjectGuid guid : m_creatures[_wg->GetOtherTeam(_teamControl)])
        if (Creature* creature = _wg->GetCreature(guid))
            _wg->HideNpc(creature);

    for (ObjectGuid guid : m_creatures[_teamControl])
        if (Creature* creature = _wg->GetCreature(guid))
        {
            creature->SetRespawnDelay(INVITE_ACCEPT_WAIT_TIME);
            _wg->ShowNpc(creature, true);
        }

    for (ObjectGuid guid : m_gameObjects[_wg->GetOtherTeam(_teamControl)])
        if (GameObject* go = _wg->GetGameObject(guid))
            go->SetRespawnTime(RESPAWN_ONE_DAY);

    for (ObjectGuid guid : m_gameObjects[_teamControl])
        if (GameObject* go = _wg->GetGameObject(guid))
            go->SetRespawnTime(RESPAWN_IMMEDIATELY);
}

void WintergraspWorkshop::prepareDelete()
{
    if (GameObject* building = _wg->GetGameObject(m_workshopGoGUID))
    {
        building->SetRespawnTime(0);                  // not save respawn time
        building->Delete();
        building = nullptr;
    }

    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
    {
        for (ObjectGuid guid : m_gameObjects[i])
            if (GameObject* go = _wg->GetGameObject(guid))
            {
                go->SetRespawnTime(0);                  // not save respawn time
                go->Delete();
                go = nullptr;
            }
        m_gameObjects[i].clear();
    }

    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
    {
        for (ObjectGuid guid : m_creatures[i])
            if (Creature* creature = _wg->GetCreature(guid))
                creature->ClearZoneScript();
        m_creatures[i].clear();
    }
}

void BattlefieldWG::FillWorkshops()
{
    m_Workshops.resize(WG_MAX_WORKSHOP);
    // Spawn workshop creatures and gameobjects
    for (uint8 i = 0; i < WG_MAX_WORKSHOP; i++)
    {
        WintergraspWorkshop* workshop = new WintergraspWorkshop(this, i);
        workshop->FillRelatedGoAndNpc();

        if (i < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
        {
            WintergraspCapturePoint* capturePoint = new WintergraspCapturePoint(this, i < BATTLEFIELD_WG_WORKSHOP_NE ? GetAttackerTeam() : GetDefenderTeam());
            capturePoint->SetCapturePointData(GetGameObject(workshop->m_workshopGoGUID));
            capturePoint->LinkToWorkshop(workshop);

            AddCapturePoint(capturePoint);
            m_WCP.push_back(capturePoint);
        }        

        if (i < BATTLEFIELD_WG_WORKSHOP_NE)
            workshop->GiveControlTo(GetAttackerTeam(), true);
        else
            workshop->GiveControlTo(GetDefenderTeam(), true);

        // Note: Capture point is added once the gameobject is created.
        m_Workshops[i] = workshop;
    }
}

void BattlefieldWG::UpdateWorkshopsAndGraves()
{
    // Update graveyard (in no war time all graveyard is to deffender, in war time, depend of base)
    for (WintergraspWorkshop* workshop : m_Workshops)
        workshop->UpdateGraveyardAndWorkshop();
}
