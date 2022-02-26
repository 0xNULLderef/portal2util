#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <sdk.hpp>
#include <utils.hpp>
#include <tier1.hpp>

typedef enum {
	DIALOG_MSG = 0,
	DIALOG_MENU,
	DIALOG_TEXT,
	DIALOG_ENTRY,
	DIALOG_ASKCONNECT
} DIALOG_TYPE;

class Engine {
public:
	using _GetPlayerNetworkIDString = const char*(*)(void* thisptr, const edict_t* pPlayerEdict);
	_GetPlayerNetworkIDString GetPlayerNetworkIDString = nullptr;
	Interface* g_pServerPluginHandler = nullptr;
	using _CreateMessage = void(__cdecl*)(void* thisptr, edict_t* pPlayerEdict, DIALOG_TYPE type, KeyValues *data, IServerPluginCallbacks *plugin);
	_CreateMessage CreateMessage = nullptr;
public:
	Engine();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("engine"); }
};

extern Engine* engine;

#endif // ENGINE_HPP
