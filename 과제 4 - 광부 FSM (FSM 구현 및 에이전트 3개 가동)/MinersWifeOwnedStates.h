#ifndef MINERSWIFE_OWNED_STATES_H
#define MINERSWIFE_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   ���ο� ���������� ���� �Ƴ��� �ൿ(����)���� ������ Ŭ������ �����Ѵ�.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "State.h"

class MinersWife;

// =========== �������� ===========
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



// =========== �� �� �� ===========
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



// =========== ȭ��� ===========
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