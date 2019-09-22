#ifndef MINERSWIFE_OWNED_STATES_H
#define MINERSWIFE_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   광부와 마찬가지로 광부 아내의 행동(상태)들을 각각의 클래스로 정의한다.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "State.h"

class MinersWife;

// =========== 전역상태 ===========
class Wifes_GlobalState : public State<MinersWife>
{  
private:
	Wifes_GlobalState() {}
	Wifes_GlobalState(const Wifes_GlobalState&);
	Wifes_GlobalState& operator=(const Wifes_GlobalState&);
 
public:
	static Wifes_GlobalState* Instance();
	virtual void Enter(MinersWife* wife) {}
	virtual void Execute(MinersWife* wife);
	virtual void Exit(MinersWife* wife) {}
};



// =========== 집 안 일 ===========
class Do_House_Work : public State<MinersWife>
{
private:
	Do_House_Work(){}
	Do_House_Work(const Do_House_Work&);
	Do_House_Work& operator=(const Do_House_Work&); 
  
public:
	static Do_House_Work* Instance();

	virtual void Enter(MinersWife* wife);
	virtual void Execute(MinersWife* wife);
	virtual void Exit(MinersWife* wife);
};



// =========== 화장실 ===========
class Visit_Bathroom : public State<MinersWife>
{
private:
	Visit_Bathroom() {}
	Visit_Bathroom(const Visit_Bathroom&);
	Visit_Bathroom& operator=(const Visit_Bathroom&);
 
public:
	static Visit_Bathroom* Instance();
  
	virtual void Enter(MinersWife* wife);
	virtual void Execute(MinersWife* wife);
	virtual void Exit(MinersWife* wife);
};

#endif