#ifndef MINERSWIFE_H
#define MINERSWIFE_H

#include <string>

#include "../Common/FSM/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinersWifeOwnedStates.h"
#include "../Common/misc/ConsoleUtils.h"
#include "Miner.h"
#include "../Common/FSM/StateMachine.h"
#include "../Common/misc/Utils.h"

class MinersWife : public BaseGameEntity
{
private:
	StateMachine<MinersWife>* m_pStateMachine;
	location_type   m_Location;
	bool            m_bCooking;

public:
	MinersWife(int id) : m_Location(shack), m_bCooking(false), BaseGameEntity(id)
	{
		m_pStateMachine = new StateMachine<MinersWife>(this);
		m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
		m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());
	}
	~MinersWife() { delete m_pStateMachine; }

	void          Update();

	virtual bool  HandleMessage(const Telegram& msg);

	StateMachine<MinersWife>* GetFSM()const { return m_pStateMachine; }

	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          Cooking()const { return m_bCooking; }
	void          SetCooking(bool val) { m_bCooking = val; }

};

#endif