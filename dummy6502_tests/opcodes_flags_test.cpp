#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.Flags")
{
	SECTION("CLI")
	{
		auto machine = TestMachine({ 0x58 });
		machine.Tick();

		REQUIRE(!machine.cpu.GetInterruptDisable());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("SEI")
	{
		auto machine = TestMachine({ 0x78 });
		machine.Tick();

		REQUIRE(machine.cpu.GetInterruptDisable());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("CLC")
	{
		auto machine = TestMachine({ 0x18 });
		machine.Tick();

		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("SEC")
	{
		auto machine = TestMachine({ 0x38 });
		machine.Tick();

		REQUIRE(machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("CLD")
	{
		auto machine = TestMachine({ 0xD8 });
		machine.Tick();
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("SED")
	{
		auto machine = TestMachine({ 0xF8 });
		machine.Tick();
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("CLV")
	{
		auto machine = TestMachine({ 0xB8 });
		machine.cpu.SetOverflow(true);
		machine.Tick();

		REQUIRE(!machine.cpu.GetOverflow());
		REQUIRE(machine.cpu.ticks == 2);
	}
}