#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.LDA")
{
	SECTION("LDA #$17")
	{
		auto machine = TestMachine({ 0xA9, 0x17 });
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x17);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("LDA #$17")
	{
		auto machine = TestMachine({ 0xA9, 0x81 });
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x81);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("LDA ($01), Y")
	{
		auto machine = TestMachine({ 0xB1, 0x01 });
		machine.cpu.y = 1;
		machine.memory_controller.Write8(0x01, 0x03);
		machine.memory_controller.Write8(0x02, 0x02);
		machine.memory_controller.Write8(0x0204, 0x30);
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x30);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("LDA ($01), Y cross boundary")
	{
		auto machine = TestMachine({ 0xB1, 0x01 });
		machine.cpu.y = 1;
		machine.memory_controller.Write8(0x01, 0xFF);
		machine.memory_controller.Write8(0x02, 0x02);
		machine.memory_controller.Write8(0x0300, 0x30);
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x30);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("LDA ($01, X)")
	{
		auto machine = TestMachine({ 0xA1, 0x01 });
		machine.cpu.x = 1;
		machine.memory_controller.Write8(0x02, 0x03);
		machine.memory_controller.Write8(0x03, 0x02);
		machine.memory_controller.Write8(0x0203, 0x30);
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x30);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 6);
	}

	SECTION("LDA $0201")
	{
		auto machine = TestMachine({ 0xAD, 0x01, 0x02 });
		machine.cpu.x = 1;
		machine.memory_controller.Write8(0x0201, 0x22);
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x22);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetNegative());
		REQUIRE(machine.cpu.ticks == 4);
	}
}