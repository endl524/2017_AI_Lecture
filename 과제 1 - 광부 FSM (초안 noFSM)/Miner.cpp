#include "Miner.h"
#include "MinerOwnedStates.h"

Miner::Miner(int id) : BaseGameEntity(id), m_Location(shack), m_Gold_Pocket(0), 
m_Gold_in_Bank(0), m_Thirst(0), m_Fatigue(0), m_Hunger(0), m_pCurrentState(Enter_GoldMine_And_Dig_for_Nugget::Instance()) {}

void Miner::Update()
{
	m_Thirst += 1;
	m_Hunger += 1;

	if (m_pCurrentState) m_pCurrentState->Execute(this);
}

void Miner::ChangeState(State* pNewState)
{
	assert (m_pCurrentState && pNewState);
	m_pCurrentState->Exit(this);
	m_pCurrentState = pNewState;
	m_pCurrentState->Enter(this);
}

bool Miner::is_Satisfied_to_Balance() const
{
	if (m_Gold_in_Bank >= Balance_Satisfaction_Threshold) return true;
	return false;
}

void Miner::Add_Gold_to_Pocket(const int& val)
{
	m_Gold_Pocket += val;
	if (m_Gold_Pocket < 0) m_Gold_Pocket = 0;
}

void Miner::Add_Gold_in_Bank(const int& val)
{
	m_Gold_in_Bank += val;
	if (m_Gold_in_Bank < 0) m_Gold_in_Bank = 0;
}

bool Miner::is_Tired() const
{
	if (m_Fatigue >= Tiredness_Threshold) return true;
	return false;
}

bool Miner::is_Thirsty() const
{
	if (m_Thirst >= Thirst_Threshold) { return true; }
	return false;
}

bool Miner::Buy_and_Drink_a_Whiskey()
{
	if (m_Gold_Pocket >= Whiskey_Price) m_Gold_Pocket -= Whiskey_Price;
	else if (m_Gold_in_Bank >= Whiskey_Price) m_Gold_in_Bank -= Whiskey_Price;
	else return false;

	m_Thirst = 0;
	return true;
}

bool Miner::is_Hungry() const
{
	if (m_Hunger > Hunger_Threshold) return true;
	return false;
}

bool Miner::Buy_and_Eat_Meal()
{
	if (m_Gold_Pocket >= Whiskey_Price) m_Gold_Pocket -= Whiskey_Price;
	else if (m_Gold_in_Bank >= Whiskey_Price) m_Gold_in_Bank -= Whiskey_Price;
	else return false;

	m_Hunger = 0;
	return true;
}