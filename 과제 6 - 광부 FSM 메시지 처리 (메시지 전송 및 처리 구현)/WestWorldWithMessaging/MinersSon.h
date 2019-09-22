#ifndef MINERSSON_H
#define MINERSSON_H

#include <string>

#include "../Common/FSM/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinersSonOwnedStates.h"
#include "../Common/misc/ConsoleUtils.h"
#include "Miner.h"
#include "../Common/FSM/StateMachine.h"
#include "../Common/misc/Utils.h"

class MinersSon : public BaseGameEntity
{
private:
	StateMachine<MinersSon>* m_pStateMachine;
	location_type   m_Location;
	bool            m_bEatingMeal;

public:

	MinersSon(int id) : m_Location(shack), m_bEatingMeal(false), BaseGameEntity(id)
	{
		m_pStateMachine = new StateMachine<MinersSon>(this);
		m_pStateMachine->SetCurrentState(PlayAtPlayground::Instance());
		m_pStateMachine->SetGlobalState(SonsGlobalState::Instance());
	}
	~MinersSon() { delete m_pStateMachine; }

	void          Update();

	virtual bool  HandleMessage(const Telegram& msg);

	StateMachine<MinersSon>* GetFSM()const { return m_pStateMachine; }

	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          eatingMeal()const { return m_bEatingMeal; }
	void          setEatingMeal(bool val) { m_bEatingMeal = val; }
};

#endif
