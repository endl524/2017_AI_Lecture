#include "Locations.h"
#include "Miner.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

int main()
{
	// (1) ���� ��ü ����.
	Miner miner(entity_Miner);

	// (2) ���� Ƚ���� ���� �ൿ ����.
	for (int i = 0; i < 30; ++i)
	{ 
		miner.Update();
		Sleep(2000);
	}

	PressAnyKeyToContinue();
	return 0;
}