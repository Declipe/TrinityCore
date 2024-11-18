#include "Battlefield.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "GameTime.h"
#include "Group.h"
#include "MiscPackets.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "WorldStatePackets.h"

void Battlefield::AddPlayer(Player* plr, bool InZone, bool IsWaitingQueue, bool IsWaitingWar, bool IsWaitingKick, time_t time)
{
    if (HasPlayer(plr))
        return;

    PlayerHolder pl;
    pl.inZone = InZone;
    pl.isWaitingQueue = IsWaitingQueue;
    pl.isWaitingWar = IsWaitingWar;
    pl.isWaitingKick = IsWaitingKick;
    pl.inQueue = false;
    pl.inWar = false;
    pl.team = plr->GetTeamId();
    pl.time = time;
    m_PlayerMap[plr->GetGUID()] = pl;
}

bool Battlefield::HasPlayer(Player* player) const
{
    PlayerHolderContainer::const_iterator itr = m_PlayerMap.find(player->GetGUID());
    if (itr != m_PlayerMap.end())
        return true;
    return false;
}

bool Battlefield::NeedToRemove(Player* player) const
{
    PlayerHolderContainer::const_iterator itr = m_PlayerMap.find(player->GetGUID());
    if (itr != m_PlayerMap.end())
        return !itr->second.inQueue;
    return false;
}

void Battlefield::RemovePlayer(Player* plr)
{
    bool founded = false;
    PlayerHolderContainer::const_iterator itr = m_PlayerMap.find(plr->GetGUID());
    if (itr != m_PlayerMap.end())
    {
        if (itr->second.inQueue)
            m_freeslots[itr->second.team]++;

        founded = true;
    }

    if (founded)
        m_PlayerMap.erase(plr->GetGUID());
}

uint32 Battlefield::TeamSize(TeamId team)
{
    uint32 result = 0;
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (itr->second.inWar && itr->second.team == team)
            result++;
    return result;
}

void Battlefield::KickPlayerFromBattlefield(ObjectGuid guid)
{
    if (Player* player = ObjectAccessor::FindPlayer(guid))
        if (player->GetZoneId() == AREA_WINTERGRASP)
        {
            if (player->isDead())
            {
                player->ResurrectPlayer(100);
                player->SpawnCorpseBones();
            }

            player->TeleportTo(KickPosition);
            RemovePlayer(player);
            player->GetSession()->SendBattlefieldLeaveMessage(BATTLEFIELD_WG, true, player->GetLevel() < GetMinimalLevel() ? BF_LEAVE_REASON_LOW_LEVEL : BF_LEAVE_REASON_EXITED);
        }
}

void Battlefield::UpdateZoneStatusInPlayerMap(Player* plr, bool inZone)
{
    PlayerHolderContainer::iterator itr = m_PlayerMap.find(plr->GetGUID());
    if (itr != m_PlayerMap.end())
        itr->second.inZone = inZone;
}

