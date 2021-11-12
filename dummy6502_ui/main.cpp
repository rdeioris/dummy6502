#include "SDL.h"
#include "imgui.h"
#include "imfilebrowser.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_sdlrenderer.h"
#include "DummyMachine.h"
#include "../dummy6502/Rom.h"
#include "../dummy6502/Ram.h"

#include "DummyVGA.h"
#include "DummyJoypad.h"
#include "RamEditor.h"
#include "RomDebugger.h"

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		return -1;
	}

	dummy6502::SharedPtrMemoryController memory_controller;
	DummyMachine machine(memory_controller);

	auto cartridge = std::vector<uint8_t>(0x8000, 0xEA);

	auto ram = std::make_shared<dummy6502::Ram>(0x800);
	auto rom = std::make_shared<dummy6502::Rom>(cartridge);
	auto joypad0 = std::make_shared<DummyJoypad>();

	memory_controller.Add(0x0000, 0x07FF, ram);
	memory_controller.Add(0x0800, 0x0FFF, ram);
	memory_controller.Add(0x1000, 0x17FF, ram);
	memory_controller.Add(0x1800, 0x1FFF, ram);

	memory_controller.Add(0x4000, 0x4000, joypad0);

	memory_controller.Add(0x8000, 0xFFFF, rom);

	machine.cpu.Reset();

	machine.tools.push_back(std::make_shared<DummyVGA>());
	machine.tools.push_back(std::make_shared<RamEditor>());
	machine.tools.push_back(joypad0);
	machine.tools.push_back(std::make_shared<RomDebugger>(memory_controller));

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Dummy6502", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1344, 756, window_flags);

	// Setup SDL_Renderer instance
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return false;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// imgui style
	ImGui::StyleColorsDark();

	// setup renderers
	ImGui_ImplSDL2_InitForSDLRenderer(window);
	ImGui_ImplSDLRenderer_Init(renderer);

	std::string cpu_exception_message;

	// Main loop
	bool done = false;
	bool cpu_running = false;
	bool nmi_on_vsync = false;
	bool open_pc_editor = false;
	char hex_buf[5] = {};

	int32_t hz = 1000000;

	int32_t frame_ticks = 0;
	int32_t current_frame_ticks = 0;

	for (std::shared_ptr<ITool> tool : machine.tools)
	{
		tool->Init(machine, window, renderer);
	}

	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				{
					done = true;
				}
			}

			for (std::shared_ptr<ITool> tool : machine.tools)
			{
				tool->Event(machine, event);
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		if (open_pc_editor)
		{
			ImGui::OpenPopup("Set PC");
		}

		if (ImGui::BeginPopupModal("Set PC", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("from $%04X to:", machine.cpu.pc);
			ImGui::SameLine();
			ImGui::InputText("", hex_buf, 5);
			ImGui::SameLine();
			if (ImGui::Button("Ok"))
			{
				uint16_t value = 0;
				int nibble0 = std::toupper(hex_buf[3]);
				int nibble1 = std::toupper(hex_buf[2]);
				int nibble2 = std::toupper(hex_buf[1]);
				int nibble3 = std::toupper(hex_buf[0]);

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

				if (nibble2 >= '0' && nibble2 <= '9')
				{
					value |= (nibble2 - '0') << 8;
				}
				else if (nibble2 >= 'A' && nibble2 <= 'F')
				{
					value |= (10 + (nibble2 - 'A')) << 8;
				}

				if (nibble3 >= '0' && nibble3 <= '9')
				{
					value |= (nibble3 - '0') << 12;
				}
				else if (nibble3 >= 'A' && nibble3 <= 'F')
				{
					value |= (10 + (nibble3 - 'A')) << 12;
				}

				machine.cpu.pc = value;

				ImGui::CloseCurrentPopup();
				open_pc_editor = false;
			}
			ImGui::EndPopup();
		}

		{

			ImGui::Begin("MOS6502", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::InputInt("Hz", &hz))
			{
				if (hz < static_cast<int32_t>(ImGui::GetIO().Framerate))
				{
					hz = static_cast<int32_t>(ImGui::GetIO().Framerate);
				}
			}

			ImGui::Checkbox("NMI on VSync", &nmi_on_vsync);

			SDL_DisplayMode mode;
			int display = SDL_GetWindowDisplayIndex(window);
			SDL_GetDesktopDisplayMode(display, &mode);
			float ddpi, hdpi, vdpi;
			SDL_GetDisplayDPI(display, &ddpi, &hdpi, &vdpi);
			ImGui::Text("Display: %d %dx%d (%dhz) DPI: %f %f %f", display, mode.w, mode.h, mode.refresh_rate, ddpi, hdpi, vdpi);

			if (cpu_running)
			{
				ImGui::AlignTextToFramePadding();
				if (ImGui::Button("Pause"))
				{
					cpu_running = false;
				}
				ImGui::SameLine();
				ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			else
			{
				if (ImGui::Button("Run"))
				{
					cpu_running = true;
				}
			}

			if (!machine.breakpoints.empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Clear Breakpoints"))
				{
					machine.breakpoints.clear();
				}
			}

			bool step_pressed = ImGui::Button("Step");
			if (step_pressed)
			{
				cpu_running = false;
			}
			if (step_pressed || cpu_running)
			{
				frame_ticks = hz / static_cast<int32_t>(ImGui::GetIO().Framerate);
				current_frame_ticks = frame_ticks + current_frame_ticks;
				do
				{
					try
					{
						uint64_t before_ticks = machine.cpu.ticks;
						machine.cpu.Step();
						uint64_t after_ticks = machine.cpu.ticks;
						current_frame_ticks -= static_cast<int32_t>(after_ticks - before_ticks);
						if (!machine.breakpoints.empty())
						{
							if (machine.breakpoints.contains(machine.cpu.pc))
							{
								cpu_running = false;
								break;
							}
						}
					}
					catch (const std::exception& e)
					{
						cpu_exception_message = e.what();
						cpu_running = false;
						break;
					}
				} while (cpu_running && current_frame_ticks > 0);
			}
			ImGui::SameLine();
			ImGui::Text("PC=$%04X Ticks: %llu", machine.cpu.pc, machine.cpu.ticks);
			if (ImGui::IsItemClicked())
			{
				open_pc_editor = true;
			}

			std::string flags = "";
			flags += machine.cpu.GetNegative() ? "N" : "-";
			flags += machine.cpu.GetOverflow() ? "V" : "-";
			flags += "---";
			flags += machine.cpu.GetInterruptDisable() ? "I" : "-";
			flags += machine.cpu.GetZero() ? "Z" : "-";
			flags += machine.cpu.GetCarry() ? "C" : "-";

			ImGui::Text("A=$%02X X=$%02X Y=$%02X S=$%02X F=$%02X (%s)", machine.cpu.a, machine.cpu.x, machine.cpu.y, machine.cpu.s, machine.cpu.flags, flags.c_str());

			ImGui::Checkbox("NMI", &machine.cpu.nmi);
			ImGui::SameLine();
			ImGui::Checkbox("IRQ", &machine.cpu.irq);
			ImGui::Text("Vectors: NMI($%04X) RESET($%04X) IRQ/BRK($%04X)", memory_controller.Read16(0xFFFA), memory_controller.Read16(0xFFFC), memory_controller.Read16(0xFFFE));
			if (ImGui::Button("Reset"))
			{
				machine.cpu.Reset();
			}

			ImGui::End();
		}

		if (!cpu_exception_message.empty())
		{
			ImGui::OpenPopup("CPU Exception");
		}

		if (ImGui::BeginPopupModal("CPU Exception", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(cpu_exception_message.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Ok"))
			{
				ImGui::CloseCurrentPopup();
				cpu_exception_message = "";
			}
			ImGui::EndPopup();
		}

		{

			ImGui::Begin("Stack");

			ImGui::Text("Top: $%02X", machine.cpu.s);

			if (ImGui::BeginTable("", 2, ImGuiTableFlags_Borders))
			{
				uint8_t stack_base = machine.cpu.s;
				stack_base++;
				for (uint16_t address = (0x0100 + stack_base); address < 0x0200; address++)
				{

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("$%02X", address - 0x0100);
					ImGui::TableNextColumn();
					ImGui::Text("%02X", memory_controller.Read8(address));
				}
				ImGui::EndTable();
			}

			ImGui::End();
		}

		// tick tools (will draw UI)
		for (std::shared_ptr<ITool> tool : machine.tools)
		{
			tool->Tick(machine);
		}

		// render
		ImGui::Render();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);

		if (cpu_running && nmi_on_vsync)
		{
			machine.cpu.Nmi();
		}
	}

	// Cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}