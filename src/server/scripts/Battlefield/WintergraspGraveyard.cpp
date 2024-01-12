#include "WintergraspGraveyard.h"

WintergraspObjectPositionData const Spirits[BATTLEFIELD_WG_GRAVEYARD_MAX] =
{
    { { 5104.750000f, 2300.944951f, 368.567993f, 0.733038f }, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE },      // TEAM_NEUTRAL NE
    { { 5103.129883f, 3462.129883f, 368.567993f, 5.270900f }, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE },      // TEAM_NEUTRAL NW
    { { 4318.439941f, 2408.070068f, 392.675995f, 6.230830f }, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE },      // TEAM_NEUTRAL SE
    { { 4336.250000f, 3235.520020f, 390.334015f, 0.628319f }, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE },      // TEAM_NEUTRAL SW
    { { 5537.479980f, 2898.909912f, 517.258972f, 4.869470f }, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE },      // TEAM_NEUTRAL KEEP
    { { 5031.839844f, 3710.750000f, 372.483002f, 3.842810f }, NPC_TAUNKA_SPIRIT_GUIDE, 0 },      // TEAM_NEUTRAL HORDE
    { { 5140.790039f, 2179.120117f, 390.950989f, 1.972220f }, 0, NPC_DWARVEN_SPIRIT_GUIDE },     // TEAM_NEUTRAL ALLIANCE
};

// 7 in sql, 7 in header
BfWGCoordGY const WGGraveyard[BATTLEFIELD_WG_GRAVEYARD_MAX] =
{
    { { 5104.750f, 2300.940f, 368.579f, 0.733038f }, 1329, BATTLEFIELD_WG_GOSSIPTEXT_GY_NE,       TEAM_NEUTRAL  },
    { { 5099.120f, 3466.036f, 368.484f, 5.317802f }, 1330, BATTLEFIELD_WG_GOSSIPTEXT_GY_NW,       TEAM_NEUTRAL  },
    { { 4314.648f, 2408.522f, 392.642f, 6.268125f }, 1333, BATTLEFIELD_WG_GOSSIPTEXT_GY_SE,       TEAM_NEUTRAL  },
    { { 4331.716f, 3235.695f, 390.251f, 0.008500f }, 1334, BATTLEFIELD_WG_GOSSIPTEXT_GY_SW,       TEAM_NEUTRAL  },
    { { 5537.986f, 2897.493f, 517.057f, 4.819249f }, 1285, BATTLEFIELD_WG_GOSSIPTEXT_GY_KEEP,     TEAM_NEUTRAL  },
    { { 5032.454f, 3711.382f, 372.468f, 3.971623f }, 1331, BATTLEFIELD_WG_GOSSIPTEXT_GY_HORDE,    TEAM_HORDE    },
    { { 5140.790f, 2179.120f, 390.950f, 1.972220f }, 1332, BATTLEFIELD_WG_GOSSIPTEXT_GY_ALLIANCE, TEAM_ALLIANCE },
};

WintergraspGraveyard::WintergraspGraveyard(BattlefieldWG* battlefield) : BfGraveyard(battlefield)
{
    m_Bf = battlefield;
    m_GossipTextId = 0;
}

WintergraspGraveyard::~WintergraspGraveyard()
{
    m_GossipTextId = 0;
}

void WintergraspGraveyard::prepareDelete()
{
    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
        if (Creature* creature = m_Bf->GetCreature(m_SpiritGuide[i]))
            creature->ClearZoneScript();
}

void BattlefieldWG::FillGraveyards()
{
    // Init Graveyards
    SetGraveyardNumber(BATTLEFIELD_WG_GRAVEYARD_MAX);
    m_WG.resize(BATTLEFIELD_WG_GRAVEYARD_MAX);

    for (uint8 i = 0; i < BATTLEFIELD_WG_GRAVEYARD_MAX; i++)
    {
        WintergraspGraveyard* graveyard = new WintergraspGraveyard(this);
        // When between games, the graveyard is controlled by the defending team
        if (WGGraveyard[i].StartControl == TEAM_NEUTRAL)
            graveyard->Initialize(m_DefenderTeam, WGGraveyard[i].GraveyardID);
        else
            graveyard->Initialize(WGGraveyard[i].StartControl, WGGraveyard[i].GraveyardID);

        graveyard->SetTextId(WGGraveyard[i].TextID);
        //m_GraveyardList is container of BfGraveyard
        m_GraveyardList[i] = graveyard;
        //m_WG is container of WintergraspGraveyard (extended class based on BfGraveyard). Need to destructor.
        m_WG[i] = graveyard;

        switch (WGGraveyard[i].StartControl)
        {
            case TEAM_NEUTRAL:
                if (Creature* spiritHorde = SpawnCreature(Spirits[i].HordeEntry, Spirits[i].Pos))
                    m_GraveyardList[i]->SetSpirit(spiritHorde, TEAM_HORDE);
                if (Creature* spiritAlliance = SpawnCreature(Spirits[i].AllianceEntry, Spirits[i].Pos))
                    m_GraveyardList[i]->SetSpirit(spiritAlliance, TEAM_ALLIANCE);
                break;
            case TEAM_HORDE:
                if (Creature* spiritHorde = SpawnCreature(Spirits[i].HordeEntry, Spirits[i].Pos))
                    m_GraveyardList[i]->SetSpirit(spiritHorde, TEAM_HORDE);
                break;
            case TEAM_ALLIANCE:
                if (Creature* spiritAlliance = SpawnCreature(Spirits[i].AllianceEntry, Spirits[i].Pos))
                    m_GraveyardList[i]->SetSpirit(spiritAlliance, TEAM_ALLIANCE);
                break;
            default:
                break;

        }
    }
}
