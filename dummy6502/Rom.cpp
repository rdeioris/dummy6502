#include "Rom.h"

dummy6502::Rom::Rom(const std::vector<uint8_t> in_data) : data(in_data)
{
}

uint8_t dummy6502::Rom::Read8(const uint16_t address)
{
	return data[address];
}

uint16_t dummy6502::Rom::Read16(const uint16_t address)
{
	return (static_cast<uint16_t>(data[address + 1]) << 8) | data[address];
}

void dummy6502::Rom::Write8(const uint16_t address, const uint8_t value)
{
}

void dummy6502::Rom::Write16(const uint16_t address, const uint16_t value)
{
}