void Battlefield::InvitePlayersInZoneToQueue()
{
    time_t futuretime = GameTime::GetGameTime() + m_TimeForAcceptInvite;
    for (PlayerHolderContainer::iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            if (player->GetLevel() >= GetMinimalLevel() && !itr->second.inQueue && !itr->second.isWaitingQueue)
            {
                itr->second.isWaitingQueue = true;
                itr->second.time = futuretime;
                player->GetSession()->SendBattlefieldInvitePlayerToQueue(m_BattleId);
            }
}

void Battlefield::InvitePlayersInQueueToWar()
{
    time_t futuretime = GameTime::GetGameTime() + m_TimeForAcceptInvite;
    for (PlayerHolderContainer::iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (itr->second.inQueue)
        {
            itr->second.isWaitingWar = true;
            itr->second.time = futuretime;
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->GetSession()->SendBattlefieldInvitePlayerToWar(m_BattleId, m_ZoneId, m_TimeForAcceptInvite);
        }
}

void Battlefield::TryInvitePlayersNotInQueueToWarOrKickThem()
{
    time_t futuretime = GameTime::GetGameTime() + m_TimeForAcceptInvite;
    for (PlayerHolderContainer::iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (!itr->second.inQueue)
        {
            // time before be kicked or time for accept invite to war
            itr->second.time = futuretime;
            if (GetFreeslot(itr->second.team))
            {
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (player->GetLevel() >= m_MinLevel)
                    {
                        itr->second.isWaitingQueue = true;
                        itr->second.isWaitingWar = true;
                        player->GetSession()->SendBattlefieldInvitePlayerToWar(m_BattleId, m_ZoneId, m_TimeForAcceptInvite);
                    }
                    else
                    {
                        itr->second.isWaitingKick = true;
                        sWorld->SendServerMessage(SERVER_MSG_STRING, "You have a low level and will removed from Wintergrasp in 20 seconds", player);
                        player->GetSession()->SendBattlefieldLeaveMessage(m_BattleId, true, BF_LEAVE_REASON_LOW_LEVEL);
                    }
                }
            }
            else
                itr->second.isWaitingKick = true;
        }
}

void Battlefield::InviteNewPlayerToQueue(Player* player, uint32 battleId, bool isInZone)
{
    if (!player)
        return;

    // we should to add each player in queue
    AddPlayer(player, isInZone, true, false, false);
    player->GetSession()->SendBattlefieldInvitePlayerToQueue(battleId);
}

void Battlefield::InviteNewPlayerToWar(Player* player, uint32 battleId, bool inZone)
{
    if (!player)
        return;

    // we should to add each player in queue, doesn't matter war status
    AddPlayer(player, inZone, true, true, false, GameTime::GetGameTime() + m_TimeForAcceptInvite);
    player->GetSession()->SendBattlefieldInvitePlayerToWar(battleId, m_ZoneId, m_TimeForAcceptInvite);
}

// Called when a player enters the zone
void Battlefield::HandlePlayerEnterZone(Player* player, uint32 /*zone*/)
{
    if (HasPlayer(player))
        UpdateZoneStatusInPlayerMap(player, true);
    else
    {
        if (IsWarTime())
        {
            if (GetFreeslot(player->GetTeamId()))
            {
                // low level
                if (player->GetLevel() < m_MinLevel)
                {
                    sWorld->SendServerMessage(SERVER_MSG_STRING, "You have a low level and will be removed from Wintergrasp in 20 seconds", player);
                    AddPlayer(player, true, false, false, true, GameTime::GetGameTime() + m_TimeForAcceptInvite);
                    return;
                }

                InviteNewPlayerToWar(player, m_BattleId, true);
            }
            else  // no free slots for player
            {
                sWorld->SendServerMessage(SERVER_MSG_STRING, "You will be removed from Wintergrasp in 20 seconds", player);
                player->GetSession()->SendBattlefieldEjectPending(m_BattleId, true);
                AddPlayer(player, true, false, false, true, GameTime::GetGameTime() + m_TimeForAcceptInvite);
            }
        }
        else
        {
            if (player->GetLevel() < m_MinLevel)
                AddPlayer(player, true, false, false, false);   // we should add him in playermap, when war will start, if level will lower - will be kicked
            else
            {
                if (m_StartGrouping && GetFreeslot(player->GetTeamId()))
                    InviteNewPlayerToQueue(player, m_BattleId, true);
                else
                    AddPlayer(player, true, false, false, false);   // we should add him in playermap, when war will start, we will try to add him in war and queue (if will exist free slot)
            }
        }
    }

    OnPlayerEnterZone(player);
}

// Called when a player leave the zone
void Battlefield::HandlePlayerLeaveZone(Player* player, uint32 /*zone*/)
{
    if (HasPlayer(player))
    {
        UpdateZoneStatusInPlayerMap(player, false);

        if (IsWarTime())
        {
            // If the player is participating to the battle
            player->GetSession()->SendBattlefieldLeaveMessage(m_BattleId, false);
            RemovePlayer(player);

            if (Group* group = player->GetGroup()) // Remove the player from the raid group
                group->RemoveMember(player->GetGUID());

            OnPlayerLeaveWar(player);
        }
        else
        {
            // remove player in piece time if he not in queue
            if (NeedToRemove(player))
                RemovePlayer(player);
        }

        //for (auto pointer : m_workshopAndCaptures)
        //    if (pointer.second._capturePoint)
        //        pointer.second._capturePoint->HandlePlayerLeave(player);
    }

    SendRemoveWorldStates(player);
    OnPlayerLeaveZone(player);
}

// Called in WorldSession::HandleBfQueueInviteResponse
void Battlefield::PlayerAcceptInviteToQueue(Player* player)
{
    PlayerHolderContainer::iterator itr = m_PlayerMap.find(player->GetGUID());
    if (itr != m_PlayerMap.end())
    {
        if (!itr->second.inQueue && itr->second.isWaitingQueue)
        {
            if (GetFreeslot(player->GetTeamId()))
            {
                m_freeslots[player->GetTeamId()]--;
                itr->second.isWaitingQueue = false;
                itr->second.inQueue = true;
                // Send notification
                player->GetSession()->SendBattlefieldQueueInviteResponse(m_BattleId, m_ZoneId);
            }
            else
            {
                itr->second.isWaitingQueue = false;
                itr->second.inQueue = false;
                // Send notification
                player->GetSession()->SendBattlefieldQueueInviteResponse(m_BattleId, m_ZoneId, false, true);
            }
        }
    }
}

// Called in WorldSession::HandleBfEntryInviteResponse
void Battlefield::PlayerAcceptInviteToWar(Player* player)
{
    if (!IsWarTime())
        return;

    PlayerHolderContainer::iterator itr = m_PlayerMap.find(player->GetGUID());
    if (itr != m_PlayerMap.end())
    {
        // if newplayer trying to enter in war we should add him in queue
        if (itr->second.isWaitingQueue)
        {
            if (GetFreeslot(player->GetTeamId()) && !player->isAFK())
            {
                m_freeslots[player->GetTeamId()]--;
                itr->second.isWaitingQueue = false;
                itr->second.isWaitingWar = false;
                itr->second.inQueue = true;
            }
            else // if player has declined invite in queue in zone, and bf has started, but no more free slots
            {
                itr->second.isWaitingQueue = false;
                itr->second.isWaitingWar = false;
                // just kick him by next update tick
                itr->second.isWaitingKick = true;
                itr->second.time = GameTime::GetGameTime();
                player->GetSession()->SendBattlefieldEjectPending(m_BattleId, true);
            }
        }

        // Check that this player already in queue
        if (itr->second.inQueue)
        {
            itr->second.isWaitingWar = false;
            itr->second.inWar = true;
            if (AddOrSetPlayerToCorrectBfGroup(player))
            {
                player->GetSession()->SendBattlefieldEntered(m_BattleId);
                OnPlayerJoinWar(player);                               //for scripting
            }
        }
    }
}

void Battlefield::PlayerDeclineInviteToWar(Player* player)
{
    PlayerHolderContainer::iterator itr = m_PlayerMap.find(player->GetGUID());
    if (itr != m_PlayerMap.end())
    {
        itr->second.isWaitingWar = false;
        itr->second.isWaitingKick = true;
        itr->second.time = GameTime::GetGameTime() + m_TimeForAcceptInvite;
        sWorld->SendServerMessage(SERVER_MSG_STRING, "You will be removed from Wintergrasp in 20 seconds", player);
        player->GetSession()->SendBattlefieldLeaveMessage(m_BattleId, false);
    }
}

// Called in WorldSession::HandleBfQueueExitRequest
void Battlefield::AskToLeaveQueue(Player* player)
{
    if (IsWarTime())
        PlayerAskToLeave(player);

    RemovePlayer(player);
    player->GetSession()->SendBattlefieldLeaveMessage(m_BattleId, false);
}

// Called in zone_wintergrasp
void Battlefield::PlayerTryToReQueueByLevelUp(Player* player)
{
    PlayerHolderContainer::iterator itr = m_PlayerMap.find(player->GetGUID());
    if (itr != m_PlayerMap.end())
    {
        if (!itr->second.inWar
            && !itr->second.isWaitingWar
            && !itr->second.inQueue
            && !itr->second.isWaitingQueue)
        {
            if (IsWarTime())
            {
                if (GetFreeslot(player->GetTeamId()))
                {
                    //InviteNewPlayerToWar - ask to join in war
                    itr->second.isWaitingQueue = true;
                    itr->second.isWaitingKick = false;
                    itr->second.time = GameTime::GetGameTime() + m_TimeForAcceptInvite;
                    player->GetSession()->SendBattlefieldInvitePlayerToWar(m_BattleId, m_ZoneId, m_TimeForAcceptInvite);
                }
                else  // no free slots for player
                {
                    // just inform pending about kick from bf
                    itr->second.isWaitingKick = true;
                    itr->second.time = GameTime::GetGameTime() + m_TimeForAcceptInvite;
                    player->GetSession()->SendBattlefieldQueueInviteResponse(m_BattleId, m_ZoneId, false, true);
                }
            }
            else
            {
                if (m_StartGrouping && GetFreeslot(player->GetTeamId()))
                {
                    //InviteNewPlayerToQueue - send invite in queue
                    itr->second.isWaitingQueue = true;
                    itr->second.time = 0;
                    player->GetSession()->SendBattlefieldInvitePlayerToQueue(m_BattleId);
                }
            }
        }
    }
}

// Called in WorldSession::HandleHearthAndResurrect
void Battlefield::PlayerAskToLeave(Player* player)
{
    // Player leaving Wintergrasp, teleport to Dalaran.
    // ToDo: confirm teleport destination.
    player->TeleportTo(571, 5804.1499f, 624.7710f, 647.7670f, 1.6400f);
}

void Battlefield::DoPlaySoundToAll(uint32 soundID)
{
    BroadcastPacketToWar(WorldPackets::Misc::PlaySound(soundID).Write());
}

void Battlefield::BroadcastPacketToWar(WorldPacket const* data) const
{
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (itr->second.inWar)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->SendDirectMessage(data);
        }
}

void Battlefield::BroadcastPacketToZone(WorldPacket const* data) const
{
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (itr->second.inZone)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->SendDirectMessage(data);
        }
}

void Battlefield::BroadcastPacketToQueue(WorldPacket const* data) const
{
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (itr->second.inQueue)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->SendDirectMessage(data);
        }
}

void Battlefield::TeamCastSpell(TeamId team, int32 spellId)
{
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (itr->second.team == team)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (spellId > 0)
                    player->CastSpell(player, uint32(spellId), true);
                else
                    player->RemoveAuraFromStack(uint32(-spellId));
            }
        }
}

void Battlefield::SendInitWorldStatesTo(Player* player)
{
    WorldPackets::WorldState::InitWorldStates packet;
    packet.MapID = m_MapId;
    packet.ZoneID = m_ZoneId;
    packet.AreaID = player->GetAreaId();
    FillInitialWorldStates(packet);

    player->SendDirectMessage(packet.Write());
}

void Battlefield::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (PlayerHolderContainer::const_iterator itr = m_PlayerMap.begin(); itr != m_PlayerMap.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            player->SendUpdateWorldState(field, value);
}

void Battlefield::SendAreaSpiritHealerQueryOpcode(Player* player, ObjectGuid guid)
{
    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
    uint32 time = m_LastResurrectTimer;  // resurrect every 30 seconds

    data << guid << time;
    player->SendDirectMessage(&data);
}
