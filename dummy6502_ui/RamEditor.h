#pragma once

#include "ITool.h"

class RamEditor : public ITool
{
public:
	virtual void Init(class DummyMachine& machine, SDL_Window* window, SDL_Renderer* renderer) override;
	virtual void Tick(class DummyMachine& machine) override;
protected:
	uint16_t ram_editor_address = 0;
	bool open_ram_editor = false;
	char hex_buf[3];
};
