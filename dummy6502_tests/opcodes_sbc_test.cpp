#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.SBC")
{
	SECTION("SEC; SBC Immediate 0x44")
	{
		auto machine = TestMachine({ 0x38, 0xE9, 0x44 });
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0xBC);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(!machine.cpu.GetOverflow());
		REQUIRE(machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("SEC; SBC Immediate 0x0")
	{
		auto machine = TestMachine({ 0x38, 0xE9, 0x00 });
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x00);
		REQUIRE(machine.cpu.GetZero());
		REQUIRE(machine.cpu.GetCarry());
		REQUIRE(!machine.cpu.GetOverflow());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("SEC; SBC Immediate 0x0 (A=1)")
	{
		auto machine = TestMachine({ 0x38, 0xE9, 0x00 });
		machine.cpu.a = 1;
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x1);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(machine.cpu.GetCarry());
		REQUIRE(!machine.cpu.GetOverflow());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("SEC; SBC Immediate 0xFD (Overflow)")
	{
		auto machine = TestMachine({ 0x38, 0xE9, 0xFD });
		machine.cpu.a = 0x7F;
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x82);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.GetOverflow());
		REQUIRE(machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 4);
	}
}
