#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//------------------------------------------------------------------------
//
//  Name:   StateMachine.h
//
//  Desc:   상태기계 클래스를 정의한다.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cassert>
#include <string>

#include "State.h"

template <class entity_type>
class StateMachine
{
private:
	entity_type* m_pOwner; // 현재 상태기계의 소유자

	State<entity_type>* m_pCurrentState; // 현재 상태
	State<entity_type>* m_pPreviousState; // 이전 상태
	State<entity_type>* m_pGlobalState; // 전역 상태 (항상 동작한다.)

public:
	StateMachine(entity_type* owner) : m_pOwner(owner), m_pCurrentState(NULL), m_pPreviousState(NULL), m_pGlobalState(NULL) {}
	virtual ~StateMachine(){}

	void SetCurrentState(State<entity_type>* s) { m_pCurrentState = s; }
	void SetGlobalState(State<entity_type>* s) { m_pGlobalState = s; }
	void SetPreviousState(State<entity_type>* s) { m_pPreviousState = s; }

	void Update() const
	{
		// 전역상태가 설정되어있다면, 전역상태 수행.
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOwner);

		// 현재상태가 설정되어있다면, 현재상태 수행.
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine::ChangeState>: trying to change to NULL state");
		
		m_pPreviousState = m_pCurrentState; // 이전 상태를 기억해둔다.
		m_pCurrentState->Exit(m_pOwner); // 현재 상태에서 빠져나와,
		m_pCurrentState = pNewState; // 새로운 상태로
		m_pCurrentState->Enter(m_pOwner); // 진입한다.
	}

	void RevertToPreviousState() { ChangeState(m_pPreviousState); } // 이전상태로 복귀한다.

	bool isInState(const State<entity_type>& st) const { return typeid(*m_pCurrentState) == typeid(st); }
	State<entity_type>* CurrentState() const { return m_pCurrentState; }
	State<entity_type>* GlobalState() const { return m_pGlobalState; }
	State<entity_type>* PreviousState() const { return m_pPreviousState; }
};

#endif