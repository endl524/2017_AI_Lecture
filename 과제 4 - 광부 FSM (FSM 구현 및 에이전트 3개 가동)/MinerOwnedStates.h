#ifndef MINER_OWNED_STATES_H
#define MINER_OWNED_STATES_H

//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   ������ �ൿ(����)���� ������ Ŭ������ �����Ѵ�. ('State' Ŭ������ ����Ѵ�.)
//			=> �� Ŭ������ '�̱���'���� �����Ѵ�.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include "State.h"

class Miner;

// =========== �� ä�� ===========
class Enter_GoldMine_And_Dig_for_Nugget : public State<Miner>
{
private:
	Enter_GoldMine_And_Dig_for_Nugget() {}
	Enter_GoldMine_And_Dig_for_Nugget(const Enter_GoldMine_And_Dig_for_Nugget&);
	Enter_GoldMine_And_Dig_for_Nugget& operator=(const Enter_GoldMine_And_Dig_for_Nugget&);

public:
	static Enter_GoldMine_And_Dig_for_Nugget* Instance();

	virtual void Enter(Miner* miner);
	virtual void Execute(Miner* miner);
	virtual void Exit(Miner* miner);
};


// =========== �� ���� ===========
class Visit_Bank_And_Deposit_Gold : public State<Miner>
{
private:
	Visit_Bank_And_Deposit_Gold() {}
	Visit_Bank_And_Deposit_Gold(const Visit_Bank_And_Deposit_Gold&);
	Visit_Bank_And_Deposit_Gold& operator=(const Visit_Bank_And_Deposit_Gold&);

public:
	static Visit_Bank_And_Deposit_Gold* Instance();

	virtual void Enter(Miner* miner);
	virtual void Execute(Miner* miner);
	virtual void Exit(Miner* miner);
};


// =========== �Ƿ� �ؼ� ===========
class Go_Home_And_Sleep_til_Rested : public State<Miner>
{
private:
	Go_Home_And_Sleep_til_Rested() {}
	Go_Home_And_Sleep_til_Rested(const Go_Home_And_Sleep_til_Rested&);
	Go_Home_And_Sleep_til_Rested& operator=(const Go_Home_And_Sleep_til_Rested&);

public:
	static Go_Home_And_Sleep_til_Rested* Instance();

	virtual void Enter(Miner* miner);
	virtual void Execute(Miner* miner);
	virtual void Exit(Miner* miner);
};


// =========== ���� �ؼ� ===========
class Quench_Thirst : public State<Miner>
{
private:
	Quench_Thirst() {}
	Quench_Thirst(const Quench_Thirst&);
	Quench_Thirst& operator=(const Quench_Thirst&);

public:
	static Quench_Thirst* Instance();
	virtual void Enter(Miner* miner);
	virtual void Execute(Miner* miner);
	virtual void Exit(Miner* miner);
};


// =========== ���ָ� �ؼ� ===========
class Relieve_Hunger : public State<Miner>
{
private:
	Relieve_Hunger() {}
	Relieve_Hunger(const Relieve_Hunger&);
	Relieve_Hunger& operator=(const Relieve_Hunger&);

public:
	static Relieve_Hunger* Instance();

	virtual void Enter(Miner* miner);
	virtual void Execute(Miner* miner);
	virtual void Exit(Miner* miner);
};

#endif