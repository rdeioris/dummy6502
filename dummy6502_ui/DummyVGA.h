#pragma once

#include "ITool.h"
#include "SDL.h"
#include <array>
#include <cstdint>
#include <vector>

class DummyVGA : public ITool
{
public:
	virtual void Init(class DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer) override;
	virtual void Tick(class DummyMachine& machine) override;

protected:
	SDL_Texture* framebuffer;

	std::vector<std::array<uint8_t, 3>> vga_colors_table;
};