#pragma once

#ifndef _TIA_unit_H
#define _TIA_unit_H

#include "d3d.h"

// GLOBALS 
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class TIA
{
public:
	TIA();
	TIA(const TIA&);
	~TIA();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	D3D* m_Direct3D;

	bool Render();

};

#endif // !TIA_unit