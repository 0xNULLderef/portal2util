#include <matchmaking.hpp>

#include <console.hpp>

Matchmaking::Matchmaking() { }

bool Matchmaking::Init() {
	this->g_MatchFramework = Interface::Create(this->Name(), "MATCHFRAMEWORK_001");
	this->GetMatchTitleGameSettingsMgr = this->g_MatchFramework->Original<_GetMatchTitleGameSettingsMgr>(20);

	// console->DumpHex((void*)GetMatchTitleGameSettingsMgr, 0x40);

	return this->g_MatchFramework && this->GetMatchTitleGameSettingsMgr;
}

void Matchmaking::Shutdown() {
	Interface::Delete(this->g_MatchFramework);
}

Matchmaking* matchmaking;