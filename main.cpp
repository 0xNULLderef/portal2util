#include <main.hpp>

#include <sdk.hpp>
#include <portal2.hpp>

// --- FEATURE INCLUDES ---
#include <variable.hpp>
#include <console.hpp>
#include <tier1.hpp>
#include <server.hpp>
#include <engine.hpp>
#include <sdl.hpp>
#include <matchmaking.hpp>
#include <vscript.hpp>
#include <command.hpp>

Plugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Plugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin);

Plugin::Plugin() {
	Portal2* portal2 = new Portal2();
	(void)portal2; // Janky ass hack so i don't need to set -Wno-unused-variable
}

CON_COMMAND(uncheat_cvar, "Forces cvar value") {
	if(args[1] && *args[1]) {
		Variable var(args[1]);
		var.Unlock();
		var.RemoveFlag(FCVAR_CHEAT);
	}
}

#define MAKE_3_BYTES_FROM_1_AND_2( x1, x2 ) (( (( uint16_t )x2) << 8 ) | (uint8_t)(x1))

CON_COMMAND(pain, "aaaaaaaaaaaaaaaaa") {
	KeyValues* pKv = new KeyValues("menu");
	// unsigned char color[4] = { 255, 255, 0, 255 };
	// pKv->SetColor("color", color);
	pKv->SetString("title", "weed");
	pKv->SetInt("level", 1);
	pKv->SetInt("time", 10);
	// char buf[4];
	// for(int i = 0; i <= 10; i++) {
		// snprintf(buf, sizeof(buf), "%d", i);
		// KeyValues* ki = pKv->FindKey(buf, true);
		// ki->SetString("command", "say test");
		// ki->SetString("msg", buf);
	// }
	engine->CreateMessage(engine->g_pServerPluginHandler->ThisPtr(), server->GetPlayerEdict(0), DIALOG_MSG, pKv, &plugin);
}

bool Plugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	console = new Console();
	if(!console->Init()) return false;

	console->Print("loading...\n");

	tier1 = new Tier1();
	if(!tier1->Init()) return false;

	server = new Server();
	if(!server->Init()) return false;

	engine = new Engine();
	if(!engine->Init()) return false;

	// sdl = new SDL();
	// if(!sdl->Init()) return false;

	matchmaking = new Matchmaking();
	if(!matchmaking->Init()) return false;

	vscript = new VScript();
	if(!vscript->Init()) return false;

	Command::RegisterAll();

	console->Print("loaded successfully!\n");

	return true;
}

void Plugin::Unload() {
	console->Print("Gracefully returning the game to it's original state.\n");
	console->Shutdown();
	vscript->Shutdown();
	matchmaking->Shutdown();
	// sdl->Shutdown();
	engine->Shutdown();
	server->Shutdown();
	Command::UnregisterAll();
	tier1->Shutdown(); // Do this one later so that it doesn't try to unregister without tier1 loaded...
}

const char* Plugin::GetPluginDescription() {
	return "portal2util (indev)";
}

// Unused callbacks
void Plugin::Pause() {}
void Plugin::UnPause() {}
void Plugin::LevelInit(char const* pMapName) {}
void Plugin::ServerActivate(void* pEdictList, int edictCount, int clientMax) {}
void Plugin::GameFrame(bool simulating) {}
void Plugin::LevelShutdown() {}
void Plugin::ClientFullyConnect(void* pEdict) {}
void Plugin::ClientActive(void* pEntity) {}
void Plugin::ClientDisconnect(void* pEntity) {}
void Plugin::ClientPutInServer(void* pEntity, char const* playername) {}
void Plugin::SetCommandClient(int index) {}
void Plugin::ClientSettingsChanged(void* pEdict) {}
int Plugin::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) { return 0; }
int Plugin::ClientCommand(void* pEntity, const void*& args) { return 0; }
int Plugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return 0; }
void Plugin::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) {}
void Plugin::OnEdictAllocated(void* edict) {}
void Plugin::OnEdictFreed(const void* edict) {}
