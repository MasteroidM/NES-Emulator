#include "olc6502.h"
#include "Bus.h"

// Datasheet: http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

olc6502::olc6502()
{
	// Initializer list of Initializer Lists
	using a = olc6502;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

olc6502::~olc6502() = default;

// Read 8-bit byte from the bus located at the specified 16-bit address
uint8_t olc6502::read(uint16_t addr)
{
	return bus->cpuRead(addr, false);
}

// Writes a byte to the bus at the specified address
void olc6502::write(uint16_t addr, uint8_t data)
{
	bus->cpuWrite(addr, data);
}

// Returns a value of a specific bit of the status register
uint8_t olc6502::GetFlag(FLAGS6502 f) const
{
	return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of teh status register
void olc6502::SetFlag(FLAGS6502 f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}

// Perform one clock cycle worth of emulation
void olc6502::clock()
{
	if (cycles == 0)
	{
		opcode = read(pc);
		pc++;

		// Get number of totsl cycles
		cycles = lookup[opcode].cycles;

		// Perofrm fetch of intermediate data using the required addressing mode
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

		// Perform operation
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		// Address Mode and Operate may have altered the number of cycles this instruction requires before its completed
		cycles += (additional_cycle1 & additional_cycle2);

	}

	cycles--;
}

// Addressing Modes

uint8_t olc6502::IMP() // Implied addressing mode, uses the accumulator for instructions like PHA
{
	fetched = accumulator;
	return 0;
}

uint8_t olc6502::IMM() // Immediate Addressing Mode, expects the next byte to be used as a value so prepare the read address to point to the next byte
{
	addr_abs = pc++;
	return 0;
}

uint8_t olc6502::ZP0() // Zero Page Addressing Mode, saves progcpuRAM bytes by allowing absolute addressing of a location in the first 0xFF bytes of address range. Requires only one byte instead of the usual two.
{
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPX() // Zero Page with X offset. Contents of the X register are added to the supplied single bit address. Useful for iterating through ranges within the first page.
{
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPY() // Zero Page with Y offset. Same as above but uses the Y register as offset.
{
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::REL() // Relative addressing. This is exclusive to branch instructions. The address must reside within -128 to +127 of the branch isntruction i.e it is not possible to branch to any address in the addressable range.
{
	addr_rel = read(pc);
	pc++;

	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;

	return 0;
}

uint8_t olc6502::ABS() // Absolute Addressing. A full 16-bit address is loaded and used.
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	return 0;
}

uint8_t olc6502::ABX() // Absolute Addressing with X offset. Adds the contents of the X register to the supplied two byte address. If the resulting address changes pages then an additional clock cycle is required.
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += x;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

uint8_t olc6502::ABY() // Absolute Addressing with Y offset. Same as above but with the Y register as offset.
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

uint8_t olc6502::IND() // Indirect Addressing. Supplied 16-bit address is read to get the actual 16-bit address.
{
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;
	
	// This is a hardware bug. If the low byte of the
	// supplied address is 0xFF, then to read the high byte of the actual address
	// we need to cross a page boundary. This doesnt actually work on the chip as 
	// designed, instead it wraps back around in the same page, yielding an 
	// invalid actual address.


	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // Normal behaviour
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);

	return 0;
}

uint8_t olc6502::IZX() // Indirect Addressing with X offset. The contents of the X register is added to the supplied 16-bit address. If a page change is caused then an additional clock cycle is required.
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

uint8_t olc6502::IZY() // Indirect Addressing with Y offset. Same as above but with the contents of the Y register. If a page change is caused then an addtional clock cycle is required.
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Instructions

// This function sources the data used by the instruction into 
// a convenient numeric variable. Some instructions dont have to 
// fetch data as the source is implied by the instruction. For example
// "INX" increments the X register. There is no additional data
// required. For all other addressing modes, the data resides at 
// the location held within addr_abs, so it is read from there. 
// Immediate address mode exploits this slightly, as that has
// set addr_abs = pc + 1, so it fetches the data from the
// next byte for example "LDA $FF" just loads the accumulator with
// 256, i.e. no far reaching memory fetch is required. "fetched"
// is a variable global to the CPU, and is set by calling this 
// function. It also returns it for convenience.
uint8_t olc6502::fetch()
{
	if (!(lookup[opcode].addrmode == &olc6502::IMP))
		fetched = read(addr_abs);
	return fetched;
}

uint8_t olc6502::AND() // Bitwise AND
{
	fetch();
	accumulator = accumulator & fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

uint8_t olc6502::BCS() // Branch if Carry Set
{
	if (GetFlag(C) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}

	return 0;
}

uint8_t olc6502::CLC() // Clear Carry Flag
{
	SetFlag(C, false);
	return 0;
}

uint8_t olc6502::CLD() // Clear Decimal Flag
{
	SetFlag(D, false);
	return 0;
}

uint8_t olc6502::CLI() // Disable Interrupts / Clear Interrupt Flag
{
	SetFlag(I, false);
	return 0;
}

uint8_t olc6502::CLV() // Clear Overflow Flag
{
	SetFlag(V, false);
	return 0;
}

uint8_t olc6502::CMP() // Compare Accumulator
{
	fetch();
	auto temp = (uint16_t)accumulator - (uint16_t)fetched;
	SetFlag(C, accumulator >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}

uint8_t olc6502::CPX() // Compare X Register
{
	fetch();
	auto temp = (uint16_t)x - (uint16_t)fetched;
	SetFlag(C, x >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::CPY() // Compare Y Register
{
	fetch();
	auto temp = (uint16_t)y - (uint16_t)fetched;
	SetFlag(C, y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::DEC() // Decrement value at memory location
{
	fetch();
	auto temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::DEX() // Decrement X Register
{
	x--;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

uint8_t olc6502::DEY() // Decrement Y Register
{
	y--;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

uint8_t olc6502::INC() // Increment Value at memory location
{
	fetch();
	auto temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


uint8_t olc6502::EOR() // Bitwise Logic OR
{
	fetch();
	accumulator = accumulator ^ fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

uint8_t olc6502::INX() // Increment X Register
{
	x++;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

uint8_t olc6502::INY() // Increment Y Register
{
	y++;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

uint8_t olc6502::JMP() // Jump to Location
{
	pc = addr_abs;
	return 0;
}

uint8_t olc6502::JSR() // Jump to Sub-Routine
{
	pc--;

	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	pc = addr_abs;
	return 0;
}

uint8_t olc6502::LDA() // Load Accumulator
{
	fetch();
	accumulator = fetched;
	SetFlag(Z, accumulator  == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

uint8_t olc6502::LDX() // Load X Register
{
	fetch();
	x = fetched;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 1;
}

uint8_t olc6502::LDY() // Load Y Register
{
	fetch();
	y = fetched;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 1;
}

uint8_t olc6502::LSR() // Logical Shift Right
{
	fetch();
	SetFlag(C, fetched & 0x0001);
	auto temp = fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::NOP() // Covers illegal opcodes and NOP opcodes
{
	switch (opcode) 
	{
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}

uint8_t olc6502::ORA() // Bitwise Logical OR
{
	fetch();
	accumulator = accumulator | fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

uint8_t olc6502::ADC() // Add with Carry
{
	fetch(); // Fetch the data that we are adding to the accumulator

	auto temp = (uint16_t)accumulator + (uint16_t)fetched + (uint16_t)GetFlag(C); // Add is performed in the 16-bit domain for emulation to capture any carry bit, which will exist in bit 8 of the 16-bit word.

	SetFlag(C, temp > 255); // Carry Flag is set if the high byte bit is 0
	SetFlag(Z, (temp & 0x00FF) == 0); // Zero Flag is set if the result is 0
	SetFlag(N, temp & 0x80); // Negative Flag is set to the most significant bit of the result
	SetFlag(V, (~((uint16_t)accumulator ^ (uint16_t)fetched) & ((uint16_t)accumulator ^ (uint16_t)temp)) & 0x0080); // Signed Overflow flag is set based on all the previously determined flags.

	accumulator = temp & 0x00FF; // Load the result into the accumulator (Its of 8-bits)
	
	return 1; // Instruction may require 1 more clock cycle.
}

uint8_t olc6502::SBC() // Subtract with Borrow
{
	// A = A + (-M + 1 + C)
	fetch();

	uint16_t value = ((uint16_t)fetched) ^ 0x00FF; // Invert bottom 8 bits with bitwise XOR

	auto temp = (uint16_t)accumulator + (uint16_t)value + (uint16_t)GetFlag(C); // Operate in 16 bit domain to capture carry out

	// Exactly same as addition
	temp = (uint16_t)accumulator + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)accumulator) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);

	accumulator = temp & 0x00FF;

	return 1;
}

uint8_t olc6502::ASL() // Arithmetic Shift Left
{
	fetch();
	uint16_t temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::BCC() // Branch if Carry Clear
{
	if (GetFlag(C) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


uint8_t olc6502::BEQ() // Branch if Equal
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BIT() // Bit Test
{
	fetch();
	auto temp = accumulator & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}

uint8_t olc6502::BMI() // Branch if Negative
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BNE() // Branch if Not Equal
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BPL() // Branch if Positive
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BRK() // Break
{
	pc++;

	SetFlag(I, 1);
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 1);
	write(0x0100 + stkp, status);
	stkp--;
	SetFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

uint8_t olc6502::BVC() // Branch if Overflow Clear
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BVS() // Branch if Overflow Set
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


uint8_t olc6502::PHA() // Push Accumulator to Stack
{
	write(0x100 + stkp, accumulator);
	stkp--;
	return 0;
}

uint8_t olc6502::PHP() // Push Status Register to Stack
{
	write(0x0100 + stkp, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	stkp--;
	return 0;
}


uint8_t olc6502::PLA() // Pop Accumulator off Stack
{
	stkp++;
	accumulator = read(0x0100 + stkp);
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);

	return 0;
}

uint8_t olc6502::PLP() // Pop Status Register off Stack
{
	stkp++;
	status = read(0x0100 + stkp);
	SetFlag(U, 1);
	return 0;
}

uint8_t olc6502::ROL() // Rotate Left
{
	fetch();
	auto temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::ROR() // Rotate Right
{
	fetch();
	auto temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::RTI() // Return from Interrupt
{
	stkp++;
	status = read(0x0100 + stkp);
	status &= ~B;
	status &= ~U;

	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;
	return 0;
}

uint8_t olc6502::RTS() // Return from Sub-Routine
{
	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;

	pc++;
	return 0;
}

uint8_t olc6502::SEC() // Set Carry Flag
{
	SetFlag(C, true);
	return 0;
}


uint8_t olc6502::SED() // Set Decimal Flag
{
	SetFlag(D, true);
	return 0;
}


uint8_t olc6502::SEI() // Enable Interrupts / Set Interupt Flag
{
	SetFlag(I, true);
	return 0;
}

uint8_t olc6502::STA() // Store Accumulator at Address
{
	write(addr_abs, accumulator);
	return 0;
}

uint8_t olc6502::STX() // Store X Register at Address
{
	write(addr_abs, x);
	return 0;
}

uint8_t olc6502::STY() // Store Y Register at Address
{
	write(addr_abs, y);
	return 0;
}

uint8_t olc6502::TAX() // Transfer Accumulator to X register
{
	x = accumulator;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

uint8_t olc6502::TAY() // Transfer Accumulator to Y Register
{
	y = accumulator;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

uint8_t olc6502::TSX() // Transfer Stack Pointer to X register
{
	x = stkp;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

uint8_t olc6502::TXA() // Transfer X Register to Accumulator
{
	accumulator = x;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 0;
}

uint8_t olc6502::TXS() // Transfer X Register to Stack Pointer
{
	stkp = x;
	return 0;
}

uint8_t olc6502::TYA() // Transfer Y Register to Accumulator
{
	accumulator = y;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 0;
}

uint8_t olc6502::XXX() // Illegal Opcode
{
	return 0;
}

void olc6502::reset() // Forces 6502 into a known state. This is hard-wired inside the CPU.
{
	accumulator = 0;
	x = 0;
	y = 0;
	stkp = 0xFD;
	status = 0x00 | U;

	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	pc = (hi << 8) | lo;

	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	cycles = 8;

}

void olc6502::irq() // Interrupt Request
{
	if (GetFlag(I) == 0) // If Interrupts are allowed
	{
		// Push Program Counter to stack. It takes two pushes since its of 16-bits.
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		// Push Status Register to Stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		// Read new Program Counter location from the fixed address
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// IRQ time
		cycles = 7;
	}
}

void olc6502::nmi() // Non-maskable Interrupt. Cannot be ignored and behaves the exact same way as IRQ but it reads the new progcpuRAM counter address from location 0xFFFA.
{
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + stkp, status);
	stkp--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	cycles = 8;
}

bool olc6502::complete() const // Indicates a clock cycle has completed
{
	return cycles == 0;
}

std::map<uint16_t, std::string> olc6502::disassemble(uint16_t nStart, uint16_t nStop) // Debugging disassemblely function.
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;

	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// Convert variables into hex strings
	auto hex = [](uint32_t n, uint8_t d)
		{
			std::string s(d, '0');
			for (int i = d - 1; i >= 0; i--, n >>= 4)
			{
				s[i] = "0123456789ABCDEF"[n & 0xF];
			}
			return s;
		};

	while (addr <= (uint32_t)nStop) // String is assembled as the instruction is decoded
	{
		line_addr = addr;

		std::string sInst = "$" + hex(addr, 4) + ": "; // Prefix line with instruction address

		uint8_t opcode = bus->cpuRead(addr, true); // Read instruction and get its name
		addr++;

		sInst += lookup[opcode].name + " ";

		// Get operands from desired locations. They mimic the actual fetch routine of the 6502 to get accurate data as part of the instruction.
		if (lookup[opcode].addrmode == &olc6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IMM)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ZP0)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ZPX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ZPY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IZX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IZY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ABS)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ABX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ABY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IND)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &olc6502::REL)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		mapLines[line_addr] = sInst; // Add the formed string to a map, using the instruction address as a key. This is convinient as the instrucitons vary in length so a incremental index is not sufficient. 
	}
	
	return mapLines;
}