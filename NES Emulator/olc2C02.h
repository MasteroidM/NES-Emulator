#pragma once

#include <cstdint>
#include <memory>

#include "Cartridge.h"
#include "olcPixelGameEngine.h"

class olc2C02
{
public:
	olc2C02();
	~olc2C02();

	// Communications with Main Bus
	uint8_t		cpuRead(uint16_t addr, bool rdonly = false);
	void		cpuWrite(uint16_t addr, uint8_t data);

	// Communications with PPU Bus
	uint8_t		ppuRead(uint16_t addr, bool rdonly = false);
	void		ppuWrite(uint16_t addr, uint8_t data);

	// Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
	void reset();

	bool nmi = false;
	bool scanline_trigger = false;

	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);
	olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
	bool frame_complete = false;

	uint8_t tblName[2][1024]; // VRAM Name Table
	uint8_t tblPalette[32]; // RAM Palettes
	uint8_t tblPattern[2][4096];

	// OAM is convenient to work with but the DMA mechanism will need access to it for writing one byte at a time.
	uint8_t* pOAM = (uint8_t*)OAM;

private:
	// Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

	olc::Pixel palScreen[0x40];
	olc::Sprite* sprScreen;
	olc::Sprite* sprNameTable[2];
	olc::Sprite* sprPatternTable[2];

	union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};

		uint8_t reg;
	} status;

	union
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};

		uint8_t reg;
	} mask;

	union PPUCTRL
	{
		struct
		{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1; // unused
			uint8_t enable_nmi : 1;
		};

		uint8_t reg;
	} control;

	union loopy_register
	{
		struct
		{
			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0x0000;
	};

	loopy_register vram_addr; // Active "Pointer" Address into nametable to extract background tile info
	loopy_register tram_addr; // Temporary store of information to be "transferred" into "pointer" at various times

	uint8_t fine_x = 0x00; // Pixel offset horizontally

	// Internal Communications
	uint8_t address_latch = 0x00;
	uint8_t ppu_data_buffer = 0x00;
	
	// Pixel "dot" position information
	int16_t scanline = 0;
	int16_t cycle = 0;
	bool odd_frame = false;

	// Background rendering
	uint8_t bg_next_tile_id = 0x00;
	uint8_t bg_next_tile_attrib = 0x00;
	uint8_t bg_next_tile_lsb = 0x00;
	uint8_t bg_next_tile_msb = 0x00;
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo = 0x0000;
	uint16_t bg_shifter_attrib_hi = 0x0000;

	// Foreground "Sprite" Rendering
	// OAM is an additional memory internal to the PPU. It is not connected via any bus.
	// It stores the locations of 64 of 8x8 (or 8x16) tiles to be drawn on the next frame.
	struct sObjectAttributeEntry
	{
		uint8_t y; // Y position of sprite
		uint8_t id; // ID of tile from pattern memory
		uint8_t attribute; // Flags define how sprite should be rendered
		uint8_t x; // X position of sprite
	} OAM[64];

	// Register to store address when the CPU manually communicates with OAM via PPU registers.
	// This is very slow and a 256-byte DMA transfer is used instead.
	uint8_t oam_addr = 0x00;

	sObjectAttributeEntry spriteScanline[8];
	uint8_t sprite_count;
	uint8_t sprite_shifter_pattern_lo[8];
	uint8_t sprite_shifter_pattern_hi[8];

	// Sprite Zero Collision Flags
	bool bSpriteZeroHitPossible = false;
	bool bSpriteZeroBeingRendered = false;
};

