#include "Config.h"
#include "Log.h"
#include "Opcodes.h"
#include "ScriptMgr.h"
#include "WardenWin.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace
{
struct BreakingNewsData
{
	bool Enabled = false;
	std::string Title;
	std::string Body;
	std::string Formatted;
};

BreakingNewsData News;

std::string EscapeLuaString(std::string const& value)
{
	std::string escaped;
	escaped.reserve(value.size());

	for (unsigned char character : value)
	{
		if (character == '\\' || character == '\'')
		{
			escaped.push_back('\\');
			escaped.push_back(static_cast<char>(character));
		}
		else if (character < 0x20 || character == 0x7F)
		{
			escaped.push_back('\\');
			escaped.push_back(static_cast<char>('0' + character / 100));
			escaped.push_back(static_cast<char>('0' + (character / 10) % 10));
			escaped.push_back(static_cast<char>('0' + character % 10));
		}
		else
			escaped.push_back(static_cast<char>(character));
	}

	return escaped;
}

bool LoadBreakingNews()
{
	std::string const path = sConfigMgr->GetStringDefault("BreakingNews.HtmlPath", "./breakingnews.html");
	News.Title = sConfigMgr->GetStringDefault("BreakingNews.Title", "Breaking News");

	if (path.empty())
	{
		TC_LOG_ERROR("module", "BreakingNews.HtmlPath cannot be empty.");
		return false;
	}

	std::ifstream file(path);
	if (!file.is_open())
	{
		TC_LOG_ERROR("module", "Failed to open Breaking News HTML file '{}'.", path);
		return false;
	}

	News.Body.clear();
	std::string line;
	while (std::getline(file, line))
	{
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		News.Body += line;
	}

	if (file.bad())
	{
		TC_LOG_ERROR("module", "Failed while reading Breaking News HTML file '{}'.", path);
		return false;
	}

	News.Formatted = "local a=ServerAlertFrame;local b=ServerAlertText;local c=ServerAlertTitle;local d=CharacterSelect;"
		"if a and b and c and d then a:SetParent(d);c:SetText('" + EscapeLuaString(News.Title) + "');b:SetText('" + EscapeLuaString(News.Body) + "');a:Show()"
		"else message('ServerAlert frame or CharacterSelect frame is unavailable.') end";
	return true;
}

std::vector<std::string> MakeChunkPayloads(std::string const& code)
{
	constexpr size_t MaxPayloadLength = WARDEN_MAX_LUA_CHECK_LENGTH;
	constexpr size_t PreferredChunkLength = 128;
	constexpr char PayloadPrefix[] = "wlbuf = wlbuf .. ";
	std::vector<std::string> payloads;

	for (size_t offset = 0; offset < code.size();)
	{
		size_t chunkLength = std::min(PreferredChunkLength, code.size() - offset);
		bool chunkAdded = false;

		while (!chunkAdded)
		{
			std::string chunk = code.substr(offset, chunkLength);
			size_t delimiterDepth = 0;
			std::string equals;
			std::string closingDelimiter;
			do
			{
				equals.assign(delimiterDepth, '=');
				closingDelimiter = "]" + equals + "]";
				if (chunk.find(closingDelimiter) == std::string::npos)
					break;
				++delimiterDepth;
			} while (true);

			std::string payload = std::string(PayloadPrefix) + "[" + equals + "[" + chunk + "]" + equals + "];";
			if (payload.size() <= MaxPayloadLength)
			{
				payloads.push_back(std::move(payload));
				offset += chunkLength;
				chunkAdded = true;
			}
			else if (chunkLength > 1)
				chunkLength /= 2;
			else
				return {};
		}
	}

	return payloads;
}

class BreakingNewsWorldScript : public WorldScript
{
public:
	BreakingNewsWorldScript() : WorldScript("BreakingNewsWorldScript") { }

	void OnConfigLoad(bool /*reload*/) override
	{
		News.Enabled = sConfigMgr->GetBoolDefault("BreakingNews.Enable", false);
		if (News.Enabled && LoadBreakingNews())
			TC_LOG_INFO("module", "Breaking News enabled: loaded {} bytes from '{}'.", News.Body.size(), sConfigMgr->GetStringDefault("BreakingNews.HtmlPath", "./breakingnews.html"));
	}
};

class BreakingNewsServerScript : public ServerScript
{
public:
	BreakingNewsServerScript() : ServerScript("BreakingNewsServerScript") { }

	void OnPacketSend(WorldSession* session, WorldPacket& packet) override
	{
		if (!News.Enabled || packet.GetOpcode() != SMSG_CHAR_ENUM || !session)
			return;

		WardenWin* warden = dynamic_cast<WardenWin*>(session->GetWarden());
		if (!warden)
		{
			TC_LOG_WARN("module", "Breaking News requires Warden.Enabled = 1 and a Windows client.");
			return;
		}

		if (!sConfigMgr->GetBoolDefault("BreakingNews.Cache", true) && !LoadBreakingNews())
			return;

		if (News.Formatted.empty())
			return;

		std::vector<std::string> payloads = MakeChunkPayloads(News.Formatted);
		if (payloads.empty())
		{
			TC_LOG_ERROR("module", "Could not split Breaking News content into Warden payloads.");
			return;
		}

		warden->ClearQueuedLuaPayloads();
		if (!warden->QueueLuaPayload("wlbuf = '';"))
		{
			TC_LOG_ERROR("module", "Could not queue the Breaking News initialization payload.");
			return;
		}

		for (std::string const& payload : payloads)
		{
			if (warden->QueueLuaPayload(payload))
				continue;

			warden->ClearQueuedLuaPayloads();
			TC_LOG_ERROR("module", "Could not queue a Breaking News content payload.");
			return;
		}

		if (!warden->QueueLuaPayload("local a,b=loadstring(wlbuf)if not a then message(b)else a()end"))
		{
			warden->ClearQueuedLuaPayloads();
			TC_LOG_ERROR("module", "Could not queue the Breaking News execution payload.");
			return;
		}

		if (sConfigMgr->GetBoolDefault("BreakingNews.Verbose", false))
			TC_LOG_INFO("module", "Queued {} Breaking News Lua payloads for account {}.", payloads.size() + 2, session->GetAccountId());

		warden->ForceChecks();
	}
};
}

void AddBreakingNewsScripts()
{
	new BreakingNewsWorldScript();
	new BreakingNewsServerScript();
}
