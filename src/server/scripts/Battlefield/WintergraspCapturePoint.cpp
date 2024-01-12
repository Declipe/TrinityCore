#include "WintergraspCapturePoint.h"
#include "WintergraspWorkshop.h"

WintergraspCapturePoint::WintergraspCapturePoint(BattlefieldWG* battlefield, TeamId teamInControl) : BfCapturePoint(battlefield)
{
    m_Bf = battlefield;
    m_team = teamInControl;
    m_Workshop = nullptr;
}

WintergraspCapturePoint::~WintergraspCapturePoint()
{
    m_Bf = nullptr;
    m_team = TEAM_NEUTRAL;
    m_Workshop = nullptr;
}

void WintergraspCapturePoint::prepareDelete()
{
    if (m_capturePointGUID)
    {
        if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
        {
            capturePoint->SetRespawnTime(0);                  // not save respawn time
            capturePoint->Delete();
            capturePoint = nullptr;
        }
        m_capturePointGUID.Clear();
    }
}

void WintergraspCapturePoint::ChangeTeam(TeamId team)
{
    ASSERT(m_Workshop);
    m_Workshop->GiveControlTo(team);
}

void WintergraspCapturePoint::LinkToWorkshop(WintergraspWorkshop* workshop)
{
    m_Workshop = workshop;
    InitState(workshop->GetTeamControl());
    workshop->LinkCapturePoint(this);
}
