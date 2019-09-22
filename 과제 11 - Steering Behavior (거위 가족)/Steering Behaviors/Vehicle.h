#ifndef VEHICLE_H
#define VEHICLE_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   Vehicle.h
//
//  Desc:   Definition of a simple vehicle that uses steering behaviors
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "../Common/Game/MovingEntity.h"
#include "../Common/2D/Vector2D.h"
#include "../Common/misc/Smoother.h"

#include <vector>
#include <list>
#include <string>

class GameWorld;
class SteeringBehavior;

class Vehicle : public MovingEntity
{
private:
	GameWorld* m_pWorld;

	SteeringBehavior* m_pSteering;

	Smoother<Vector2D>* m_pHeadingSmoother;

	Vector2D m_vSmoothedHeading;

	static Vehicle* m_Leader_Vehicle;
	static Vehicle* m_Last_Vehicle;
	bool is_Catched;

	bool m_bSmoothingOn;

	double m_dTimeElapsed;

	std::vector<Vector2D> m_vecVehicleVB;

	void InitializeBuffer();

	Vehicle(const Vehicle&);
	Vehicle& operator=(const Vehicle&);

public:
	Vehicle(GameWorld* world, Vector2D position, double rotation, Vector2D velocity,
		double mass, double max_force, double max_speed, double max_turn_rate, double scale);
	~Vehicle();

	void Update(double time_elapsed);

	void Render();

	SteeringBehavior* const Steering() const { return m_pSteering; }
	GameWorld* const World() const { return m_pWorld; }

	Vector2D SmoothedHeading() const { return m_vSmoothedHeading; }

	bool isSmoothingOn() const { return m_bSmoothingOn; }
	void SmoothingOn() { m_bSmoothingOn = true; }
	void SmoothingOff() { m_bSmoothingOn = false; }
	void ToggleSmoothing() { m_bSmoothingOn = !m_bSmoothingOn; }

	bool Get_is_Catched() const { return is_Catched; }
	void Set_is_Catched(const bool& b) { is_Catched = b; }

	double TimeElapsed() const { return m_dTimeElapsed; }

	void Set_Leader_Vehicle(Vehicle* v) { m_Leader_Vehicle = v; }
	Vehicle* Get_Leader_Vehicle() { return m_Leader_Vehicle; }
	void Set_Last_Vehicle(Vehicle* v) { m_Last_Vehicle = v; }
	Vehicle* Get_Last_Vehicle() { return m_Last_Vehicle; }
};

#endif