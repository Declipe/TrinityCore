//
// Arena Replay V112/PR1735 adapted for TrinityCore 3.3.5.
//
#include "ArenaTeamMgr.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "CharacterCache.h"
#include "Chat.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "GameEventMgr.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "Timer.h"
#include "UpdateFields.h"
#include "WorldSession.h"
#include "World.h"
#include "ChatPackets.h"
#include "QueryPackets.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include <DBCStores.h>
#include <algorithm>
#include <cmath>
#include <array>
#include <cstring>
#include <deque>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cctype>
#include <zlib.h>

namespace
{
    constexpr uint32 ARENA_REPLAY_V2_MAGIC = 0x32565241; // "ARV2" little-endian
    constexpr uint32 ARENA_REPLAY_V2_VERSION = 2;
    constexpr uint32 ARENA_REPLAY_FAKE_GUID_BASE = 0xF0000000u;
    constexpr uint32 ARENA_REPLAY_SEND_CAP_PER_UPDATE = 800;
    constexpr uint32 ARENA_REPLAY_LOAD_GRACE_MS = 15000;
    constexpr uint32 ARENA_REPLAY_START_DELAY_MS = 500;
    constexpr uint32 ARENA_REPLAY_PRELOAD_MS = 500;
    constexpr uint32 ARENA_REPLAY_DEDUPE_WINDOW_MS = 20;

    std::vector<Opcodes> const WatchList =
    {
        SMSG_BATTLEGROUND_PLAYER_JOINED,
        SMSG_BATTLEGROUND_PLAYER_LEFT,
        SMSG_NOTIFICATION,
        SMSG_AURA_UPDATE,
        SMSG_WORLD_STATE_UI_TIMER_UPDATE,
        SMSG_COMPRESSED_UPDATE_OBJECT,
        SMSG_AURA_UPDATE_ALL,
        SMSG_NAME_QUERY_RESPONSE,
        SMSG_DESTROY_OBJECT,
        MSG_MOVE_START_FORWARD,
        MSG_MOVE_SET_FACING,
        MSG_MOVE_HEARTBEAT,
        MSG_MOVE_JUMP,
        SMSG_MONSTER_MOVE,
        MSG_MOVE_FALL_LAND,
        SMSG_PERIODICAURALOG,
        SMSG_ARENA_UNIT_DESTROYED,
        MSG_MOVE_START_STRAFE_RIGHT,
        MSG_MOVE_STOP_STRAFE,
        MSG_MOVE_START_STRAFE_LEFT,
        MSG_MOVE_STOP,
        MSG_MOVE_START_BACKWARD,
        MSG_MOVE_START_TURN_LEFT,
        MSG_MOVE_STOP_TURN,
        MSG_MOVE_START_TURN_RIGHT,
        SMSG_SPELL_START,
        SMSG_SPELL_GO,
        SMSG_SPELL_FAILURE,
        SMSG_SPELL_FAILED_OTHER,
        MSG_CHANNEL_START,
        MSG_CHANNEL_UPDATE,
        SMSG_FORCE_RUN_SPEED_CHANGE,
        SMSG_ATTACK_START,
        SMSG_POWER_UPDATE,
        SMSG_ATTACKERSTATEUPDATE,
        SMSG_SPELLDAMAGESHIELD,
        SMSG_SPELLHEALLOG,
        SMSG_SPELLENERGIZELOG,
        SMSG_SPELLNONMELEEDAMAGELOG,
        SMSG_ATTACK_STOP,
        SMSG_SPELLLOGEXECUTE,
        SMSG_EMOTE,
        SMSG_SPELL_DELAYED,
        SMSG_AI_REACTION,
        SMSG_PET_NAME_QUERY_RESPONSE,
        SMSG_CANCEL_AUTO_REPEAT,
        SMSG_UPDATE_OBJECT,
        SMSG_FORCE_FLIGHT_SPEED_CHANGE,
        SMSG_GAMEOBJECT_QUERY_RESPONSE,
        SMSG_FORCE_SWIM_SPEED_CHANGE,
        SMSG_GAMEOBJECT_DESPAWN_ANIM,
        SMSG_CANCEL_COMBAT
    };

    struct PacketRecord
    {
        uint32 TimestampMs = 0;
        WorldPacket Packet;
    };

    struct ReplayActor
    {
        ObjectGuid OriginalGuid;
        ObjectGuid FakeGuid;
        std::string Name;
        uint8 Race = 0;
        uint8 Class = 0;
        uint8 Gender = 0;
        uint32 Team = 0;
    };

    struct MatchRecord
    {
        BattlegroundTypeId TypeId = BATTLEGROUND_TYPE_NONE;
        uint8 ArenaTypeId = 0;
        uint32 MapId = 0;
        uint32 RecordStartMs = 0;
        uint32 InProgressStartMs = 0;
        uint32 PreStartPacketCount = 0;
        std::vector<ReplayActor> Actors;
        std::vector<PacketRecord> Packets;

        std::unordered_map<uint64, uint32> RecentPacketHashTimes;
    };

    struct PlaybackState
    {
        MatchRecord Match;
        uint32 ViewerLowGuid = 0;
        uint32 BgInstanceId = 0;
        uint32 CreatedMs = 0;
        uint32 PlaybackStartMs = 0;
        size_t Cursor = 0;
        bool PlaybackClockStarted = false;
        bool SentInitialNameResponses = false;
        bool SentReplayASInitial = false;
        bool Finished = false;
        uint32 LastOriginalActorDestroyMs = 0;
        uint32 LastNameColorUpdateMs = 0;
        uint32 NameColorUpdateBursts = 0;
        uint32 LastChannelClearMs = 0;
        uint32 ChannelClearBursts = 0;

        bool ProbePaused = false;
        uint32 ProbeStepBudget = 0;
    };

    std::unordered_map<uint32, MatchRecord> Records;
    std::unordered_map<uint32, PlaybackState> ActiveReplays;

    uint64 ReplayUnixMilliseconds()
    {
        using namespace std::chrono;
        return uint64(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    }

    std::string ReplayWallClockString()
    {
        using namespace std::chrono;
        system_clock::time_point now = system_clock::now();
        std::time_t nowTime = system_clock::to_time_t(now);
        std::tm tm{};
        localtime_r(&nowTime, &tm);
        uint64 ms = uint64(duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000u);

        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms;
        return ss.str();
    }

    std::string ReplaySanitizeLogText(std::string text)
    {
        for (char& c : text)
        {
            if (c == '\r' || c == '\n' || c == '\t')
                c = ' ';
        }
        if (text.size() > 160)
            text.resize(160);
        return text;
    }

    void LogReplayManualMarker(Player* player, char const* source, std::string const& text)
    {
        if (!player)
            return;

        uint32 viewerLowGuid = player->GetGUID().GetCounter();
        auto activeItr = ActiveReplays.find(viewerLowGuid);
        ObjectGuid selected = player->GetTarget();
        uint32 nowMs = getMSTime();

        if (activeItr == ActiveReplays.end())
        {
            TC_LOG_ERROR("arena.replay",
                "REPLAY_MARK wall={} unixMs={} source={} viewer={} active=0 map={} pos=({:.3f},{:.3f},{:.3f}) selectedRaw={} selectedLow={} text='{}'",
                ReplayWallClockString(), ReplayUnixMilliseconds(), source ? source : "unknown", viewerLowGuid,
                player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(),
                selected.GetRawValue(), selected.GetCounter(), ReplaySanitizeLogText(text));
            return;
        }

        PlaybackState const& state = activeItr->second;
        uint32 elapsedMs = state.PlaybackClockStarted && nowMs >= state.PlaybackStartMs ? nowMs - state.PlaybackStartMs : 0;

        uint32 nextFrameTs = 0;
        uint32 nextOpcode = 0;
        uint32 nextSize = 0;
        if (state.Cursor < state.Match.Packets.size())
        {
            PacketRecord const& next = state.Match.Packets[state.Cursor];
            nextFrameTs = next.TimestampMs;
            nextOpcode = next.Packet.GetOpcode();
            nextSize = uint32(next.Packet.size());
        }

        TC_LOG_ERROR("arena.replay",
            "REPLAY_MARK wall={} unixMs={} source={} viewer={} active=1 bg={} map={} pos=({:.3f},{:.3f},{:.3f}) selectedRaw={} selectedLow={} cursor={} total={} elapsed={} nextFrameTs={} nextOpcode={} nextSize={} finished={} paused={} stepBudget={} text='{}'",
            ReplayWallClockString(), ReplayUnixMilliseconds(), source ? source : "unknown", viewerLowGuid,
            state.BgInstanceId, player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(),
            selected.GetRawValue(), selected.GetCounter(), uint32(state.Cursor), uint32(state.Match.Packets.size()), elapsedMs,
            nextFrameTs, nextOpcode, nextSize, state.Finished ? 1u : 0u, state.ProbePaused ? 1u : 0u, state.ProbeStepBudget, ReplaySanitizeLogText(text));
    }

    bool IsWatchedOpcode(uint16 opcode)
    {
        return std::find(WatchList.begin(), WatchList.end(), opcode) != WatchList.end();
    }

    bool IsPreStartVisualOpcode(uint16 opcode)
    {
        switch (opcode)
        {
            case SMSG_UPDATE_OBJECT:
            case SMSG_COMPRESSED_UPDATE_OBJECT:
            case SMSG_DESTROY_OBJECT:
            case SMSG_NAME_QUERY_RESPONSE:
            case SMSG_PET_NAME_QUERY_RESPONSE:
            case SMSG_AURA_UPDATE:
            case SMSG_AURA_UPDATE_ALL:
            case SMSG_POWER_UPDATE:
            case SMSG_FORCE_RUN_SPEED_CHANGE:
            case SMSG_FORCE_FLIGHT_SPEED_CHANGE:
            case SMSG_FORCE_SWIM_SPEED_CHANGE:
                return true;
            default:
                return false;
        }
    }

    bool ShouldRecordPacket(Battleground const* bg, WorldPacket const& packet)
    {
        if (!bg)
            return false;

        BattlegroundStatus const status = bg->GetStatus();
        if (status == STATUS_WAIT_JOIN)
            return IsPreStartVisualOpcode(packet.GetOpcode());
        if (status == STATUS_IN_PROGRESS)
            return IsWatchedOpcode(packet.GetOpcode());
        return false;
    }

    uint64 PacketHash(WorldPacket const& packet)
    {
        uint64 hash = 1469598103934665603ull;
        auto mixByte = [&hash](uint8 b)
        {
            hash ^= b;
            hash *= 1099511628211ull;
        };

        uint16 opcode = packet.GetOpcode();
        mixByte(uint8(opcode & 0xFF));
        mixByte(uint8((opcode >> 8) & 0xFF));

        uint8 const* data = packet.size() ? packet.contents() : nullptr;
        for (size_t i = 0; i < packet.size(); ++i)
            mixByte(data[i]);

        return hash;
    }

    bool IsDuplicateRecentPacket(MatchRecord& record, WorldPacket const& packet, uint32 nowMs)
    {
        uint64 hash = PacketHash(packet);
        auto itr = record.RecentPacketHashTimes.find(hash);
        if (itr != record.RecentPacketHashTimes.end() && nowMs - itr->second <= ARENA_REPLAY_DEDUPE_WINDOW_MS)
            return true;

        record.RecentPacketHashTimes[hash] = nowMs;

        if (record.RecentPacketHashTimes.size() > 4096)
        {
            for (auto it = record.RecentPacketHashTimes.begin(); it != record.RecentPacketHashTimes.end();)
            {
                if (nowMs - it->second > 1000)
                    it = record.RecentPacketHashTimes.erase(it);
                else
                    ++it;
            }
        }
        return false;
    }

    bool IsTeamRecorder(Battleground* bg, Player const* player)
    {
        if (!bg || !player)
            return false;

        for (auto const& it : bg->GetPlayers())
        {
            if (it.second.Team != player->GetBGTeam())
                continue;
            return it.first.GetRawValue() == player->GetGUID().GetRawValue();
        }
        return true;
    }

    uint8 PackedGuidMask(uint64 raw)
    {
        uint8 mask = 0;
        for (uint8 i = 0; i < 8; ++i)
        {
            if (uint8((raw >> (i * 8)) & 0xFF))
                mask |= uint8(1 << i);
        }
        return mask;
    }

    uint32 MakeMaskStableFakePlayerCounter(uint32 originalCounter, uint32 salt)
    {
        uint32 counter = 0;
        for (uint8 i = 0; i < 4; ++i)
        {
            uint8 originalByte = uint8((originalCounter >> (i * 8)) & 0xFF);
            if (!originalByte)
                continue;

            uint8 candidate = uint8((uint32(originalByte) + 37u + salt * 53u + i * 29u) & 0xFFu);
            if (!candidate)
                candidate = uint8(1u + ((salt + i * 17u) % 255u));

            counter |= uint32(candidate) << (i * 8);
        }
        return counter;
    }

    void AssignFakeGuids(MatchRecord& match, uint32 viewerLowGuid)
    {
        std::unordered_set<uint32> usedCounters;

        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.OriginalGuid.IsPlayer())
                usedCounters.insert(actor.OriginalGuid.GetCounter());
        }

