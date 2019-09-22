#include "MinerOwnedStates.h"
#include "State.h"
#include "Miner.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif



// =========== 금 채굴 ===========

Enter_GoldMine_And_Dig_for_Nugget* Enter_GoldMine_And_Dig_for_Nugget::Instance() // 싱글톤 객체 생성.
{
	static Enter_GoldMine_And_Dig_for_Nugget instance;
	return &instance;
}

void Enter_GoldMine_And_Dig_for_Nugget::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != goldmine)
	{
		pMiner->ChangeLocation(goldmine);
		SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "\n\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 금광에 들어왔다.";
	}
}

void Enter_GoldMine_And_Dig_for_Nugget::Execute(Miner* pMiner)
{
	pMiner->Increase_Fatigue(); // 피로도 증가.
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 쾅.. 쾅.. (금 캐는 중..)";
	
	pMiner->Add_Gold_to_Pocket(1); // 금 획득.
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 금을 얻었다!";
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
	cout << " (소지금: " << pMiner->Get_Gold_Pocket() << ")";
	
	if (pMiner->is_Tired()) // 우선적으로 피로도가 많이 쌓였다면,
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (지쳤다..) 당장 집으로 가야한다!";
		pMiner->ChangeState(Go_Home_And_Sleep_til_Rested::Instance()); // 집으로 이동.
	}
	else if (pMiner->is_Gold_Pocket_Full()) // 주머니에 금이 가득 찼다면,
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 주머니가 가득 찼다! 은행으로 가야 한다!";
		pMiner->ChangeState(Visit_Bank_And_Deposit_Gold::Instance()); // 은행으로 이동.
	}
	else if (pMiner->is_Thirsty()) // 목이 마르다면,
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (목이 마르다..) 주점으로 가야겠다!";
		pMiner->ChangeState(Quench_Thirst::Instance()); // '갈증 해소' 상태로 전환.
	}
	else if (pMiner->is_Hungry()) // 배가 고프다면,
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (배가 너무 고프다..) 식당으로 가야겠다!";
		pMiner->ChangeState(Relieve_Hunger::Instance()); // '굶주림 해소' 상태로 전환.
	}

	// 모두 아니라면 금 채굴을 계속 한다.
}

void Enter_GoldMine_And_Dig_for_Nugget::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (후..) 금광에서 나왔다.";
}



// =========== 금 저축 ===========

Visit_Bank_And_Deposit_Gold* Visit_Bank_And_Deposit_Gold::Instance()  // 싱글톤 객체 생성.
{
	static Visit_Bank_And_Deposit_Gold instance;
	return &instance;
}

void Visit_Bank_And_Deposit_Gold::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != bank)
	{
		pMiner->ChangeLocation(bank);
		SetTextColor(FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "\n\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 은행에 들어왔다.";
	}
}

void Visit_Bank_And_Deposit_Gold::Execute(Miner* pMiner)
{
	pMiner->Add_Gold_in_Bank(pMiner->Get_Gold_Pocket());
	pMiner->Set_Gold_Pocket(0);
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 은행에 금을 맡겼다!";
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_RED);
	cout << " (은행 잔고: " << pMiner->Get_Gold_in_Bank() << ")";

	if (pMiner->is_Hungry())
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (배가 너무 고프다..) 식당으로 가야겠다!";
		pMiner->ChangeState(Relieve_Hunger::Instance());
	}
	else if (pMiner->is_Thirsty())
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (목이 마르다..) 주점으로 가야겠다!";
		pMiner->ChangeState(Quench_Thirst::Instance());
	}
	else if (pMiner->is_Satisfied_to_Balance())
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 좋아! 돈은 이제 충분하니 집에 가자!";
		pMiner->ChangeState(Go_Home_And_Sleep_til_Rested::Instance());
	}
	else 
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 아직 부족해! 다시 금을 캐러 가볼까?";
		pMiner->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
	}
}

void Visit_Bank_And_Deposit_Gold::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_RED);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 은행에서 나왔다.";
}



// =========== 피로 해소 ===========

Go_Home_And_Sleep_til_Rested* Go_Home_And_Sleep_til_Rested::Instance()
{
	static Go_Home_And_Sleep_til_Rested instance;
	return &instance;
}

void Go_Home_And_Sleep_til_Rested::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != shack)
	{
		pMiner->ChangeLocation(shack);
		SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\n\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 집에 들어왔다.";
	}
}

