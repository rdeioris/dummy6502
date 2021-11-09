#include "Cpu.h"

#define OPCODE(opcode, addressing_mode, op) opcodes[opcode] = {&Cpu::addressing_mode, &Cpu::op, #op}

dummy6502::Cpu::Cpu(IMemoryController& in_memory_controller)
	: memory_controller(in_memory_controller)
{
	for (int32_t i = 0; i < 256; i++)
	{
		OPCODE(i, Implied, UnimplementedOpcode);
	}

	OPCODE(0x69, Immediate, ADC);
	OPCODE(0x65, ZeroPage, ADC);
	OPCODE(0x75, ZeroPageX, ADC);
	OPCODE(0x6D, AbsoluteValue, ADC);
	OPCODE(0x7D, AbsoluteValueX, ADC);
	OPCODE(0x79, AbsoluteValueY, ADC);
	OPCODE(0x61, IndirectX, ADC);
	OPCODE(0x71, IndirectY, ADC);

	OPCODE(0x29, Immediate, AND);
	OPCODE(0x25, ZeroPage, AND);
	OPCODE(0x35, ZeroPageX, AND);
	OPCODE(0x2D, AbsoluteValue, AND);
	OPCODE(0x3D, AbsoluteValueX, AND);
	OPCODE(0x39, AbsoluteValueY, AND);
	OPCODE(0x21, IndirectX, AND);
	OPCODE(0x31, IndirectY, AND);

	OPCODE(0x00, Implied, BRK);
	OPCODE(0x20, AbsoluteAddress, JSR);
	OPCODE(0xEA, Implied, NOP);
	OPCODE(0x58, Implied, CLI);

	OPCODE(0x4C, AbsoluteAddress, JMP);
	OPCODE(0x6C, Indirect, JMP);

	OPCODE(0x40, Implied, RTI);
	OPCODE(0x60, Implied, RTS);



	OPCODE(0xA9, Immediate, LDA);

	OPCODE(0xA2, Immediate, LDX);

	OPCODE(0xA0, Immediate, LDY);

	OPCODE(0x8D, AbsoluteAddress, STA);

	OPCODE(0xE8, Implied, INX);

	OPCODE(0x10, BranchAddress, BPL);
	OPCODE(0x30, BranchAddress, BMI);
	OPCODE(0x50, BranchAddress, BVC);
	OPCODE(0x70, BranchAddress, BVS);
	OPCODE(0x90, BranchAddress, BCC);
	OPCODE(0xB0, BranchAddress, BCS);
	OPCODE(0xD0, BranchAddress, BNE);
	OPCODE(0xF0, BranchAddress, BEQ);

	OPCODE(0xE9, Immediate, SBC);

	OPCODE(0x8A, Implied, TXA);

	OPCODE(0x4A, Accumulator, LSR);
	OPCODE(0x46, ZeroPage, LSR);
	OPCODE(0x56, ZeroPageX, LSR);
	OPCODE(0x4E, AbsoluteValue, LSR);
	OPCODE(0x5E, AbsoluteValueX, LSR);
}

uint16_t dummy6502::Cpu::Implied()
{
	addressing_mode_disassembly = "";
	return 0;
}

uint16_t dummy6502::Cpu::Accumulator()
{
	addressing_mode_disassembly = "A";
	return 0;
}

uint16_t dummy6502::Cpu::Immediate()
{
	opcode_value = Read8FromPc();
	addressing_mode_disassembly = std::format("#${:02X}", opcode_value);
	ticks++;
	return 1;
}

uint16_t dummy6502::Cpu::ZeroPage()
{
	uint8_t zero_page_address = Read8FromPc();
	ticks++;
	opcode_value = memory_controller.Read8(zero_page_address);
	ticks++;
	return 1;
}

uint16_t dummy6502::Cpu::ZeroPageX()
{
	uint8_t zero_page_address = Read8FromPc();
	addressing_mode_disassembly = std::format("${:02X}", zero_page_address);
	ticks++;
	zero_page_address += x;
	ticks++;
	opcode_value = memory_controller.Read8(zero_page_address);
	ticks++;
	return 1;
}

uint16_t dummy6502::Cpu::IndirectX()
{
	uint8_t zero_page_address = Read8FromPc();
	addressing_mode_disassembly = std::format("(${:02X}, X)", zero_page_address);
	ticks++;
	zero_page_address += x;
	ticks++;
	uint16_t indirect_address = memory_controller.Read16(zero_page_address);
	ticks += 2;
	opcode_value = memory_controller.Read8(indirect_address);
	ticks++;
	return 1;
}