        for (size_t i = 0; i < match.Actors.size(); ++i)
        {
            ReplayActor& actor = match.Actors[i];
            uint32 originalCounter = actor.OriginalGuid.GetCounter();
            uint8 originalMask = PackedGuidMask(actor.OriginalGuid.GetRawValue());
            uint32 chosenCounter = 0;

            for (uint32 attempt = 0; attempt < 2048; ++attempt)
            {
                uint32 salt = uint32(i + 1) * 97u + attempt + (viewerLowGuid & 0xFFu);
                uint32 candidate = MakeMaskStableFakePlayerCounter(originalCounter, salt);

                if (!candidate)
                    continue;
                if (usedCounters.find(candidate) != usedCounters.end())
                    continue;

                ObjectGuid fake = ObjectGuid::Create<HighGuid::Player>(candidate);
                if (fake == actor.OriginalGuid)
                    continue;
                if (PackedGuidMask(fake.GetRawValue()) != originalMask)
                    continue;

                chosenCounter = candidate;
                break;
            }

            if (!chosenCounter)
            {
                actor.FakeGuid = actor.OriginalGuid;
                TC_LOG_ERROR("arena.replay", "Replay fake GUID fallback used for original={} mask={}",
                    actor.OriginalGuid.GetRawValue(), uint32(originalMask));
                continue;
            }

            usedCounters.insert(chosenCounter);
            actor.FakeGuid = ObjectGuid::Create<HighGuid::Player>(chosenCounter);

            TC_LOG_INFO("arena.replay", "Replay fake GUID map original={} fake={} originalMask={} fakeMask={}",
                actor.OriginalGuid.GetRawValue(), actor.FakeGuid.GetRawValue(),
                uint32(originalMask), uint32(PackedGuidMask(actor.FakeGuid.GetRawValue())));
        }
    }

    void RefreshActorsFromBattleground(Battleground* bg, MatchRecord& match)
    {
        if (!bg)
            return;

        std::unordered_set<uint64> seen;
        for (ReplayActor const& actor : match.Actors)
            seen.insert(actor.OriginalGuid.GetRawValue());

        for (auto const& it : bg->GetPlayers())
        {
            if (seen.find(it.first.GetRawValue()) != seen.end())
                continue;

            Player* player = bg->_GetPlayer(it.first, it.second.OfflineRemoveTime != 0, "arena replay actor capture");
            if (!player)
                continue;

            ReplayActor actor;
            actor.OriginalGuid = player->GetGUID();
            actor.Name = player->GetName();
            actor.Race = player->GetRace();
            actor.Class = player->GetClass();
            actor.Gender = uint8(player->GetGender());
            actor.Team = it.second.Team;

            match.Actors.push_back(actor);
            seen.insert(actor.OriginalGuid.GetRawValue());
        }
    }

    MatchRecord& GetOrCreateRecord(Battleground* bg)
    {
        MatchRecord& record = Records[bg->GetInstanceID()];

        if (!record.RecordStartMs)
            record.RecordStartMs = getMSTime();

        record.TypeId = bg->GetTypeID(false);
        if (record.TypeId == BATTLEGROUND_AA)
            record.TypeId = bg->GetTypeID(true);

        record.ArenaTypeId = bg->GetArenaType();
        record.MapId = bg->GetMapId();

        RefreshActorsFromBattleground(bg, record);
        return record;
    }

    MatchRecord& GetOrCreateRecord(Battlefield* bf)
    {
        MatchRecord& record = Records[bf->GetZoneId()];

        if (!record.RecordStartMs)
            record.RecordStartMs = getMSTime();

        record.TypeId = BATTLEGROUND_TYPE_NONE;
        record.ArenaTypeId = 0;
        record.MapId = bf->GetMapId();

        std::unordered_set<uint64> seen;
        for (ReplayActor const& actor : record.Actors)
            seen.insert(actor.OriginalGuid.GetRawValue());

        for (auto const& it : bf->GetPlayerMap())
        {
            if (seen.find(it.first.GetRawValue()) != seen.end())
                continue;

            Player* player = ObjectAccessor::FindConnectedPlayer(it.first);
            if (!player)
                continue;

            ReplayActor actor;
            actor.OriginalGuid = player->GetGUID();
            actor.Name = player->GetName();
            actor.Race = player->GetRace();
            actor.Class = player->GetClass();
            actor.Gender = uint8(player->GetGender());
            actor.Team = player->GetBGTeam();

            record.Actors.push_back(actor);
            seen.insert(actor.OriginalGuid.GetRawValue());
        }

        return record;
    }

    std::vector<uint8> ToRawGuidBytes(uint64 raw)
    {
        std::vector<uint8> bytes(8);
        for (uint8 i = 0; i < 8; ++i)
            bytes[i] = uint8((raw >> (i * 8)) & 0xFF);
        return bytes;
    }

    std::vector<uint8> ToPackedGuidBytes(uint64 raw)
    {
        std::vector<uint8> out;
        out.reserve(9);
        uint8 mask = 0;
        std::vector<uint8> nonZero;
        nonZero.reserve(8);

        for (uint8 i = 0; i < 8; ++i)
        {
            uint8 b = uint8((raw >> (i * 8)) & 0xFF);
            if (b)
            {
                mask |= uint8(1 << i);
                nonZero.push_back(b);
            }
        }
        out.push_back(mask);
        out.insert(out.end(), nonZero.begin(), nonZero.end());
        return out;
    }

    ReplayActor const* FindReplayActorByOriginalGuid(MatchRecord const& match, ObjectGuid guid)
    {
        if (guid.IsEmpty())
            return nullptr;
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.OriginalGuid == guid)
                return &actor;
        }
        return nullptr;
    }

    ReplayActor const* FindReplayActorByOriginalGuidOrCounter(MatchRecord const& match, ObjectGuid guid)
    {
        if (guid.IsEmpty())
            return nullptr;
        uint32 const counter = guid.GetCounter();
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.OriginalGuid == guid)
                return &actor;
            if (counter && actor.OriginalGuid.GetCounter() == counter)
                return &actor;
        }
        return nullptr;
    }

    ReplayActor const* FindReplayActorByGuid(MatchRecord const& match, ObjectGuid guid)
    {
        if (guid.IsEmpty())
            return nullptr;
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.OriginalGuid == guid || actor.FakeGuid == guid)
                return &actor;
        }
        return nullptr;
    }

    ReplayActor const* FindReplayActorByGuidOrCounter(MatchRecord const& match, ObjectGuid guid)
    {
        if (guid.IsEmpty())
            return nullptr;
        uint32 const counter = guid.GetCounter();
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.OriginalGuid == guid || actor.FakeGuid == guid)
                return &actor;
            if (counter && (actor.OriginalGuid.GetCounter() == counter || actor.FakeGuid.GetCounter() == counter))
                return &actor;
        }
        return nullptr;
    }

    bool IsReplayActorGuid(MatchRecord const& match, ObjectGuid guid)
    {
        return FindReplayActorByGuidOrCounter(match, guid) != nullptr;
    }

    bool HasRemaining(std::vector<uint8> const& payload, size_t pos, size_t count)
    {
        return pos <= payload.size() && count <= payload.size() - pos;
    }

    bool ReadUInt8(std::vector<uint8> const& payload, size_t& pos, uint8& value)
    {
        if (!HasRemaining(payload, pos, 1))
            return false;
        value = payload[pos++];
        return true;
    }

    bool ReadUInt16(std::vector<uint8> const& payload, size_t& pos, uint16& value)
    {
        if (!HasRemaining(payload, pos, 2))
            return false;
        value = uint16(payload[pos]) | (uint16(payload[pos + 1]) << 8);
        pos += 2;
        return true;
    }

    bool ReadUInt32(std::vector<uint8> const& payload, size_t& pos, uint32& value)
    {
        if (!HasRemaining(payload, pos, 4))
            return false;
        value = uint32(payload[pos]) | (uint32(payload[pos + 1]) << 8) | (uint32(payload[pos + 2]) << 16) | (uint32(payload[pos + 3]) << 24);
        pos += 4;
        return true;
    }

    void WriteUInt16(std::vector<uint8>& payload, size_t pos, uint16 value)
    {
        if (!HasRemaining(payload, pos, 2))
            return;
        payload[pos] = uint8(value & 0xFF);
        payload[pos + 1] = uint8((value >> 8) & 0xFF);
    }

    void WriteUInt32(std::vector<uint8>& payload, size_t pos, uint32 value)
    {
        if (!HasRemaining(payload, pos, 4))
            return;
        payload[pos] = uint8(value & 0xFF);
        payload[pos + 1] = uint8((value >> 8) & 0xFF);
        payload[pos + 2] = uint8((value >> 16) & 0xFF);
        payload[pos + 3] = uint8((value >> 24) & 0xFF);
    }

    bool ReadUInt64At(std::vector<uint8> const& payload, size_t pos, uint64& value)
    {
        if (!HasRemaining(payload, pos, 8))
            return false;
        value = 0;
        for (uint8 i = 0; i < 8; ++i)
            value |= uint64(payload[pos + i]) << (i * 8);
        return true;
    }

    void WriteUInt64(std::vector<uint8>& payload, size_t pos, uint64 value)
    {
        if (!HasRemaining(payload, pos, 8))
            return;
        for (uint8 i = 0; i < 8; ++i)
            payload[pos + i] = uint8((value >> (i * 8)) & 0xFF);
    }

    bool ReadPackedGuid(std::vector<uint8> const& payload, size_t& pos, ObjectGuid& guid)
    {
        uint8 mask = 0;
        if (!ReadUInt8(payload, pos, mask))
            return false;

        uint64 raw = 0;
        for (uint8 i = 0; i < 8; ++i)
        {
            if (!(mask & (1 << i)))
                continue;

            uint8 byte = 0;
            if (!ReadUInt8(payload, pos, byte))
                return false;

            raw |= uint64(byte) << (i * 8);
        }
        guid.SetRawValue(raw);
        return true;
    }

    bool SkipPackedGuid(std::vector<uint8> const& payload, size_t& pos)
    {
        ObjectGuid ignored;
        return ReadPackedGuid(payload, pos, ignored);
    }

    bool WritePackedGuidInPlace(std::vector<uint8>& payload, size_t start, size_t end, ObjectGuid guid)
    {
        std::vector<uint8> packed = ToPackedGuidBytes(guid.GetRawValue());
        if (packed.size() != end - start)
        {
            TC_LOG_ERROR("arena.replay", "Replay packed GUID rewrite refused: oldSize={} newSize={} guid={}",
                uint32(end - start), uint32(packed.size()), guid.GetRawValue());
            return false;
        }
        if (!HasRemaining(payload, start, packed.size()))
            return false;

        std::copy(packed.begin(), packed.end(), payload.begin() + start);
        return true;
    }

    bool WritePackedGuidResize(std::vector<uint8>& payload, size_t start, size_t end, ObjectGuid guid, size_t* posAfter = nullptr)
    {
        if (start > end || end > payload.size())
            return false;

        std::vector<uint8> packed = ToPackedGuidBytes(guid.GetRawValue());
        payload.erase(payload.begin() + start, payload.begin() + end);
        payload.insert(payload.begin() + start, packed.begin(), packed.end());

        if (posAfter)
            *posAfter = start + packed.size();
        return true;
    }

    bool RewritePackedGuidAtResize(std::vector<uint8>& payload, size_t& pos, MatchRecord const& match, ObjectGuid* parsedGuid = nullptr)
    {
        size_t const guidStart = pos;
        ObjectGuid guid;
        if (!ReadPackedGuid(payload, pos, guid))
            return false;

        size_t const guidEnd = pos;
        if (parsedGuid)
            *parsedGuid = guid;

        if (ReplayActor const* actor = FindReplayActorByOriginalGuidOrCounter(match, guid))
        {
            if (!WritePackedGuidResize(payload, guidStart, guidEnd, actor->FakeGuid, &pos))
                return false;
        }
        return true;
    }

    bool RewriteFirstPackedGuidIfReplayActor(std::vector<uint8>& payload, MatchRecord const& match)
    {
        size_t pos = 0;
        size_t guidStart = pos;

        ObjectGuid guid;
        if (!ReadPackedGuid(payload, pos, guid))
            return false;

        size_t guidEnd = pos;
        if (ReplayActor const* actor = FindReplayActorByOriginalGuid(match, guid))
            return WritePackedGuidInPlace(payload, guidStart, guidEnd, actor->FakeGuid);
        return false;
    }

    ReplayActor const* RewriteFirstPackedGuidIfReplayActorOrCounter(std::vector<uint8>& payload, MatchRecord const& match)
    {
        size_t pos = 0;
        size_t guidStart = pos;

        ObjectGuid guid;
        if (!ReadPackedGuid(payload, pos, guid))
            return nullptr;

        size_t guidEnd = pos;
        if (ReplayActor const* actor = FindReplayActorByOriginalGuidOrCounter(match, guid))
        {
            if (WritePackedGuidInPlace(payload, guidStart, guidEnd, actor->FakeGuid))
                return actor;
        }
        return nullptr;
    }

    uint32 RewriteAllPackedReplayActorGuidByteSequences(std::vector<uint8>& payload, MatchRecord const& match)
    {
        uint32 replacements = 0;
        for (ReplayActor const& actor : match.Actors)
        {
            std::vector<uint8> from = ToPackedGuidBytes(actor.OriginalGuid.GetRawValue());
            std::vector<uint8> to = ToPackedGuidBytes(actor.FakeGuid.GetRawValue());

            if (from.empty() || from.size() != to.size())
                continue;

            auto it = payload.begin();
            while (it != payload.end())
            {
                it = std::search(it, payload.end(), from.begin(), from.end());
                if (it == payload.end())
                    break;

                std::copy(to.begin(), to.end(), it);
                it += to.size();
                ++replacements;
            }
        }
        return replacements;
    }

    bool RewritePackedGuidAt(std::vector<uint8>& payload, size_t& pos, MatchRecord const& match)
    {
        size_t guidStart = pos;
        ObjectGuid guid;
        if (!ReadPackedGuid(payload, pos, guid))
            return false;

        size_t guidEnd = pos;
        if (ReplayActor const* actor = FindReplayActorByOriginalGuid(match, guid))
            WritePackedGuidInPlace(payload, guidStart, guidEnd, actor->FakeGuid);
        return true;
    }

    bool RewriteAuraUpdatePacketGuids(std::vector<uint8>& payload, MatchRecord const& match)
    {
        constexpr uint8 REPLAY_AFLAG_CASTER = 0x08;
        constexpr uint8 REPLAY_AFLAG_DURATION = 0x20;
        constexpr uint32 REPLAY_SPELL_GHOST = 8326;

        size_t pos = 0;
        if (!RewritePackedGuidAt(payload, pos, match))
            return false;

        std::vector<uint8> filtered;
        filtered.reserve(payload.size());
        filtered.insert(filtered.end(), payload.begin(), payload.begin() + pos);

        while (pos < payload.size())
        {
            size_t const entryStart = pos;
            uint8 slot = 0;
            if (!ReadUInt8(payload, pos, slot))
                return false;

            uint32 spellId = 0;
            if (!ReadUInt32(payload, pos, spellId))
                return false;

            if (!spellId)
            {
                filtered.insert(filtered.end(), payload.begin() + entryStart, payload.begin() + pos);
                continue;
            }

            uint8 flags = 0;
            uint8 casterLevel = 0;
            uint8 charges = 0;

            if (!ReadUInt8(payload, pos, flags) || !ReadUInt8(payload, pos, casterLevel) || !ReadUInt8(payload, pos, charges))
                return false;

            if (!(flags & REPLAY_AFLAG_CASTER))
            {
                if (!RewritePackedGuidAt(payload, pos, match))
                    return false;
            }

            if (flags & REPLAY_AFLAG_DURATION)
            {
                if (!HasRemaining(payload, pos, 8))
                    return false;
                pos += 8;
            }

            if (spellId == REPLAY_SPELL_GHOST)
            {
                filtered.push_back(slot);
                filtered.push_back(0);
                filtered.push_back(0);
                filtered.push_back(0);
                filtered.push_back(0);
                continue;
            }

            filtered.insert(filtered.end(), payload.begin() + entryStart, payload.begin() + pos);
        }

        payload.swap(filtered);
        return true;
    }

    bool FirstRawGuidIsOriginalReplayActor(std::vector<uint8> const& payload, MatchRecord const& match, ObjectGuid* outGuid = nullptr)
    {
        uint64 raw = 0;
        if (!ReadUInt64At(payload, 0, raw))
            return false;

        ObjectGuid guid;
        guid.SetRawValue(raw);
        if (FindReplayActorByOriginalGuid(match, guid))
        {
            if (outGuid)
                *outGuid = guid;
            return true;
        }
        return false;
    }

    bool RewriteFirstRawGuidIfReplayActor(std::vector<uint8>& payload, MatchRecord const& match)
    {
        uint64 raw = 0;
        if (!ReadUInt64At(payload, 0, raw))
            return false;

        ObjectGuid guid;
        guid.SetRawValue(raw);
        if (ReplayActor const* actor = FindReplayActorByOriginalGuid(match, guid))
        {
            WriteUInt64(payload, 0, actor->FakeGuid.GetRawValue());
            return true;
        }
        return false;
    }

    void WriteUInt64BytesLE(std::array<uint8, 8>& bytes, uint64 value)
    {
        for (uint8 i = 0; i < 8; ++i)
            bytes[i] = uint8((value >> (i * 8)) & 0xFF);
    }

    uint32 RewriteAllRawReplayActorGuids(std::vector<uint8>& payload, MatchRecord const& match)
    {
        uint32 replacements = 0;
        if (payload.size() < 8)
            return replacements;

        for (ReplayActor const& actor : match.Actors)
        {
            std::array<uint8, 8> originalBytes{};
            std::array<uint8, 8> fakeBytes{};

            WriteUInt64BytesLE(originalBytes, actor.OriginalGuid.GetRawValue());
            WriteUInt64BytesLE(fakeBytes, actor.FakeGuid.GetRawValue());

            for (size_t i = 0; i + 8 <= payload.size(); ++i)
            {
                if (std::equal(originalBytes.begin(), originalBytes.end(), payload.begin() + i))
                {
                    std::copy(fakeBytes.begin(), fakeBytes.end(), payload.begin() + i);
                    ++replacements;
                    i += 7;
                }
            }
        }
        return replacements;
    }

    bool IsMovementLikeOpcode(uint16 opcode)
    {
        switch (opcode)
        {
            case MSG_MOVE_START_FORWARD:
            case MSG_MOVE_SET_FACING:
            case MSG_MOVE_HEARTBEAT:
            case MSG_MOVE_JUMP:
            case MSG_MOVE_FALL_LAND:
            case MSG_MOVE_START_STRAFE_RIGHT:
            case MSG_MOVE_STOP_STRAFE:
            case MSG_MOVE_START_STRAFE_LEFT:
            case MSG_MOVE_STOP:
            case MSG_MOVE_START_BACKWARD:
            case MSG_MOVE_START_TURN_LEFT:
            case MSG_MOVE_STOP_TURN:
            case MSG_MOVE_START_TURN_RIGHT:
            case SMSG_FORCE_RUN_SPEED_CHANGE:
            case SMSG_FORCE_FLIGHT_SPEED_CHANGE:
            case SMSG_FORCE_SWIM_SPEED_CHANGE:
            case SMSG_MONSTER_MOVE:
                return true;
            default:
                return false;
        }
    }

    bool RewriteRawGuidAt(std::vector<uint8>& payload, size_t pos, MatchRecord const& match)
    {
        uint64 raw = 0;
        if (!ReadUInt64At(payload, pos, raw))
            return false;

        ObjectGuid guid;
        guid.SetRawValue(raw);
        if (ReplayActor const* actor = FindReplayActorByOriginalGuid(match, guid))
            WriteUInt64(payload, pos, actor->FakeGuid.GetRawValue());
        return true;
    }

    bool RewriteRawGuidAndAdvance(std::vector<uint8>& payload, size_t& pos, MatchRecord const& match)
    {
        if (!HasRemaining(payload, pos, 8))
            return false;

        RewriteRawGuidAt(payload, pos, match);
        pos += 8;
        return true;
    }

    bool SkipReplayCString(std::vector<uint8> const& payload, size_t& pos)
    {
        while (pos < payload.size())
        {
            uint8 ch = payload[pos++];
            if (!ch)
                return true;
        }
        return false;
    }

    bool RewriteSpellTargetDataGuids(std::vector<uint8>& payload, size_t& pos, MatchRecord const& match)
    {
        constexpr uint32 REPLAY_TARGET_FLAG_UNIT            = 0x00000002;
        constexpr uint32 REPLAY_TARGET_FLAG_ITEM            = 0x00000010;
        constexpr uint32 REPLAY_TARGET_FLAG_SOURCE_LOCATION = 0x00000020;
        constexpr uint32 REPLAY_TARGET_FLAG_DEST_LOCATION   = 0x00000040;
        constexpr uint32 REPLAY_TARGET_FLAG_CORPSE_ENEMY    = 0x00000200;
        constexpr uint32 REPLAY_TARGET_FLAG_GAMEOBJECT      = 0x00000800;
        constexpr uint32 REPLAY_TARGET_FLAG_TRADE_ITEM      = 0x00001000;
        constexpr uint32 REPLAY_TARGET_FLAG_STRING          = 0x00002000;
        constexpr uint32 REPLAY_TARGET_FLAG_CORPSE_ALLY     = 0x00008000;
        constexpr uint32 REPLAY_TARGET_FLAG_UNIT_MINIPET    = 0x00010000;

        uint32 targetFlags = 0;
        if (!ReadUInt32(payload, pos, targetFlags))
            return false;

        if (targetFlags & (REPLAY_TARGET_FLAG_UNIT | REPLAY_TARGET_FLAG_CORPSE_ALLY | REPLAY_TARGET_FLAG_GAMEOBJECT |
                           REPLAY_TARGET_FLAG_CORPSE_ENEMY | REPLAY_TARGET_FLAG_UNIT_MINIPET))
        {
            if (!RewritePackedGuidAt(payload, pos, match))
                return false;
        }

        if (targetFlags & (REPLAY_TARGET_FLAG_ITEM | REPLAY_TARGET_FLAG_TRADE_ITEM))
        {
            if (!RewritePackedGuidAt(payload, pos, match))
                return false;
        }

        if (targetFlags & REPLAY_TARGET_FLAG_SOURCE_LOCATION)
        {
            if (!RewritePackedGuidAt(payload, pos, match))
                return false;
            if (!HasRemaining(payload, pos, 12))
                return false;
            pos += 12;
        }

        if (targetFlags & REPLAY_TARGET_FLAG_DEST_LOCATION)
        {
            if (!RewritePackedGuidAt(payload, pos, match))
                return false;
            if (!HasRemaining(payload, pos, 12))
                return false;
            pos += 12;
        }

        if (targetFlags & REPLAY_TARGET_FLAG_STRING)
        {
            if (!SkipReplayCString(payload, pos))
                return false;
        }
        return true;
    }

    bool RewriteSpellCastDataGuids(std::vector<uint8>& payload, MatchRecord const& match, bool hasGoTargets)
    {
        constexpr uint8 REPLAY_SPELL_MISS_REFLECT = 11;

        std::vector<uint8> original = payload;
        size_t pos = 0;

        if (!RewritePackedGuidAt(payload, pos, match))
        {
            payload.swap(original);
            return false;
        }
        if (!RewritePackedGuidAt(payload, pos, match))
        {
            payload.swap(original);
            return false;
        }

        if (!HasRemaining(payload, pos, 13))
        {
            payload.swap(original);
            return false;
        }
        pos += 13;

        if (hasGoTargets)
        {
            uint8 hitCount = 0;
            if (!ReadUInt8(payload, pos, hitCount))
            {
                payload.swap(original);
                return false;
            }

            for (uint8 i = 0; i < hitCount; ++i)
            {
                if (!RewriteRawGuidAndAdvance(payload, pos, match))
                {
                    payload.swap(original);
                    return false;
                }
            }

            uint8 missCount = 0;
            if (!ReadUInt8(payload, pos, missCount))
            {
                payload.swap(original);
                return false;
            }

            for (uint8 i = 0; i < missCount; ++i)
            {
                if (!RewriteRawGuidAndAdvance(payload, pos, match))
                {
                    payload.swap(original);
                    return false;
                }

                uint8 missReason = 0;
                if (!ReadUInt8(payload, pos, missReason))
                {
                    payload.swap(original);
                    return false;
                }

                if (missReason == REPLAY_SPELL_MISS_REFLECT)
                {
                    if (!HasRemaining(payload, pos, 1))
                    {
                        payload.swap(original);
                        return false;
                    }
                    pos += 1;
                }
            }
        }

        if (!RewriteSpellTargetDataGuids(payload, pos, match))
        {
            payload.swap(original);
            return false;
        }
        return true;
    }

    void RewriteTwoPackedGuids(std::vector<uint8>& payload, MatchRecord const& match)
    {
        std::vector<uint8> original = payload;
        size_t pos = 0;
        if (!RewritePackedGuidAt(payload, pos, match) || !RewritePackedGuidAt(payload, pos, match))
            payload.swap(original);
    }

    void RewritePacketPackedGuidAfterUInt32(std::vector<uint8>& payload, MatchRecord const& match)
    {
        std::vector<uint8> original = payload;
        size_t pos = 4;
        if (!RewritePackedGuidAt(payload, pos, match))
            payload.swap(original);
    }

    void RewriteRawGuidPair(std::vector<uint8>& payload, MatchRecord const& match)
    {
        RewriteRawGuidAt(payload, 0, match);
        RewriteRawGuidAt(payload, 8, match);
    }

    void RewriteNonUpdatePacketGuids(uint16 opcode, std::vector<uint8>& payload, MatchRecord const& match)
    {
        if (opcode == SMSG_MONSTER_MOVE)
        {
            uint32 rawReplacements = RewriteAllRawReplayActorGuids(payload, match);
            if (!rawReplacements)
                RewriteFirstPackedGuidIfReplayActor(payload, match);
            return;
        }

        if (IsMovementLikeOpcode(opcode))
        {
            RewriteFirstPackedGuidIfReplayActor(payload, match);
            return;
        }

        switch (opcode)
        {
            case SMSG_SPELL_START:
                RewriteSpellCastDataGuids(payload, match, false);
                return;
            case SMSG_SPELL_GO:
                RewriteSpellCastDataGuids(payload, match, true);
                return;
            case SMSG_SPELL_FAILURE:
            case SMSG_SPELL_FAILED_OTHER:
                RewriteFirstPackedGuidIfReplayActorOrCounter(payload, match);
                return;
            case MSG_CHANNEL_START:
            case MSG_CHANNEL_UPDATE:
                RewriteFirstPackedGuidIfReplayActorOrCounter(payload, match);
                return;
            case SMSG_ATTACK_START:
                RewriteRawGuidPair(payload, match);
                return;
            case SMSG_ATTACK_STOP:
                RewriteTwoPackedGuids(payload, match);
                return;
            case SMSG_ATTACKERSTATEUPDATE:
                RewritePacketPackedGuidAfterUInt32(payload, match);
                {
                    std::vector<uint8> original = payload;
                    size_t pos = 4;
                    if (RewritePackedGuidAt(payload, pos, match) && RewritePackedGuidAt(payload, pos, match))
                        return;
                    payload.swap(original);
                }
                return;
            case SMSG_SPELLNONMELEEDAMAGELOG:
            case SMSG_PERIODICAURALOG:
            case SMSG_SPELLHEALLOG:
            case SMSG_SPELLENERGIZELOG:
                RewriteTwoPackedGuids(payload, match);
                return;
            case SMSG_SPELLDAMAGESHIELD:
                RewriteRawGuidPair(payload, match);
                return;
            case SMSG_SPELLLOGEXECUTE:
            {
                RewriteFirstPackedGuidIfReplayActor(payload, match);
                RewriteAllPackedReplayActorGuidByteSequences(payload, match);
                return;
            }
            case SMSG_AURA_UPDATE:
            case SMSG_AURA_UPDATE_ALL:
            {
                if (!RewriteAuraUpdatePacketGuids(payload, match))
                    RewriteFirstPackedGuidIfReplayActor(payload, match);

                RewriteAllPackedReplayActorGuidByteSequences(payload, match);
                return;
            }
            case SMSG_SPELL_DELAYED:
            case SMSG_POWER_UPDATE:
            case SMSG_CANCEL_AUTO_REPEAT:
                RewriteFirstPackedGuidIfReplayActor(payload, match);
                return;
            case SMSG_EMOTE:
                RewriteRawGuidAt(payload, 4, match);
                return;
            case SMSG_AI_REACTION:
                RewriteFirstRawGuidIfReplayActor(payload, match);
                return;
            case SMSG_DESTROY_OBJECT:
            case SMSG_ARENA_UNIT_DESTROYED:
            {
                ObjectGuid originalActorGuid;
                if (FirstRawGuidIsOriginalReplayActor(payload, match, &originalActorGuid))
                {
                    payload.clear();
                    return;
                }
                RewriteFirstRawGuidIfReplayActor(payload, match);
                return;
            }
            case SMSG_BATTLEGROUND_PLAYER_JOINED:
            case SMSG_BATTLEGROUND_PLAYER_LEFT:
                RewriteRawGuidAt(payload, 0, match);
                return;
            default:
                return;
        }
    }

    bool ReplayActorShouldUseFriendlyGreenName(ReplayActor const& actor)
    {
        return actor.Team == HORDE || actor.Team == 67;
    }

    uint32 ReplayFriendlyFactionTemplateForViewer(Player const* /*viewer*/)
    {
        return FACTION_FRIENDLY;
    }

    uint32 ReplayYellowFactionTemplateForActor(ReplayActor const& /*actor*/)
    {
        return FACTION_CREATURE;
    }

    uint32 ReplayNameColorFactionTemplateForActor(Player const* /*viewer*/, ReplayActor const& actor)
    {
        return ReplayActorShouldUseFriendlyGreenName(actor)
            ? FACTION_FRIENDLY
            : FACTION_CREATURE;
    }

    uint32 ReplayGreenNameUnitBytes2ForActor(ReplayActor const& actor, uint32 originalBytes2)
    {
        if (!ReplayActorShouldUseFriendlyGreenName(actor))
            return originalBytes2;

        uint32 shift = uint32(UNIT_BYTES_2_OFFSET_PVP_FLAG) * 8u;
        uint32 clearMask = ~(0xFFu << shift);
        return originalBytes2 & clearMask;
    }

    uint32 ReplayGreenNameUnitFlagsForActor(ReplayActor const& actor, uint32 originalFlags)
    {
        if (!ReplayActorShouldUseFriendlyGreenName(actor))
            return originalFlags;

        uint32 flags = originalFlags;
        flags &= ~UNIT_FLAG_NON_ATTACKABLE;
        flags &= ~UNIT_FLAG_PACIFIED;
        flags &= ~UNIT_FLAG_IMMUNE_TO_PC;
        return flags;
    }

    uint32 ReplayGreenNamePlayerFlagsForActor(ReplayActor const& actor, uint32 originalFlags)
    {
        if (!ReplayActorShouldUseFriendlyGreenName(actor))
            return originalFlags;

        uint32 flags = originalFlags;
        flags &= ~PLAYER_FLAGS_IN_PVP;
        flags &= ~PLAYER_FLAGS_CONTESTED_PVP;
        return flags;
    }

    bool ReplayUpdateFieldIsSafeForRemoteReplayActor(uint32 fieldIndex)
    {
        if (fieldIndex < OBJECT_END)
            return fieldIndex != OBJECT_FIELD_PADDING;

        if (fieldIndex < UNIT_END)
        {
            if (fieldIndex == UNIT_FIELD_CRITTER || fieldIndex == UNIT_FIELD_CRITTER + 1)
                return false;

            if (fieldIndex >= UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER && fieldIndex < UNIT_FIELD_LEVEL)
                return false;

            if (fieldIndex == UNIT_FIELD_RANGEDATTACKTIME)
                return false;

            if (fieldIndex >= UNIT_FIELD_MINDAMAGE && fieldIndex <= UNIT_FIELD_MAXOFFHANDDAMAGE)
                return false;

            if (fieldIndex == UNIT_FIELD_PETEXPERIENCE || fieldIndex == UNIT_FIELD_PETNEXTLEVELEXP)
                return false;

            if (fieldIndex >= UNIT_FIELD_STAT0 && fieldIndex < UNIT_FIELD_BASE_MANA)
                return false;

            if (fieldIndex == UNIT_FIELD_BASE_HEALTH)
                return false;

            if (fieldIndex >= UNIT_FIELD_ATTACK_POWER && fieldIndex <= UNIT_FIELD_MAXHEALTHMODIFIER)
                return false;

            if (fieldIndex == UNIT_FIELD_PADDING)
                return false;

            return true;
        }

        if (fieldIndex >= PLAYER_DUEL_ARBITER && fieldIndex <= PLAYER_GUILD_TIMESTAMP)
            return true;

        if (fieldIndex >= PLAYER_VISIBLE_ITEM_1_ENTRYID && fieldIndex <= PLAYER_FAKE_INEBRIATION)
            return true;

        return false;
    }

    struct ReplayUpdateFieldValue
    {
        uint32 FieldIndex = 0;
        uint32 Value = 0;
    };

    void AppendUInt32LE(std::vector<uint8>& out, uint32 value)
    {
        out.push_back(uint8(value & 0xFF));
        out.push_back(uint8((value >> 8) & 0xFF));
        out.push_back(uint8((value >> 16) & 0xFF));
        out.push_back(uint8((value >> 24) & 0xFF));
    }

    void RewriteTargetGuidInKeptValues(std::vector<ReplayUpdateFieldValue>& keptValues, MatchRecord const& match, ObjectGuid blockGuid)
    {
        size_t lowIndex = std::numeric_limits<size_t>::max();
        size_t highIndex = std::numeric_limits<size_t>::max();
        uint32 low = 0;
        uint32 high = 0;

        for (size_t i = 0; i < keptValues.size(); ++i)
        {
            if (keptValues[i].FieldIndex == UNIT_FIELD_TARGET)
            {
                lowIndex = i;
                low = keptValues[i].Value;
            }
            else if (keptValues[i].FieldIndex == UNIT_FIELD_TARGET + 1)
            {
                highIndex = i;
                high = keptValues[i].Value;
            }
        }

        if (lowIndex == std::numeric_limits<size_t>::max() || highIndex == std::numeric_limits<size_t>::max())
            return;

        ObjectGuid originalTarget;
        originalTarget.SetRawValue(uint64(low) | (uint64(high) << 32));
        if (ReplayActor const* targetActor = FindReplayActorByOriginalGuidOrCounter(match, originalTarget))
        {
            uint64 targetFakeRaw = targetActor->FakeGuid.GetRawValue();
            keptValues[lowIndex].Value = uint32(targetFakeRaw & 0xFFFFFFFFu);
            keptValues[highIndex].Value = uint32((targetFakeRaw >> 32) & 0xFFFFFFFFu);
        }
    }

    bool PatchUpdateValuesBlock(std::vector<uint8>& payload, size_t& pos, MatchRecord const& match, ObjectGuid blockGuid)
    {
        size_t blockPayloadStart = pos;

        uint8 blockCount = 0;
        if (!ReadUInt8(payload, pos, blockCount))
            return false;

        std::vector<uint32> masks;
        masks.reserve(blockCount);

        for (uint8 i = 0; i < blockCount; ++i)
        {
            uint32 mask = 0;
            if (!ReadUInt32(payload, pos, mask))
                return false;

            masks.push_back(mask);
        }

        ReplayActor const* actor = FindReplayActorByOriginalGuidOrCounter(match, blockGuid);
        uint64 fakeRaw = actor ? actor->FakeGuid.GetRawValue() : 0;
        uint32 fakeLow = uint32(fakeRaw & 0xFFFFFFFFu);
        uint32 fakeHigh = uint32((fakeRaw >> 32) & 0xFFFFFFFFu);

        std::vector<ReplayUpdateFieldValue> keptValues;
        keptValues.reserve(32);
        uint32 removedPrivateFields = 0;

        for (uint32 block = 0; block < masks.size(); ++block)
        {
            uint32 mask = masks[block];

            for (uint8 bit = 0; bit < 32; ++bit)
            {
                if (!(mask & (uint32(1) << bit)))
                    continue;

                if (!HasRemaining(payload, pos, 4))
                    return false;

                uint32 fieldIndex = block * 32 + bit;
                uint32 value = uint32(payload[pos]) | (uint32(payload[pos + 1]) << 8) | (uint32(payload[pos + 2]) << 16) | (uint32(payload[pos + 3]) << 24);
                pos += 4;

                if (actor && !ReplayUpdateFieldIsSafeForRemoteReplayActor(fieldIndex))
                {
                    ++removedPrivateFields;
                    continue;
                }

                if (actor)
                {
                    if (fieldIndex == OBJECT_FIELD_GUID)
                        value = fakeLow;
                    else if (fieldIndex == OBJECT_FIELD_GUID + 1)
                        value = fakeHigh;
                    else if (fieldIndex == UNIT_FIELD_FACTIONTEMPLATE)
                        value = ReplayNameColorFactionTemplateForActor(nullptr, *actor);
                    else if (fieldIndex == UNIT_FIELD_BYTES_2)
                        value = ReplayGreenNameUnitBytes2ForActor(*actor, value);
                    else if (fieldIndex == UNIT_FIELD_FLAGS)
                        value = ReplayGreenNameUnitFlagsForActor(*actor, value);
                    else if (fieldIndex == PLAYER_FLAGS)
                        value = ReplayGreenNamePlayerFlagsForActor(*actor, value);
                }

                keptValues.push_back({ fieldIndex, value });
            }
        }

        RewriteTargetGuidInKeptValues(keptValues, match, blockGuid);

        std::vector<uint32> newMasks;
        if (!keptValues.empty())
        {
            uint32 highestField = keptValues.back().FieldIndex;
            newMasks.assign((highestField / 32) + 1, 0);

            for (ReplayUpdateFieldValue const& kept : keptValues)
                newMasks[kept.FieldIndex / 32] |= (uint32(1) << (kept.FieldIndex % 32));
        }

        if (newMasks.size() > 0xFF)
            return false;

        std::vector<uint8> rebuilt;
        rebuilt.reserve(1 + newMasks.size() * 4 + keptValues.size() * 4);
        rebuilt.push_back(uint8(newMasks.size()));

        for (uint32 mask : newMasks)
            AppendUInt32LE(rebuilt, mask);

        for (ReplayUpdateFieldValue const& kept : keptValues)
            AppendUInt32LE(rebuilt, kept.Value);

        size_t blockPayloadEnd = pos;
        payload.erase(payload.begin() + blockPayloadStart, payload.begin() + blockPayloadEnd);
        payload.insert(payload.begin() + blockPayloadStart, rebuilt.begin(), rebuilt.end());
        pos = blockPayloadStart + rebuilt.size();

        return true;
    }

    bool SkipMovementCreateData(std::vector<uint8>& payload, size_t& pos, MatchRecord const& match, ObjectGuid blockGuid, uint8 objectTypeId = 0xFF)
    {
        constexpr uint8 REPLAY_TYPEID_PLAYER = 4;
        constexpr uint16 REPLAY_UPDATEFLAG_SELF = 0x0001;
        constexpr uint16 REPLAY_UPDATEFLAG_TRANSPORT = 0x0002;
        constexpr uint16 REPLAY_UPDATEFLAG_HAS_TARGET = 0x0004;
        constexpr uint16 REPLAY_UPDATEFLAG_UNKNOWN = 0x0008;
        constexpr uint16 REPLAY_UPDATEFLAG_LOWGUID = 0x0010;
        constexpr uint16 REPLAY_UPDATEFLAG_LIVING = 0x0020;
        constexpr uint16 REPLAY_UPDATEFLAG_STATIONARY_POSITION = 0x0040;
        constexpr uint16 REPLAY_UPDATEFLAG_VEHICLE = 0x0080;
        constexpr uint16 REPLAY_UPDATEFLAG_POSITION = 0x0100;
        constexpr uint16 REPLAY_UPDATEFLAG_ROTATION = 0x0200;

        constexpr uint32 REPLAY_MOVEMENTFLAG_ONTRANSPORT = 0x00000200;
        constexpr uint32 REPLAY_MOVEMENTFLAG_FALLING = 0x00001000;
        constexpr uint32 REPLAY_MOVEMENTFLAG_SWIMMING = 0x00200000;
        constexpr uint32 REPLAY_MOVEMENTFLAG_FLYING = 0x02000000;
        constexpr uint32 REPLAY_MOVEMENTFLAG_SPLINE_ELEVATION = 0x04000000;
        constexpr uint32 REPLAY_MOVEMENTFLAG_SPLINE_ENABLED = 0x08000000;
        constexpr uint16 REPLAY_MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING = 0x0020;
        constexpr uint16 REPLAY_MOVEMENTFLAG2_INTERPOLATED_MOVEMENT = 0x0400;

        size_t flagsPos = pos;
        uint16 flags = 0;
        if (!ReadUInt16(payload, pos, flags))
            return false;

        bool clearedSelfFromPlayerCreate = false;

        if ((flags & REPLAY_UPDATEFLAG_SELF) && objectTypeId == REPLAY_TYPEID_PLAYER && IsReplayActorGuid(match, blockGuid))
        {
            flags &= ~REPLAY_UPDATEFLAG_SELF;
            WriteUInt16(payload, flagsPos, flags);
            clearedSelfFromPlayerCreate = true;
        }

        if (flags & REPLAY_UPDATEFLAG_LIVING)
        {
            size_t movementStart = pos;
            uint32 movementFlags = 0;
            uint16 extraMovementFlags = 0;

            if (!ReadUInt32(payload, pos, movementFlags) || !ReadUInt16(payload, pos, extraMovementFlags))
                return false;

            if (!HasRemaining(payload, pos, 20))
                return false;
            pos += 20;

            if (movementFlags & REPLAY_MOVEMENTFLAG_ONTRANSPORT)
            {
                if (!RewritePackedGuidAtResize(payload, pos, match))
                    return false;

                if (!HasRemaining(payload, pos, 21))
                    return false;
                pos += 21;

                if (extraMovementFlags & REPLAY_MOVEMENTFLAG2_INTERPOLATED_MOVEMENT)
                {
                    if (!HasRemaining(payload, pos, 4))
                        return false;
                    pos += 4;
                }
            }

            if ((movementFlags & (REPLAY_MOVEMENTFLAG_SWIMMING | REPLAY_MOVEMENTFLAG_FLYING)) ||
                (extraMovementFlags & REPLAY_MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING))
            {
                if (!HasRemaining(payload, pos, 4))
                    return false;
                pos += 4;
            }

            if (!HasRemaining(payload, pos, 4))
                return false;
            pos += 4;

            if (movementFlags & REPLAY_MOVEMENTFLAG_FALLING)
            {
                if (!HasRemaining(payload, pos, 16))
                    return false;
                pos += 16;
            }

            if (movementFlags & REPLAY_MOVEMENTFLAG_SPLINE_ELEVATION)
            {
                if (!HasRemaining(payload, pos, 4))
                    return false;
                pos += 4;
            }

            if (!HasRemaining(payload, pos, 36))
                return false;
            pos += 36;

            if (movementFlags & REPLAY_MOVEMENTFLAG_SPLINE_ENABLED)
            {
                TC_LOG_ERROR("arena.replay", "Replay update-object parser refused spline-enabled movement block guid={} movementStart={}",
                    blockGuid.GetRawValue(), uint32(movementStart));
                return false;
            }
        }
        else
        {
            if (flags & REPLAY_UPDATEFLAG_POSITION)
            {
                if (!RewritePackedGuidAtResize(payload, pos, match))
                    return false;

                if (!HasRemaining(payload, pos, 32))
                    return false;
                pos += 32;
            }
            else if (flags & REPLAY_UPDATEFLAG_STATIONARY_POSITION)
            {
                if (!HasRemaining(payload, pos, 16))
                    return false;
                pos += 16;
            }
        }

        if (flags & REPLAY_UPDATEFLAG_UNKNOWN)
        {
            if (!HasRemaining(payload, pos, 4))
                return false;
            pos += 4;
        }

        if (flags & REPLAY_UPDATEFLAG_LOWGUID)
        {
            if (!HasRemaining(payload, pos, 4))
                return false;

            if (clearedSelfFromPlayerCreate)
                WriteUInt32(payload, pos, 0x00000008);

            pos += 4;
        }

        if (flags & REPLAY_UPDATEFLAG_HAS_TARGET)
        {
            if (!RewritePackedGuidAtResize(payload, pos, match))
                return false;
        }

        if (flags & REPLAY_UPDATEFLAG_TRANSPORT)
        {
            if (!HasRemaining(payload, pos, 4))
                return false;
            pos += 4;
        }

        if (flags & REPLAY_UPDATEFLAG_VEHICLE)
        {
            if (!HasRemaining(payload, pos, 8))
                return false;
            pos += 8;
        }

        if (flags & REPLAY_UPDATEFLAG_ROTATION)
        {
            if (!HasRemaining(payload, pos, 8))
                return false;
            pos += 8;
        }
        return true;
    }

    bool RewriteUpdateObjectPayloadBlocks(std::vector<uint8>& payload, MatchRecord const& match, size_t pos, uint32 blockCount, char const* layoutName, size_t& endPos)
    {
        constexpr uint8 REPLAY_UPDATETYPE_VALUES = 0;
        constexpr uint8 REPLAY_UPDATETYPE_MOVEMENT = 1;
        constexpr uint8 REPLAY_UPDATETYPE_CREATE_OBJECT = 2;
        constexpr uint8 REPLAY_UPDATETYPE_CREATE_OBJECT2 = 3;
        constexpr uint8 REPLAY_UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4;
        constexpr uint8 REPLAY_UPDATETYPE_NEAR_OBJECTS = 5;

        for (uint32 block = 0; block < blockCount; ++block)
        {
            uint8 updateType = 0;
            if (!ReadUInt8(payload, pos, updateType))
                return false;

            if (updateType == REPLAY_UPDATETYPE_OUT_OF_RANGE_OBJECTS || updateType == REPLAY_UPDATETYPE_NEAR_OBJECTS)
            {
                uint32 guidCount = 0;
                if (!ReadUInt32(payload, pos, guidCount))
                    return false;

                for (uint32 i = 0; i < guidCount; ++i)
                {
                    if (!RewritePackedGuidAtResize(payload, pos, match))
                        return false;
                }
                continue;
            }

            if (updateType > REPLAY_UPDATETYPE_NEAR_OBJECTS)
                return false;

            ObjectGuid blockGuid;
            if (!RewritePackedGuidAtResize(payload, pos, match, &blockGuid))
                return false;

            switch (updateType)
            {
                case REPLAY_UPDATETYPE_VALUES:
                {
                    if (!PatchUpdateValuesBlock(payload, pos, match, blockGuid))
                        return false;
                    break;
                }
                case REPLAY_UPDATETYPE_MOVEMENT:
                {
                    if (!SkipMovementCreateData(payload, pos, match, blockGuid))
                        return false;
                    break;
                }
                case REPLAY_UPDATETYPE_CREATE_OBJECT:
                case REPLAY_UPDATETYPE_CREATE_OBJECT2:
                {
                    uint8 objectTypeId = 0;
                    if (!ReadUInt8(payload, pos, objectTypeId))
                        return false;

                    if (!SkipMovementCreateData(payload, pos, match, blockGuid, objectTypeId))
                        return false;

                    if (!PatchUpdateValuesBlock(payload, pos, match, blockGuid))
                        return false;
                    break;
                }
                default:
                    return false;
            }
        }

        endPos = pos;
        return true;
    }

    bool TryRewriteUpdateObjectPayloadLayout(std::vector<uint8> const& originalPayload, MatchRecord const& match, bool hasHeaderByte, std::vector<uint8>& rewritten, size_t& endPos)
    {
        rewritten = originalPayload;
        endPos = 0;

        size_t pos = 0;
        uint32 blockCount = 0;

        if (!ReadUInt32(rewritten, pos, blockCount))
            return false;

        if (hasHeaderByte)
        {
            uint8 transportOrMapHeader = 0;
            if (!ReadUInt8(rewritten, pos, transportOrMapHeader))
                return false;
            if (transportOrMapHeader > 1)
                return false;
        }

        char const* layoutName = hasHeaderByte ? "with-header-byte" : "no-header-byte";
        if (!RewriteUpdateObjectPayloadBlocks(rewritten, match, pos, blockCount, layoutName, endPos))
            return false;

        if (endPos != rewritten.size())
            return false;

        return true;
    }

    bool RewriteUpdateObjectPayload(std::vector<uint8>& payload, MatchRecord const& match)
    {
        std::vector<uint8> rewritten;
        size_t endPos = 0;

        if (TryRewriteUpdateObjectPayloadLayout(payload, match, false, rewritten, endPos))
        {
            payload.swap(rewritten);
            return true;
        }

        if (TryRewriteUpdateObjectPayloadLayout(payload, match, true, rewritten, endPos))
        {
            payload.swap(rewritten);
            return true;
        }

        TC_LOG_ERROR("arena.replay", "Replay update-object parser failed both layouts size={}", uint32(payload.size()));
        return false;
    }

    bool RewriteCompressedUpdateObjectPayloadToUncompressed(std::vector<uint8>& payload, MatchRecord const& match)
    {
        if (payload.size() < 4)
            return false;

        uint32 uncompressedSize = uint32(payload[0]) | (uint32(payload[1]) << 8) | (uint32(payload[2]) << 16) | (uint32(payload[3]) << 24);
        if (!uncompressedSize || uncompressedSize > 16 * 1024 * 1024)
            return false;

        std::vector<uint8> decompressed(uncompressedSize);
        uLongf actualSize = uncompressedSize;

        int zResult = uncompress(decompressed.data(), &actualSize, payload.data() + 4, uLong(payload.size() - 4));
        if (zResult != Z_OK || actualSize != uncompressedSize)
            return false;

        if (!RewriteUpdateObjectPayload(decompressed, match))
            return false;

        payload.swap(decompressed);
        return true;
    }

    size_t CountBytes(std::vector<uint8> const& payload, std::vector<uint8> const& needle)
    {
        if (needle.empty())
            return 0;

        size_t count = 0;
        auto it = payload.begin();
        while (it != payload.end())
        {
            it = std::search(it, payload.end(), needle.begin(), needle.end());
            if (it == payload.end())
                break;

            ++count;
            ++it;
        }
        return count;
    }

    size_t CountActorGuidHitsInPayload(std::vector<uint8> const& payload, MatchRecord const& match)
    {
        size_t hits = 0;
        for (ReplayActor const& actor : match.Actors)
        {
            uint64 original = actor.OriginalGuid.GetRawValue();
            if (!original)
                continue;

            hits += CountBytes(payload, ToRawGuidBytes(original));
            hits += CountBytes(payload, ToPackedGuidBytes(original));
        }
        return hits;
    }

    bool ExtractCompressedUpdatePayloadBytes(std::vector<uint8> const& payload, std::vector<uint8>& decompressed)
    {
        decompressed.clear();
        if (payload.size() < 4)
            return false;

        uint32 uncompressedSize = uint32(payload[0]) | (uint32(payload[1]) << 8) | (uint32(payload[2]) << 16) | (uint32(payload[3]) << 24);
        if (!uncompressedSize || uncompressedSize > 16 * 1024 * 1024)
            return false;

        decompressed.assign(uncompressedSize, 0);
        uLongf actualSize = uncompressedSize;
        int zResult = uncompress(decompressed.data(), &actualSize, payload.data() + 4, uLong(payload.size() - 4));
        if (zResult != Z_OK || actualSize != uncompressedSize)
        {
            decompressed.clear();
            return false;
        }
        return true;
    }

    bool PacketPayloadContainsOriginalActorGuid(uint16 opcode, std::vector<uint8> const& payload, MatchRecord const& match)
    {
        if (opcode == SMSG_COMPRESSED_UPDATE_OBJECT)
        {
            std::vector<uint8> decompressed;
            if (!ExtractCompressedUpdatePayloadBytes(payload, decompressed))
                return false;
            return CountActorGuidHitsInPayload(decompressed, match) > 0;
        }
        return CountActorGuidHitsInPayload(payload, match) > 0;
    }

    bool ExtractCompressedUpdatePayloadForAudit(WorldPacket const& packet, std::vector<uint8>& decompressed)
    {
        std::vector<uint8> payload(packet.size());
        if (!payload.empty())
            std::memcpy(payload.data(), packet.contents(), payload.size());
        return ExtractCompressedUpdatePayloadBytes(payload, decompressed);
    }

    struct ReplayAudit
    {
        uint32 UpdatePackets = 0;
        uint32 CompressedUpdatePackets = 0;
        uint32 AuraPackets = 0;
        uint32 AuraUpdatePackets = 0;
        uint32 AuraUpdateAllPackets = 0;
        uint32 ZeroTimeUpdatePackets = 0;
        uint32 ActorGuidHits = 0;
        uint32 ZeroTimeActorGuidHits = 0;
    };

    ReplayAudit BuildReplayAudit(MatchRecord const& match)
    {
        ReplayAudit audit;
        for (PacketRecord const& frame : match.Packets)
        {
            if (frame.Packet.GetOpcode() == SMSG_AURA_UPDATE)
            {
                ++audit.AuraPackets;
                ++audit.AuraUpdatePackets;
            }
            else if (frame.Packet.GetOpcode() == SMSG_AURA_UPDATE_ALL)
            {
                ++audit.AuraPackets;
                ++audit.AuraUpdateAllPackets;
            }

            if (frame.Packet.GetOpcode() == SMSG_UPDATE_OBJECT)
            {
                ++audit.UpdatePackets;
                if (frame.TimestampMs == 0)
                    ++audit.ZeroTimeUpdatePackets;

                std::vector<uint8> payload(frame.Packet.size());
                if (!payload.empty())
                    std::memcpy(payload.data(), frame.Packet.contents(), payload.size());

                size_t hits = CountActorGuidHitsInPayload(payload, match);
                audit.ActorGuidHits += uint32(hits);
                if (frame.TimestampMs == 0)
                    audit.ZeroTimeActorGuidHits += uint32(hits);
            }
            else if (frame.Packet.GetOpcode() == SMSG_COMPRESSED_UPDATE_OBJECT)
            {
                ++audit.CompressedUpdatePackets;
                if (frame.TimestampMs == 0)
                    ++audit.ZeroTimeUpdatePackets;

                std::vector<uint8> payload;
                if (ExtractCompressedUpdatePayloadForAudit(frame.Packet, payload))
                {
                    size_t hits = CountActorGuidHitsInPayload(payload, match);
                    audit.ActorGuidHits += uint32(hits);
                    if (frame.TimestampMs == 0)
                        audit.ZeroTimeActorGuidHits += uint32(hits);
                }
            }
        }
        return audit;
    }

    std::string ReplayPacketProbeGuids(WorldPacket const& packet, MatchRecord const& match)
    {
        std::ostringstream ss;
        std::vector<uint8> payload(packet.size());
        if (!payload.empty())
            std::memcpy(payload.data(), packet.contents(), payload.size());

        uint32 originalPackedHits = 0;
        uint32 originalRawHits = 0;
        uint32 fakePackedHits = 0;
        uint32 fakeRawHits = 0;

        for (ReplayActor const& actor : match.Actors)
        {
            originalPackedHits += CountBytes(payload, ToPackedGuidBytes(actor.OriginalGuid.GetRawValue()));
            fakePackedHits += CountBytes(payload, ToPackedGuidBytes(actor.FakeGuid.GetRawValue()));

            if (payload.size() >= 8)
            {
                uint64 const originalRaw = actor.OriginalGuid.GetRawValue();
                uint64 const fakeRaw = actor.FakeGuid.GetRawValue();

                for (size_t i = 0; i + 8 <= payload.size(); ++i)
                {
                    uint64 raw = uint64(payload[i]) | (uint64(payload[i + 1]) << 8) | (uint64(payload[i + 2]) << 16) | (uint64(payload[i + 3]) << 24) |
                                 (uint64(payload[i + 4]) << 32) | (uint64(payload[i + 5]) << 40) | (uint64(payload[i + 6]) << 48) | (uint64(payload[i + 7]) << 56);

                    if (raw == originalRaw)
                        ++originalRawHits;
                    else if (raw == fakeRaw)
                        ++fakeRawHits;
                }
            }
        }

        ss << "origPacked=" << originalPackedHits << " origRaw=" << originalRawHits << " fakePacked=" << fakePackedHits << " fakeRaw=" << fakeRawHits;
        return ss.str();
    }

    void LogReplayPacketProbe(Player* viewer, PlaybackState const& state, PacketRecord const& frame, WorldPacket const& packet, size_t cursorBeforeSend, uint32 elapsedMs)
    {
        if (!viewer)
            return;
        TC_LOG_ERROR("arena.replay",
            "REPLAY_PROBE_V103 wall={} unixMs={} viewer={} bg={} cursor={} total={} elapsed={} frameTs={} opcode={} size={} selectedRaw={} selectedLow={} paused={} stepBudget={} {}",
            ReplayWallClockString(), ReplayUnixMilliseconds(), viewer->GetGUID().GetCounter(), state.BgInstanceId,
            uint32(cursorBeforeSend), uint32(state.Match.Packets.size()), elapsedMs, frame.TimestampMs, packet.GetOpcode(), uint32(packet.size()),
            viewer->GetTarget().GetRawValue(), viewer->GetTarget().GetCounter(), state.ProbePaused ? 1u : 0u, state.ProbeStepBudget, ReplayPacketProbeGuids(packet, state.Match));
    }

    bool BuildPlaybackPacket(PacketRecord const& frame, MatchRecord const& match, WorldPacket& out)
    {
        uint16 outOpcode = frame.Packet.GetOpcode();
        if (outOpcode == SMSG_BATTLEGROUND_PLAYER_JOINED || outOpcode == SMSG_BATTLEGROUND_PLAYER_LEFT)
            return false;

        std::vector<uint8> payload(frame.Packet.size());
        if (!payload.empty())
            std::memcpy(payload.data(), frame.Packet.contents(), payload.size());

        bool rewriteOk = true;

        if (frame.Packet.GetOpcode() == SMSG_COMPRESSED_UPDATE_OBJECT)
        {
            rewriteOk = RewriteCompressedUpdateObjectPayloadToUncompressed(payload, match);
            if (!rewriteOk)
                return false;
            outOpcode = SMSG_UPDATE_OBJECT;
        }
        else if (frame.Packet.GetOpcode() == SMSG_UPDATE_OBJECT)
        {
            rewriteOk = RewriteUpdateObjectPayload(payload, match);
        }
        else
        {
            RewriteNonUpdatePacketGuids(frame.Packet.GetOpcode(), payload, match);
        }

        if (outOpcode == SMSG_UPDATE_OBJECT || outOpcode == SMSG_COMPRESSED_UPDATE_OBJECT)
        {
            if (PacketPayloadContainsOriginalActorGuid(outOpcode, payload, match))
                return false;
        }

        if (payload.empty() && (outOpcode == SMSG_DESTROY_OBJECT || outOpcode == SMSG_ARENA_UNIT_DESTROYED))
            return false;

        out = WorldPacket(outOpcode, payload.size());
        if (!payload.empty())
            out.append(payload.data(), payload.size());
        return true;
    }

    void SendReplayNameResponse(WorldSession* session, ReplayActor const& actor)
    {
        if (!session)
            return;

        WorldPackets::Query::QueryPlayerNameResponse response;
        response.Player = actor.FakeGuid;
        response.Result = RESPONSE_SUCCESS;

        WorldPackets::Query::PlayerGuidLookupData& data = response.Data.emplace();
        data.Name = actor.Name;
        data.Race = actor.Race;
        data.Sex = Gender(actor.Gender);
        data.ClassID = actor.Class;

        session->SendPacket(response.Write());
    }

    void SendDestroyObjectToReplayViewer(Player* viewer, ObjectGuid guid, char const* reason)
    {
        if (!viewer || !viewer->GetSession() || guid.IsEmpty())
            return;
        WorldPacket data(SMSG_DESTROY_OBJECT, 8 + 1);
        data << uint64(guid.GetRawValue());
        data << uint8(0);
        viewer->GetSession()->SendPacket(&data);
    }

    bool IsViewerReadyForReplay(Player const* viewer, PlaybackState const& state)
    {
        if (!viewer)
            return false;
        Battleground const* bg = viewer->GetBattleground();
        if (!bg)
            return false;
        if (bg->GetInstanceID() != state.BgInstanceId)
            return false;
        if (!bg->IsReplay())
            return false;
        if (!viewer->IsInWorld())
            return false;
        if (!viewer->GetMap())
            return false;
        if (viewer->GetMapId() != state.Match.MapId)
            return false;
        return true;
    }

    bool TrySkipReplayCountdown(Player* viewer, PlaybackState const& state)
    {
        if (!viewer || !viewer->GetSession())
            return false;

        Battleground* bg = viewer->GetBattleground();
        if (!bg || !bg->IsReplay() || bg->GetInstanceID() != state.BgInstanceId)
            return false;

        if (!bg->FindBgMap())
            return false;

        if (!bg->GetPlayersSize())
            return false;

        if (bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            bg->StartingEventOpenDoors();
            return true;
        }

        bg->StartingEventOpenDoors();
        bg->SetStartDelayTime(0);
        bg->SetStatus(STATUS_IN_PROGRESS);

        WorldPacket status;
        uint32 queueSlot = 0;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&status, bg, queueSlot, STATUS_IN_PROGRESS, 0, 0, bg->GetArenaType(), viewer->GetBGTeam());
        viewer->SendDirectMessage(&status);
        return true;
    }

    void SendInitialReplayNameResponses(Player* viewer, PlaybackState& state)
    {
        if (!viewer || !viewer->GetSession())
            return;
        for (ReplayActor const& actor : state.Match.Actors)
            SendReplayNameResponse(viewer->GetSession(), actor);
        state.SentInitialNameResponses = true;
    }

    std::string ReplayASGuidString(ObjectGuid guid)
    {
        std::ostringstream ss;
        ss << "0x" << std::uppercase << std::hex << std::setw(16) << std::setfill('0') << guid.GetRawValue();
        return ss.str();
    }

    void SendReplayASRaw(Player* viewer, std::string const& payload)
    {
        if (!viewer || !viewer->GetSession())
            return;

        WorldPackets::Chat::Chat packet;
        packet.Initialize(CHAT_MSG_WHISPER, LANG_ADDON, viewer, viewer, std::string("ASSUN\t") + payload);
        viewer->SendDirectMessage(packet.Write());
    }

    void SendReplayASCommand(Player* viewer, ObjectGuid targetGuid, char const* prefix, std::string const& value)
    {
        if (!targetGuid.IsPlayer())
            return;
        SendReplayASRaw(viewer, ReplayASGuidString(targetGuid) + ";" + prefix + "=" + value + ";");
    }

    void SendReplayGreenNameValueUpdate(Player* viewer, ReplayActor const& actor)
    {
        if (!viewer || !viewer->GetSession())
            return;

        constexpr uint8 REPLAY_UPDATETYPE_VALUES = 0;
        uint32 const factionField = UNIT_FIELD_FACTIONTEMPLATE;

        if (!ReplayActorShouldUseFriendlyGreenName(actor))
        {
            WorldPacket data(SMSG_UPDATE_OBJECT, 32);
            data << uint32(1);
            data << uint8(REPLAY_UPDATETYPE_VALUES);
            data << actor.FakeGuid.WriteAsPacked();

            uint8 const blockCount = uint8(factionField / 32 + 1);
            data << uint8(blockCount);
            for (uint8 block = 0; block < blockCount; ++block)
            {
                uint32 mask = 0;
                if (factionField / 32 == block)
                    mask |= uint32(1) << (factionField % 32);
                data << uint32(mask);
            }
            data << uint32(ReplayYellowFactionTemplateForActor(actor));
            viewer->GetSession()->SendPacket(&data);
            return;
        }

        uint32 const field0 = UNIT_FIELD_FACTIONTEMPLATE;
        uint32 const field1 = UNIT_FIELD_FLAGS;
        uint32 const field2 = UNIT_FIELD_BYTES_2;
        uint32 const field3 = PLAYER_FLAGS;
        uint32 const maxField = PLAYER_FLAGS;
        uint8 const blockCount = uint8(maxField / 32 + 1);

        WorldPacket data(SMSG_UPDATE_OBJECT, 128);
        data << uint32(1);
        data << uint8(REPLAY_UPDATETYPE_VALUES);
        data << actor.FakeGuid.WriteAsPacked();
        data << uint8(blockCount);

        for (uint8 block = 0; block < blockCount; ++block)
        {
            uint32 mask = 0;
            if (field0 / 32 == block)
                mask |= uint32(1) << (field0 % 32);
            if (field1 / 32 == block)
                mask |= uint32(1) << (field1 % 32);
            if (field2 / 32 == block)
                mask |= uint32(1) << (field2 % 32);
            if (field3 / 32 == block)
                mask |= uint32(1) << (field3 % 32);
            data << uint32(mask);
        }

        data << uint32(ReplayFriendlyFactionTemplateForViewer(viewer));
        data << uint32(ReplayGreenNameUnitFlagsForActor(actor, 0));
        data << uint32(ReplayGreenNameUnitBytes2ForActor(actor, 0));
        data << uint32(ReplayGreenNamePlayerFlagsForActor(actor, 0));
        viewer->GetSession()->SendPacket(&data);
    }

    void SendReplayGreenNameUpdates(Player* viewer, PlaybackState& state, char const* reason)
    {
        if (!viewer || !viewer->GetSession())
            return;
        uint32 sent = 0;
        for (ReplayActor const& actor : state.Match.Actors)
        {
            SendReplayGreenNameValueUpdate(viewer, actor);
            ++sent;
        }
    }

    void MaybeSendReplayGreenNameUpdates(Player* viewer, PlaybackState& state, uint32 nowMs)
    {
        constexpr uint32 NAME_COLOR_UPDATE_INTERVAL_MS = 250;
        if (state.LastNameColorUpdateMs && nowMs - state.LastNameColorUpdateMs < NAME_COLOR_UPDATE_INTERVAL_MS)
            return;

        state.LastNameColorUpdateMs = nowMs;
        ++state.NameColorUpdateBursts;
        SendReplayGreenNameUpdates(viewer, state, "persistent actor friendly/green update");
    }

    void SendReplayActorChannelClearValueUpdate(Player* viewer, ReplayActor const& actor)
    {
        if (!viewer || !viewer->GetSession())
            return;

        uint32 const field0 = UNIT_FIELD_CHANNEL_OBJECT;
        uint32 const field1 = UNIT_FIELD_CHANNEL_OBJECT + 1;
        uint32 const field2 = UNIT_CHANNEL_SPELL;
        uint8 const blockCount = uint8((field2 / 32) + 1);

        WorldPacket data(SMSG_UPDATE_OBJECT, 64);
        data << uint32(1);
        data << uint8(0); // UPDATETYPE_VALUES
        data << actor.FakeGuid.WriteAsPacked();
        data << uint8(blockCount);

        for (uint8 block = 0; block < blockCount; ++block)
        {
            uint32 mask = 0;
            if (field0 / 32 == block)
                mask |= uint32(1) << (field0 % 32);
            if (field1 / 32 == block)
                mask |= uint32(1) << (field1 % 32);
            if (field2 / 32 == block)
                mask |= uint32(1) << (field2 % 32);
            data << uint32(mask);
        }
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        viewer->GetSession()->SendPacket(&data);
    }

    void SendReplayActorChannelClearUpdates(Player* viewer, PlaybackState& state, char const* reason)
    {
        if (!viewer || !viewer->GetSession())
            return;
        for (ReplayActor const& actor : state.Match.Actors)
            SendReplayActorChannelClearValueUpdate(viewer, actor);
    }

    void MaybeSendReplayActorChannelClearUpdates(Player* viewer, PlaybackState& state, uint32 nowMs)
    {
        constexpr uint32 CHANNEL_CLEAR_INTERVAL_MS = 250;
        if (state.LastChannelClearMs && nowMs - state.LastChannelClearMs < CHANNEL_CLEAR_INTERVAL_MS)
            return;

        state.LastChannelClearMs = nowMs;
        ++state.ChannelClearBursts;
        SendReplayActorChannelClearUpdates(viewer, state, "periodic stale channel visual cleanup");
    }

    void SendReplayASCommand(Player* viewer, ObjectGuid targetGuid, char const* prefix, uint32 value)
    {
        SendReplayASCommand(viewer, targetGuid, prefix, std::to_string(value));
    }

    uint32 ReplayASPowerTypeForClass(uint8 playerClass)
    {
        switch (playerClass)
        {
            case CLASS_WARRIOR:
                return POWER_RAGE;
            case CLASS_ROGUE:
                return POWER_ENERGY;
            case CLASS_DEATH_KNIGHT:
                return POWER_RUNIC_POWER;
            default:
                return POWER_MANA;
        }
    }

    void SendReplayASInitial(Player* viewer, PlaybackState& state)
    {
        if (!viewer || !viewer->GetSession())
            return;

        SendReplayASRaw(viewer, "ENABLE");
        SendReplayASRaw(viewer, "REQUESTRESET");

        uint32 durationSeconds = state.Match.Packets.empty() ? 0 : state.Match.Packets.back().TimestampMs / IN_MILLISECONDS;

        for (ReplayActor const& actor : state.Match.Actors)
        {
            ObjectGuid guid = actor.FakeGuid;
            uint32 powerType = ReplayASPowerTypeForClass(actor.Class);
            uint32 maxPower = powerType == POWER_RAGE || powerType == POWER_RUNIC_POWER ? 100u : (powerType == POWER_ENERGY ? 100u : 1u);
            uint32 currentPower = powerType == POWER_RAGE || powerType == POWER_RUNIC_POWER ? 0u : maxPower;
            uint32 team = actor.Team ? actor.Team : ALLIANCE;

            SendReplayASCommand(viewer, guid, "NME", actor.Name.empty() ? "Replay" : actor.Name);
            SendReplayASCommand(viewer, guid, "TEM", team);
            SendReplayASCommand(viewer, guid, "CLA", uint32(actor.Class));
            SendReplayASCommand(viewer, guid, "MHP", 100u);
            SendReplayASCommand(viewer, guid, "CHP", 100u);
            SendReplayASCommand(viewer, guid, "STA", 1u);
            SendReplayASCommand(viewer, guid, "PWT", powerType);
            SendReplayASCommand(viewer, guid, "MPW", maxPower);
            SendReplayASCommand(viewer, guid, "CPW", currentPower);
            SendReplayASCommand(viewer, guid, "PHP", 0u);
            SendReplayASCommand(viewer, guid, "PET", 0u);
            SendReplayASCommand(viewer, guid, "RES", 1u);
            SendReplayASCommand(viewer, guid, "CDC", 1u);
            SendReplayASCommand(viewer, guid, "TIM", durationSeconds);
        }

        state.SentReplayASInitial = true;
        SendReplayGreenNameUpdates(viewer, state, "initial replay addon setup");
    }

    constexpr uint32 REPLAY_OBJECT_FIELD_GUID_LOW       = 0x0000;
    constexpr uint32 REPLAY_OBJECT_FIELD_GUID_HIGH      = 0x0001;
    constexpr uint32 REPLAY_UNIT_FIELD_CHARMEDBY_LOW    = 0x000C;
    constexpr uint32 REPLAY_UNIT_FIELD_CHARMEDBY_HIGH   = 0x000D;
    constexpr uint32 REPLAY_UNIT_FIELD_SUMMONEDBY_LOW   = 0x000E;
    constexpr uint32 REPLAY_UNIT_FIELD_SUMMONEDBY_HIGH  = 0x000F;
    constexpr uint32 REPLAY_UNIT_FIELD_CREATEDBY_LOW    = 0x0010;
    constexpr uint32 REPLAY_UNIT_FIELD_CREATEDBY_HIGH   = 0x0011;
    constexpr uint32 REPLAY_UNIT_FIELD_HEALTH           = 0x0018;
    constexpr uint32 REPLAY_UNIT_FIELD_POWER1           = 0x0019;
    constexpr uint32 REPLAY_UNIT_FIELD_MAXHEALTH        = 0x0020;
    constexpr uint32 REPLAY_UNIT_FIELD_MAXPOWER1        = 0x0021;

    uint32 ReplayASPowerFieldForType(uint32 powerType)
    {
        return REPLAY_UNIT_FIELD_POWER1 + powerType;
    }

    uint32 ReplayASMaxPowerFieldForType(uint32 powerType)
    {
        return REPLAY_UNIT_FIELD_MAXPOWER1 + powerType;
    }

    bool ReplayASReadFieldMap(std::vector<uint8> const& payload, size_t& pos, std::unordered_map<uint32, uint32>& values)
    {
        uint8 maskBlockCount = 0;
        if (!ReadUInt8(payload, pos, maskBlockCount))
            return false;

        std::vector<uint32> masks;
        masks.reserve(maskBlockCount);

        for (uint8 i = 0; i < maskBlockCount; ++i)
        {
            uint32 mask = 0;
            if (!ReadUInt32(payload, pos, mask))
                return false;
            masks.push_back(mask);
        }

        for (uint32 block = 0; block < masks.size(); ++block)
        {
            uint32 mask = masks[block];
            for (uint8 bit = 0; bit < 32; ++bit)
            {
                if (!(mask & (uint32(1) << bit)))
                    continue;

                uint32 value = 0;
                if (!ReadUInt32(payload, pos, value))
                    return false;
                values[block * 32 + bit] = value;
            }
        }
        return true;
    }

    bool ReplayASGetField(std::unordered_map<uint32, uint32> const& values, uint32 field, uint32& value)
    {
        auto itr = values.find(field);
        if (itr == values.end())
            return false;
        value = itr->second;
        return true;
    }

    ObjectGuid ReplayASGuidFromFields(std::unordered_map<uint32, uint32> const& values, uint32 lowField, uint32 highField)
    {
        uint32 low = 0;
        uint32 high = 0;
        if (!ReplayASGetField(values, lowField, low) || !ReplayASGetField(values, highField, high))
            return ObjectGuid::Empty;
        ObjectGuid guid;
        guid.SetRawValue(uint64(low) | (uint64(high) << 32));
        return guid;
    }

    ReplayActor const* FindReplayASPetCapableFallbackOwner(MatchRecord const& match)
    {
        ReplayActor const* fallback = nullptr;
        uint32 count = 0;
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.Class != CLASS_HUNTER && actor.Class != CLASS_WARLOCK && actor.Class != CLASS_DEATH_KNIGHT && actor.Class != CLASS_MAGE)
                continue;
            fallback = &actor;
            ++count;
        }
        return count == 1 ? fallback : nullptr;
    }

    ReplayActor const* FindReplayASOwnerFromAnyGuidPair(MatchRecord const& match, std::unordered_map<uint32, uint32> const& values)
    {
        ObjectGuid ownerGuid = ReplayASGuidFromFields(values, REPLAY_UNIT_FIELD_CHARMEDBY_LOW, REPLAY_UNIT_FIELD_CHARMEDBY_HIGH);
        if (ReplayActor const* owner = FindReplayActorByGuid(match, ownerGuid))
            return owner;

        ownerGuid = ReplayASGuidFromFields(values, REPLAY_UNIT_FIELD_SUMMONEDBY_LOW, REPLAY_UNIT_FIELD_SUMMONEDBY_HIGH);
        if (ReplayActor const* owner = FindReplayActorByGuid(match, ownerGuid))
            return owner;

        ownerGuid = ReplayASGuidFromFields(values, REPLAY_UNIT_FIELD_CREATEDBY_LOW, REPLAY_UNIT_FIELD_CREATEDBY_HIGH);
        if (ReplayActor const* owner = FindReplayActorByGuid(match, ownerGuid))
            return owner;

        for (auto const& lowItr : values)
        {
            auto highItr = values.find(lowItr.first + 1);
            if (highItr == values.end())
                continue;

            ObjectGuid candidate;
            candidate.SetRawValue(uint64(lowItr.second) | (uint64(highItr->second) << 32));
            if (ReplayActor const* owner = FindReplayActorByGuid(match, candidate))
                return owner;
        }
        return FindReplayASPetCapableFallbackOwner(match);
    }

    void SendReplayASPlayerStatusFromValues(Player* viewer, MatchRecord const& match, ObjectGuid objectGuid, std::unordered_map<uint32, uint32> const& values)
    {
        ReplayActor const* actor = FindReplayActorByGuid(match, objectGuid);
        if (!actor)
            return;

        ObjectGuid uiGuid = actor->FakeGuid;
        uint32 health = 0;
        uint32 maxHealth = 0;

        if (ReplayASGetField(values, REPLAY_UNIT_FIELD_MAXHEALTH, maxHealth) && maxHealth > 0)
            SendReplayASCommand(viewer, uiGuid, "MHP", maxHealth);

        if (ReplayASGetField(values, REPLAY_UNIT_FIELD_HEALTH, health))
        {
            SendReplayASCommand(viewer, uiGuid, "CHP", health);
            SendReplayASCommand(viewer, uiGuid, "STA", health > 0 ? 1u : 0u);
        }

        uint32 powerType = ReplayASPowerTypeForClass(actor->Class);
        SendReplayASCommand(viewer, uiGuid, "PWT", powerType);

        uint32 maxPower = 0;
        if (ReplayASGetField(values, ReplayASMaxPowerFieldForType(powerType), maxPower) && maxPower > 0)
            SendReplayASCommand(viewer, uiGuid, "MPW", maxPower);

        uint32 currentPower = 0;
        if (ReplayASGetField(values, ReplayASPowerFieldForType(powerType), currentPower))
            SendReplayASCommand(viewer, uiGuid, "CPW", currentPower);

        ObjectGuid targetGuid = ReplayASGuidFromFields(values, UNIT_FIELD_TARGET, UNIT_FIELD_TARGET + 1);
        if (!targetGuid.IsEmpty())
        {
            if (ReplayActor const* targetActor = FindReplayActorByGuid(match, targetGuid))
                SendReplayASCommand(viewer, uiGuid, "TRG", ReplayASGuidString(targetActor->FakeGuid));
            else
                SendReplayASCommand(viewer, uiGuid, "TRG", 0u);
        }
    }

    void SendReplayASPetStatusFromValues(Player* viewer, MatchRecord const& match, std::unordered_map<uint32, uint32> const& values)
    {
        ReplayActor const* owner = FindReplayASOwnerFromAnyGuidPair(match, values);
        if (!owner)
            return;

        uint32 health = 0;
        uint32 maxHealth = 0;

        if (!ReplayASGetField(values, REPLAY_UNIT_FIELD_HEALTH, health))
            return;

        uint32 pct = 0;
        if (ReplayASGetField(values, REPLAY_UNIT_FIELD_MAXHEALTH, maxHealth) && maxHealth > 0)
            pct = std::min<uint32>(100u, uint32(std::ceil(double(health) * 100.0 / double(maxHealth))));
        else
            pct = health > 0 ? 100u : 0u;

        SendReplayASCommand(viewer, owner->FakeGuid, "PET", 0u);
        SendReplayASCommand(viewer, owner->FakeGuid, "PHP", pct);
    }

    bool SendReplayASStatusFromUpdatePayload(Player* viewer, MatchRecord const& match, std::vector<uint8>& payload)
    {
        constexpr uint8 REPLAY_UPDATETYPE_VALUES = 0;
        constexpr uint8 REPLAY_UPDATETYPE_MOVEMENT = 1;
        constexpr uint8 REPLAY_UPDATETYPE_CREATE_OBJECT = 2;
        constexpr uint8 REPLAY_UPDATETYPE_CREATE_OBJECT2 = 3;
        constexpr uint8 REPLAY_UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4;
        constexpr uint8 REPLAY_UPDATETYPE_NEAR_OBJECTS = 5;

        size_t pos = 0;
        uint32 blockCount = 0;

        if (!ReadUInt32(payload, pos, blockCount))
            return false;

        for (uint32 block = 0; block < blockCount; ++block)
        {
            uint8 updateType = 0;
            if (!ReadUInt8(payload, pos, updateType))
                return false;

            if (updateType == REPLAY_UPDATETYPE_OUT_OF_RANGE_OBJECTS || updateType == REPLAY_UPDATETYPE_NEAR_OBJECTS)
            {
                uint32 guidCount = 0;
                if (!ReadUInt32(payload, pos, guidCount))
                    return false;
                for (uint32 i = 0; i < guidCount; ++i)
                {
                    if (!SkipPackedGuid(payload, pos))
                        return false;
                }
                continue;
            }

            ObjectGuid blockGuid;
            if (!ReadPackedGuid(payload, pos, blockGuid))
                return false;

            switch (updateType)
            {
                case REPLAY_UPDATETYPE_VALUES:
                {
                    std::unordered_map<uint32, uint32> values;
                    if (!ReplayASReadFieldMap(payload, pos, values))
                        return false;

                    if (FindReplayActorByGuid(match, blockGuid))
                        SendReplayASPlayerStatusFromValues(viewer, match, blockGuid, values);
                    else
                        SendReplayASPetStatusFromValues(viewer, match, values);
                    break;
                }
                case REPLAY_UPDATETYPE_MOVEMENT:
                {
                    if (!SkipMovementCreateData(payload, pos, match, blockGuid))
                        return false;
                    break;
                }
                case REPLAY_UPDATETYPE_CREATE_OBJECT:
                case REPLAY_UPDATETYPE_CREATE_OBJECT2:
                {
                    uint8 objectTypeId = 0;
                    if (!ReadUInt8(payload, pos, objectTypeId))
                        return false;

                    if (!SkipMovementCreateData(payload, pos, match, blockGuid, objectTypeId))
                        return false;

                    std::unordered_map<uint32, uint32> values;
                    if (!ReplayASReadFieldMap(payload, pos, values))
                        return false;

                    if (FindReplayActorByGuid(match, blockGuid))
                        SendReplayASPlayerStatusFromValues(viewer, match, blockGuid, values);
                    else
                        SendReplayASPetStatusFromValues(viewer, match, values);
                    break;
                }
                default:
                    return false;
            }
        }
        return true;
    }

    bool SendReplayASStatusFromPlaybackPacket(Player* viewer, WorldPacket const& packet, MatchRecord const& match)
    {
        if (packet.GetOpcode() == SMSG_UPDATE_OBJECT)
        {
            std::vector<uint8> payload(packet.size());
            if (!payload.empty())
                std::memcpy(payload.data(), packet.contents(), payload.size());
            return SendReplayASStatusFromUpdatePayload(viewer, match, payload);
        }

        if (packet.GetOpcode() == SMSG_COMPRESSED_UPDATE_OBJECT)
        {
            if (packet.size() < 4)
                return false;

            std::vector<uint8> payload(packet.size());
            std::memcpy(payload.data(), packet.contents(), payload.size());

            uint32 uncompressedSize = uint32(payload[0]) | (uint32(payload[1]) << 8) | (uint32(payload[2]) << 16) | (uint32(payload[3]) << 24);
            if (!uncompressedSize || uncompressedSize > 16 * 1024 * 1024)
                return false;

            std::vector<uint8> decompressed(uncompressedSize);
            uLongf actualSize = uncompressedSize;

            int zResult = uncompress(decompressed.data(), &actualSize, payload.data() + 4, uLong(payload.size() - 4));
            if (zResult != Z_OK || actualSize != uncompressedSize)
                return false;

            return SendReplayASStatusFromUpdatePayload(viewer, match, decompressed);
        }
        return false;
    }

    bool ExtractReplayASSpell(WorldPacket const& packet, ObjectGuid& caster, uint32& spellId, int32& castTime)
    {
        if (packet.GetOpcode() != SMSG_SPELL_START)
            return false;

        std::vector<uint8> payload(packet.size());
        if (!payload.empty())
            std::memcpy(payload.data(), packet.contents(), payload.size());

        size_t pos = 0;
        ObjectGuid casterGuid;
        ObjectGuid casterUnit;
        if (!ReadPackedGuid(payload, pos, casterGuid) || !ReadPackedGuid(payload, pos, casterUnit))
            return false;

        uint8 castId = 0;
        uint32 castFlags = 0;
        uint32 castTimeRaw = 0;
        if (!ReadUInt8(payload, pos, castId) || !ReadUInt32(payload, pos, spellId) || !ReadUInt32(payload, pos, castFlags) || !ReadUInt32(payload, pos, castTimeRaw))
            return false;

        caster = casterUnit.IsPlayer() ? casterUnit : casterGuid;
        castTime = int32(castTimeRaw);
        return caster.IsPlayer() && spellId != 0;
    }

    bool ExtractReplayASAttackStart(WorldPacket const& packet, ObjectGuid& attacker, ObjectGuid& victim)
    {
        if (packet.GetOpcode() != SMSG_ATTACK_START || packet.size() < 16)
            return false;

        std::vector<uint8> payload(packet.size());
        std::memcpy(payload.data(), packet.contents(), payload.size());

        uint64 a = 0;
        uint64 v = 0;
        if (!ReadUInt64At(payload, 0, a) || !ReadUInt64At(payload, 8, v))
            return false;

        attacker.SetRawValue(a);
        victim.SetRawValue(v);
        return attacker.IsPlayer();
    }

    struct ReplayASAuraSlotState
    {
        uint32 SpellId = 0;
        uint8 IsDebuff = 0;
        uint8 Stack = 0;
        std::string Caster = "0";
    };

    std::unordered_map<uint64, ReplayASAuraSlotState> ReplayASAuraSlotCache;

    uint64 ReplayASAuraCacheKey(Player* viewer, ObjectGuid targetGuid, uint8 slot)
    {
        uint64 viewerPart = viewer ? uint64(viewer->GetGUID().GetCounter()) : 0;
        return (viewerPart << 40) ^ (targetGuid.GetRawValue() << 8) ^ uint64(slot);
    }

    bool ReplayASShouldShowAuraInDefaultFrame(uint32 spellId, uint8 flags, uint32 maxDurationMs, uint32 remainingMs)
    {
        constexpr uint32 REPLAY_SPELL_GHOST = 8326;
        constexpr uint8 REPLAY_AFLAG_EFFECT1  = 0x01;
        constexpr uint8 REPLAY_AFLAG_EFFECT2  = 0x02;
        constexpr uint8 REPLAY_AFLAG_EFFECT3  = 0x04;
        constexpr uint8 REPLAY_AFLAG_POSITIVE = 0x10;
        constexpr uint8 REPLAY_AFLAG_NEGATIVE = 0x80;

        if (spellId == REPLAY_SPELL_GHOST)
            return false;

        bool hasEffect = (flags & (REPLAY_AFLAG_EFFECT1 | REPLAY_AFLAG_EFFECT2 | REPLAY_AFLAG_EFFECT3)) != 0;
        bool hasDefaultFramePolarity = (flags & (REPLAY_AFLAG_POSITIVE | REPLAY_AFLAG_NEGATIVE)) != 0;

        if (!hasEffect || !hasDefaultFramePolarity)
            return false;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
            return false;

        constexpr uint32 REPLAY_SPELL_ATTR0_PASSIVE = 0x00000040;
        constexpr uint32 REPLAY_SPELL_ATTR0_HIDDEN_CLIENTSIDE = 0x00000080;

        if (spellInfo->Attributes & (REPLAY_SPELL_ATTR0_PASSIVE | REPLAY_SPELL_ATTR0_HIDDEN_CLIENTSIDE))
            return false;

        return true;
    }

    void SendReplayASAuraCommand(Player* viewer, ObjectGuid targetGuid, uint8 remove, uint8 stack, uint32 remainingMs,
        uint32 maxDurationMs, uint32 spellId, uint8 dispelType, uint8 isDebuff, std::string const& caster)
    {
        std::ostringstream ss;
        ss << uint32(remove) << "," << uint32(stack) << "," << remainingMs << "," << maxDurationMs << "," << spellId << "," << uint32(dispelType) << "," << uint32(isDebuff) << "," << caster;
        SendReplayASCommand(viewer, targetGuid, "AUR", ss.str());
    }

    bool SendReplayASAurasFromPlaybackPacket(Player* viewer, WorldPacket const& packet, MatchRecord const& match)
    {
        if (packet.GetOpcode() != SMSG_AURA_UPDATE && packet.GetOpcode() != SMSG_AURA_UPDATE_ALL)
            return false;

        constexpr uint8 REPLAY_AFLAG_CASTER   = 0x08;
        constexpr uint8 REPLAY_AFLAG_DURATION = 0x20;
        constexpr uint8 REPLAY_AFLAG_NEGATIVE = 0x80;

        std::vector<uint8> payload(packet.size());
        if (!payload.empty())
            std::memcpy(payload.data(), packet.contents(), payload.size());

        size_t pos = 0;
        ObjectGuid targetGuid;
        if (!ReadPackedGuid(payload, pos, targetGuid))
            return false;

        ReplayActor const* targetActor = FindReplayActorByGuid(match, targetGuid);
        if (!targetActor)
            return false;

        ObjectGuid uiGuid = targetActor->FakeGuid;
        bool sentAny = false;

        while (pos < payload.size())
        {
            uint8 slot = 0;
            if (!ReadUInt8(payload, pos, slot))
                return sentAny;

            uint32 spellId = 0;
            if (!ReadUInt32(payload, pos, spellId))
                return sentAny;

            uint64 cacheKey = ReplayASAuraCacheKey(viewer, uiGuid, slot);

            if (spellId == 0)
            {
                auto itr = ReplayASAuraSlotCache.find(cacheKey);
                if (itr != ReplayASAuraSlotCache.end() && itr->second.SpellId)
                {
                    SendReplayASAuraCommand(viewer, uiGuid, 1, itr->second.Stack, 0, 0, itr->second.SpellId, 0, itr->second.IsDebuff, itr->second.Caster);
                    ReplayASAuraSlotCache.erase(itr);
                    sentAny = true;
                }
                continue;
            }

            uint8 flags = 0;
            uint8 casterLevel = 0;
            uint8 stackOrCharges = 0;

            if (!ReadUInt8(payload, pos, flags) || !ReadUInt8(payload, pos, casterLevel) || !ReadUInt8(payload, pos, stackOrCharges))
                return sentAny;

            ObjectGuid casterGuid;
            std::string casterString = "0";

            if (!(flags & REPLAY_AFLAG_CASTER))
            {
                if (!ReadPackedGuid(payload, pos, casterGuid))
                    return sentAny;
                if (ReplayActor const* casterActor = FindReplayActorByGuid(match, casterGuid))
                    casterString = ReplayASGuidString(casterActor->FakeGuid);
                else if (!casterGuid.IsEmpty())
                    casterString = ReplayASGuidString(casterGuid);
            }
            else
            {
                casterString = ReplayASGuidString(uiGuid);
            }

            uint32 maxDurationMs = 0;
            uint32 remainingMs = 0;

            if (flags & REPLAY_AFLAG_DURATION)
            {
                if (!ReadUInt32(payload, pos, maxDurationMs) || !ReadUInt32(payload, pos, remainingMs))
                    return sentAny;
            }

            if (!ReplayASShouldShowAuraInDefaultFrame(spellId, flags, maxDurationMs, remainingMs))
            {
                auto itr = ReplayASAuraSlotCache.find(cacheKey);
                if (itr != ReplayASAuraSlotCache.end())
                {
                    SendReplayASAuraCommand(viewer, uiGuid, 1, itr->second.Stack, 0, 0, itr->second.SpellId, 0, itr->second.IsDebuff, itr->second.Caster);
                    ReplayASAuraSlotCache.erase(itr);
                }
                continue;
            }

            uint8 isDebuff = (flags & REPLAY_AFLAG_NEGATIVE) ? 1 : 0;
            uint8 dispelType = 0;

            ReplayASAuraSlotState state;
            state.SpellId = spellId;
            state.IsDebuff = isDebuff;
            state.Stack = stackOrCharges;
            state.Caster = casterString;
            ReplayASAuraSlotCache[cacheKey] = state;

            SendReplayASAuraCommand(viewer, uiGuid, 0, stackOrCharges, remainingMs, maxDurationMs, spellId, dispelType, isDebuff, casterString);
            sentAny = true;
        }
        return sentAny;
    }

    void SendReplayASForPlaybackPacket(Player* viewer, WorldPacket const& packet, MatchRecord const& match)
    {
        SendReplayASStatusFromPlaybackPacket(viewer, packet, match);
        SendReplayASAurasFromPlaybackPacket(viewer, packet, match);

        ObjectGuid caster;
        uint32 spellId = 0;
        int32 castTime = 0;
        if (ExtractReplayASSpell(packet, caster, spellId, castTime) && IsReplayActorGuid(match, caster))
        {
            SendReplayASCommand(viewer, caster, "SPE", std::to_string(spellId) + "," + std::to_string(castTime));
            return;
        }

        ObjectGuid attacker;
        ObjectGuid victim;
        if (ExtractReplayASAttackStart(packet, attacker, victim) && IsReplayActorGuid(match, attacker))
        {
            if (victim.IsPlayer())
            {
                if (ReplayActor const* victimActor = FindReplayActorByGuid(match, victim))
                    SendReplayASCommand(viewer, attacker, "TRG", ReplayASGuidString(victimActor->FakeGuid));
                else
                    SendReplayASCommand(viewer, attacker, "TRG", ReplayASGuidString(victim));
            }
            SendReplayASCommand(viewer, attacker, "SPE", "6603,0");
            return;
        }
    }

    bool TrySendFakeNameQueryResponse(WorldSession* session, ObjectGuid guid)
    {
        if (!session || !session->GetPlayer())
            return false;

        uint32 viewerLowGuid = session->GetPlayer()->GetGUID().GetCounter();
        auto active = ActiveReplays.find(viewerLowGuid);
        if (active == ActiveReplays.end())
            return false;

        for (ReplayActor const& actor : active->second.Match.Actors)
        {
            if (actor.FakeGuid == guid)
            {
                SendReplayNameResponse(session, actor);
                return true;
            }
        }
        return false;
    }

    void FinishPlaybackForViewer(uint32 viewerLowGuid, PlaybackState& state)
    {
        state.Finished = true;
        Player* viewer = ObjectAccessor::FindPlayerByLowGUID(viewerLowGuid);
        if (!viewer || !viewer->GetSession())
            return;

        ChatHandler(viewer->GetSession()).PSendSysMessage("Replay finished. Staying in the replay instance so the final frame remains visible; teleport back when done.");
    }

    std::string EscapeReplaySqlString(std::string value)
    {
        CharacterDatabase.EscapeString(value);
        return value;
    }

    std::string JoinActorGuidList(MatchRecord const& match, uint32 team)
    {
        std::string out;
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.Team != team)
                continue;
            if (!out.empty())
                out += ", ";
            out += std::to_string(actor.OriginalGuid.GetRawValue());
        }
        return out;
    }

    std::string JoinActorNameList(MatchRecord const& match, uint32 team)
    {
        std::string out;
        for (ReplayActor const& actor : match.Actors)
        {
            if (actor.Team != team)
                continue;
            if (!out.empty())
                out += " ";
            out += actor.Name;
        }
        return out.empty() ? "Unknown" : out;
    }

    uint32 NormalizeReplayWinnerTeam(MatchRecord const& match, uint32 winner)
    {
        if (winner == ALLIANCE || winner == HORDE)
            return winner;

        for (ReplayActor const& actor : match.Actors)
            if (actor.Team == ALLIANCE)
                return ALLIANCE;

        for (ReplayActor const& actor : match.Actors)
            if (actor.Team == HORDE)
                return HORDE;

        return ALLIANCE;
    }

    void GetReplayTeamInfo(Battleground* bg, MatchRecord const& match, uint32 team, std::string& teamName, uint32& rating, uint32& mmr)
    {
        teamName.clear();
        rating = 0;
        mmr = 0;

        if (bg && bg->isArena() && bg->isRated())
        {
            if (ArenaTeam* arenaTeam = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdForTeam(team)))
            {
                teamName = arenaTeam->GetName();
                rating = arenaTeam->GetRating();
            }
            mmr = bg->GetArenaMatchmakerRating(team);
        }

        if (teamName.empty())
            teamName = JoinActorNameList(match, team);
    }

    uint32 GetNextArenaReplayIdForMessage()
    {
        QueryResult result = CharacterDatabase.Query("SELECT COALESCE(MAX(`id`), 0) FROM `character_arena_replays`");
        if (!result)
            return 0;
        return (*result)[0].GetUInt32() + 1;
    }

    bool ReplayExistsInAcTable(uint32 replayId)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT `id` FROM `character_arena_replays` WHERE `id` = {} LIMIT 1", replayId);
        return bool(result);
    }

    std::vector<uint32> LoadSavedReplayIds(uint32 characterId)
    {
        std::vector<uint32> replayIds;
        QueryResult result = CharacterDatabase.PQuery(
            "SELECT `replay_id` FROM `character_saved_replays` WHERE `character_id` = {} ORDER BY `id` DESC LIMIT 20", characterId);

        if (!result)
            return replayIds;

        do
        {
            replayIds.push_back((*result)[0].GetUInt32());
        }
        while (result->NextRow());
        return replayIds;
    }

    void FavoriteReplayForPlayer(Player* player, uint32 replayId)
    {
        if (!player || !player->GetSession())
            return;

        if (!ReplayExistsInAcTable(replayId))
            return;

        CharacterDatabase.Execute(
            Trinity::StringFormat("INSERT IGNORE INTO `character_saved_replays` (`character_id`, `replay_id`) VALUES ({}, {})",
            player->GetGUID().GetCounter(), replayId).c_str());
    }

    void DeleteOldArenaReplaysFromConfig()
    {
        int32 daysConfig = sConfigMgr->GetIntDefault("ArenaReplay.DeleteReplaysAfterDays", 30);
        uint32 days = daysConfig > 0 ? uint32(daysConfig) : 0;
        if (!days)
            return;

        bool deleteSaved = sConfigMgr->GetBoolDefault("ArenaReplay.DeleteSavedReplays", false);

        if (deleteSaved)
        {
            CharacterDatabase.Execute(
                Trinity::StringFormat("DELETE FROM `character_arena_replays` WHERE `timestamp` < (NOW() - INTERVAL {} DAY)", days).c_str());
            CharacterDatabase.Execute(
                "DELETE FROM `character_saved_replays` WHERE `replay_id` NOT IN (SELECT `id` FROM `character_arena_replays`)");
        }
        else
        {
            CharacterDatabase.Execute(
                Trinity::StringFormat("DELETE FROM `character_arena_replays` WHERE `timestamp` < (NOW() - INTERVAL {} DAY) AND `id` NOT IN (SELECT `replay_id` FROM `character_saved_replays`)", days).c_str());
        }
    }

    void SerializeMatchData(MatchRecord const& match, ByteBuffer& buffer)
    {
        buffer << uint32(ARENA_REPLAY_V2_MAGIC);
        buffer << uint32(ARENA_REPLAY_V2_VERSION);

        buffer << uint32(match.Actors.size());
        for (ReplayActor const& actor : match.Actors)
        {
            buffer << uint64(actor.OriginalGuid.GetRawValue());
            buffer << actor.Name;
            buffer << uint8(actor.Race);
            buffer << uint8(actor.Class);
            buffer << uint8(actor.Gender);
            buffer << uint32(actor.Team);
        }

        buffer << uint32(match.Packets.size());
        for (PacketRecord const& frame : match.Packets)
        {
            buffer << uint32(frame.Packet.size());
            buffer << uint32(frame.TimestampMs);
            buffer << uint16(frame.Packet.GetOpcode());

            if (frame.Packet.size() > 0)
                buffer.append(frame.Packet.contents(), frame.Packet.size());
        }
    }

    void DeserializeV1Frames(MatchRecord& record, ByteBuffer& buffer)
    {
        uint32 packetSize = 0;
        uint32 packetTimestamp = 0;
        uint16 opcode = 0;

        while (buffer.rpos() <= buffer.size() - 1)
        {
            if (buffer.size() - buffer.rpos() < 10)
                break;

            buffer >> packetSize;
            buffer >> packetTimestamp;
            buffer >> opcode;

            WorldPacket packet(opcode, packetSize);
            if (packetSize > 0)
            {
                if (buffer.size() - buffer.rpos() < packetSize)
                    break;

                std::vector<uint8> tmp(packetSize, 0);
                buffer.read(tmp.data(), packetSize);
                packet.append(tmp.data(), packetSize);
            }
            record.Packets.push_back({ packetTimestamp, packet });
        }
    }

    bool DeserializeMatchDataFromBytes(MatchRecord& record, uint32 arenaTypeId, uint32 typeId, uint32 mapId, std::vector<uint8> const& data)
    {
        record.ArenaTypeId = uint8(arenaTypeId);
        record.TypeId = BattlegroundTypeId(typeId);
        record.MapId = mapId;

        if (data.empty())
            return false;

        ByteBuffer buffer;
        buffer.append(data.data(), data.size());

        if (buffer.size() < 4)
            return false;

        uint32 magic = 0;
        buffer >> magic;

        if (magic != ARENA_REPLAY_V2_MAGIC)
        {
            buffer.rpos(0);
            DeserializeV1Frames(record, buffer);
            return !record.Packets.empty();
        }

        uint32 version = 0;
        buffer >> version;

        if (version != ARENA_REPLAY_V2_VERSION)
        {
            TC_LOG_ERROR("arena.replay", "Unsupported arena replay version {}", version);
            return false;
        }

        uint32 actorCount = 0;
        buffer >> actorCount;

        for (uint32 i = 0; i < actorCount; ++i)
        {
            uint64 originalRaw = 0;
            ReplayActor actor;

            buffer >> originalRaw;
            actor.OriginalGuid.SetRawValue(originalRaw);
            buffer >> actor.Name;
            buffer >> actor.Race;
            buffer >> actor.Class;
            buffer >> actor.Gender;
            buffer >> actor.Team;

            record.Actors.push_back(actor);
        }

        uint32 packetCount = 0;
        buffer >> packetCount;

        for (uint32 i = 0; i < packetCount; ++i)
        {
            if (buffer.size() - buffer.rpos() < 10)
                break;

            uint32 packetSize = 0;
            uint32 packetTimestamp = 0;
            uint16 opcode = 0;

            buffer >> packetSize;
            buffer >> packetTimestamp;
            buffer >> opcode;

            WorldPacket packet(opcode, packetSize);
            if (packetSize > 0)
            {
                if (buffer.size() - buffer.rpos() < packetSize)
                    break;

                std::vector<uint8> tmp(packetSize, 0);
                buffer.read(tmp.data(), packetSize);
                packet.append(tmp.data(), packetSize);
            }
            record.Packets.push_back({ packetTimestamp, packet });
        }
        return !record.Packets.empty();
    }

    bool DeserializeMatchDataFromBlob(MatchRecord& record, Field* fields)
    {
        std::vector<uint8> decoded = fields[4].GetBinary();
        uint32 contentSize = fields[3].GetUInt32();
        if (contentSize && decoded.size() != contentSize)
            TC_LOG_ERROR("arena.replay", "Replay contentSize mismatch id={} contentSize={} decodedSize={}",
                fields[0].GetUInt32(), contentSize, uint32(decoded.size()));

        return DeserializeMatchDataFromBytes(record, fields[1].GetUInt32(), fields[2].GetUInt32(), fields[5].GetUInt32(), decoded);
    }

    std::vector<uint32> LoadLast10Replays()
    {
        std::vector<uint32> replayIds;
        QueryResult result = CharacterDatabase.Query("SELECT `id` FROM `character_arena_replays` ORDER BY `id` DESC LIMIT 10");
        if (!result)
            return replayIds;

        do
        {
            replayIds.push_back((*result)[0].GetUInt32());
        }
        while (result->NextRow());
        return replayIds;
    }

    bool LoadReplayDataForPlayer(Player* player, uint32 matchId, MatchRecord& record)
    {
        QueryResult result = CharacterDatabase.PQuery(
            "SELECT `id`, `arenaTypeId`, `typeId`, `contentSize`, `contents`, `mapId`, `timesWatched` "
            "FROM `character_arena_replays` WHERE `id` = {} LIMIT 1", matchId);

        if (!result)
            return false;

        Field* fields = result->Fetch();
        if (!fields || !DeserializeMatchDataFromBlob(record, fields))
            return false;

        CharacterDatabase.Execute(Trinity::StringFormat("UPDATE `character_arena_replays` SET `timesWatched` = `timesWatched` + 1 WHERE `id` = {}", matchId).c_str());

        if (record.Packets.empty())
            return false;

        AssignFakeGuids(record, player->GetGUID().GetCounter());

        ChatHandler(player->GetSession()).PSendSysMessage("Replay loaded: packets=%u, actors=%u",
            uint32(record.Packets.size()), uint32(record.Actors.size()));
        return true;
    }

    bool StartReplay(Player* player, uint32 replayId)
    {
        if (!player || !player->GetSession())
            return false;

        ChatHandler handler(player->GetSession());
        MatchRecord record;
        if (!LoadReplayDataForPlayer(player, replayId, record))
            return false;

        Battleground* bg = sBattlegroundMgr->CreateNewBattleground(record.TypeId, GetBattlegroundBracketByLevel(record.MapId, player->GetLevel()), record.ArenaTypeId, false);
        if (!bg)
        {
            handler.SetSentErrorMessage(true);
            return false;
        }

        uint32 viewerLowGuid = player->GetGUID().GetCounter();
        player->SetBattlegroundEntryPoint();

        player->SetIsSpectator(true);
        bg->toggleReplay(player->GetGUID());
        player->SetPendingSpectatorForBG(bg->GetInstanceID());
        bg->StartBattleground();

        BattlegroundTypeId bgTypeId = bg->GetTypeID();
        uint32 queueSlot = 0;
        TeamId teamId = TEAM_NEUTRAL;
        WorldPacket status;

        player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId);
        sBattlegroundMgr->SendToBattleground(player, bg->GetInstanceID(), bgTypeId);
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&status, bg, queueSlot, STATUS_IN_PROGRESS, 0, 0, bg->GetArenaType(), teamId);
        player->GetSession()->SendPacket(&status);

        PlaybackState state;
        state.Match = std::move(record);
        state.ViewerLowGuid = viewerLowGuid;
        state.BgInstanceId = bg->GetInstanceID();
        state.CreatedMs = getMSTime();

        ActiveReplays[viewerLowGuid] = std::move(state);
        return true;
    }

    void SaveReplay(Battleground* bg, uint32 winner)
    {
        if (!bg)
            return;

        if (!sConfigMgr->GetBoolDefault("ArenaReplay.Enable", true))
            return;

        if (bg->isArena())
        {
            if (!bg->isRated() && !sConfigMgr->GetBoolDefault("ArenaReplay.SaveUnratedArenas", true))
                return;
        }
        else
        {
            if (!sConfigMgr->GetBoolDefault("ArenaReplay.SaveBattlegrounds", true))
                return;
        }

        auto recordItr = Records.find(bg->GetInstanceID());
        if (recordItr == Records.end())
            return;

        MatchRecord& match = recordItr->second;
        RefreshActorsFromBattleground(bg, match);

        if (match.Packets.empty())
        {
            Records.erase(recordItr);
            return;
        }

        uint32 durationMs = match.Packets.empty() ? 0 : match.Packets.back().TimestampMs;
        int32 validArenaSecondsConfig = sConfigMgr->GetIntDefault("ArenaReplay.ValidArenaDuration", 0);
        uint32 validArenaSeconds = validArenaSecondsConfig > 0 ? uint32(validArenaSecondsConfig) : 0;
        if (validArenaSeconds && durationMs < validArenaSeconds * IN_MILLISECONDS)
        {
            Records.erase(recordItr);
            return;
        }

        ByteBuffer buffer;
        SerializeMatchData(match, buffer);
        std::vector<uint8> rawReplay(buffer.contents(), buffer.contents() + buffer.size());

        uint32 winnerTeam = NormalizeReplayWinnerTeam(match, winner);
        uint32 loserTeam = bg->GetOtherTeam(winnerTeam);

        std::string winnerPlayerGuids = JoinActorGuidList(match, winnerTeam);
        std::string loserPlayerGuids = JoinActorGuidList(match, loserTeam);

        std::string winnerTeamName;
        std::string loserTeamName;
        uint32 winnerTeamRating = 0;
        uint32 loserTeamRating = 0;
        uint32 winnerTeamMMR = 0;
        uint32 loserTeamMMR = 0;

        GetReplayTeamInfo(bg, match, winnerTeam, winnerTeamName, winnerTeamRating, winnerTeamMMR);
        GetReplayTeamInfo(bg, match, loserTeam, loserTeamName, loserTeamRating, loserTeamMMR);

        uint32 predictedReplayId = GetNextArenaReplayIdForMessage();

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ARENA_REPLAYS);
        stmt->setUInt32(0, uint32(match.ArenaTypeId));
        stmt->setUInt32(1, uint32(match.TypeId));
        stmt->setUInt32(2, uint32(rawReplay.size()));
        stmt->setBinary(3, rawReplay);
        stmt->setUInt32(4, bg->GetMapId());
        stmt->setString(5, winnerTeamName);
        stmt->setUInt32(6, winnerTeamRating);
        stmt->setUInt32(7, winnerTeamMMR);
        stmt->setString(8, loserTeamName);
        stmt->setUInt32(9, loserTeamRating);
        stmt->setUInt32(10, loserTeamMMR);
        stmt->setString(11, winnerPlayerGuids);
        stmt->setString(12, loserPlayerGuids);
        CharacterDatabase.Execute(stmt);

        for (auto const& bgPlayer : bg->GetPlayers())
        {
            Player* player = bg->_GetPlayer(bgPlayer.first, bgPlayer.second.OfflineRemoveTime != 0, "arena replay save message");
            if (player && player->GetSession())
            {
                if (predictedReplayId)
                    ChatHandler(player->GetSession()).PSendSysMessage("Replay saved. Match ID: %u", predictedReplayId);
                else
                    ChatHandler(player->GetSession()).PSendSysMessage("Replay saved.");
            }
        }
        Records.erase(recordItr);
    }
}

    bool RestartReplayForViewer(Player* viewer);

    bool TryHandleReplayRestartAddonMessage(WorldSession* session, WorldPacket const& packet)
    {
        if (!session || !session->GetPlayer())
            return false;

        if (packet.GetOpcode() != CMSG_MESSAGECHAT)
            return false;

        WorldPacket copy(packet);
        copy.rpos(0);

        uint32 type = 0;
        uint32 lang = 0;

        copy >> type;
        copy >> lang;

        if (type != CHAT_MSG_WHISPER || lang != LANG_ADDON)
            return false;

        std::string to;
        copy >> to;

        std::string msg = copy.ReadCString(false);
        if (msg != "ASSUN\tRESTART")
            return false;

        RestartReplayForViewer(session->GetPlayer());
        return true;
    }

    void TryHandleReplayProbeChatCommand(WorldSession* session, WorldPacket const& packet)
    {
        if (!session || !session->GetPlayer())
            return;

        if (packet.GetOpcode() != CMSG_MESSAGECHAT)
            return;

        WorldPacket copy(packet);
        copy.rpos(0);

        uint32 type = 0;
        uint32 lang = 0;
        copy >> type;
        copy >> lang;

        if (lang == LANG_ADDON)
            return;

        std::string text;
        if (type == CHAT_MSG_WHISPER)
        {
            std::string to;
            copy >> to;
            text = copy.ReadCString(false);
        }
        else
            text = copy.ReadCString(false);

        if (text.empty() || text[0] != '.')
            return;

        std::string lowered = text;
        std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) { return char(std::tolower(c)); });

        bool isCooldownMarker = lowered.find("cooldown") != std::string::npos;
        bool isReplayMark = lowered.find("replaymark") != std::string::npos || lowered.find("replay mark") != std::string::npos;
        bool isReplayProbe = lowered.find("replayprobe") != std::string::npos || lowered.find("replay probe") != std::string::npos;
        bool isReplay_Step = lowered.find("replaystep") != std::string::npos || lowered.find("replay step") != std::string::npos;
        bool isReplayResume = lowered.find("replayresume") != std::string::npos || lowered.find("replay resume") != std::string::npos;
        bool isReplayPause = lowered.find("replaypause") != std::string::npos || lowered.find("replay pause") != std::string::npos;

        if (!isCooldownMarker && !isReplayMark && !isReplayProbe && !isReplay_Step && !isReplayResume && !isReplayPause)
            return;

        Player* player = session->GetPlayer();
        LogReplayManualMarker(player, "chat-command", text);

        uint32 viewerLowGuid = player->GetGUID().GetCounter();
        auto activeItr = ActiveReplays.find(viewerLowGuid);
        if (activeItr == ActiveReplays.end())
            return;

        PlaybackState& state = activeItr->second;

        if (isReplayMark || isReplayProbe || isReplayPause)
        {
            state.ProbePaused = true;
            state.ProbeStepBudget = 0;
        }
        else if (isReplay_Step)
        {
            state.ProbePaused = true;
            ++state.ProbeStepBudget;
        }
        else if (isReplayResume)
        {
            state.ProbePaused = false;
            state.ProbeStepBudget = 0;
        }
    }

    bool ShouldRecordBattlefieldPacket(Battlefield* bf, WorldPacket const& packet)
    {
        if (!bf)
            return false;
        return bf->IsWarTime() && IsWatchedOpcode(packet.GetOpcode());
    }

