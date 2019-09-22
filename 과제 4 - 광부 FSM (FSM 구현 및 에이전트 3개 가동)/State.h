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

template <class entity_type> // �پ��� ��ü�� ������ �� �ֵ��� template ��ü�� ����.
class State
{
public:
	virtual ~State(){}

	virtual void Enter(entity_type*) = 0;
	virtual void Execute(entity_type*) = 0;
	virtual void Exit(entity_type*) = 0;
};

#endif