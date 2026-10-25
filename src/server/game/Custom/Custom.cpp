#include "Custom.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Chat.h"
#include "BattlegroundQueue.h"
#include "World.h"
#include "WorldSession.h"
#include <cstdarg>

uint8 Unit::GetRace(bool forceoriginal) const
{
    if (GetTypeId() == TYPEID_PLAYER)
    {
        Player* pPlayer = ((Player*)this);

        if (forceoriginal)
            return pPlayer->getCFSRace();

        if (pPlayer->InArena())
            return GetByteValue(UNIT_FIELD_BYTES_0, 0);

        if (!pPlayer->IsPlayingNative())
            return pPlayer->getFRace();
    }

    return GetByteValue(UNIT_FIELD_BYTES_0, 0);
}

bool Player::SendRealNameQuery()
{
    if (IsPlayingNative())
        return false;

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, (8 + 1 + 1 + 1 + 1 + 1 + 10));
    data.appendPackGUID(GetGUID().IsEmpty());                             // player guid
    data << uint8(0);                                       // added in 3.1; if > 1, then end of packet
    data << GetName();                                   // played name
    data << uint8(0);                                       // realm name for cross realm BG usage
    data << uint8(getCFSRace());
    data << uint8(GetGender());
    data << uint8(GetClass());
    data << uint8(0);                                   // is not declined

    return true;
}

void Player::SetFakeRace()
{
    m_FakeRace = GetCFSTeam() == ALLIANCE ? RACE_BLOODELF : RACE_HUMAN;
}

bool Player::SendBattleGroundChat(uint32 msgtype, std::string message)
{
    // Select distance to broadcast to.
    float distance = msgtype == CHAT_MSG_SAY ? sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY) : sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL);

    if (Battleground* pBattleGround = GetBattleground())
    {
        if (pBattleGround->isArena()) // Only fake chat in BG's. CFBG should not interfere with arenas.
            return false;

        for (Battleground::BattlegroundPlayerMap::const_iterator itr = pBattleGround->GetPlayers().begin(); itr != pBattleGround->GetPlayers().end(); ++itr)
        {
            if (Player* pPlayer = ObjectAccessor::FindPlayer(itr->first))
            {
                if (GetDistance2d(pPlayer->GetPositionX(), pPlayer->GetPositionY()) <= distance)
                {
                    WorldPacket data(SMSG_MESSAGECHAT, 200);

                    if (GetTeam() == pPlayer->GetTeam())
                        BuildPlayerChat(&data, msgtype, message, LANG_UNIVERSAL);
                    else if (msgtype != CHAT_MSG_EMOTE)
                        BuildPlayerChat(&data, msgtype, message, pPlayer->GetTeam() == ALLIANCE ? LANG_ORCISH : LANG_COMMON);

                    pPlayer->GetSession()->SendPacket(&data);
                }
            }
        }
        return true;
    }
    else
        return false;
}

void Player::MorphFit(bool value)
{
    if (!IsPlayingNative() && value)
    {
        if (GetCFSTeam() == HORDE)
        {
            if (GetGender() == GENDER_MALE)
            {
                SetDisplayId(19723);
                SetNativeDisplayId(19723);
            }
            else
            {
                SetDisplayId(19724);
                SetNativeDisplayId(19724);
            }
        }
        else
        {
            if (GetGender() == GENDER_MALE)
            {
                SetDisplayId(20578);
                SetNativeDisplayId(20578);
            }
            else
            {
                SetDisplayId(20579);
                SetNativeDisplayId(20579);
            }
        }
    }
    else
        InitDisplayIds();
}

void Player::FitPlayerInTeam(bool action, Battleground* pBattleGround)
{
    if (!pBattleGround)
        pBattleGround = GetBattleground();

    if ((!pBattleGround || pBattleGround->isArena()) && action)
        return;

    if(!IsPlayingNative() && action)
        SetFactionForRace(GetRace());
    else
        SetFactionForRace(getCFSRace());

    if (action)
        SetForgetBGPlayers(true);
    else
        SetForgetInListPlayers(true);

    MorphFit(action);

    if (pBattleGround && action)
        SendChatMessage("{}You are playing for the {}{} in this {}", MSG_COLOR_WHITE, GetTeam() == ALLIANCE ? MSG_COLOR_DARKBLUE"alliance" : MSG_COLOR_RED"horde", MSG_COLOR_WHITE, pBattleGround->GetName().c_str());
}

void Player::DoForgetPlayersInList()
{
    // m_FakePlayers is filled from a vector within the battleground
    // they were in previously so all players that have been in that BG will be invalidated.
    for (FakePlayers::const_iterator itr = m_FakePlayers.begin(); itr != m_FakePlayers.end(); ++itr)
    {
        WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
        data << *itr;
        GetSession()->SendPacket(&data);
        if (Player* pPlayer = ObjectAccessor::FindPlayer(ObjectGuid::Create<HighGuid::Player>(*itr)))
            GetSession()->SendNameQueryOpcode(pPlayer->GetGUID());
    }
    m_FakePlayers.clear();
}

void Player::DoForgetPlayersInBG(Battleground* pBattleGround)
{
    for (Battleground::BattlegroundPlayerMap::const_iterator itr = pBattleGround->GetPlayers().begin(); itr != pBattleGround->GetPlayers().end(); ++itr)
    {
        // Here we invalidate players in the bg to the added player
        WorldPacket data1(SMSG_INVALIDATE_PLAYER, 8);
        data1 << itr->first;
        GetSession()->SendPacket(&data1);

        if (Player* pPlayer = ObjectAccessor::FindPlayer(itr->first))
        {
            GetSession()->SendNameQueryOpcode(pPlayer->GetGUID()); // Send namequery answer instantly if player is available
            // Here we invalidate the player added to players in the bg
            WorldPacket data2(SMSG_INVALIDATE_PLAYER, 8);
            data2 << GetGUID();
            pPlayer->GetSession()->SendPacket(&data2);
            pPlayer->GetSession()->SendNameQueryOpcode(GetGUID());
        }
    }
}

