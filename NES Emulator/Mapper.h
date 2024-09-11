#pragma once

#include <cstdint>

enum MIRROR
{
	HARDWARE,
	HORIZONTAL,
	VERTICAL,
	ONESCREEN_LO,
	ONESCREEN_HI,
};

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

	virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t &data) = 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) = 0;
	virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

	virtual void reset() = 0;
	
	virtual MIRROR mirror(); // Get Mirror Mode if Mapper is in control
	
	// IRQ Interface
	virtual bool irqState();
	virtual void irqClear();

	// Scanline counting
	virtual void scanline();

protected:
	// Stored locally as many mappers need this information.
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;
};

