#include <engine.hpp>

#include <interface.hpp>
#include <offsets.hpp>

Engine::Engine() {}

bool Engine::Init() {
	if(auto g_VEngineServer = Interface::Create(this->Name(), "VEngineServer022", false)) {
		this->GetPlayerNetworkIDString = g_VEngineServer->Original<_GetPlayerNetworkIDString>(Offsets::GetPlayerNetworkIDString);
		Interface::Delete(g_VEngineServer);
	}
	if(auto g_VEngineClient = Interface::Create(this->Name(), "VEngineClient015", false)) {
		auto test = g_VEngineClient->Original(158);
		void* sv = *(void**)(test + 6 + *(uintptr_t*)(test + 8) + *(uintptr_t*)(test + 21));
		// console->Print("%x\n", sv);
		auto isv = Interface::Create(sv, false);
		auto temp = isv->Original<void*>(0);
		(void)temp;
		// DumpHex(temp, 0x40);
		Interface::Delete(g_VEngineClient);
	}
	return this->GetPlayerNetworkIDString;
}

void Engine::Shutdown() {
	this->GetPlayerNetworkIDString = nullptr;
}

Engine* engine;