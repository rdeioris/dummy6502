#include "RomDebugger.h"
#include "DummyMachine.h"
#include "../dummy6502/Rom.h"

void RomDebugger::Init(DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer)
{
	uint16_t disassembly_base = 0x8000;
	while (disassembly_base <= 0xFFF7)
	{
		uint16_t current_line = disassembly_base;
		std::string line = machine.cpu.GetDisassembly(disassembly_base);
		disassembly.push_back({ current_line, line });
	}

}

void RomDebugger::Tick(DummyMachine& machine)
{
	file_dialog.Display();

	if (file_dialog.HasSelected())
	{
		auto cartridge = std::vector<uint8_t>(0x8000, 0xEA);
		SDL_RWops* ops = SDL_RWFromFile(file_dialog.GetSelected().string().c_str(), "r+b");
		SDL_RWread(ops, cartridge.data(), 1, 0x8000);
		SDL_RWclose(ops);
		memory_controller.Remap(0x8000, 0xFFFF, std::make_shared<dummy6502::Rom>(cartridge));
		file_dialog.ClearSelected();
		disassembly.clear();
		uint16_t disassembly_base = 0x8000;
		while (disassembly_base <= 0xFFF7)
		{
			uint16_t current_line = disassembly_base;
			std::string line = machine.cpu.GetDisassembly(disassembly_base);
			disassembly.push_back({ current_line, line });
		}
	}

	{

		ImGui::Begin("ROM");
		if (ImGui::Button("Load ROM"))
		{
			file_dialog.Open();
		}

		ImGui::BeginTable("", 0x11, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible);
		ImGuiListClipper clipper;
		clipper.Begin(32768 / 16);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				uint16_t address = row * 16;
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("$%04X", (0x8000 + address));
				for (uint16_t cell = address; cell < address + 16; cell++)
				{
					ImGui::TableNextColumn();
					if (cell + 0x8000 == machine.cpu.pc)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
					}
					ImGui::Text("%02X", machine.cpu.memory_controller.Read8(0x8000 + cell));
					if (cell + 0x8000 == machine.cpu.pc)
					{
						ImGui::PopStyleColor();
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("$%04X", (0x8000 + cell));
				}
			}
		}
		ImGui::EndTable();

		ImGui::End();
	}

	{

		ImGui::Begin("Debugger");

		ImGui::BeginTable("", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible);
		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(disassembly.size()));
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();
				if (disassembly[row].first == machine.cpu.pc)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
				}
				ImGui::TableNextColumn();
				ImGui::Text("$%04X", disassembly[row].first);
				ImGui::TableNextColumn();
				uint16_t end = 0xFFFF;
				if (row < disassembly.size() - 1)
				{
					end = disassembly[row + 1].first;
				}
				for (uint16_t cell = disassembly[row].first; cell < end; cell++)
				{
					ImGui::Text("$%02X", memory_controller.Read8(cell));
					ImGui::SameLine();
				}
				ImGui::TableNextColumn();
				ImGui::Text(disassembly[row].second.c_str());
				if (disassembly[row].first == machine.cpu.pc)
				{
					ImGui::PopStyleColor();
				}
			}
		}
		ImGui::EndTable();

		ImGui::End();
	}

}