class BGReplayServerScript : public ServerScript
{
public:
    BGReplayServerScript() : ServerScript("BGReplayServerScript") { }

    void OnPacketSend(WorldSession* session, WorldPacket& packet) override
    {
        if (!session || !session->GetPlayer())
            return;

        Player* player = session->GetPlayer();
        Battleground* bg = player->GetBattleground();
        Battlefield* bf = nullptr;

        if (!bg)
        {
            bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());
            if (!bf || !bf->IsWarTime() || !bf->HasPlayer(player))
                bf = nullptr;
        }

        if (bg)
        {
            if (bg->IsReplay())
                return;

            if (!bg->isArena() && !sConfigMgr->GetBoolDefault("ArenaReplay.SaveBattlegrounds", true))
                return;

            if (!ShouldRecordPacket(bg, packet))
                return;

            if (!IsTeamRecorder(bg, player))
                return;

            MatchRecord& record = GetOrCreateRecord(bg);
            uint32 nowMs = getMSTime();

            if (IsDuplicateRecentPacket(record, packet, nowMs))
                return;

            uint32 timestamp = 0;
            if (bg->GetStatus() == STATUS_WAIT_JOIN)
            {
                timestamp = 0;
                ++record.PreStartPacketCount;
            }
            else
            {
                if (!record.InProgressStartMs)
                {
                    record.InProgressStartMs = nowMs;
                }
                timestamp = ARENA_REPLAY_PRELOAD_MS + (nowMs - record.InProgressStartMs);
            }
            record.Packets.push_back({ timestamp, WorldPacket(packet) });
        }
        else if (bf)
        {
            if (!sConfigMgr->GetBoolDefault("ArenaReplay.SaveBattlefields", true))
                return;

            if (!ShouldRecordBattlefieldPacket(bf, packet))
                return;

            MatchRecord& record = GetOrCreateRecord(bf);
            uint32 nowMs = getMSTime();

            if (IsDuplicateRecentPacket(record, packet, nowMs))
                return;

            uint32 timestamp = 0;
            if (!record.InProgressStartMs)
            {
                record.InProgressStartMs = nowMs;
            }
            timestamp = ARENA_REPLAY_PRELOAD_MS + (nowMs - record.InProgressStartMs);
            record.Packets.push_back({ timestamp, WorldPacket(packet) });
        }
    }

    void OnPacketReceive(WorldSession* session, WorldPacket& packet) override
    {
        if (!session || !session->GetPlayer())
            return;

        if (packet.GetOpcode() == CMSG_MESSAGECHAT)
        {
            TryHandleReplayRestartAddonMessage(session, packet);
            TryHandleReplayProbeChatCommand(session, packet);
            return;
        }

        if (packet.GetOpcode() != CMSG_NAME_QUERY)
            return;

        WorldPacket copy(packet);
        copy.rpos(0);

        ObjectGuid guid;
        copy >> guid;
        TrySendFakeNameQueryResponse(session, guid);
    }
};

    bool UpdatePlaybackForViewer(Player* viewer)
    {
        if (!viewer || !viewer->GetSession())
            return false;

        uint32 viewerLowGuid = viewer->GetGUID().GetCounter();
        auto activeItr = ActiveReplays.find(viewerLowGuid);
        if (activeItr == ActiveReplays.end())
            return false;

        PlaybackState& state = activeItr->second;
        uint32 nowMs = getMSTime();

        if (state.Finished)
        {
            Battleground* currentBg = viewer->GetBattleground();
            if (!currentBg || !currentBg->IsReplay() || currentBg->GetInstanceID() != state.BgInstanceId)
                ActiveReplays.erase(activeItr);
            return true;
        }

        if (!IsViewerReadyForReplay(viewer, state))
        {
            if (nowMs - state.CreatedMs > ARENA_REPLAY_LOAD_GRACE_MS)
            {
                ActiveReplays.erase(activeItr);
                return true;
            }
            return true;
        }

        if (!state.PlaybackClockStarted)
        {
            bool skippedCountdown = TrySkipReplayCountdown(viewer, state);
            state.PlaybackStartMs = nowMs + ARENA_REPLAY_START_DELAY_MS;
            state.PlaybackClockStarted = true;

            SendReplayActorChannelClearUpdates(viewer, state, "playback armed initial channel visual cleanup");
        }

        if (!state.SentInitialNameResponses)
            SendInitialReplayNameResponses(viewer, state);

        if (!state.SentReplayASInitial)
            SendReplayASInitial(viewer, state);

        if (nowMs < state.PlaybackStartMs)
            return true;

        uint32 elapsedMs = nowMs - state.PlaybackStartMs;
        if (state.ProbePaused && state.ProbeStepBudget == 0)
            return true;

        uint32 sendCap = state.ProbePaused ? state.ProbeStepBudget : ARENA_REPLAY_SEND_CAP_PER_UPDATE;
        uint32 sentThisUpdate = 0;

        while (state.Cursor < state.Match.Packets.size() && state.Match.Packets[state.Cursor].TimestampMs <= elapsedMs && sentThisUpdate < sendCap)
        {
            PacketRecord const& frame = state.Match.Packets[state.Cursor];
            WorldPacket out;
            if (!BuildPlaybackPacket(frame, state.Match, out))
            {
                ++state.Cursor;
                ++sentThisUpdate;
                continue;
            }

            LogReplayPacketProbe(viewer, state, frame, out, state.Cursor, elapsedMs);
            viewer->GetSession()->SendPacket(&out);
            SendReplayASForPlaybackPacket(viewer, out, state.Match);

            ++state.Cursor;
            ++sentThisUpdate;
        }

        if (state.ProbePaused && sentThisUpdate > 0)
        {
            if (state.ProbeStepBudget > sentThisUpdate)
                state.ProbeStepBudget -= sentThisUpdate;
            else
                state.ProbeStepBudget = 0;
        }

        if (state.Cursor > 0)
        {
            MaybeSendReplayGreenNameUpdates(viewer, state, nowMs);
            MaybeSendReplayActorChannelClearUpdates(viewer, state, nowMs);
        }

        if (state.Cursor >= state.Match.Packets.size())
            FinishPlaybackForViewer(viewerLowGuid, state);
        return true;
    }

    void SendDestroyFakeReplayActorObjects(Player* viewer, PlaybackState& state, char const* reason)
    {
        if (!viewer || !viewer->GetSession())
            return;

        uint32 sent = 0;
        for (ReplayActor const& actor : state.Match.Actors)
        {
            SendDestroyObjectToReplayViewer(viewer, actor.FakeGuid, reason);
            ++sent;
        }
    }

    bool RestartReplayForViewer(Player* viewer)
    {
        if (!viewer || !viewer->GetSession())
            return false;

        uint32 viewerLowGuid = viewer->GetGUID().GetCounter();
        auto activeItr = ActiveReplays.find(viewerLowGuid);
        if (activeItr == ActiveReplays.end())
            return false;

        PlaybackState& state = activeItr->second;
        Battleground* bg = viewer->GetBattleground();
        if (!bg || !bg->IsReplay() || bg->GetInstanceID() != state.BgInstanceId)
            return false;

        SendReplayASRaw(viewer, "DISABLE");
        SendDestroyFakeReplayActorObjects(viewer, state, "replay restart");

        state.Cursor = 0;
        state.CreatedMs = getMSTime();
        state.PlaybackStartMs = 0;
        state.PlaybackClockStarted = false;
        state.SentInitialNameResponses = false;
        state.SentReplayASInitial = false;
        state.Finished = false;
        state.LastOriginalActorDestroyMs = 0;
        state.LastNameColorUpdateMs = 0;
        state.NameColorUpdateBursts = 0;
        state.LastChannelClearMs = 0;
        state.ChannelClearBursts = 0;
        state.ProbePaused = false;
        state.ProbeStepBudget = 0;
        return true;
    }

