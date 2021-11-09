#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.Regs")
{
	SECTION("TAX")
	{
		auto machine = TestMachine({ 0xAA });
		machine.cpu.a = 0x30;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x30);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("TXA")
	{
		auto machine = TestMachine({ 0x8A });
		machine.cpu.x = 0x17;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x17);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("DEX")
	{
		auto machine = TestMachine({ 0xCA });
		machine.Tick();

		REQUIRE(machine.cpu.x == 0xFF);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("INX")
	{
		auto machine = TestMachine({ 0xE8 });
		machine.Tick();

		REQUIRE(machine.cpu.x == 1);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("TAY")
	{
		auto machine = TestMachine({ 0xA8 });
		machine.cpu.a = 0x0C;
		machine.Tick();

		REQUIRE(machine.cpu.y == 0x0C);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("TYA")
	{
		auto machine = TestMachine({ 0x98 });
		machine.cpu.y = 0x0B;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x0B);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("DEY")
	{
		auto machine = TestMachine({ 0x88 });
		machine.Tick();

		REQUIRE(machine.cpu.y == 0xFF);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("INY")
	{
		auto machine = TestMachine({ 0xC8 });
		machine.Tick();

		REQUIRE(machine.cpu.y == 1);
		REQUIRE(machine.cpu.ticks == 2);
	}
}