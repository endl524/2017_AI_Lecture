#include "MinersSonOwnedStates.h"
#include "MinerOwnedStates.h"
#include "MinersSon.h"
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

SonsGlobalState* SonsGlobalState::Instance()
{
	static SonsGlobalState instance;

	return &instance;
}

void SonsGlobalState::Execute(MinersSon* son)
{
	if ((RandFloat() < 0.1) && !son->GetFSM()->isInState(*WashHands::Instance()))
	{
		son->GetFSM()->ChangeState(WashHands::Instance());
	}
}

bool SonsGlobalState::OnMessage(MinersSon* son, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_ComebackHome:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(son->ID()) << " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(son->ID()) << ": I'm Going Mom!";

		son->GetFSM()->ChangeState(EatMeal::Instance());
	}
	return true;
	}

	return false;
}

PlayAtPlayground* PlayAtPlayground::Instance()
{
	static PlayAtPlayground instance;

	return &instance;
}

void PlayAtPlayground::Enter(MinersSon* son)
{
	if (son->Location() != playground)
	{
		son->ChangeLocation(playground);
		cout << "\n" << GetNameOfEntity(son->ID()) << ": " << "������ ���� ����!";
	}
}

void PlayAtPlayground::Execute(MinersSon* son)
{
	switch (RandInt(0, 2))
	{
	case 0:
		cout << "\n" << GetNameOfEntity(son->ID()) << ": �̲���Ʋ�� ����.";
		break;

	case 1:
		cout << "\n" << GetNameOfEntity(son->ID()) << ": �׳׸� ����.";
		break;

	case 2:
		cout << "\n" << GetNameOfEntity(son->ID()) << ": �𷡼��� �׾Ҵ�.";
		break;
	}
}

void PlayAtPlayground::Exit(MinersSon* son)
{
}

bool PlayAtPlayground::OnMessage(MinersSon* son, const Telegram& msg)
{
	return false;
}

//=================================

WashHands* WashHands::Instance()
{
	static WashHands instance;

	return &instance;
}

void WashHands::Enter(MinersSon* son)
{
	cout << "\n" << GetNameOfEntity(son->ID()) << ": �� ������ ����!";
}

void WashHands::Execute(MinersSon* son)
{
	cout << "\n" << GetNameOfEntity(son->ID()) << ": (����)";

	son->GetFSM()->RevertToPreviousState();
}

void WashHands::Exit(MinersSon* son)
{
	cout << "\n" << GetNameOfEntity(son->ID()) << ": ���� ����������.";
}

bool WashHands::OnMessage(MinersSon* son, const Telegram& msg)
{
	return false;
}

//=================================

EatMeal* EatMeal::Instance()
{
	static EatMeal instance;

	return &instance;
}

void EatMeal::Enter(MinersSon* son)
{
	// ���� ��ġ�� ���̶�� ���� �Ʒ� ������ ������ �ʿ䰡 ����.
	// (= ������ ����ϰ� ������ �� �ִ�.)
	if (son->Location() != shack)
	{
		cout << "\n" << GetNameOfEntity(son->ID()) << ": ���� ����";
		son->ChangeLocation(shack);
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, son->ID(), ent_Elsa, Msg_ImGoing, NO_ADDITIONAL_INFO);
	}
}

void EatMeal::Execute(MinersSon* son)
{
	cout << "\n" << GetNameOfEntity(son->ID()) << ": ���ִ�";
	son->GetFSM()->RevertToPreviousState();
}

void EatMeal::Exit(MinersSon* son)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	cout << "\n" << GetNameOfEntity(son->ID()) << ": �� �Ծ���.";
}

bool EatMeal::OnMessage(MinersSon* son, const Telegram& msg)
{
	return false;
}