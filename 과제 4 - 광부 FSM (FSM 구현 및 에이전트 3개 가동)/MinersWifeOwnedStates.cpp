#include "MinersWifeOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


// =========== 전역상태 ===========
Wifes_GlobalState* Wifes_GlobalState::Instance()
{
	static Wifes_GlobalState instance;
	return &instance;
}

void Wifes_GlobalState::Execute(MinersWife* wife)
{
	// 10%의 확률로 화장실을 가게된다.
	if (RandFloat() <= 0.1) wife->GetFSM()->ChangeState(Visit_Bathroom::Instance());
}



// =========== 집 안 일 ===========
Do_House_Work* Do_House_Work::Instance()
{
	static Do_House_Work instance;
	return &instance;
}

void Do_House_Work::Enter(MinersWife* wife)
{
	wife->ChangeLocation(shack);
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 집 안 일을 해야겠다.";
}

void Do_House_Work::Execute(MinersWife* wife)
{
	switch(RandInt(0, 2))
	{
	case 0: cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 바닥을 청소한다."; break;
	case 1: cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 설거지를 한다."; break;
	case 2: cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 어질러진 물건들을 정리한다."; break;
	}
}

void Do_House_Work::Exit(MinersWife* wife)
{
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 집 안 일을 잠시 멈췄다.";
}



// =========== 화장실 ===========
Visit_Bathroom* Visit_Bathroom::Instance()
{
	static Visit_Bathroom instance;
	return &instance;
}

void Visit_Bathroom::Enter(MinersWife* wife)
{
	wife->ChangeLocation(shack_bathroom);
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 화장실에 들어왔다.";
}

void Visit_Bathroom::Execute(MinersWife* wife)
{
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 손을 씻고 화장을 고쳤다.";
	wife->GetFSM()->RevertToPreviousState();
}

void Visit_Bathroom::Exit(MinersWife* wife)
{
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": 화장실에서 나왔다.";
}