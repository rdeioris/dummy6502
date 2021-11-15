#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.Stack")
{
	SECTION("TXS")
	{
		auto machine = TestMachine({ 0x9A });
		machine.cpu.x = 0xAB;
		machine.Tick();

		REQUIRE(machine.cpu.s == 0xAB);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("TSX")
	{
		auto machine = TestMachine({ 0xBA });
		machine.cpu.s = 0xCD;
		machine.Tick();

		REQUIRE(machine.cpu.x == 0xCD);
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("PHA")
	{
		auto machine = TestMachine({ 0x48 });
		machine.cpu.a = 0x22;
		machine.cpu.s = 0xFF;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x1FF) == 0x22);
		REQUIRE(machine.cpu.s == 0xFE);
		REQUIRE(machine.cpu.ticks == 3);
	}

	SECTION("PLA")
	{
		auto machine = TestMachine({ 0x68 });
		machine.cpu.s = 0xFF;
		machine.cpu.Push(0x30);
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x30);
		REQUIRE(machine.cpu.s == 0xFF);
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("PHP")
	{
		auto machine = TestMachine({ 0x08 });
		machine.cpu.flags = 0x22;
		machine.cpu.s = 0xFF;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x1FF) == 0x22);
		REQUIRE(machine.cpu.s == 0xFE);
		REQUIRE(machine.cpu.ticks == 3);
	}

	SECTION("PLP")
	{
		auto machine = TestMachine({ 0x28 });
		machine.cpu.s = 0xFF;
		machine.cpu.Push(0x11);
		machine.Tick();

		REQUIRE(machine.cpu.flags == 0x11);
		REQUIRE(machine.cpu.s == 0xFF);
		REQUIRE(machine.cpu.ticks == 4);
	}
}