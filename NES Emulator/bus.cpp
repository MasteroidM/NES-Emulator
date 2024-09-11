#include "bus.h"

Bus::Bus()
{
	// Connect CPU to Communication Bus
	cpu.ConnectBus(this);
}

Bus::~Bus() = default;

void Bus::SetSampleFrequency(uint32_t sample_rate)
{
	dAudioTimePerSystemSample = 1.0 / (double)sample_rate;
	dAudioTimePerNESClock = 1.0 / 5369318.0; // PPU Clock Frequency
}


void Bus::cpuWrite(uint16_t addr, uint8_t data)
{

	if (cart->cpuWrite(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		cpuRAM[addr & 0x07FF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		ppu.cpuWrite(addr & 0x0007, data);
	}
	else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017)
	{
		apu.cpuWrite(addr, data);
	}
	else if (addr == 0x4014)
	{
		// Write to this address initiates a DMA Transfer
		dma_page = data;
		dma_addr = 0x00;
		dma_transfer = true;
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		// "Lock in" controller state at this time
		controller_state[addr & 0x0001] = controller[addr & 0x0001];
	}
	

}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;
	if (cart->cpuRead(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		data = cpuRAM[addr & 0x07FF];
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		data = ppu.cpuRead(addr & 0x0007, bReadOnly);
	}
	else if (addr == 0x4015)
	{
		// APU Read Status
		data = apu.cpuRead(addr);
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		// Read out the MSB of the controller status word
		data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);

}

void Bus::reset()
{
	cpu.reset();
	cart->reset();
	ppu.reset();
	nSystemClockCounter = 0;
	dma_page = 0x00;
	dma_addr = 0x00;
	dma_data = 0x00;
	dma_dummy = true;
	dma_transfer = false;
}

bool Bus::clock()
{
	ppu.clock(); // PPU is the fastest clock frequency

	apu.clock(); // Clock the APU as well

	if (nSystemClockCounter % 3 == 0) // CPU runs 3 times slower than the PPU
	{
		if (dma_transfer) // Is the system performing a DMA transfer from CPU memory to OAM memory on PPU?
		{
			if (dma_dummy) // Yes, wait until the next even CPU clock cycle
			{
				if (nSystemClockCounter % 2 == 1) // Wait here until 1 or 2 cycles have elapsed
				{
					dma_dummy = false; // Allow DMA to start
				}
			}
			else
			{
				if (nSystemClockCounter % 2 == 0) // DMA Start
				{
					dma_data = cpuRead(dma_page << 8 | dma_addr); // On even clock cycles, read from CPU Bus
				}
				else
				{
					ppu.pOAM[dma_addr] = dma_data; // On odd clock cycles, write to PPU OAM
					dma_addr++; // Increment the lo byte of the address

					if (dma_addr == 0x00) // If it wraps around, we know 256 bytes have been written so end DMA.
					{
						dma_transfer = false;
						dma_dummy = true;
					}
				}
			}
		}
		else
		{
			cpu.clock(); // Clock as normal
		}
	}

	// Audio Synchronization
	bool bAudioSampleReady = false;
	dAudioTime += dAudioTimePerNESClock;
	
	if (dAudioTime >= dAudioTimePerSystemSample)
	{
		dAudioTime -= dAudioTimePerSystemSample;
		dAudioSample = apu.GetOutputSample();
		bAudioSampleReady = true;
	}

	// PPU is capable of emitting an interrupt to indicate the vertical blanking period has been entered.
	// If it has, we need to send that IRQ to the CPU.
	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nmi();
	}

	// Check if cartridge is requesting IRQ
	if (cart->GetMapper()->irqState())
	{
		cart->GetMapper()->irqClear();
		cpu.irq();
	}

	nSystemClockCounter++;

	return bAudioSampleReady;
}