void Go_Home_And_Sleep_til_Rested::Execute(Miner* pMiner)
{ 
	if (!pMiner->is_Tired())
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (힘 세고 강한 아침!) 금을 캐러 가자!";
		pMiner->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
	}
	else
	{
		while (pMiner->Decrease_Fatigue())
		{
			SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ZZz.. (피로도: " << pMiner->Get_Fatigue() << ")";
		}
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ZZz.. (피로도: " << pMiner->Get_Fatigue() << ")";
	}
}

void Go_Home_And_Sleep_til_Rested::Exit(Miner* pMiner)
{ 
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 집에서 나왔다.";
}



// =========== 갈증 해소 ===========

Quench_Thirst* Quench_Thirst::Instance()
{
	static Quench_Thirst instance;
	return &instance;
}

void Quench_Thirst::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != saloon)
	{    
		pMiner->ChangeLocation(saloon);
		SetTextColor(FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "\n\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 주점에 들어왔다.";
	}
}

void Quench_Thirst::Execute(Miner* pMiner)
{
	if (pMiner->is_Thirsty())
	{
		if (pMiner->Buy_and_Drink_a_Whiskey())
		{
			SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 위스키를 한잔 사서 마셨다.";
			SetTextColor(FOREGROUND_RED | FOREGROUND_BLUE);
			cout << " (소지금: " << pMiner->Get_Gold_Pocket() << ", 은행 잔고: " << pMiner->Get_Gold_in_Bank() << ")";

			if (pMiner->is_Hungry())
			{
				SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (배도 고프다..) 밥 먹으러 가야겠다!";
				pMiner->ChangeState(Relieve_Hunger::Instance());
			}
			if (pMiner->is_Tired())
			{
				SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (취했다..) 집에 가야겠다..";
				pMiner->ChangeState(Go_Home_And_Sleep_til_Rested::Instance());
			}
			else
			{
				SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 힘이 솟는군! 금광으로 가자!";

				if (pMiner->is_Gold_Pocket_Full())
				{
					cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (주머니가 가득 차있다.) 금광으로 가기 전에 우선 주머니를 비워야겠다.";
					pMiner->ChangeState(Visit_Bank_And_Deposit_Gold::Instance());
				}
				else pMiner->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
			}
		}
		else
		{
			SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (위스키를 살 돈이 전혀 없다..) 목이 마르지만 금광으로 돌아가자..";
			pMiner->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
		}
	}
	else
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "\nERROR!\nERROR!\nERROR!";
	} 
}

void Quench_Thirst::Exit(Miner* pMiner)
{ 
	SetTextColor(FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 주점에서 나왔다.";
}



// =========== 굶주림 해소 ===========

Relieve_Hunger* Relieve_Hunger::Instance()
{
	static Relieve_Hunger instance;
	return &instance;
}

void Relieve_Hunger::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != restaurant)
	{
		pMiner->ChangeLocation(restaurant);
		SetTextColor(FOREGROUND_GREEN);
		cout << "\n\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 식당에 들어왔다.";
	}
}

void Relieve_Hunger::Execute(Miner* pMiner)
{
	if (pMiner->is_Hungry())
	{
		if (pMiner->Buy_and_Eat_Meal())
		{
			SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (꺼억!) 밥을 사먹었다.";
			SetTextColor(FOREGROUND_GREEN);
			cout << " (소지금: " << pMiner->Get_Gold_Pocket() << ", 은행 잔고: " << pMiner->Get_Gold_in_Bank() << ")";
			
			if (pMiner->is_Thirsty())
			{
				SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (목도 마르는것 같다..) 주점에 가야겠다!";
				pMiner->ChangeState(Quench_Thirst::Instance());
			}
			else if (pMiner->is_Tired())
			{
				SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (식사를 했더니 노곤노곤 해졌다..) 집으로 돌아가야겠다..";
				pMiner->ChangeState(Go_Home_And_Sleep_til_Rested::Instance());
			}
			else
			{
				SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 힘이 솟는군! 금광으로 가자!";

				if (pMiner->is_Gold_Pocket_Full())
				{
					cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (주머니가 가득 차있다.) 금광으로 가기 전에 우선 주머니를 비워야겠다.";
					pMiner->ChangeState(Visit_Bank_And_Deposit_Gold::Instance());
				}
				else pMiner->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
			}
		}
		else
		{
			SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (식사를 할 돈이 전혀 없다..) 배가 고프지만 금광으로 돌아가자..";
			pMiner->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
		}
	}
	else
	{
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\nERROR!\nERROR!\nERROR!";
	}
}

void Relieve_Hunger::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_GREEN);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": 식당에서 나왔다.";
}

