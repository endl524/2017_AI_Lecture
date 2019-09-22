#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SteeringBehaviors.h
//
//  Desc:   class to encapsulate steering behaviors for a Vehicle
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>
#include <windows.h>
#include <string>
#include <list>

#include "../Common/2D/Vector2D.h"
#include "constants.h"
#include "Path.h"
#include "ParamLoader.h"

class Vehicle;
class CController;
class Wall2D;
class BaseGameEntity;

//--------------------------- Constants ----------------------------------

const double WanderRad = 1.2; // 배회 반경을 구성하는 원의 각도.
const double WanderDist = 2.0; // 배회 반경을 구성하는 원의 지름.
const double WanderJitterPerSec = 80.0;
const double WaypointSeekDist = 20; // 경로 탐색 거리.

//------------------------------------------------------------------------

class SteeringBehavior
{
public:
	enum summing_method { weighted_average, prioritized, dithered };

private:
	enum behavior_type
	{
		none = 0x00000, // 아무것도 안함.
		seek = 0x00002, // (목표 지점을) 찾기.
		flee = 0x00004, // (목표로 부터) 도주.
		arrive = 0x00008, // (목적지 까지) 이동.
		wander = 0x00010, // (목표 지점 주위 '일정 반경 안의 지점'을) 배회.
		cohesion = 0x00020, // 
		separation = 0x00040, // 
		allignment = 0x00080, // 
		obstacle_avoidance = 0x00100, // (전방에 충돌박스를 두어 장애물을 미리 감지하여) 회피.
		wall_avoidance = 0x00200, // (전방에 세갈래의 촉수를 두고, 벽과의 충돌 깊이를 감지하여) 회피.
		follow_path = 0x00400, // 
		pursuit = 0x00800, // (목표 객체가 이동 하려는 지점으로 이동하여) 추격.
		evade = 0x01000, // 회피.
		interpose = 0x02000, // 두 객체 사이의 지점으로 이동.
		hide = 0x04000, // 피하려는 객체와 현 객체 사이에 장애물을 둘 수 있도록 이동.
		flock = 0x08000, // 객체들이 무리지어 다님.
		offset_pursuit = 0x10000, // (목표 지점에서 offset 만큼 떨어진 지점을 따라) 추격.
		catching = 0x20000, // 목표에게 붙잡혔는지 검사하는 상태.
		catched = 0x40000 // 목표에게 붙잡힌 이후 상태 (계속 꼬리 물기로 이동).
	};

private:
	Vehicle* m_pVehicle; // 행동 객체를 소유하는 객체

	Vector2D m_vSteeringForce; // 조정 힘

	Vehicle* m_pTargetAgent1; // 목표 객체 1
	Vehicle* m_pTargetAgent2; // 목표 객체 2

	Vector2D m_vTarget; // 목표 지점

	double m_dDBoxLength; // 장애물 회피 '감지 박스'의 길이

	std::vector<Vector2D> m_Feelers; // 벽 회피 '감지 촉수'의 정점 버퍼.
	double m_dWallDetectionFeelerLength; // 감지 촉수의 길이

	Vector2D m_vWanderTarget; // '배회 반경 원' 위의 한 지점.
	double m_dWanderRadius;
	double m_dWanderDistance;
	double m_dWanderJitter;

	// 각 행동들의 가중치 값.
	double m_dWeightSeparation;
	double m_dWeightCohesion;
	double m_dWeightAlignment;
	double m_dWeightWander;
	double m_dWeightObstacleAvoidance;
	double m_dWeightWallAvoidance;
	double m_dWeightSeek;
	double m_dWeightFlee;
	double m_dWeightArrive;
	double m_dWeightPursuit;
	double m_dWeightOffsetPursuit;
	double m_dWeightInterpose;
	double m_dWeightHide;
	double m_dWeightEvade;
	double m_dWeightFollowPath;
	double m_dWeightCatching;
	double m_dWeightCatched;

	double m_dViewDistance; // 시야 거리.

	Path* m_pPath; // 경로 객체
	double m_dWaypointSeekDistSq; // 경로 추적에서 다음 웨이포인트를 찾기 전, 웨이포인트와 유지해야 할 거리 값.

	Vector2D m_vOffset; // 오프셋 추적에 사용되는 오프셋 값.

