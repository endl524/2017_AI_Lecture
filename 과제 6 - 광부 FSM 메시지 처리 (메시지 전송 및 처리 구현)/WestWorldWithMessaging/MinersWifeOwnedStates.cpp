#include "MinersWifeOwnedStates.h"
#include "MinerOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "../Common/Time/CrudeTimer.h"
#include "../Common/Messaging/MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

WifesGlobalState* WifesGlobalState::Instance()
{
	static WifesGlobalState instance;

	return &instance;
}


void WifesGlobalState::Execute(MinersWife* wife)
{
	if ((RandFloat() < 0.1) &&
		!wife->GetFSM()->isInState(*VisitBathroom::Instance()))
	{
		wife->GetFSM()->ChangeState(VisitBathroom::Instance());
	}
}

bool WifesGlobalState::OnMessage(MinersWife* wife, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_HiHoneyImHome:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: "
			<< Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(wife->ID()) << ": Hi honey. Let me make you some of mah fine country stew";

		wife->GetFSM()->ChangeState(CookStew::Instance());
	}
	return true;

	case Msg_ImGoing:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(wife->ID()) << ": �����!";
	}
	return true;
	}

	return false;
}

DoHouseWork* DoHouseWork::Instance()
{
	static DoHouseWork instance;

	return &instance;
}

void DoHouseWork::Enter(MinersWife* wife)
{
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": Time to do some more housework!";
}


void DoHouseWork::Execute(MinersWife* wife)
{
	switch (RandInt(0, 2))
	{
	case 0:

		cout << "\n" << GetNameOfEntity(wife->ID()) << ": Moppin' the floor";

		break;

	case 1:

		cout << "\n" << GetNameOfEntity(wife->ID()) << ": Washin' the dishes";

		break;

	case 2:

		cout << "\n" << GetNameOfEntity(wife->ID()) << ": Makin' the bed";

		break;
	}
}

void DoHouseWork::Exit(MinersWife* wife)
{
}

bool DoHouseWork::OnMessage(MinersWife* wife, const Telegram& msg)
{
	return false;
}

VisitBathroom* VisitBathroom::Instance()
{
	static VisitBathroom instance;

	return &instance;
}

void VisitBathroom::Enter(MinersWife* wife)
{
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": Walkin' to the can. Need to powda mah pretty li'lle nose";
}


void VisitBathroom::Execute(MinersWife* wife)
{
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": Ahhhhhh! Sweet relief!";

	wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinersWife* wife)
{
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": Leavin' the Jon";
}


bool VisitBathroom::OnMessage(MinersWife* wife, const Telegram& msg)
{
	return false;
}

CookStew* CookStew::Instance()
{
	static CookStew instance;

	return &instance;
}

void CookStew::Enter(MinersWife* wife)
{
	// �丮�� �����ϰ� �Ǹ� �����̰� �ִ� '�丮 �Ϸ�'�޽����� �ڽſ��� ������.
	if (!wife->Cooking())
	{
		cout << "\n" << GetNameOfEntity(wife->ID()) << ": Putting the stew in the oven";
		Dispatch->DispatchMessage(1.5, wife->ID(), wife->ID(), Msg_StewReady, NO_ADDITIONAL_INFO);
		wife->SetCooking(true);
	}
}

void CookStew::Execute(MinersWife* wife)
{
	// �丮�� ������ �� ���´� '�丮 �Ϸ�' �޽����� ������ �� ���� ����.
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": Fussin' over food";
}

void CookStew::Exit(MinersWife* wife)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": Puttin' the stew on the table";
}

bool CookStew::OnMessage(MinersWife* wife, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// '�丮' ������ �� '�丮 �Ϸ�' �޽����� �����ϰ� �Ǹ�,
	// ���ο� �Ƶ鿡�� 'ȣ�� �޽���'�� ��� �����ϰ� '�丮' ���¿��� �������´�.
	switch (msg.Msg)
	{
	case Msg_StewReady:
	{
		cout << "\nMessage received by " << GetNameOfEntity(wife->ID()) << " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(wife->ID()) << ": StewReady! Lets eat";

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, wife->ID(), ent_Miner_Bob, Msg_StewReady, NO_ADDITIONAL_INFO);
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, wife->ID(), ent_David, Msg_ComebackHome, NO_ADDITIONAL_INFO);

		wife->SetCooking(false);

		wife->GetFSM()->ChangeState(DoHouseWork::Instance());
	}
	return true;
	}

	return false;
}