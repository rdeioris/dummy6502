#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.JMP")
{
	SECTION("JMP $1234")
	{
		auto machine = TestMachine({ 0x4C, 0x34, 0x12 });
		machine.Tick();

		REQUIRE(machine.cpu.pc == 0x1234);
		REQUIRE(machine.cpu.ticks == 3);
	}

	SECTION("JMP ($0400)")
	{
		auto machine = TestMachine({ 0x6C, 0x00, 0x04 });
		machine.memory_controller.Write8(0x0400, 0x78);
		machine.memory_controller.Write8(0x0401, 0x56);
		machine.Tick();

		REQUIRE(machine.cpu.pc == 0x5678);
		REQUIRE(machine.cpu.ticks == 5);
	}
}