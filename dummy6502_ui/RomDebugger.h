#pragma once

#include "ITool.h"
#include "../dummy6502/MemoryController.h"
#include <cstdint>
#include <string>
#include <vector>
#include "imgui.h"
#include "imfilebrowser.h"

class RomDebugger : public ITool
{
public:
	RomDebugger(dummy6502::SharedPtrMemoryController& in_memory_controller) : memory_controller(in_memory_controller)
	{

	}

	virtual void Init(class DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer) override;
	virtual void Tick(class DummyMachine& machine) override;
protected:
	void LoadRom(DummyMachine& machine);

	ImGui::FileBrowser file_dialog;
	std::vector<std::pair<uint16_t, std::string>> disassembly;

	dummy6502::SharedPtrMemoryController& memory_controller;

	std::filesystem::path rom_image;
};
