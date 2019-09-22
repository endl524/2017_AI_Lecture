#ifndef MINERSSON_OWNED_STATES_H
#define MINERSSON_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersSonOwnedStates.h
//
//  Desc:   광부와 마찬가지로 광부 아들의 행동(상태)들을 각각의 클래스로 정의한다.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include "State.h"

class MinersSon;


// =========== 전역상태 ===========
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



// =========== 놀이터 ===========
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



// =========== 손 씻기 ===========
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