#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.JSR")
{
	SECTION("JSR $1234")
	{
		auto machine = TestMachine({ 0x20, 0x34, 0x12 });
		machine.Tick();

		REQUIRE(machine.cpu.pc == 0x1234);
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("JSR $1234; NOP; RTS")
	{
		auto machine = TestMachine({ 0x20, 0x03, 0x10, 0xEA, 0x60 });
		machine.Tick(3);

		REQUIRE(machine.cpu.pc == 0x1003);
		REQUIRE(machine.cpu.ticks == 14);
	}
}