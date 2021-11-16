#include "DummyJoypad.h"
#include "DummyMachine.h"
#include "SDL.h"
#include "imgui.h"

void DummyJoypad::Init(DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer)
{

}

void DummyJoypad::Event(DummyMachine& machine, SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_UP)
		{
			SetBit(0);
		}
		else if (event.key.keysym.sym == SDLK_DOWN)
		{
			SetBit(1);
		}
		else if (event.key.keysym.sym == SDLK_LEFT)
		{
			SetBit(2);
		}
		else if (event.key.keysym.sym == SDLK_RIGHT)
		{
			SetBit(3);
		}
		else if (event.key.keysym.sym == SDLK_RETURN)
		{
			SetBit(4);
		}
		else if (event.key.keysym.sym == SDLK_SPACE)
		{
			SetBit(5);
		}
	}
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_UP)
		{
			ClearBit(0);
		}
		else if (event.key.keysym.sym == SDLK_DOWN)
		{
			ClearBit(1);
		}
		else if (event.key.keysym.sym == SDLK_LEFT)
		{
			ClearBit(2);
		}
		else if (event.key.keysym.sym == SDLK_RIGHT)
		{
			ClearBit(3);
		}
		else if (event.key.keysym.sym == SDLK_RETURN)
		{
			ClearBit(4);
		}
		else if (event.key.keysym.sym == SDLK_SPACE)
		{
			ClearBit(5);
		}
	}
}

void DummyJoypad::Tick(DummyMachine& machine)
{
	auto& memory_controller = machine.cpu.memory_controller;

	ImGui::Begin("Joypad0", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Buttons: $%02X", memory_controller.Read8(0x4000));
	ImGui::End();
}