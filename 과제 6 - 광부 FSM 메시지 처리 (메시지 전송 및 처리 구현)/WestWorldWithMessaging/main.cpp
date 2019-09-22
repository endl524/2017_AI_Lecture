#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "MinersSon.h"
#include "EntityManager.h"
#include "../Common/Messaging/MessageDispatcher.h"
#include "../Common/misc/ConsoleUtils.h"
#include "EntityNames.h"

std::ofstream os;

int main()
{
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif

	srand((unsigned)time(NULL));

	Miner* Bob = new Miner(ent_Miner_Bob);
	MinersWife* Elsa = new MinersWife(ent_Elsa);
	MinersSon* David = new MinersSon(ent_David);

	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(David);

	for (int i = 0; i < 20; ++i)
	{
		Bob->Update();
		Elsa->Update();
		David->Update();

		Dispatch->DispatchDelayedMessages();

		Sleep(800);
	}

	delete Bob;
	delete Elsa;
	delete David;

	PressAnyKeyToContinue();
	return 0;
}