class BGReplayPlayerScript : public PlayerScript
{
public:
    BGReplayPlayerScript() : PlayerScript("BGReplayPlayerScript") { }

    void OnUpdate(Player* player, uint32 /*p_time*/) override
    {
        UpdatePlaybackForViewer(player);
    }

    void OnMapChanged(Player* player) override
    {
        if (UpdatePlaybackForViewer(player))
        {
            // do nothing
        }
    }

    void OnLogout(Player* player) override
    {
        if (!player)
            return;
        ActiveReplays.erase(player->GetGUID().GetCounter());
    }
};

class BGReplayWorldScript : public WorldScript
{
public:
    BGReplayWorldScript() : WorldScript("BGReplayWorldScript") { }

    void OnConfigLoad(bool /*reload*/) override
    {
        DeleteOldArenaReplaysFromConfig();
    }

    void OnUpdate(uint32 /*diff*/) override
    {
    }
};

class BGReplayBGScript : public BGScript
{
public:
    BGReplayBGScript() : BGScript("BGReplayBGScript") { }

    void OnBattlegroundEnd(Battleground* bg, uint32 winner) override
    {
        if (!bg)
            return;

        if (!bg->IsReplay())
            SaveReplay(bg, winner);
    }

    void OnBattlegroundUpdate(Battleground* bg, uint32 /*diff*/) override
    {
    }
};

