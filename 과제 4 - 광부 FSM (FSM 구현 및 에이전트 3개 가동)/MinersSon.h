#ifndef MINERSSON_H
#define MINERSSON_H

#include <string>

#include "State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinersSonOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "Miner.h"
#include "StateMachine.h"
#include "misc/Utils.h"

class MinersSon : public BaseGameEntity
{
private:
	StateMachine<MinersSon>* m_pStateMachine;
	location_type m_Location;

public:
	MinersSon(int id) : BaseGameEntity(id), m_Location(shack)
	{
		m_pStateMachine = new StateMachine<MinersSon>(this);
		m_pStateMachine->SetCurrentState(Play_at_Playground::Instance());
		m_pStateMachine->SetGlobalState(Sons_GlobalState::Instance());
	}
	~MinersSon() { delete m_pStateMachine; }

	void Update();

	StateMachine<MinersSon>* GetFSM()const { return m_pStateMachine; }

	location_type Location() const { return m_Location; }
	void ChangeLocation(const location_type loc) { m_Location = loc; }
};

#endif
