#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   ��ü�� ���¸� ����(����)�ϱ� ���� �⺻���� '�߻� Ŭ����'.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

class Miner;

class State
{
public:
	virtual ~State(){}

	virtual void Enter(Miner*) = 0;
	virtual void Execute(Miner*) = 0;
	virtual void Exit(Miner*) = 0;
};

#endif