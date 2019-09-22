#include "MinerOwnedStates.h"
#include "State.h"
#include "Miner.h"
#include "Locations.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif



// =========== �� ä�� ===========

Enter_GoldMine_And_Dig_for_Nugget* Enter_GoldMine_And_Dig_for_Nugget::Instance() // �̱��� ��ü ����.
{
	static Enter_GoldMine_And_Dig_for_Nugget instance;
	return &instance;
}

void Enter_GoldMine_And_Dig_for_Nugget::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != goldmine)
	{
		pMiner->ChangeLocation(goldmine);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": �ݱ��� ���Դ�.";
	}
}

void Enter_GoldMine_And_Dig_for_Nugget::Execute(Miner* pMiner)
{
	pMiner->Increase_Fatigue(); // �Ƿε� ����.
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ��.. ��.. (�� ĳ�� ��..)";

	pMiner->Add_Gold_to_Pocket(1); // �� ȹ��.
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���� �����! (������: " << pMiner->Get_Gold_Pocket() << ")";

	if (pMiner->is_Tired()) // �켱������ �Ƿε��� ���� �׿��ٸ�,
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (���ƴ�..) ���� ������ �����Ѵ�!";
		pMiner->Get_FSM()->ChangeState(Go_Home_And_Sleep_til_Rested::Instance()); // ������ �̵�.
	}
	else if (pMiner->is_Gold_Pocket_Full()) // �ָӴϿ� ���� ���� á�ٸ�,
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": �ָӴϰ� ���� á��! �������� ���� �Ѵ�!";
		pMiner->Get_FSM()->ChangeState(Visit_Bank_And_Deposit_Gold::Instance()); // �������� �̵�.
	}
	else if (pMiner->is_Thirsty()) // ���� �����ٸ�,
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (���� ������..) �������� ���߰ڴ�!";
		pMiner->Get_FSM()->ChangeState(Quench_Thirst::Instance()); // '���� �ؼ�' ���·� ��ȯ.
	}
	else if (pMiner->is_Hungry()) // �谡 �����ٸ�,
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�谡 �ʹ� ������..) �Ĵ����� ���߰ڴ�!";
		pMiner->Get_FSM()->ChangeState(Relieve_Hunger::Instance()); // '���ָ� �ؼ�' ���·� ��ȯ.
	}

	// ��� �ƴ϶�� �� ä���� ��� �Ѵ�.
}

void Enter_GoldMine_And_Dig_for_Nugget::Exit(Miner* pMiner)
{
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (��..) �ݱ����� ���Դ�.";
}



// =========== �� ���� ===========

Visit_Bank_And_Deposit_Gold* Visit_Bank_And_Deposit_Gold::Instance()  // �̱��� ��ü ����.
{
	static Visit_Bank_And_Deposit_Gold instance;
	return &instance;
}

void Visit_Bank_And_Deposit_Gold::Enter(Miner* pMiner)
{
	if (pMiner->Get_Location() != bank)
	{
		pMiner->ChangeLocation(bank);
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���࿡ ���Դ�.";
	}
}

void Visit_Bank_And_Deposit_Gold::Execute(Miner* pMiner)
{
	pMiner->Add_Gold_in_Bank(pMiner->Get_Gold_Pocket());
	pMiner->Set_Gold_Pocket(0);
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���࿡ ���� �ð��! (���� �ܰ�: " << pMiner->Get_Gold_in_Bank() << ")";

	if (pMiner->is_Hungry())
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�谡 �ʹ� ������..) �Ĵ����� ���߰ڴ�!";
		pMiner->Get_FSM()->ChangeState(Relieve_Hunger::Instance());
	}
	else if (pMiner->is_Thirsty())
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (���� ������..) �������� ���߰ڴ�!";
		pMiner->Get_FSM()->ChangeState(Quench_Thirst::Instance());
	}
	else if (pMiner->is_Satisfied_to_Balance())
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ����! ���� ���� ����ϴ� ���� ����!";
		pMiner->Get_FSM()->ChangeState(Go_Home_And_Sleep_til_Rested::Instance());
	}
	else
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���� ������! �ٽ� ���� ĳ�� ������?";
		pMiner->Get_FSM()->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
	}
}

void Visit_Bank_And_Deposit_Gold::Exit(Miner* pMiner)
{
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���࿡�� ���Դ�.";
}



// =========== �Ƿ� �ؼ� ===========

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
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���� ���Դ�.";
	}
}

