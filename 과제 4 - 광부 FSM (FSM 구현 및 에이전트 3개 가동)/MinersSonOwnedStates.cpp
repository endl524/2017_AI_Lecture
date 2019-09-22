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



// =========== �������� ===========
Sons_GlobalState* Sons_GlobalState::Instance()
{
	static Sons_GlobalState instance;
	return &instance;
}

void Sons_GlobalState::Execute(MinersSon* son)
{
	// 10% Ȯ���� ���� �İԵȴ�.
	if (RandFloat() < 0.1) son->GetFSM()->ChangeState(Wash_Hands::Instance());
}



// =========== ������ ===========
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
		cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": �����Ϳ� �Դ�.";		
	}
}

void Play_at_Playground::Execute(MinersSon* son)
{
	switch (RandInt(0, 3))
	{
	case 0: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": �̲���Ʋ�� ����!"; break;
	case 1: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": �𷡼��� �׾Ҵ�!"; break;
	case 2: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": �׳׸� ����!"; break;
	case 3: cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": ���� �پ�ٳ��!"; break;
	}
}

void Play_at_Playground::Exit(MinersSon* son)
{
	cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": �����͸� ������.";
}




// =========== �� �ı� ===========
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
		cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": ������ ���ư�, ȭ��ǿ� ���Դ�.";
	}
}

void Wash_Hands::Execute(MinersSon* son)
{
	cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": ���� ����������!";
	son->GetFSM()->RevertToPreviousState();
}

void Wash_Hands::Exit(MinersSon* son)
{
	cout << "\n" << Get_Name_of_Entity(son->Get_ID()) << ": (�ٽ� �����ͷ� ����!) ȭ��ǿ��� ���Դ�.";
}