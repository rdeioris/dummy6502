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

	SECTION("STA $0402, X")
	{
		auto machine = TestMachine({ 0x9D, 0x02, 0x04 });
		machine.cpu.a = 0x17;
		machine.cpu.x = 1;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0403) == 0x17);
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("STA $0402, Y")
	{
		auto machine = TestMachine({ 0x99, 0x02, 0x04 });
		machine.cpu.a = 0x17;
		machine.cpu.y = 1;
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0403) == 0x17);
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("STA ($00, X)")
	{
		auto machine = TestMachine({ 0x81, 0x00 });
		machine.cpu.a = 0x17;
		machine.cpu.x = 1;
		machine.memory_controller.Write8(1, 0x03);
		machine.memory_controller.Write8(2, 0x04);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0403) == 0x17);
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("STA ($00), Y")
	{
		auto machine = TestMachine({ 0x91, 0x00 });
		machine.cpu.a = 0x17;
		machine.cpu.y = 1;
		machine.memory_controller.Write8(0, 0x03);
		machine.memory_controller.Write8(1, 0x04);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0404) == 0x17);
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("STA ($00), Y cross boundary")
	{
		auto machine = TestMachine({ 0x91, 0x00 });
		machine.cpu.a = 0x17;
		machine.cpu.y = 1;
		machine.memory_controller.Write8(0, 0xFF);
		machine.memory_controller.Write8(1, 0x04);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0500) == 0x17);
		REQUIRE(machine.cpu.ticks == 6);
	}
}
