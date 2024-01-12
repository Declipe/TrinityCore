#include "BattlefieldWG.h"

/*#########################
 *####### Graveyards ######
 *#########################*/

class WintergraspGraveyard : public BfGraveyard
{
public:
    WintergraspGraveyard(BattlefieldWG* Bf);
    ~WintergraspGraveyard();

    void SetTextId(uint32 textId) { m_GossipTextId = textId; }
    uint32 GetTextId() const { return m_GossipTextId; }

    void prepareDelete();

protected:
    uint32 m_GossipTextId;
};
