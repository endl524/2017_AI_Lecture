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


// =========== �������� ===========
Wifes_GlobalState* Wifes_GlobalState::Instance()
{
	static Wifes_GlobalState instance;
	return &instance;
}

void Wifes_GlobalState::Execute(MinersWife* wife)
{
	// 10%�� Ȯ���� ȭ����� ���Եȴ�.
	if (RandFloat() <= 0.1) wife->GetFSM()->ChangeState(Visit_Bathroom::Instance());
}



// =========== �� �� �� ===========
Do_House_Work* Do_House_Work::Instance()
{
	static Do_House_Work instance;
	return &instance;
}

void Do_House_Work::Enter(MinersWife* wife)
{
	wife->ChangeLocation(shack);
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": �� �� ���� �ؾ߰ڴ�.";
}

void Do_House_Work::Execute(MinersWife* wife)
{
	switch(RandInt(0, 2))
	{
	case 0: cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": �ٴ��� û���Ѵ�."; break;
	case 1: cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": �������� �Ѵ�."; break;
	case 2: cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": �������� ���ǵ��� �����Ѵ�."; break;
	}
}

void Do_House_Work::Exit(MinersWife* wife)
{
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": �� �� ���� ��� �����.";
}



// =========== ȭ��� ===========
Visit_Bathroom* Visit_Bathroom::Instance()
{
	static Visit_Bathroom instance;
	return &instance;
}

void Visit_Bathroom::Enter(MinersWife* wife)
{
	wife->ChangeLocation(shack_bathroom);
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": ȭ��ǿ� ���Դ�.";
}

void Visit_Bathroom::Execute(MinersWife* wife)
{
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": ���� �İ� ȭ���� ���ƴ�.";
	wife->GetFSM()->RevertToPreviousState();
}

void Visit_Bathroom::Exit(MinersWife* wife)
{
	cout << "\n" << Get_Name_of_Entity(wife->Get_ID()) << ": ȭ��ǿ��� ���Դ�.";
}