#include <fstream>
#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "MinersSon.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

std::ofstream os;

int main()
{
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif
	Miner Bob(entity_Miner);
	MinersWife Elsa(entity_MinersWife);
	MinersSon David(entity_MinersSon);

	for (int i = 0; i < 20; ++i)
	{ 
		Bob.Update();
		Elsa.Update();
		David.Update();
		Sleep(2000);
	}

	PressAnyKeyToContinue();
	return 0;
}






