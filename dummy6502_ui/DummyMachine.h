#pragma once

#include "../dummy6502/Cpu.h"
#include "ITool.h"
#include <set>

class DummyMachine
{
public:

	DummyMachine(dummy6502::IMemoryController& memory_controller) :
		cpu(memory_controller)
	{
	}

	dummy6502::Cpu cpu;

	std::vector<std::shared_ptr<ITool>> tools;
	std::set<uint16_t> breakpoints;
};
