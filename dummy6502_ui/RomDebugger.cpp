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

	file_dialog.SetTitle("Select ROM");
}

void RomDebugger::LoadRom(DummyMachine& machine)
{
	auto cartridge = std::vector<uint8_t>(0x8000, 0xEA);
	SDL_RWops* ops = SDL_RWFromFile(rom_image.string().c_str(), "r+b");
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

void RomDebugger::Tick(DummyMachine& machine)
{
	file_dialog.Display();

	if (file_dialog.HasSelected())
	{
		rom_image = file_dialog.GetSelected();
		LoadRom(machine);
	}

	{

		ImGui::Begin("ROM");
		if (ImGui::Button("Load ROM"))
		{
			file_dialog.Open();
		}

		if (!rom_image.empty())
		{
			ImGui::SameLine();
			if (ImGui::Button("Reload ROM"))
			{
				LoadRom(machine);
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s", rom_image.string().c_str());
			}
		}

		if (ImGui::BeginTable("ROM table", 0x11, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible))
		{
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
		}

		ImGui::End();
	}

	{

		ImGui::Begin("Debugger");

		bool seek_to_pc = false;

		if (ImGui::Button("Seek to PC"))
		{
			seek_to_pc = true;
		}

		ImGui::BeginChild("Opcodes");

		if (seek_to_pc)
		{
			for (size_t row = 0; row < disassembly.size(); row++)
			{
				if (disassembly[row].first >= machine.cpu.pc)
				{
					ImGui::SetScrollY(row * (ImGui::GetTextLineHeight() + 4));
					break;
				}
			}
		}

		if (ImGui::BeginTable("Debugger table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible))
		{
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
					bool is_breakpoint = false;
					if (machine.breakpoints.contains(disassembly[row].first))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 255, 255));
						is_breakpoint = true;
					}
					ImGui::Text("$%04X", disassembly[row].first);
					if (is_breakpoint)
					{
						ImGui::PopStyleColor();
					}
					if (ImGui::IsItemClicked())
					{
						if (machine.breakpoints.contains(disassembly[row].first))
						{
							machine.breakpoints.erase(disassembly[row].first);
						}
						else
						{
							machine.breakpoints.insert(disassembly[row].first);
						}
					}
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
		}

		ImGui::EndChild();

		ImGui::End();
	}

}