#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.OpCodes.ADC")
{
	SECTION("ADC Immediate 0x44")
	{
		auto machine = TestMachine({ 0x69, 0x44 });
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x44);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("ADC Immediate 0x00")
	{
		auto machine = TestMachine({ 0x69, 0x00 });
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x00);
		REQUIRE(machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 2);
	}

	SECTION("ADC Immediate Carry")
	{
		auto machine = TestMachine({ 0x69, 0x01 });
		machine.cpu.a = 0xff;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x00);
		REQUIRE(machine.cpu.GetZero());
		REQUIRE(machine.cpu.GetCarry());
		REQUIRE(!machine.cpu.GetNegative());
	}

	SECTION("ADC Immediate + ADC Immediate")
	{
		auto machine = TestMachine({ 0x69, 0x01, 0x69, 0x01 });
		machine.cpu.a = 0x1;
		machine.Tick(2);

		REQUIRE(machine.cpu.a == 0x03);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("ADC ZeroPage")
	{
		auto machine = TestMachine({ 0x65, 0x00});
		machine.ram.Write8(0, 1);
		machine.cpu.a = 0x1;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x02);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 3);
	}

	SECTION("ADC ZeroPageX")
	{
		auto machine = TestMachine({ 0x75, 0x00 });
		machine.ram.Write8(2, 1);
		machine.cpu.a = 0x1;
		machine.cpu.x = 2;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x02);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("ADC ZeroPageX + Carry")
	{
		auto machine = TestMachine({ 0x75, 0x00 });
		machine.ram.Write8(2, 1);
		machine.cpu.a = 0x1;
		machine.cpu.x = 2;
		machine.cpu.SetCarry(true);
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x03);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("ADC Absolute")
	{
		auto machine = TestMachine({ 0x6D, 0x02, 0x01 });
		machine.ram.Write8(0x0102, 3);
		machine.cpu.a = 0x1;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x04);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("ADC Absolute, X")
	{
		auto machine = TestMachine({ 0x7D, 0xFF, 0x01 });
		machine.ram.Write8(0x0200, 4);
		machine.cpu.x = 0x1;
		machine.cpu.a = 0x1;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x05);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 5);
	}

	SECTION("ADC Absolute, Y")
	{
		auto machine = TestMachine({ 0x7D, 0xFE, 0x01 });
		machine.ram.Write8(0x01FF, 0x0A);
		machine.cpu.x = 0x1;
		machine.cpu.a = 0x1;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x0B);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 4);
	}

	SECTION("ADC ZeroPageX Wrap")
	{
		auto machine = TestMachine({ 0x75, 0xFE });
		machine.ram.Write8(3, 1);
		machine.cpu.a = 0x1;
		machine.cpu.x = 5;
		machine.Tick();

		REQUIRE(machine.cpu.a == 0x02);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 4);
	}
}