bool BattlegroundQueue::CheckCrossFactionMatch(Battleground* bg)
{
    if (!sWorld->getBoolConfig(BATTLEGROUND_CROSSFACTION_ENABLED) || bg->isArena())
        return false;

    FillXPlayersToBG(bg, true);

    if (sBattlegroundMgr->isTesting() && (m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() || m_SelectionPools[TEAM_HORDE].GetPlayerCount()))
        return true;

    uint32 minPlayersPerTeam = bg->GetMinPlayersPerTeam();
    return m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() >= minPlayersPerTeam
        && m_SelectionPools[TEAM_HORDE].GetPlayerCount() >= minPlayersPerTeam;
}

bool BattlegroundQueue::FillXPlayersToBG(Battleground* bg, bool start)
{
    uint32 queuedPeople = 0;
    for (GroupQueueInfo const* group : m_QueuedGroups[BG_QUEUE_CROSSFACTION])
        if (!group->IsInvitedToBGInstanceGUID)
            queuedPeople += group->Players.size();

    if (!sWorld->getBoolConfig(BATTLEGROUND_CROSSFACTION_ENABLED)
        || (!sBattlegroundMgr->isTesting() && queuedPeople < bg->GetMinPlayersPerTeam() * 2 && start))
        return false;

    int32 allianceFree = start ? bg->GetMaxPlayersPerTeam() : bg->GetFreeSlotsForTeam(ALLIANCE);
    int32 hordeFree = start ? bg->GetMaxPlayersPerTeam() : bg->GetFreeSlotsForTeam(HORDE);
    m_SelectionPools[TEAM_ALLIANCE].Init();
    m_SelectionPools[TEAM_HORDE].Init();
    int32 allianceAvailable = allianceFree;
    int32 hordeAvailable = hordeFree;

    if (start)
    {
        QueuedGroupMap allianceGroups;
        QueuedGroupMap hordeGroups;
        uint32 queuedAlliance = 0;
        uint32 queuedHorde = 0;

        for (GroupQueueInfo* group : m_QueuedGroups[BG_QUEUE_CROSSFACTION])
        {
            if (group->IsInvitedToBGInstanceGUID)
                continue;

            if (group->CFSTeam == ALLIANCE)
            {
                allianceGroups.emplace(group->Players.size(), group);
                queuedAlliance += group->Players.size();
            }
            else
            {
                hordeGroups.emplace(group->Players.size(), group);
                queuedHorde += group->Players.size();
            }
        }

        uint32 smallestTeam = std::min(std::min(uint32(allianceFree), queuedAlliance), std::min(uint32(hordeFree), queuedHorde));
        allianceAvailable -= PreAddPlayers(allianceGroups, smallestTeam, allianceFree);
        hordeAvailable -= PreAddPlayers(hordeGroups, smallestTeam, hordeFree);
    }

    QueuedGroupMap queuedGroups;
    for (GroupQueueInfo* group : m_QueuedGroups[BG_QUEUE_CROSSFACTION])
        queuedGroups.emplace(group->Players.size(), group);

    for (auto itr = queuedGroups.rbegin(); itr != queuedGroups.rend(); ++itr)
    {
        GroupQueueInfo* group = itr->second;
        GroupsQueueType const& alliancePool = m_SelectionPools[TEAM_ALLIANCE].SelectedGroups;
        GroupsQueueType const& hordePool = m_SelectionPools[TEAM_HORDE].SelectedGroups;

        if (group->IsInvitedToBGInstanceGUID
            || std::find(alliancePool.begin(), alliancePool.end(), group) != alliancePool.end()
            || std::find(hordePool.begin(), hordePool.end(), group) != hordePool.end()
            || (m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() >= bg->GetMinPlayersPerTeam()
                && m_SelectionPools[TEAM_HORDE].GetPlayerCount() >= bg->GetMinPlayersPerTeam()))
            continue;

        bool moreAllianceSpace = allianceAvailable < hordeAvailable;
        if (allianceAvailable != hordeAvailable)
            group->Team = moreAllianceSpace ? HORDE : ALLIANCE;

        bool alliance = group->Team == ALLIANCE;
        if (m_SelectionPools[alliance ? TEAM_ALLIANCE : TEAM_HORDE].AddGroup(group, alliance ? allianceFree : hordeFree))
            (alliance ? allianceAvailable : hordeAvailable) -= group->Players.size();
    }

    return true;
}

int32 BattlegroundQueue::PreAddPlayers(QueuedGroupMap const& preGroupMap, int32 maxAdd, uint32 maxInTeam)
{
    int32 leftToAdd = maxAdd;
    int32 added = 0;
    for (auto itr = preGroupMap.rbegin(); itr != preGroupMap.rend(); ++itr)
    {
        int32 playerSize = itr->first;
        bool alliance = itr->second->CFSTeam == ALLIANCE;
        if (playerSize <= leftToAdd && m_SelectionPools[alliance ? TEAM_ALLIANCE : TEAM_HORDE].AddGroup(itr->second, maxInTeam))
        {
            leftToAdd -= playerSize;
            added += playerSize;
        }
    }

    return added;
}

void Player::SendChatMessage(const char *format, ...)
{
    if (!IsInWorld())
        return;

    if (format)
    {
        va_list ap;
        char str [2048];
        va_start(ap, format);
        vsnprintf(str, 2048, format, ap);
        va_end(ap);

        ChatHandler(GetSession()).SendSysMessage(str);
    }
}
