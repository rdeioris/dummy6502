#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.DEC")
{
	SECTION("DEC 0x00")
	{
		auto machine = TestMachine({ 0xC6, 0x00 });
		machine.memory_controller.Write8(0, 3);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x00) == 2);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("DEC 0x01, X")
	{
		auto machine = TestMachine({ 0xD6, 0x01 });
		machine.cpu.x = 1;
		machine.memory_controller.Write8(2, 0x0A);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x02) == 0x09);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("DEC 0x0203")
	{
		auto machine = TestMachine({ 0xCE, 0x03, 0x02 });
		machine.memory_controller.Write8(0x0203, 0x0B);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0203) == 0x0A);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("DEC 0x0203, X")
	{
		auto machine = TestMachine({ 0xDE, 0x03, 0x02 });
		machine.cpu.x = 2;
		machine.memory_controller.Write8(0x0205, 0x0C);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0205) == 0x0B);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 7);
	}
}

TEST_CASE("dummy6502.OpCodes.INC")
{
	SECTION("INC 0x0203, X")
	{
		auto machine = TestMachine({ 0xFE, 0x03, 0x02 });
		machine.cpu.x = 2;
		machine.memory_controller.Write8(0x0205, 0x0C);
		machine.Tick();

		REQUIRE(machine.memory_controller.Read8(0x0205) == 0x0D);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 7);
	}
}