#ifndef MINERSSON_OWNED_STATES_H
#define MINERSSON_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersSonOwnedStates.h
//
//  Desc:   ���ο� ���������� ���� �Ƶ��� �ൿ(����)���� ������ Ŭ������ �����Ѵ�.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include "State.h"

class MinersSon;


// =========== �������� ===========
class Sons_GlobalState : public State<MinersSon>
{
private:
	Sons_GlobalState() {}
	Sons_GlobalState(const Sons_GlobalState&);
	Sons_GlobalState& operator=(const Sons_GlobalState&);

public:
	static Sons_GlobalState* Instance();
	virtual void Enter(MinersSon* son) {}
	virtual void Execute(MinersSon* son);
	virtual void Exit(MinersSon* son) {}
};



// =========== ������ ===========
class Play_at_Playground : public State<MinersSon>
{

private:
	Play_at_Playground() {}
	Play_at_Playground(const Play_at_Playground&);
	Play_at_Playground& operator=(const Play_at_Playground&);

public:
	static Play_at_Playground* Instance();
	virtual void Enter(MinersSon* son);
	virtual void Execute(MinersSon* son);
	virtual void Exit(MinersSon* son);
};



// =========== �� �ı� ===========
class Wash_Hands : public State<MinersSon>
{
private:
	Wash_Hands() {}
	Wash_Hands(const Wash_Hands&);
	Wash_Hands& operator=(const Wash_Hands&);

public:
	static Wash_Hands* Instance();
	virtual void Enter(MinersSon* son);
	virtual void Execute(MinersSon* son);
	virtual void Exit(MinersSon* son);
};

#endif