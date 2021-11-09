#pragma once

#include "../dummy6502/MemoryController.h"
#include "ITool.h"

class DummyJoypad : public dummy6502::IMemoryMappable, public ITool
{
public:
	void SetBit(const uint8_t bit)
	{
		mask |= 1 << bit;
	}

	void ClearBit(const uint8_t bit)
	{
		mask &= ~(1 << bit);
	}

	uint8_t Read8(const uint16_t address)
	{
		return mask;
	}

	uint16_t Read16(const uint16_t address)
	{
		return mask;
	}

	void Write8(const uint16_t address, const uint8_t value)
	{
	}

	void Write16(const uint16_t address, const uint16_t value)
	{
	}

	virtual void Init(class DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer) override;
	virtual void Event(class DummyMachine& machine, SDL_Event& event) override;
	virtual void Tick(class DummyMachine& machine) override;
protected:
	uint8_t mask = 0;
};