enum ReplayGossipActions : uint32
{
    REPLAY_GOSSIP_MATCH_ID = 900000001,
    REPLAY_GOSSIP_FAVORITE_MATCH_ID = 900000002,
    REPLAY_GOSSIP_MY_FAVORITES = 900000003,
    REPLAY_GOSSIP_BACK = 900000004
};

class ReplayGossip : public CreatureScript
{
public:
    ReplayGossip() : CreatureScript("ReplayGossip") { }

    struct replayAI : public ScriptedAI
    {
        replayAI(Creature* creature) : ScriptedAI(creature) { }

        void ShowMainMenu(Player* player)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Replay a Match ID", GOSSIP_SENDER_MAIN, REPLAY_GOSSIP_MATCH_ID, "Enter replay match ID", 0, true);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Favorite a Match ID", GOSSIP_SENDER_MAIN, REPLAY_GOSSIP_FAVORITE_MATCH_ID, "Enter replay match ID to favorite", 0, true);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "My favorite matches", GOSSIP_SENDER_MAIN, REPLAY_GOSSIP_MY_FAVORITES);

            std::vector<uint32> matchIds = LoadLast10Replays();
            if (matchIds.empty())
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "No replays found.", GOSSIP_SENDER_MAIN, REPLAY_GOSSIP_BACK);
            else
            {
                for (uint32 matchId : matchIds)
                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, matchId);
            }
            SendGossipMenuFor(player, 1775757, me->GetGUID());
        }

        bool OnGossipHello(Player* player) override
        {
            ShowMainMenu(player);
            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 action = GetGossipActionFor(player, gossipListId);
            player->PlayerTalkClass->ClearMenus();

            if (action == REPLAY_GOSSIP_MY_FAVORITES)
            {
                std::vector<uint32> matchIds = LoadSavedReplayIds(player->GetGUID().GetCounter());
                if (matchIds.empty())
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "No favorite replays found.", GOSSIP_SENDER_MAIN, REPLAY_GOSSIP_BACK);
                else
                    for (uint32 matchId : matchIds)
                        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay favorite match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, matchId);

                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Back", GOSSIP_SENDER_MAIN, REPLAY_GOSSIP_BACK);
                SendGossipMenuFor(player, 1775757, me->GetGUID());
                return true;
            }

            if (action == REPLAY_GOSSIP_BACK)
            {
                ShowMainMenu(player);
                return true;
            }

            StartReplay(player, action);
            CloseGossipMenuFor(player);
            return true;
        }

        bool OnGossipSelectCode(Player* player, uint32 /*menuId*/, uint32 gossipListId, char const* code) override
        {
            uint32 action = GetGossipActionFor(player, gossipListId);
            player->PlayerTalkClass->ClearMenus();

            if (!code || !*code)
            {
                CloseGossipMenuFor(player);
                return false;
            }

            uint32 replayId = 0;
            try
            {
                replayId = uint32(std::stoul(code));
            }
            catch (...)
            {
                CloseGossipMenuFor(player);
                return false;
            }

            if (action == REPLAY_GOSSIP_MATCH_ID)
            {
                StartReplay(player, replayId);
                CloseGossipMenuFor(player);
                return true;
            }

            if (action == REPLAY_GOSSIP_FAVORITE_MATCH_ID)
            {
                FavoriteReplayForPlayer(player, replayId);
                CloseGossipMenuFor(player);
                return true;
            }

            CloseGossipMenuFor(player);
            return false;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new replayAI(creature);
    }
};

