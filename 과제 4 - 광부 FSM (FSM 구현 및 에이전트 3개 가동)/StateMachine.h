#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//------------------------------------------------------------------------
//
//  Name:   StateMachine.h
//
//  Desc:   ���±�� Ŭ������ �����Ѵ�.
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
	entity_type* m_pOwner; // ���� ���±���� ������

	State<entity_type>* m_pCurrentState; // ���� ����
	State<entity_type>* m_pPreviousState; // ���� ����
	State<entity_type>* m_pGlobalState; // ���� ���� (�׻� �����Ѵ�.)

public:
	StateMachine(entity_type* owner) : m_pOwner(owner), m_pCurrentState(NULL), m_pPreviousState(NULL), m_pGlobalState(NULL) {}
	virtual ~StateMachine(){}

	void SetCurrentState(State<entity_type>* s) { m_pCurrentState = s; }
	void SetGlobalState(State<entity_type>* s) { m_pGlobalState = s; }
	void SetPreviousState(State<entity_type>* s) { m_pPreviousState = s; }

	void Update() const
	{
		// �������°� �����Ǿ��ִٸ�, �������� ����.
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOwner);

		// ������°� �����Ǿ��ִٸ�, ������� ����.
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine::ChangeState>: trying to change to NULL state");
		
		m_pPreviousState = m_pCurrentState; // ���� ���¸� ����صд�.
		m_pCurrentState->Exit(m_pOwner); // ���� ���¿��� ��������,
		m_pCurrentState = pNewState; // ���ο� ���·�
		m_pCurrentState->Enter(m_pOwner); // �����Ѵ�.
	}

	void RevertToPreviousState() { ChangeState(m_pPreviousState); } // �������·� �����Ѵ�.

	bool isInState(const State<entity_type>& st) const { return typeid(*m_pCurrentState) == typeid(st); }
	State<entity_type>* CurrentState() const { return m_pCurrentState; }
	State<entity_type>* GlobalState() const { return m_pGlobalState; }
	State<entity_type>* PreviousState() const { return m_pPreviousState; }
};

#endif