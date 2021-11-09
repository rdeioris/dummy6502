#include "DummyVGA.h"
#include "DummyMachine.h"
#include "imgui.h"

void DummyVGA::Init(DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer)
{
	vga_colors_table = {
		{ 0, 0, 0},
		{ 170, 0, 0},
		{ 0, 170, 0},
		{ 170, 85, 0},
		{ 0, 0, 170},
		{ 170, 0, 170},
		{ 0, 170, 170},
		{ 170, 170, 170},
		{ 85, 85, 85},
		{ 255, 85, 85},
		{ 85, 255, 85},
		{ 255, 255, 85},
		{ 85, 85, 255},
		{ 255, 85, 255},
		{ 85, 255, 255},
		{ 255, 255, 255},
	};

	framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 16, 16);
}

void DummyVGA::Tick(DummyMachine& machine)
{
	ImGui::Begin("CRT", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Image(framebuffer, ImVec2(256, 256));
	ImGui::End();

	uint8_t* pixels = nullptr;
	int pitch = 0;
	SDL_LockTexture(framebuffer, nullptr, (void**)&pixels, &pitch);
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			uint8_t color_index = machine.cpu.memory_controller.Read8(0x200 + (y * 16 + x)) % 16;
			pixels[y * pitch + (x * 4)] = vga_colors_table[color_index][0];
			pixels[y * pitch + (x * 4) + 1] = vga_colors_table[color_index][1];
			pixels[y * pitch + (x * 4) + 2] = vga_colors_table[color_index][2];
			pixels[y * pitch + (x * 4) + 3] = 255;
		}
	}
	SDL_UnlockTexture(framebuffer);

}