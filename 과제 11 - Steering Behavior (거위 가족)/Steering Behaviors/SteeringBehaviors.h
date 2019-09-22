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

const double WanderRad = 1.2; // ��ȸ �ݰ��� �����ϴ� ���� ����.
const double WanderDist = 2.0; // ��ȸ �ݰ��� �����ϴ� ���� ����.
const double WanderJitterPerSec = 80.0;
const double WaypointSeekDist = 20; // ��� Ž�� �Ÿ�.

//------------------------------------------------------------------------

class SteeringBehavior
{
public:
	enum summing_method { weighted_average, prioritized, dithered };

private:
	enum behavior_type
	{
		none = 0x00000, // �ƹ��͵� ����.
		seek = 0x00002, // (��ǥ ������) ã��.
		flee = 0x00004, // (��ǥ�� ����) ����.
		arrive = 0x00008, // (������ ����) �̵�.
		wander = 0x00010, // (��ǥ ���� ���� '���� �ݰ� ���� ����'��) ��ȸ.
		cohesion = 0x00020, // 
		separation = 0x00040, // 
		allignment = 0x00080, // 
		obstacle_avoidance = 0x00100, // (���濡 �浹�ڽ��� �ξ� ��ֹ��� �̸� �����Ͽ�) ȸ��.
		wall_avoidance = 0x00200, // (���濡 �������� �˼��� �ΰ�, ������ �浹 ���̸� �����Ͽ�) ȸ��.
		follow_path = 0x00400, // 
		pursuit = 0x00800, // (��ǥ ��ü�� �̵� �Ϸ��� �������� �̵��Ͽ�) �߰�.
		evade = 0x01000, // ȸ��.
		interpose = 0x02000, // �� ��ü ������ �������� �̵�.
		hide = 0x04000, // ���Ϸ��� ��ü�� �� ��ü ���̿� ��ֹ��� �� �� �ֵ��� �̵�.
		flock = 0x08000, // ��ü���� �������� �ٴ�.
		offset_pursuit = 0x10000, // (��ǥ �������� offset ��ŭ ������ ������ ����) �߰�.
		catching = 0x20000, // ��ǥ���� ���������� �˻��ϴ� ����.
		catched = 0x40000 // ��ǥ���� ������ ���� ���� (��� ���� ����� �̵�).
	};

private:
	Vehicle* m_pVehicle; // �ൿ ��ü�� �����ϴ� ��ü

	Vector2D m_vSteeringForce; // ���� ��

	Vehicle* m_pTargetAgent1; // ��ǥ ��ü 1
	Vehicle* m_pTargetAgent2; // ��ǥ ��ü 2

	Vector2D m_vTarget; // ��ǥ ����

	double m_dDBoxLength; // ��ֹ� ȸ�� '���� �ڽ�'�� ����

	std::vector<Vector2D> m_Feelers; // �� ȸ�� '���� �˼�'�� ���� ����.
	double m_dWallDetectionFeelerLength; // ���� �˼��� ����

	Vector2D m_vWanderTarget; // '��ȸ �ݰ� ��' ���� �� ����.
	double m_dWanderRadius;
	double m_dWanderDistance;
	double m_dWanderJitter;

	// �� �ൿ���� ����ġ ��.
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

	double m_dViewDistance; // �þ� �Ÿ�.

	Path* m_pPath; // ��� ��ü
	double m_dWaypointSeekDistSq; // ��� �������� ���� ��������Ʈ�� ã�� ��, ��������Ʈ�� �����ؾ� �� �Ÿ� ��.

	Vector2D m_vOffset; // ������ ������ ���Ǵ� ������ ��.

	int m_iFlags; // '�ൿ'���� Ȱ��ȭ�� ����ϴ� '���̳ʸ� �÷���'

	enum Deceleration { slow = 3, normal = 2, fast = 1 };
	Deceleration m_Deceleration; // �������� �����ϱ� �� ������ �ӵ��� ũ��.

	bool m_bCellSpaceOn; // �� ������ Ȱ��ȭ �Ǿ��°�?

	summing_method m_SummingMethod; // ���� �� ���.

	bool On(behavior_type bt) { return (m_iFlags & bt) == bt; } // Ư�� �ൿ�� Ȱ��ȭ �Ǿ��ִ°��� �˻�.

	bool AccumulateForce(Vector2D &sf, Vector2D ForceToAdd); // ���� ���� ����.

	void CreateFeelers(); // �� ȸ�� ���� �˼��� ����.

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