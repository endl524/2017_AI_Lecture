#ifndef MINER_H
#define MINER_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   ���� ��ü Ŭ����. ('BaseGameEntity' Ŭ������ ����Ѵ�.)
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinerOwnedStates.h"
#include "StateMachine.h"

const int Max_Gold = 10; // �� ������ �ִ�
const int Balance_Satisfaction_Threshold = 10; // ���� �ܰ� ������ �Ӱ谪
const int Thirst_Threshold = 6; // �񸶸� �Ӱ谪
const int Tiredness_Threshold = 15; // �Ƿε� �Ӱ谪
const int Hunger_Threshold = 8; // ����� �Ӱ谪
const int Whiskey_Price = 2; // ����Ű ����
const int Meal_Price = 2; // �Ļ� ����


class Miner : public BaseGameEntity
{
private:
	StateMachine<Miner>*  m_pStateMachine; // Miner�� ���� ���
	location_type m_Location; // �� ��ġ

	int m_Gold_Pocket; // ������
	int m_Gold_in_Bank; // ���� �ܰ�

	int m_Thirst; // �񸶸� ��ġ
	int m_Fatigue; // �Ƿε�
	int	m_Hunger; // ����� ��ġ

public:
	Miner::Miner(int id) : BaseGameEntity(id), m_Location(shack), m_Gold_Pocket(0), m_Gold_in_Bank(0), m_Thirst(0), m_Fatigue(0), m_Hunger(0)
	{
		m_pStateMachine = new StateMachine<Miner>(this);
		m_pStateMachine->SetCurrentState(Go_Home_And_Sleep_til_Rested::Instance());
	}
	~Miner(){ delete m_pStateMachine; }

	void Update();

	StateMachine<Miner>* Get_FSM() const { return m_pStateMachine; }

	location_type Get_Location() const { return m_Location; }
	void ChangeLocation(const location_type loc) { m_Location = loc; }

	int Get_Gold_Pocket() const { return m_Gold_Pocket; }
	void Set_Gold_Pocket(const int& val) { m_Gold_Pocket = val; }
	void Add_Gold_to_Pocket(const int& val);
	bool is_Gold_Pocket_Full() const { return m_Gold_Pocket >= Max_Gold; }

	bool is_Tired() const;
	int Decrease_Fatigue() { m_Fatigue -= 1; return m_Fatigue; }
	void Increase_Fatigue() { m_Fatigue += 1; }
	int Get_Fatigue() const { return m_Fatigue; }

	bool is_Satisfied_to_Balance() const;
	void Set_Gold_in_Bank(const int& val) { m_Gold_in_Bank = val; }
	void Add_Gold_in_Bank(const int& val);
	int Get_Gold_in_Bank() const { return m_Gold_in_Bank; }

	bool is_Thirsty() const;
	bool Buy_and_Drink_a_Whiskey();

	bool is_Hungry() const;
	bool Buy_and_Eat_Meal();
};

#endif
