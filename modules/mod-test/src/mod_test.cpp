#include "Config.h"
#include "Log.h"
#include "ScriptMgr.h"

class ModuleTestWorldScript : public WorldScript
{
public:
	ModuleTestWorldScript() : WorldScript("ModuleTestWorldScript") { }

	void OnStartup() override
	{
		if (sConfigMgr->GetBoolDefault("ModTest.Enable", true))
			TC_LOG_INFO("server.loading", "{}", sConfigMgr->GetStringDefault("ModTest.Message", "Test module loaded successfully."));
	}
};

void AddSC_mod_test()
{
	new ModuleTestWorldScript();
}
