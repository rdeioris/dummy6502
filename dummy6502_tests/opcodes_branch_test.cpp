#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.BEQ")
{
	SECTION("LDA #0; BEQ -2")
	{
		auto machine = TestMachine({ 0xA9, 0x00, 0xF0, 0xFC });
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x00);
		REQUIRE(machine.cpu.GetZero());
		REQUIRE(machine.cpu.pc == 0x1000);
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("LDA #$10; BEQ -2")
	{
		auto machine = TestMachine({ 0xA9, 0x10, 0xF0, 0xFC });
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x10);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(machine.cpu.pc == 0x1004);
		REQUIRE(machine.cpu.ticks == 4);
	}
}

TEST_CASE("dummy6502.OpCodes.BMI")
{

	SECTION("LDA #$F0; BMI -2")
	{
		auto machine = TestMachine({ 0xA9, 0xF0, 0x30, 0xFC });
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0xF0);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(machine.cpu.GetNegative());
		REQUIRE(machine.cpu.pc == 0x1000);
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("LDA #$0F; BMI -2")
	{
		auto machine = TestMachine({ 0xA9, 0x0F, 0x30, 0xFC });
		machine.Tick();
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x0F);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.pc == 0x1004);
		REQUIRE(machine.cpu.ticks == 4);
	}
}