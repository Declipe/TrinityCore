#include "BfCapturePoint.h"
#include "BattlefieldWG.h"

/* ######################### *
 *  WintergraspCapturePoint  *
 * ######################### */
class WintergraspCapturePoint : public BfCapturePoint
{
public:
    WintergraspCapturePoint(BattlefieldWG* battlefield, TeamId teamInControl);
    ~WintergraspCapturePoint();

    void LinkToWorkshop(WintergraspWorkshop* workshop);

    void ChangeTeam(TeamId oldteam) override;
    TeamId GetTeam() const { return m_team; }

    void prepareDelete();

private:
    WintergraspWorkshop* m_Workshop;    
};