uint16_t dummy6502::Cpu::IndirectY()
{
	uint8_t zero_page_address = Read8FromPc();
	addressing_mode_disassembly = std::format("(${:02X}), Y", zero_page_address);
	ticks++;
	uint16_t indirect_address = memory_controller.Read16(zero_page_address);
	ticks += 2;
	uint16_t page = indirect_address & 0xFF00;
	indirect_address += y;
	if (page != (indirect_address & 0xFF00))
	{
		ticks++;
	}
	ticks++;
	opcode_value = memory_controller.Read8(indirect_address);
	ticks++;
	return 1;
}

uint16_t dummy6502::Cpu::Indirect()
{
	uint16_t indirect_address = Read16FromPc();
	addressing_mode_disassembly = std::format("(${:04X})", indirect_address);
	ticks += 2;
	// special case (well, bug) for cross boundary indirect jump
	if ((indirect_address & 0x00FF) == 0xFF)
	{
		opcode_address = memory_controller.Read8(indirect_address);
		indirect_address &= 0xFF00;
		opcode_address |= (memory_controller.Read8(indirect_address)) << 8;
	}
	else
	{
		opcode_address = memory_controller.Read16(indirect_address);
	}
	ticks += 2;
	return 2;
}

uint16_t dummy6502::Cpu::AbsoluteValue()
{
	uint16_t absolute_address = Read16FromPc();
	addressing_mode_disassembly = std::format("${:04X}", absolute_address);
	ticks += 2;
	opcode_value = memory_controller.Read8(absolute_address);
	ticks++;
	return 2;
}

uint16_t dummy6502::Cpu::AbsoluteValueX()
{
	uint16_t absolute_address = Read16FromPc();
	addressing_mode_disassembly = std::format("${:04X}, X", absolute_address);
	ticks += 2;
	uint16_t page = absolute_address & 0xFF00;
	absolute_address += x;
	if (page != (absolute_address & 0xFF00))
	{
		ticks++;
	}
	opcode_value = memory_controller.Read8(absolute_address);
	ticks++;
	return 2;
}

uint16_t dummy6502::Cpu::AbsoluteValueY()
{
	uint16_t absolute_address = Read16FromPc();
	addressing_mode_disassembly = std::format("${:04X}, Y", absolute_address);
	ticks += 2;
	uint16_t page = absolute_address & 0xFF00;
	absolute_address += y;
	if (page != (absolute_address & 0xFF00))
	{
		ticks++;
	}
	opcode_value = memory_controller.Read8(absolute_address);
	ticks++;
	return 2;
}

uint16_t dummy6502::Cpu::AbsoluteAddress()
{
	opcode_address = Read16FromPc();
	addressing_mode_disassembly = std::format("${:04X}", opcode_address);
	ticks += 2;
	return 2;
}

uint16_t dummy6502::Cpu::BranchAddress()
{
	uint8_t unsigned_address = Read8FromPc();
	int8_t* signed_address = reinterpret_cast<int8_t*>(&unsigned_address);
	opcode_address = static_cast<uint16_t>(static_cast<int32_t>(pc) + 1 + *signed_address);
	ticks++;
	addressing_mode_disassembly = std::format("${:04X}", opcode_address);
	return 1;
}

uint8_t dummy6502::Cpu::Read8FromPc()
{
	return memory_controller.Read8(pc);
}

uint16_t dummy6502::Cpu::Read16FromPc()
{
	return memory_controller.Read16(pc);
}

void dummy6502::Cpu::Step()
{
	current_opcode = Read8FromPc();
	pc++;

	pc += opcodes[current_opcode].addressing_mode(this);
	opcodes[current_opcode].op(this);

	ticks++;

	// check for interrupts
	if (nmi)
	{
		nmi = false;
		Push(pc >> 8); // high byte
		Push(pc & 0xff); // low byte
		Push(flags);
		pc = memory_controller.Read16(0xfffa);
		SetInterruptDisable(true);
		ticks += 7;
	}
	else if (irq)
	{
		irq = false;
		if (!GetInterruptDisable())
		{
			Push(pc >> 8); // high byte
			Push(pc & 0xff); // low byte
			Push(flags);
			pc = memory_controller.Read16(0xfffe);
			SetInterruptDisable(true);
			ticks += 7;
		}
	}
}

std::string dummy6502::Cpu::GetDisassembly(uint16_t& address)
{
	uint16_t tmp_pc = pc;
	pc = address;
	uint8_t opcode = memory_controller.Read8(address);
	pc++;
	address++;
	address += opcodes[opcode].addressing_mode(this);
	pc = tmp_pc;
	return std::format("{} {}", opcodes[opcode].opcode_name, addressing_mode_disassembly);
}

void dummy6502::Cpu::Nmi()
{
	nmi = true;
}

void dummy6502::Cpu::Irq()
{
	irq = true;
}

