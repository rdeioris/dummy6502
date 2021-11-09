#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.STA")
{
	SECTION("STA $02")
	{
		auto machine = TestMachine({ 0x85, 0x02 });
		machine.cpu.a = 0x30;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x02) == 0x30);
		REQUIRE(machine.cpu.ticks == 3);
	}

	SECTION("STA $02, X")
	{
		auto machine = TestMachine({ 0x95, 0x02 });
		machine.cpu.a = 0x30;
		machine.cpu.x = 5;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x07) == 0x30);
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("STA $0402")
	{
		auto machine = TestMachine({ 0x8D, 0x02, 0x04 });
		machine.cpu.a = 0x17;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0402) == 0x17);
		REQUIRE(machine.cpu.ticks == 4);
	}
}
