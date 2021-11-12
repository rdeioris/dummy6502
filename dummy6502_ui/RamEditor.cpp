#include "RamEditor.h"
#include "DummyMachine.h"
#include "imgui.h"

void RamEditor::Init(DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer)
{
}

void RamEditor::Tick(DummyMachine& machine)
{
	if (open_ram_editor)
	{
		ImGui::OpenPopup("RAM Editor");
	}

	if (ImGui::BeginPopupModal("RAM Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("$%04X: ", ram_editor_address);
		ImGui::SameLine();
		ImGui::InputText("", hex_buf, 3);
		ImGui::SameLine();
		if (ImGui::Button("Ok"))
		{
			uint8_t value = 0;
			int nibble0 = std::toupper(hex_buf[1]);
			int nibble1 = std::toupper(hex_buf[0]);

			if (nibble0 >= '0' && nibble0 <= '9')
			{
				value = nibble0 - '0';
			}
			else if (nibble0 >= 'A' && nibble0 <= 'F')
			{
				value = 10 + (nibble0 - 'A');
			}

			if (nibble1 >= '0' && nibble1 <= '9')
			{
				value |= (nibble1 - '0') << 4;
			}
			else if (nibble1 >= 'A' && nibble1 <= 'F')
			{
				value |= (10 + (nibble1 - 'A')) << 4;
			}

			machine.cpu.memory_controller.Write8(ram_editor_address, value);

			ImGui::CloseCurrentPopup();
			open_ram_editor = false;
		}
		ImGui::EndPopup();
	}

	{

		ImGui::Begin("RAM");

		for (int page = 0; page < 8; page++)
		{
			uint16_t page_base = page * 256;
			std::string table_name = "Page $0" + std::to_string(page);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(table_name.c_str());
			ImGui::SameLine();
			if (ImGui::Button(("Clear Page###clear_" + table_name).c_str()))
			{
				for (uint16_t address = page_base; address < (page_base + 256); address++)
				{
					machine.cpu.memory_controller.Write8(address, 0);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button(("Shuffle Page###shuffle_" + table_name).c_str()))
			{
				for (uint16_t address = page_base; address < (page_base + 256); address++)
				{
					machine.cpu.memory_controller.Write8(address, std::rand() % 256);
				}
			}
			
			if (ImGui::BeginTable("RAM table", 0x10, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible))
			{
				for (uint16_t address = page_base; address < (page_base + 256); address++)
				{
					if (address % 0x10 == 0)
					{
						ImGui::TableNextRow();
					}
					ImGui::TableNextColumn();
					ImGui::Text("%02X", machine.cpu.memory_controller.Read8(address));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("$%04X", address);
					if (ImGui::IsItemClicked())
					{
						ram_editor_address = address;
						std::snprintf(hex_buf, 3, "%02X", machine.cpu.memory_controller.Read8(ram_editor_address));
						open_ram_editor = true;
					}
				}
				ImGui::EndTable();
			}

			ImGui::NewLine();
		}

		ImGui::End();
	}

}