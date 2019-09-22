#ifndef MINER_OWNED_STATES_H
#define MINER_OWNED_STATES_H

#include "../Common/FSM/State.h"

class Miner;
struct Telegram;

class EnterMineAndDigForNugget : public State<Miner>
{
private:
  EnterMineAndDigForNugget(){}
  EnterMineAndDigForNugget(const EnterMineAndDigForNugget&);
  EnterMineAndDigForNugget& operator=(const EnterMineAndDigForNugget&);
 
public:
  static EnterMineAndDigForNugget* Instance();

  virtual void Enter(Miner* miner);

  virtual void Execute(Miner* miner);

  virtual void Exit(Miner* miner);

  virtual bool OnMessage(Miner* agent, const Telegram& msg);
};

class VisitBankAndDepositGold : public State<Miner>
{
private:
	VisitBankAndDepositGold() {}
	VisitBankAndDepositGold(const VisitBankAndDepositGold&);
	VisitBankAndDepositGold& operator=(const VisitBankAndDepositGold&);

public:
	static VisitBankAndDepositGold* Instance();
	virtual void Enter(Miner* miner);
	virtual void Execute(Miner* miner);
	virtual void Exit(Miner* miner);
	virtual bool OnMessage(Miner* agent, const Telegram& msg);
};

class GoHomeAndSleepTilRested : public State<Miner>
{
private:
	GoHomeAndSleepTilRested() {}
	GoHomeAndSleepTilRested(const GoHomeAndSleepTilRested&);
	GoHomeAndSleepTilRested& operator=(const GoHomeAndSleepTilRested&);

public:
	static GoHomeAndSleepTilRested* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* agent, const Telegram& msg);
};

class QuenchThirst : public State<Miner>
{
private:
	QuenchThirst() {}
	QuenchThirst(const QuenchThirst&);
	QuenchThirst& operator=(const QuenchThirst&);

public:
	static QuenchThirst* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* agent, const Telegram& msg);
};

class EatStew : public State<Miner>
{
private:
	EatStew() {}
	EatStew(const EatStew&);
	EatStew& operator=(const EatStew&);

public:
	static EatStew* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* agent, const Telegram& msg);
};

#endif