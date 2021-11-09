#pragma once
#include <array>
#include <cstdint>
#include <format>
#include <functional>
#include <sstream>
#include "MemoryController.h"

namespace dummy6502
{
	class Cpu
	{
	public:
		class UnimplementedOpcodeException : public std::exception
		{
		public:
			UnimplementedOpcodeException(uint8_t opcode)
			{
				std::stringstream stream;
				stream << "Unknown opcode $" << std::uppercase << std::hex << static_cast<uint32_t>(opcode);
				message = stream.str();
			}
			const char* what() const noexcept override
			{
				return message.c_str();
			}
		protected:
			std::string message;
		};

		Cpu() = delete;
		Cpu(IMemoryController& in_memory_controller);

		uint16_t pc = 0;
		uint8_t a = 0;
		uint8_t x = 0;
		uint8_t y = 0;
		uint8_t flags = 0;
		uint8_t s = 0;

		uint8_t current_opcode = 0;

		bool irq = false;
		bool nmi = false;

		uint64_t ticks = 0;

		uint8_t Read8FromPc();
		uint16_t Read16FromPc();
		void Write8FromPc(uint8_t value);
		void Write16FromPc(uint16_t value);

		IMemoryController& memory_controller;

		void Reset();
		void Step();

		uint16_t Implied();
		uint16_t Immediate();
		uint16_t ZeroPage();
		uint16_t ZeroPageX();
		uint16_t ZeroPageY();
		uint16_t AbsoluteValue();
		uint16_t AbsoluteValueX();
		uint16_t AbsoluteValueXAlwaysCross();
		uint16_t AbsoluteValueY();
		uint16_t IndirectX();
		uint16_t IndirectY();
		uint16_t IndirectYAlwaysCross();

		uint16_t AbsoluteAddress();
		uint16_t AbsoluteAddressX();
		uint16_t AbsoluteAddressY();
		uint16_t AbsoluteAddressPlusOne();
		uint16_t AbsoluteAddressXPlusOne();
		uint16_t AbsoluteAddressYPlusOne();

		uint16_t BranchAddress();

		uint16_t Accumulator();
		uint16_t Indirect();

		void Push(const uint8_t value);
		uint8_t Pop();

		void Nmi();
		void Irq();

		void ADC();
		void AND();
		void ASL();
		void BIT();
		void BCC();
		void BCS();
		void BEQ();
		void BNE();
		void BMI();
		void BPL();
		void BVC();
		void BVS();
		void BRK();
		void CLI();
		void CLC();
		void SEC();
		void SEI();
		void CLV();
		void CLD();
		void SED();
		void CMP();
		void CPX();
		void CPY();
		void DEC();
		void EOR();
		void INC();
		void INX();
		void JMP();
		void JSR();
		void LDA();
		void LDX();
		void LDY();
		void NOP();
		void ORA();
		void RTI();
		void RTS();
		void SBC();
		void STA();
		void TAX();
		void TXA();
		void INY();
		void DEX();
		void DEY();
		void TAY();
		void TYA();
		void LSR();
		void ROL();
		void ROR();
		void TXS();
		void TSX();
		void PHA();
		void PLA();
		void PHP();
		void PLP();
		void STX();
		void STY();

		void UnimplementedOpcode();

		void SetBit(uint8_t& byte, const uint8_t bit, const bool value);
		bool GetBit(const uint8_t byte, const uint8_t bit) const;
		void SetCarry(const bool value) { SetBit(flags, 0, value); }
		void SetZero(const bool value) { SetBit(flags, 1, value); }
		void SetInterruptDisable(const bool value) { SetBit(flags, 2, value); }
		void SetDecimal(const bool value) { SetBit(flags, 3, value); }
		void SetOverflow(const bool value) { SetBit(flags, 6, value); }
		void SetNegative(const bool value) { SetBit(flags, 7, value); }

		bool GetCarry() const { return GetBit(flags, 0); }
		bool GetZero() const { return GetBit(flags, 1); }
		bool GetOverflow() const { return GetBit(flags, 6); }
		bool GetNegative() const { return GetBit(flags, 7); }
		bool GetInterruptDisable() const { return GetBit(flags, 2); }

		std::string GetDisassembly(uint16_t& address);

	protected:
		struct Opcode
		{
			std::function<uint16_t(Cpu*)> addressing_mode;
			std::function<void(Cpu*)> op;
			std::string opcode_name;
		};

		uint8_t opcode_value = 0;
		uint16_t opcode_address = 0;

		std::string addressing_mode_disassembly;

		std::array<Opcode, 256> opcodes = {};
	};
}

