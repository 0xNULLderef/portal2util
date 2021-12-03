#ifndef MATCHMAKING_HPP
#define MATCHMAKING_HPP

#include <interface.hpp>
#include <utils.hpp>

class Matchmaking {
public:
	Interface* g_MatchFramework = nullptr;

	using _GetMatchTitleGameSettingsMgr = IMatchTitleGameSettingsMgr*(__cdecl*)();
	_GetMatchTitleGameSettingsMgr GetMatchTitleGameSettingsMgr = nullptr;

public:
	Matchmaking();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("matchmaking"); }

	// DECL_DETOUR_T();
};

extern Matchmaking* matchmaking;

#endif // MATCHMAKING_HPP
