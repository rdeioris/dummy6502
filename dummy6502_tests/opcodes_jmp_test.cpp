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
}