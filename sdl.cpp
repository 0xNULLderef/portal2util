#include <sdl.hpp>

#include <memory.hpp>
#include <console.hpp>

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <assert.h>

DETOUR_NV(void, SDL_GL_SwapWindow, SDL_Window* window) {
	assert(false && "Hit SDL_GL_SwapWindow!!!");
	if(!sdl->m_bInitDone) {
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
		sdl->m_bInitDone = true;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);

	ImGui::NewFrame();

	if(ImGui::IsKeyPressed(SDL_SCANCODE_INSERT)) {
		sdl->m_bIsOpen = !sdl->m_bIsOpen;
	}

	if(sdl->m_bIsOpen) {
		ImGui::Begin("Window");

		ImGui::Text("lmao");

		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow_Original(window);
}

DETOUR_NV(int, SDL_PollEvent, SDL_Event* event) {
	auto result = SDL_PollEvent_Original(event);
	if(result && ImGui_ImplSDL2_ProcessEvent(event) && sdl->m_bIsOpen) event->type = 0;
	return result;
}

template <typename T = uintptr_t, typename U = void*> void SDL_Hook(void* pBase, T detour, U& original) {
	uintptr_t uiBase = reinterpret_cast<uintptr_t>(pBase);
	uintptr_t* uiRealAddress = reinterpret_cast<uintptr_t*>(uiBase + 9 + *reinterpret_cast<uintptr_t*>(uiBase + 10) + *reinterpret_cast<uintptr_t*>(uiBase + 16));
	original = *reinterpret_cast<U*>(uiRealAddress);
	*uiRealAddress = reinterpret_cast<uintptr_t>(detour);
}

template <typename T = void*> void SDL_Unhook(void* pBase, T& detour) {
	uintptr_t uiBase = reinterpret_cast<uintptr_t>(pBase);
	uintptr_t* uiRealAddress = reinterpret_cast<uintptr_t*>(uiBase + 9 + *reinterpret_cast<uintptr_t*>(uiBase + 10) + *reinterpret_cast<uintptr_t*>(uiBase + 16));
	*uiRealAddress = reinterpret_cast<uintptr_t>(detour);
}

SDL::SDL() { }

bool SDL::Init() {
	this->m_pSDL = Memory::GetModuleHandleByName(this->Name());
	if(this->m_pSDL) {
		SDL_Hook(Memory::GetSymbolAddress(this->m_pSDL, "SDL_GL_SwapWindow"), SDL_GL_SwapWindow_Hook, SDL_GL_SwapWindow_Original);
		SDL_Hook(Memory::GetSymbolAddress(this->m_pSDL, "SDL_PollEvent"), SDL_PollEvent_Hook, SDL_PollEvent_Original);
	}
	return this->m_pSDL;
}

void SDL::Shutdown() {
	if(this->m_pSDL) {
		SDL_Unhook(Memory::GetSymbolAddress(this->m_pSDL, "SDL_GL_SwapWindow"), SDL_GL_SwapWindow_Original);
		SDL_Unhook(Memory::GetSymbolAddress(this->m_pSDL, "SDL_PollEvent"), SDL_PollEvent_Original);
	}
}

SDL* sdl;
