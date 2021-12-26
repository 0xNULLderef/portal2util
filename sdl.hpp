#ifndef SDL_HPP
#define SDL_HPP

#include <utils.hpp>
#include <SDL2/SDL.h>

class SDL {
public:
	void* m_pSDL = nullptr;
	bool m_bInitDone = false;
	bool m_bIsOpen = false;

public:
	SDL();
	bool Init();
	void Shutdown();
	const char* Name() { return "libSDL2-2.0.so.0"; }
};

extern SDL* sdl;

#endif // SDL_HPP