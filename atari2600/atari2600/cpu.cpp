#include "cpu.h"
#include <stdio.h>
#include <time.h>

Cpu::Cpu() 
{
	printf("Created new cpu");
}

Cpu::~Cpu() 
{
	printf("Destroying cpu");
}

void Cpu::Init() 
{
	opcode = 0;
	pc = 0;		//check where it does start the program
}

void Cpu::EmulateCycle() 
{
	pc += 1;
}