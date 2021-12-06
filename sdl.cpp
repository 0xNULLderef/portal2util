#include <sdl.hpp>

#include <memory.hpp>
#include <console.hpp>

DETOUR_NV(void, SDL_GL_SwapWindow, SDL_Window* window) {
	SDL_GL_SwapWindow_Original(window);
}

template <typename T = uintptr_t, typename U = void*> void SDL_Hook(void* pBase, T detour, U& original) {
	uintptr_t uiBase = reinterpret_cast<uintptr_t>(pBase);
	uintptr_t* uiRealAddress = reinterpret_cast<uintptr_t*>(uiBase + 5 + *reinterpret_cast<uintptr_t*>(uiBase + 6) + *reinterpret_cast<uintptr_t*>(uiBase + 12));
	original = *reinterpret_cast<U*>(uiRealAddress);
	*uiRealAddress = reinterpret_cast<uintptr_t>(detour);
}

template <typename T = void*> void SDL_Unhook(void* pBase, T& detour) {
	uintptr_t uiBase = reinterpret_cast<uintptr_t>(pBase);
	uintptr_t* uiRealAddress = reinterpret_cast<uintptr_t*>(uiBase + 5 + *reinterpret_cast<uintptr_t*>(uiBase + 6) + *reinterpret_cast<uintptr_t*>(uiBase + 12));
	*uiRealAddress = reinterpret_cast<uintptr_t>(detour);
}

SDL::SDL() { }

bool SDL::Init() {
	this->m_pSDL = Memory::GetModuleHandleByName(this->Name());
	if(this->m_pSDL) {
		SDL_Hook(Memory::GetSymbolAddress(this->m_pSDL, "SDL_GL_SwapWindow"), SDL_GL_SwapWindow_Hook, SDL_GL_SwapWindow_Original);
	}
	return this->m_pSDL;
}

void SDL::Shutdown() {
	if(this->m_pSDL) {
		SDL_Unhook(Memory::GetSymbolAddress(this->m_pSDL, "SDL_GL_SwapWindow"), SDL_GL_SwapWindow_Original);
	}
}

SDL* sdl;