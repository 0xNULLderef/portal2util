#include <engine.hpp>

#include <interface.hpp>
#include <offsets.hpp>

Engine::Engine() {}

#include <console.hpp>

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		console->Print("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			console->Print(" ");
			if ((i+1) % 16 == 0) {
				console->Print("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					console->Print(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					console->Print("   ");
				}
				console->Print("|  %s \n", ascii);
			}
		}
	}
}

bool Engine::Init() {
	if(auto g_VEngineServer = Interface::Create(this->Name(), "VEngineServer022", false)) {
		this->GetPlayerNetworkIDString = g_VEngineServer->Original<_GetPlayerNetworkIDString>(Offsets::GetPlayerNetworkIDString);
		Interface::Delete(g_VEngineServer);
	}
	if(auto g_VEngineClient = Interface::Create(this->Name(), "VEngineClient015", false)) {
		auto test = g_VEngineClient->Original(158);
		void* sv = *(void**)(test + 6 + *(uintptr_t*)(test + 8) + *(uintptr_t*)(test + 21));
		console->Print("%x", sv);
		auto isv = Interface::Create(sv, false);
		auto temp = isv->Original<void*>(0);
		DumpHex(temp, 0x40);
		Interface::Delete(g_VEngineClient);
	}
	return this->GetPlayerNetworkIDString;
}

void Engine::Shutdown() {
	this->GetPlayerNetworkIDString = nullptr;
}

Engine* engine;