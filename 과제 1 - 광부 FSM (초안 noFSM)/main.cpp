#include "Locations.h"
#include "Miner.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

int main()
{
	// (1) 堡何 按眉 积己.
	Miner miner(entity_Miner);

	// (2) 老沥 冉荐狼 堡何 青悼 荐青.
	for (int i = 0; i < 30; ++i)
	{ 
		miner.Update();
		Sleep(2000);
	}

	PressAnyKeyToContinue();
	return 0;
}