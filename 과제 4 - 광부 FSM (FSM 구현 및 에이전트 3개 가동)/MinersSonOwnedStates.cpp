#include "MinersSonOwnedStates.h"
#include "MinersSon.h"
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
Sons_GlobalState* Sons_GlobalState::Instance()
{
	static Sons_GlobalState instance;
	return &instance;
}

void Sons_GlobalState::Execute(MinersSon* son)
{
	// 10% 확률로 손을 씻게된다.
	if (RandFloat() < 0.1) son->GetFSM()->ChangeState(Wash_Hands::Instance());
}



// =========== 놀이터 ===========
Play_at_Playground* Play_at_Playground::Instance()
{
	static Play_at_Playground instance;
	return &instance;
}

void Play_at_Playground::Enter(MinersSon* son)
{
	if (son->Location() != playground)
	{
		son->ChangeLocation(playground);
		cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 놀이터에 왔다.";		
	}
}

void Play_at_Playground::Execute(MinersSon* son)
{
	switch (RandInt(0, 3))
	{
	case 0: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 미끄럼틀을 탔다!"; break;
	case 1: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 모래성을 쌓았다!"; break;
	case 2: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 그네를 탔다!"; break;
	case 3: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 마구 뛰어다녔다!"; break;
	}
}

void Play_at_Playground::Exit(MinersSon* son)
{
	cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 놀이터를 떠났다.";
}




// =========== 손 씻기 ===========
Wash_Hands* Wash_Hands::Instance()
{
	static Wash_Hands instance;
	return &instance;
}

void Wash_Hands::Enter(MinersSon* son)
{
	if (son->Location() != shack)
	{
		son->ChangeLocation(shack_bathroom);
		cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 집으로 돌아가, 화장실에 들어왔다.";
	}
}

void Wash_Hands::Execute(MinersSon* son)
{
	cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": 손이 깨끗해졌다!";
	son->GetFSM()->RevertToPreviousState();
}

void Wash_Hands::Exit(MinersSon* son)
{
	cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": (다시 놀이터로 가자!) 화장실에서 나왔다.";
}