#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   개체의 상태를 관리(변경)하기 위한 기본적인 '추상 클래스'.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

template <class entity_type> // 다양한 개체를 정의할 수 있도록 template 객체로 구현.
class State
{
public:
	virtual ~State(){}

	virtual void Enter(entity_type*) = 0;
	virtual void Execute(entity_type*) = 0;
	virtual void Exit(entity_type*) = 0;
};

#endif