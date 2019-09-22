#include "MinersSon.h"

void MinersSon::Update()
{
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED);
	m_pStateMachine->Update();
}