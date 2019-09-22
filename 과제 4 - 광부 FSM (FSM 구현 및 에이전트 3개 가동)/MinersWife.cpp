#include "MinersWife.h"

void MinersWife::Update()
{
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	m_pStateMachine->Update();
}