void dummy6502::Cpu::SetBit(uint8_t& byte, const uint8_t bit, const bool value)
{
	if (value)
	{
		byte |= 1 << bit;
	}
	else
	{
		byte &= ~(1 << bit);
	}
}

bool dummy6502::Cpu::GetBit(const uint8_t byte, const uint8_t bit) const
{
	return byte & (1 << bit);
}

void dummy6502::Cpu::Reset()
{
	ticks = 0;
	a = 0;
	x = 0;
	y = 0;
	flags = 0;
	SetInterruptDisable(true);
	s = 0xFD;
	pc = memory_controller.Read16(0xfffc);
}

void dummy6502::Cpu::Push(uint8_t value)
{
	uint16_t stack_address = 0x0100 + s;
	memory_controller.Write8(stack_address, value);
	s--;
}

uint8_t dummy6502::Cpu::Pop()
{
	s++;
	uint16_t stack_address = 0x0100 + s;
	return memory_controller.Read8(stack_address);
}

void dummy6502::Cpu::UnimplementedOpcode()
{
	throw UnimplementedOpcodeException(current_opcode);
}

void dummy6502::Cpu::ADC()
{
	uint8_t old_a = a;
	a = old_a + opcode_value + (GetCarry() ? 1 : 0);
	SetCarry(a < old_a);
	SetZero(a == 0);
	SetOverflow(((a ^ opcode_value) & (a ^ old_a)) >> 7);
	SetNegative(a & 0x80);
}

void dummy6502::Cpu::AND()
{
	a &= opcode_value;
	SetZero(a == 0);
	SetNegative(a & 0x80);
}

void dummy6502::Cpu::TXA()
{
	a = x;
	SetZero(a == 0);
	SetNegative(a & 0x80);
}

void dummy6502::Cpu::SBC()
{
	uint8_t old_a = a;
	a = old_a - opcode_value - (GetCarry() ? 1 : 0);
	SetCarry(a > old_a);
	SetZero(a == 0);
	SetOverflow(((a ^ opcode_value) & (a ^ old_a)) >> 7);
	SetNegative(a & 0x80);
}

void dummy6502::Cpu::LSR()
{
	uint8_t old_a = a;
	a >>= 1;
	SetCarry(old_a & 0x01);
	SetZero(a == 0);
	SetNegative(a & 0x80);
}

void dummy6502::Cpu::BRK()
{
	// reduce the tick count as it will be accounted by the virtual interrupt
	ticks--;
	Irq();
}

void dummy6502::Cpu::NOP()
{
	ticks++;
}

void dummy6502::Cpu::CLI()
{
	SetInterruptDisable(false);
	ticks++;
}

void dummy6502::Cpu::LDA()
{
	a = opcode_value;
	SetZero(a == 0);
	SetNegative(a & 0x80);
}

void dummy6502::Cpu::LDX()
{
	x = opcode_value;
	SetZero(x == 0);
	SetNegative(x & 0x80);
}

void dummy6502::Cpu::LDY()
{
	y = opcode_value;
	SetZero(y == 0);
	SetNegative(y & 0x80);
}

void dummy6502::Cpu::STA()
{
	memory_controller.Write8(opcode_address, a);
}

void dummy6502::Cpu::JSR()
{
	// we push next address - 1
	pc -= 1;
	Push(pc >> 8);
	Push(pc & 0xff);
	pc = opcode_address;
	ticks += 5;
}

void dummy6502::Cpu::JMP()
{
	pc = opcode_address;
}

void dummy6502::Cpu::INX()
{
	x++;
	SetZero(x == 0);
	SetNegative(x & 0x80);
	ticks++;
}

void dummy6502::Cpu::RTI()
{
	flags = Pop();
	pc = Pop();
	pc |= static_cast<uint16_t>(Pop()) << 8;
	ticks += 5;
}

void dummy6502::Cpu::RTS()
{
	pc = Pop();
	pc |= static_cast<uint16_t>(Pop()) << 8;
	// return address was -1
	pc++;
	ticks += 5;
}

void dummy6502::Cpu::BEQ()
{
	if (GetZero())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BNE()
{
	if (!GetZero())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BMI()
{
	if (GetNegative())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BPL()
{
	if (!GetNegative())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BCS()
{
	if (GetCarry())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BCC()
{
	if (!GetCarry())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BVS()
{
	if (GetOverflow())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}

void dummy6502::Cpu::BVC()
{
	if (!GetOverflow())
	{
		ticks++;
		uint16_t nextop_page = (pc + 1) & 0xFF00;
		uint16_t branch_page = opcode_address & 0xFF00;
		if (nextop_page != (opcode_address & 0xFF00))
		{
			ticks++;
		}
		pc = opcode_address;
	}
}