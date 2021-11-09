#include "Ram.h"

dummy6502::Ram::Ram(const uint16_t size)
{
	data.resize(size);
}

uint8_t dummy6502::Ram::Read8(const uint16_t address)
{
	return data[address];
}

uint16_t dummy6502::Ram::Read16(const uint16_t address)
{
	return (static_cast<uint16_t>(data[address + 1]) << 8) | data[address];
}

void dummy6502::Ram::Write8(const uint16_t address, const uint8_t value)
{
	data[address] = value;
}

void dummy6502::Ram::Write16(const uint16_t address, const uint16_t value)
{
	data[address + 1] = value >> 8;
	data[address] = value & 0xFF;
}