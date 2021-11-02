#pragma once

#include "../dummy6502/Cpu.h"
#include "../dummy6502/Ram.h"
#include "../dummy6502/Rom.h"

class TestMachine
{
public:
	TestMachine(const std::vector<uint8_t> opcodes) :
		cpu(memory_controller),
		ram(0x1000),
		rom(opcodes),
		rom_end({ 0x10, 0x01, 0x00, 0x10, 0x20, 0x10 })
	{
		memory_controller.Add(0, 0x0FFF, ram);
		memory_controller.Add(0x1000, 0x1FFF, rom);
		memory_controller.Add(0xFFFA, 0xFFFF, rom_end);

		cpu.Reset();
	}

	void Tick(uint32_t n = 1)
	{
		for (uint32_t i = 0; i < n; i++)
		{
			cpu.Step();
		}
	}

	dummy6502::RefMemoryController memory_controller;
	dummy6502::Cpu cpu;
	dummy6502::Ram ram;
	dummy6502::Rom rom;
	dummy6502::Rom rom_end;

};
