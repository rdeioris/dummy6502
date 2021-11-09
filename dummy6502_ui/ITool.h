#pragma once

#include "SDL.h"

class ITool
{
public:
	virtual void Init(class DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer) = 0;
	virtual void Event(class DummyMachine& machine, SDL_Event& event) {};
	virtual void Tick(class DummyMachine& machine) = 0;
};
