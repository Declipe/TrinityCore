#include "AutoShutdown.h"
#include "Config.h"
#include "CustomConfig.h"
#include "Duration.h"
#include "Language.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "StringConvert.h"
#include "StringFormat.h"
#include "TaskScheduler.h"
#include "Util.h"
#include "World.h"

namespace
{
    TaskScheduler scheduler;

    time_t GetNextResetTime(time_t time, uint8 hour, uint8 minute, uint8 second)
    {
        tm tmLcl = TimeBreakdown(time);
        tmLcl.tm_hour = hour;
        tmLcl.tm_min = minute;
        tmLcl.tm_sec = second;

        time_t miLc = mktime(&tmLcl);

        if (miLc <= time)
            miLc += DAY;

        return miLc;
    }
}

AutoShutdown* AutoShutdown::instance()
{
    static AutoShutdown instance;
    return &instance;
}

void AutoShutdown::Init()
{
    isEnable = sGameConfig->GetBoolConfig("AutoShutdown.Enabled");

    if (!isEnable)
        return;

    std::string configTime = sGameConfig->GetStringConfig("AutoShutdown.Time");
    auto const& tokens = Trinity::Tokenize(configTime, ':', false);

    if (tokens.size() != 3)
    {
        TC_LOG_ERROR("server", "> AutoShutdown: Incorrect time in config option 'AutoShutdown.Time' - '{}'", configTime);
        isEnable = false;
        return;
    }

    auto CheckTime = [tokens](std::initializer_list<uint8> index)
        {
            for (auto const& itr : index)
            {
                if (Trinity::StringTo<uint8>(tokens.at(itr)) == std::nullopt)
                    return false;
            }

            return true;
        };

    if (!CheckTime({ 0, 1, 2 }))
    {
        TC_LOG_ERROR("server", "> AutoShutdown: Incorrect time in config option 'AutoShutdown.Time' - '{}'", configTime);
        isEnable = false;
        return;
    }

    uint8 hour = *Trinity::StringTo<uint8>(tokens.at(0));
    uint8 minute = *Trinity::StringTo<uint8>(tokens.at(1));
    uint8 second = *Trinity::StringTo<uint8>(tokens.at(2));

    if (hour > 23)
    {
        TC_LOG_ERROR("server", "> AutoShutdown: Incorrect hour in config option 'AutoShutdown.Time' - '{}'", configTime);
        isEnable = false;
    }
    else if (minute >= 60)
    {
        TC_LOG_ERROR("server", "> AutoShutdown: Incorrect minute in config option 'AutoShutdown.Time' - '{}'", configTime);
        isEnable = false;
    }
    else if (second >= 60)
    {
        TC_LOG_ERROR("server", "> AutoShutdown: Incorrect second in config option 'AutoShutdown.Time' - '{}'", configTime);
        isEnable = false;
    }

    auto nowTime = time(nullptr);
    uint64 nextResetTime = GetNextResetTime(nowTime, hour, minute, second);
    uint32 diffToShutdown = nextResetTime - static_cast<uint32>(nowTime);

    if (diffToShutdown < 10)
    {
        TC_LOG_WARN("server", "> AutoShutdown: Next time to shutdown < 10 seconds, Set next day");
        nextResetTime += DAY;
    }

    diffToShutdown = nextResetTime - static_cast<uint32>(nowTime);

    TC_LOG_INFO("server", " ");
    TC_LOG_INFO("server", "> AutoShutdown: System loading");

    scheduler.CancelAll();
    sWorld->ShutdownCancel();

    TC_LOG_INFO("server", "> AutoShutdown: Next time to shutdown - {}", TimeToHumanReadable(nextResetTime));
    TC_LOG_INFO("server", "> AutoShutdown: Remaining time to shutdown - {}", secsToTimeString(diffToShutdown));
    TC_LOG_INFO("server", " ");

    uint32 preAnnSeconds = sGameConfig->GetIntConfig("AutoShutdown.PreAnnounce.Seconds");
    if (preAnnSeconds > DAY)
    {
        TC_LOG_ERROR("server", "> AutoShutdown: Ahah, how could this happen? Time to preannouce more 1 day? ({}). Set to 1 hour (3600)", preAnnSeconds);
        preAnnSeconds = 3600;
    }

    uint32 timeToPreAnn = static_cast<uint32>(nextResetTime) - preAnnSeconds;
    uint32 diffToPreAnn = timeToPreAnn - static_cast<uint32>(nowTime);

    if (diffToShutdown < preAnnSeconds)
    {
        timeToPreAnn = static_cast<uint32>(nowTime) + 1;
        diffToPreAnn = 1;
        preAnnSeconds = diffToShutdown;
    }

    scheduler.Schedule(Seconds(diffToPreAnn), [preAnnSeconds](TaskContext /*context*/)
        {
            std::string preAnnMessForm = sGameConfig->GetStringConfig("AutoShutdown.PreAnnounce.Message");
            std::string mge = std::string((preAnnMessForm, secsToTimeString(preAnnSeconds)));

            TC_LOG_INFO("server", "> {}", mge);

            // sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str());
            sWorld->SendServerMessage(SERVER_MSG_STRING, preAnnMessForm);
            sWorld->ShutdownServ(preAnnSeconds, SHUTDOWN_MASK_RESTART, SHUTDOWN_EXIT_CODE);
        });
}

void AutoShutdown::OnUpdate(uint32 diff)
{
    if (!isEnable)
        return;

    scheduler.Update(diff);
}