bool IsFakeReplayPlayerGuid(ObjectGuid guid, std::string& name, uint8& race, uint8& gender, uint8& classId)
{
    for (auto const& active : ActiveReplays)
    {
        for (ReplayActor const& actor : active.second.Match.Actors)
        {
            if (actor.FakeGuid == guid)
            {
                name = actor.Name;
                race = actor.Race;
                gender = actor.Gender;
                classId = actor.Class;
                return true;
            }
        }
    }
    return false;
}

void SaveBattlefieldReplay(Battlefield* bf)
{
    if (!bf)
        return;

    if (!sConfigMgr->GetBoolDefault("ArenaReplay.Enable", true))
        return;

    if (!sConfigMgr->GetBoolDefault("ArenaReplay.SaveBattlefields", true))
        return;

    auto recordItr = Records.find(bf->GetZoneId());
    if (recordItr == Records.end())
        return;

    MatchRecord& match = recordItr->second;

    std::unordered_set<uint64> seen;
    for (ReplayActor const& actor : match.Actors)
        seen.insert(actor.OriginalGuid.GetRawValue());

    for (auto const& it : bf->GetPlayerMap())
    {
        if (seen.find(it.first.GetRawValue()) != seen.end())
            continue;

        Player* player = ObjectAccessor::FindConnectedPlayer(it.first);
        if (!player)
            continue;

        ReplayActor actor;
        actor.OriginalGuid = player->GetGUID();
        actor.Name = player->GetName();
        actor.Race = player->GetRace();
        actor.Class = player->GetClass();
        actor.Gender = uint8(player->GetGender());
        actor.Team = player->GetBGTeam();

        match.Actors.push_back(actor);
        seen.insert(actor.OriginalGuid.GetRawValue());
    }

    if (match.Packets.empty())
    {
        Records.erase(recordItr);
        return;
    }

    ByteBuffer buffer;
    SerializeMatchData(match, buffer);
    std::vector<uint8> rawReplay(buffer.contents(), buffer.contents() + buffer.size());

    uint32 winnerTeam = bf->GetDefenderTeam();
    uint32 loserTeam = bf->GetAttackerTeam();

    std::string winnerPlayerGuids = JoinActorGuidList(match, winnerTeam);
    std::string loserPlayerGuids = JoinActorGuidList(match, loserTeam);

    std::string winnerTeamName = (winnerTeam == ALLIANCE) ? "Alliance" : "Horde";
    std::string loserTeamName = (loserTeam == ALLIANCE) ? "Alliance" : "Horde";
    uint32 winnerTeamRating = 0;
    uint32 loserTeamRating = 0;
    uint32 winnerTeamMMR = 0;
    uint32 loserTeamMMR = 0;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ARENA_REPLAYS);
    stmt->setUInt32(0, 0);
    stmt->setUInt32(1, uint32(BATTLEGROUND_TYPE_NONE));
    stmt->setUInt32(2, uint32(rawReplay.size()));
    stmt->setBinary(3, rawReplay);
    stmt->setUInt32(4, bf->GetMapId());
    stmt->setString(5, winnerTeamName);
    stmt->setUInt32(6, winnerTeamRating);
    stmt->setUInt32(7, winnerTeamMMR);
    stmt->setString(8, loserTeamName);
    stmt->setUInt32(9, loserTeamRating);
    stmt->setUInt32(10, loserTeamMMR);
    stmt->setString(11, winnerPlayerGuids);
    stmt->setString(12, loserPlayerGuids);
    CharacterDatabase.Execute(stmt);

    Records.erase(recordItr);
}

extern bool (*IsFakeReplayPlayerGuidPtr)(ObjectGuid, std::string&, uint8&, uint8&, uint8&);
extern void (*SaveBattlefieldReplayPtr)(Battlefield*);

void AddBGReplayScripts()
{
    IsFakeReplayPlayerGuidPtr = &IsFakeReplayPlayerGuid;
    SaveBattlefieldReplayPtr = &SaveBattlefieldReplay;

    new BGReplayServerScript();
    new BGReplayWorldScript();
    new BGReplayPlayerScript();
    new BGReplayBGScript();
    new ReplayGossip();
}