	int m_iFlags; // '행동'들의 활성화를 기록하는 '바이너리 플래그'

	enum Deceleration { slow = 3, normal = 2, fast = 1 };
	Deceleration m_Deceleration; // 목적지에 도착하기 전 감속할 속도의 크기.

	bool m_bCellSpaceOn; // 셀 공간이 활성화 되었는가?

	summing_method m_SummingMethod; // 조정 합 방식.

	bool On(behavior_type bt) { return (m_iFlags & bt) == bt; } // 특정 행동이 활성화 되어있는가를 검사.

	bool AccumulateForce(Vector2D &sf, Vector2D ForceToAdd); // 조정 힘을 누적.

	void CreateFeelers(); // 벽 회피 감지 촉수를 생성.

	Vector2D Seek(Vector2D TargetPos);
	Vector2D Flee(Vector2D TargetPos);
	Vector2D Arrive(Vector2D TargetPos, Deceleration deceleration);
	Vector2D Pursuit(const Vehicle* agent);
	Vector2D OffsetPursuit(const Vehicle* agent, const Vector2D offset);
	Vector2D Evade(const Vehicle* agent);
	Vector2D Wander();
	Vector2D ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles);
	Vector2D WallAvoidance(const std::vector<Wall2D> &walls);
	Vector2D FollowPath();
	Vector2D Interpose(const Vehicle* VehicleA, const Vehicle* VehicleB);
	Vector2D Hide(const Vehicle* hunter, const std::vector<BaseGameEntity*>& obstacles);
	Vector2D Catching(const Vehicle* agent);
	Vector2D Catched(const Vehicle* agent);

	Vector2D Cohesion(const std::vector<Vehicle*> &agents);
	Vector2D Separation(const std::vector<Vehicle*> &agents);
	Vector2D Alignment(const std::vector<Vehicle*> &agents);
	Vector2D CohesionPlus(const std::vector<Vehicle*> &agents);
	Vector2D SeparationPlus(const std::vector<Vehicle*> &agents);
	Vector2D AlignmentPlus(const std::vector<Vehicle*> &agents);

	Vector2D CalculateWeightedSum();
	Vector2D CalculatePrioritized();
	Vector2D CalculateDithered();

	Vector2D GetHidingPosition(const Vector2D& posOb, const double radiusOb, const Vector2D& posHunter);