void Go_Home_And_Sleep_til_Rested::Execute(Miner* pMiner)
{
	if (!pMiner->is_Tired())
	{
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�� ���� ���� ��ħ!) ���� ĳ�� ����!";
		pMiner->Get_FSM()->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
	}
	else
	{
		while (pMiner->Decrease_Fatigue())
		{
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ZZz.. (�Ƿε�: " << pMiner->Get_Fatigue() << ")";
		}
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ZZz.. (�Ƿε�: " << pMiner->Get_Fatigue() << ")";
	}
}

void Go_Home_And_Sleep_til_Rested::Exit(Miner* pMiner)
{
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ������ ���Դ�.";
}



// =========== ���� �ؼ� ===========

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
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ������ ���Դ�.";
	}
}

void Quench_Thirst::Execute(Miner* pMiner)
{
	if (pMiner->is_Thirsty())
	{
		if (pMiner->Buy_and_Drink_a_Whiskey())
		{
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ����Ű�� ���� �缭 ���̴�. (������: " << pMiner->Get_Gold_Pocket() << ", ���� �ܰ�: " << pMiner->Get_Gold_in_Bank() << ")";

			if (pMiner->is_Hungry())
			{
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�赵 ������..) �� ������ ���߰ڴ�!";
				pMiner->Get_FSM()->ChangeState(Relieve_Hunger::Instance());
			}
			if (pMiner->is_Tired())
			{
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (���ߴ�..) ���� ���߰ڴ�..";
				pMiner->Get_FSM()->ChangeState(Go_Home_And_Sleep_til_Rested::Instance());
			}
			else
			{
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���� �ڴ±�! �ݱ����� ����!";

				if (pMiner->is_Gold_Pocket_Full())
				{
					cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�ָӴϰ� ���� ���ִ�.) �ݱ����� ���� ���� �켱 �ָӴϸ� ����߰ڴ�.";
					pMiner->Get_FSM()->ChangeState(Visit_Bank_And_Deposit_Gold::Instance());
				}
				else pMiner->Get_FSM()->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
			}
		}
		else
		{
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (����Ű�� �� ���� ���� ����..) ���� �������� �ݱ����� ���ư���..";
			pMiner->Get_FSM()->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
		}
	}
	else cout << "\nERROR!\nERROR!\nERROR!";
}

void Quench_Thirst::Exit(Miner* pMiner)
{
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": �������� ���Դ�.";
}



// =========== ���ָ� �ؼ� ===========

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
		cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": �Ĵ翡 ���Դ�.";
	}
}

void Relieve_Hunger::Execute(Miner* pMiner)
{
	if (pMiner->is_Hungry())
	{
		if (pMiner->Buy_and_Eat_Meal())
		{
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (����!) ���� ��Ծ���. (������: " << pMiner->Get_Gold_Pocket() << ", ���� �ܰ�: " << pMiner->Get_Gold_in_Bank() << ")";

			if (pMiner->is_Thirsty())
			{
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�� �����°� ����..) ������ ���߰ڴ�!";
				pMiner->Get_FSM()->ChangeState(Quench_Thirst::Instance());
			}
			else if (pMiner->is_Tired())
			{
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�Ļ縦 �ߴ��� ����� ������..) ������ ���ư��߰ڴ�..";
				pMiner->Get_FSM()->ChangeState(Go_Home_And_Sleep_til_Rested::Instance());
			}
			else
			{
				cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": ���� �ڴ±�! �ݱ����� ����!";

				if (pMiner->is_Gold_Pocket_Full())
				{
					cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�ָӴϰ� ���� ���ִ�.) �ݱ����� ���� ���� �켱 �ָӴϸ� ����߰ڴ�.";
					pMiner->Get_FSM()->ChangeState(Visit_Bank_And_Deposit_Gold::Instance());
				}
				else pMiner->Get_FSM()->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
			}
		}
		else
		{
			cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": (�Ļ縦 �� ���� ���� ����..) �谡 �������� �ݱ����� ���ư���..";
			pMiner->Get_FSM()->ChangeState(Enter_GoldMine_And_Dig_for_Nugget::Instance());
		}
	}
	else cout << "\nERROR!\nERROR!\nERROR!";
}

void Relieve_Hunger::Exit(Miner* pMiner)
{
	cout << "\n" << Get_Name_of_Entity(pMiner->Get_ID()) << ": �Ĵ翡�� ���Դ�.";
}

