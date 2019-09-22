#ifndef MINERSWIFE_OWNED_STATES_H
#define MINERSWIFE_OWNED_STATES_H

#include "../Common/FSM/State.h"

class MinersWife;

class WifesGlobalState : public State<MinersWife>
{
private:
	WifesGlobalState() {}
	WifesGlobalState(const WifesGlobalState&);
	WifesGlobalState& operator=(const WifesGlobalState&);

public:
	static WifesGlobalState* Instance();

	virtual void Enter(MinersWife* wife) {}

	virtual void Execute(MinersWife* wife);

	virtual void Exit(MinersWife* wife) {}

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};

class DoHouseWork : public State<MinersWife>
{
private:
	DoHouseWork() {}
	DoHouseWork(const DoHouseWork&);
	DoHouseWork& operator=(const DoHouseWork&);

public:
	static DoHouseWork* Instance();

	virtual void Enter(MinersWife* wife);

	virtual void Execute(MinersWife* wife);

	virtual void Exit(MinersWife* wife);

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);

};

class VisitBathroom : public State<MinersWife>
{
private:
	VisitBathroom() {}
	VisitBathroom(const VisitBathroom&);
	VisitBathroom& operator=(const VisitBathroom&);

public:
	static VisitBathroom* Instance();

	virtual void Enter(MinersWife* wife);

	virtual void Execute(MinersWife* wife);

	virtual void Exit(MinersWife* wife);

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);

};

class CookStew : public State<MinersWife>
{
private:
	CookStew() {}
	CookStew(const CookStew&);
	CookStew& operator=(const CookStew&);

public:
	static CookStew* Instance();

	virtual void Enter(MinersWife* wife);

	virtual void Execute(MinersWife* wife);

	virtual void Exit(MinersWife* wife);

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};


#endif