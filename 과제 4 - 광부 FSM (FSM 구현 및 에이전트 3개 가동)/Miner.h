#ifndef MINER_H
#define MINER_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   광부 개체 클래스. ('BaseGameEntity' 클래스를 상속한다.)
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

const int Max_Gold = 10; // 금 소지량 최댓값
const int Balance_Satisfaction_Threshold = 10; // 은행 잔고 만족도 임계값
const int Thirst_Threshold = 6; // 목마름 임계값
const int Tiredness_Threshold = 15; // 피로도 임계값
const int Hunger_Threshold = 8; // 배고픔 임계값
const int Whiskey_Price = 2; // 위스키 가격
const int Meal_Price = 2; // 식사 가격


class Miner : public BaseGameEntity
{
private:
	StateMachine<Miner>*  m_pStateMachine; // Miner의 상태 기계
	location_type m_Location; // 현 위치

	int m_Gold_Pocket; // 소지금
	int m_Gold_in_Bank; // 은행 잔고

	int m_Thirst; // 목마름 수치
	int m_Fatigue; // 피로도
	int	m_Hunger; // 배고픔 수치

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
