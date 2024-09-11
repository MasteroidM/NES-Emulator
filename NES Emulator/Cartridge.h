#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "Mapper_000.h"
#include "Mapper_001.h"
#include "Mapper_002.h"
#include "Mapper_003.h"
#include "Mapper_004.h"
#include "Mapper_066.h"

class Cartridge
{
public:
	Cartridge(const std::string& sFileName);
	~Cartridge();

	// Communications with Main Bus
	bool		cpuRead(uint16_t addr, uint8_t &data);
	bool		cpuWrite(uint16_t addr, uint8_t data);

	// Communications with PPU Bus
	bool		ppuRead(uint16_t addr, uint8_t &data);
	bool		ppuWrite(uint16_t addr, uint8_t data);

	bool ImageValid();
	void reset();
	MIRROR Mirror();

	std::shared_ptr<Mapper> GetMapper();

	//enum MIRROR
	//{
	//	HORIZONTAL,
	//	VERTICAL,
	//	ONESCREEN_LO,
	//	ONESCREEN_HI,
	//} mirror = HORIZONTAL;

private:
	std::vector<uint8_t> vPRGMemory;
	std::vector<uint8_t> vCHRMemory;

	uint8_t nMapperID = 0;
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;

	std::shared_ptr<Mapper> pMapper;

	bool bImageValid = false;

	MIRROR hw_mirror = HORIZONTAL;
};

