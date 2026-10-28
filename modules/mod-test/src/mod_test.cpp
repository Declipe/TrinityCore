#include "Log.h"
#include "ScriptMgr.h"

class ModuleTestWorldScript : public WorldScript
{
public:
	ModuleTestWorldScript() : WorldScript("ModuleTestWorldScript") { }

	void OnStartup() override
	{
		TC_LOG_INFO("server.loading", "Test module loaded successfully.");
	}
};

void AddSC_mod_test()
{
	new ModuleTestWorldScript();
}
