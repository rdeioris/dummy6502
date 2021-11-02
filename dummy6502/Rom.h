#pragma once
#include "MemoryController.h"

namespace dummy6502
{
	class Rom : public IMemoryMappable
	{
	public:
		Rom() = delete;
		Rom(const std::vector<uint8_t> in_data);
		uint8_t Read8(const uint16_t address);
		uint16_t Read16(const uint16_t address);
		void Write8(const uint16_t address, const uint8_t value);
		void Write16(const uint16_t address, const uint16_t value);
	protected:
		const std::vector<uint8_t> data;
	};
}

