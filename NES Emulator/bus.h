#pragma once

#include <cstdint>
#include <array>

#include "olc6502.h"
#include "olc2C02.h"
#include "olc2A03.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus(); 
	~Bus();

	// Devices on the bus

	// 6502 Derived Processor
	olc6502 cpu;

	// The 2C02 Picture Processing Unit
	olc2C02 ppu;

	// The 2A03 Audio Processing Unit
	olc2A03 apu;

	// Fake RAM
	std::array<uint8_t, 2048> cpuRAM = { 0x00 };

	// Controllers
	std::array<uint8_t, 2> controller;

	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

	// Bus Read & Write
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

	// System Interface
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	bool clock();

	// System Audio Synchronization
	void SetSampleFrequency(uint32_t sample_rate);
	double dAudioSample = 0.0;

private:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;

	// Internal cache of controller state
	uint8_t controller_state[2];

	// DMA from CPU Bus Memory to OAM Memory
	uint8_t dma_page = 0x00;
	uint8_t dma_addr = 0x00;
	uint8_t dma_data = 0x00;

	// Dummy cycle of idleness needed for an "even" clock cycle
	bool dma_dummy = true;

	// Flag to indicate DMA transfer is happening
	bool dma_transfer = false;

	// System Audio Synchronization
	double dAudioTime = 0.0;
	double dAudioGlobalTime = 0.0;
	double dAudioTimePerNESClock = 0.0;
	double dAudioTimePerSystemSample = 0.0f;
};

