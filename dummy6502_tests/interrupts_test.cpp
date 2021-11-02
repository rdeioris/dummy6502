#include "catch.hpp"
#include "TestMachine.h"

TEST_CASE("dummy6502.Interrupts.IRQ")
{
	SECTION("CLI NOP NOP BRK")
	{
		auto machine = TestMachine({ 0x58, 0xEA, 0xEA, 0x00 });
		machine.Tick(4);

		REQUIRE(machine.cpu.pc == 0x1020);

		REQUIRE(machine.cpu.GetInterruptDisable());

		REQUIRE(machine.cpu.a == 0x00);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 13);
	}

	SECTION("CLI BRK (NOP * 30) LDA #17 RTI")
	{
		auto nop_32 = std::vector<uint8_t>(32, 0xEA);
		nop_32[0] = 0x58; // CLI
		nop_32[1] = 0x00; // BRK
		nop_32.push_back(0xA9); // LDA
		nop_32.push_back(0x11); // #17
		nop_32.push_back(0x40); // RTI
		auto machine = TestMachine(nop_32);
		machine.Tick(2);

		REQUIRE(machine.cpu.pc == 0x1020);
		REQUIRE(machine.cpu.GetInterruptDisable());

		machine.Tick(2);

		REQUIRE(machine.cpu.pc == 0x1002);

		REQUIRE(machine.cpu.a == 0x11);
		REQUIRE(!machine.cpu.GetZero());
		REQUIRE(!machine.cpu.GetCarry());
		REQUIRE(machine.cpu.ticks == 17);
	}
}
