#ifndef MINERSSON_OWNED_STATES_H
#define MINERSSON_OWNED_STATES_H

#include "../Common/FSM/State.h"

class MinersSon;

class SonsGlobalState : public State<MinersSon>
{
private:
	SonsGlobalState() {}
	SonsGlobalState(const SonsGlobalState&);
	SonsGlobalState& operator=(const SonsGlobalState&);

public:
	static SonsGlobalState* Instance();

	virtual void Enter(MinersSon* son) {}

	virtual void Execute(MinersSon* son);

	virtual void Exit(MinersSon* son) {}

	virtual bool OnMessage(MinersSon* son, const Telegram& msg);
};

class PlayAtPlayground : public State<MinersSon>
{
private:
	PlayAtPlayground() {}
	PlayAtPlayground(const PlayAtPlayground&);
	PlayAtPlayground& operator=(const PlayAtPlayground&);

public:
	static PlayAtPlayground* Instance();

	virtual void Enter(MinersSon* son);

	virtual void Execute(MinersSon* son);

	virtual void Exit(MinersSon* son);

	virtual bool OnMessage(MinersSon* son, const Telegram& msg);
};

class WashHands : public State<MinersSon>
{
private:
	WashHands() {}
	WashHands(const WashHands&);
	WashHands& operator=(const WashHands&);

public:
	static WashHands* Instance();

	virtual void Enter(MinersSon* son);

	virtual void Execute(MinersSon* son);

	virtual void Exit(MinersSon* son);

	virtual bool OnMessage(MinersSon* son, const Telegram& msg);
};

class EatMeal : public State<MinersSon>
{
private:
	EatMeal() {}
	EatMeal(const EatMeal&);
	EatMeal& operator=(const EatMeal&);

public:
	static EatMeal* Instance();

	virtual void Enter(MinersSon* son);

	virtual void Execute(MinersSon* son);

	virtual void Exit(MinersSon* son);

	virtual bool OnMessage(MinersSon* son, const Telegram& msg);
};

#endif