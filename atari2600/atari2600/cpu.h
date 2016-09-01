#pragma once

#ifndef _MICRO_6502_H
#define _MICRO_6502_H

class Cpu
{
public:
	Cpu();
	~Cpu();

private:	
	unsigned short opcode;			// Current opcode
	unsigned char  memory[128];		// Memory (size = 128 B)		

	// registers
	unsigned char  acc;				// accumulator
	unsigned char  X;				
	unsigned char  Y;				 
	unsigned char  status;			// 8 flags (see doc: https://emu-docs.org/CPU%2065xx/6502.txt for more details about it)
	unsigned short pc;				// program counter
	unsigned short sp;				// stack pointer
	
	void Init();
	void EmulateCycle();
};

#endif // !_MICRO_6502_H