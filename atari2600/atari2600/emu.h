#pragma once
// Filename: EMU.h
#ifndef _EMUL_H_
#define _EMUL_H_

// PRE-PROCESSING DIRECTIVES
//#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "inputclass.h"
#include "tia.h"
#include "cpu.h"

// FUNCTION PROTOTYPES 
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class Emulator
{
public:
	Emulator();
	Emulator(const Emulator&);
	~Emulator();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	TIA* m_Graphics; 
	Cpu cpu6502;
	InputClass* m_Input;	// Need to update this class to keyboard or something like that
};

static Emulator* ApplicationHandle = 0;
#endif