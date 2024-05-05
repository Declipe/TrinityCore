#include "AutoShutdown.h"
#include "Config.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "TaskScheduler.h"

class AutoShutdown_World : public WorldScript
{
public:
    AutoShutdown_World() : WorldScript("AutoShutdown_World") { }

    void OnStartup() override
    {
        Ark->Init();
    }

    void OnConfigLoad(bool reload) override
    {
        if (reload)
            Ark->Init();
    }

    void OnUpdate(uint32 diff) override
    {
        Ark->OnUpdate(diff);
    }
};

void AddSC_AutoShutdown()
{
    new AutoShutdown_World();
}
