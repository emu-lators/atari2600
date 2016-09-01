#include <stdlib.h>
#include <iostream>
#include "emu.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static Emulator* emulator;
	bool result;

	// Create the emulator object.
	emulator = new Emulator;
	if (!emulator)
	{
		return 0;
	}

	// Initialize and run the emulator object.
	result = emulator->Initialize();
	if (result)
	{
		emulator->Run();
	}

	// Shutdown and release the emulator object.
	emulator->Shutdown();
	delete emulator;
	emulator = 0;

	return 0;
}