public:
	SteeringBehavior(Vehicle* agent);
	virtual ~SteeringBehavior();

	Vector2D Calculate();

	//calculates the component of the steering force that is parallel
	//with the vehicle heading
	double    ForwardComponent();

	//calculates the component of the steering force that is perpendicuar
	//with the vehicle heading
	double    SideComponent();

	//renders visual aids and info for seeing how each behavior is
	//calculated
	void      RenderAids();

	void      SetTarget(const Vector2D t) { m_vTarget = t; }

	void      SetTargetAgent1(Vehicle* Agent) { m_pTargetAgent1 = Agent; }
	void      SetTargetAgent2(Vehicle* Agent) { m_pTargetAgent2 = Agent; }

	void      SetOffset(const Vector2D offset) { m_vOffset = offset; }
	Vector2D  GetOffset()const { return m_vOffset; }

	void      SetPath(std::list<Vector2D> new_path) { m_pPath->Set(new_path); }
	void      CreateRandomPath(int num_waypoints, int mx, int my, int cx, int cy)const
	{
		m_pPath->CreateRandomPath(num_waypoints, mx, my, cx, cy);
	}

	Vector2D Force()const { return m_vSteeringForce; }

	void      ToggleSpacePartitioningOnOff() { m_bCellSpaceOn = !m_bCellSpaceOn; }
	bool      isSpacePartitioningOn()const { return m_bCellSpaceOn; }

	void      SetSummingMethod(summing_method sm) { m_SummingMethod = sm; }


	void FleeOn() { m_iFlags |= flee; }
	void SeekOn() { m_iFlags |= seek; }
	void ArriveOn() { m_iFlags |= arrive; }
	void WanderOn() { m_iFlags |= wander; }
	void PursuitOn(Vehicle* v) { m_iFlags |= pursuit; m_pTargetAgent1 = v; }
	void EvadeOn(Vehicle* v) { m_iFlags |= evade; m_pTargetAgent1 = v; }
	void CohesionOn() { m_iFlags |= cohesion; }
	void SeparationOn() { m_iFlags |= separation; }
	void AlignmentOn() { m_iFlags |= allignment; }
	void ObstacleAvoidanceOn() { m_iFlags |= obstacle_avoidance; }
	void WallAvoidanceOn() { m_iFlags |= wall_avoidance; }
	void FollowPathOn() { m_iFlags |= follow_path; }
	void InterposeOn(Vehicle* v1, Vehicle* v2) { m_iFlags |= interpose; m_pTargetAgent1 = v1; m_pTargetAgent2 = v2; }
	void HideOn(Vehicle* v) { m_iFlags |= hide; m_pTargetAgent1 = v; }
	void OffsetPursuitOn(Vehicle* v1, const Vector2D offset) { m_iFlags |= offset_pursuit; m_vOffset = offset; m_pTargetAgent1 = v1; }
	void FlockingOn() { CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn(); }
	void CatchingOn(Vehicle* v) { m_iFlags |= catching; m_pTargetAgent1 = v; }
	void CatchedOn() { m_iFlags |= catched; }

	void FleeOff() { if (On(flee)) m_iFlags ^= flee; }
	void SeekOff() { if (On(seek)) m_iFlags ^= seek; }
	void ArriveOff() { if (On(arrive)) m_iFlags ^= arrive; }
	void WanderOff() { if (On(wander)) m_iFlags ^= wander; }
	void PursuitOff() { if (On(pursuit)) m_iFlags ^= pursuit; }
	void EvadeOff() { if (On(evade)) m_iFlags ^= evade; }
	void CohesionOff() { if (On(cohesion)) m_iFlags ^= cohesion; }
	void SeparationOff() { if (On(separation)) m_iFlags ^= separation; }
	void AlignmentOff() { if (On(allignment)) m_iFlags ^= allignment; }
	void ObstacleAvoidanceOff() { if (On(obstacle_avoidance)) m_iFlags ^= obstacle_avoidance; }
	void WallAvoidanceOff() { if (On(wall_avoidance)) m_iFlags ^= wall_avoidance; }
	void FollowPathOff() { if (On(follow_path)) m_iFlags ^= follow_path; }
	void InterposeOff() { if (On(interpose)) m_iFlags ^= interpose; }
	void HideOff() { if (On(hide)) m_iFlags ^= hide; }
	void OffsetPursuitOff() { if (On(offset_pursuit)) m_iFlags ^= offset_pursuit; }
	void FlockingOff() { CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff(); }
	void CatchingOff() { if (On(catching)) m_iFlags ^= catching; }
	void CatchedOff() { if (On(catched)) m_iFlags ^= catched; }

	bool isFleeOn() { return On(flee); }
	bool isSeekOn() { return On(seek); }
	bool isArriveOn() { return On(arrive); }
	bool isWanderOn() { return On(wander); }
	bool isPursuitOn() { return On(pursuit); }
	bool isEvadeOn() { return On(evade); }
	bool isCohesionOn() { return On(cohesion); }
	bool isSeparationOn() { return On(separation); }
	bool isAlignmentOn() { return On(allignment); }
	bool isObstacleAvoidanceOn() { return On(obstacle_avoidance); }
	bool isWallAvoidanceOn() { return On(wall_avoidance); }
	bool isFollowPathOn() { return On(follow_path); }
	bool isInterposeOn() { return On(interpose); }
	bool isHideOn() { return On(hide); }
	bool isOffsetPursuitOn() { return On(offset_pursuit); }
	bool isCatchingOn() { return On(catching); }
	bool isCatchedOn() { return On(catched); }

	double DBoxLength() const { return m_dDBoxLength; }
	const std::vector<Vector2D>& GetFeelers()const { return m_Feelers; }

	double WanderJitter() const { return m_dWanderJitter; }
	double WanderDistance() const { return m_dWanderDistance; }
	double WanderRadius() const { return m_dWanderRadius; }

	double SeparationWeight() const { return m_dWeightSeparation; }
	double AlignmentWeight() const { return m_dWeightAlignment; }
	double CohesionWeight() const { return m_dWeightCohesion; }

};

#endif