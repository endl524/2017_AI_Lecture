#include "Vehicle.h"
#include "../Common/2D/C2DMatrix.h"
#include "../Common/2D/Geometry.h"
#include "SteeringBehaviors.h"
#include "../Common/2D/Transformations.h"
#include "GameWorld.h"
#include "../Common/misc/CellSpacePartition.h"
#include "../Common/misc/cgdi.h"

using std::vector;
using std::list;

Vehicle* Vehicle::m_Leader_Vehicle = nullptr;
Vehicle* Vehicle::m_Last_Vehicle = nullptr;

Vehicle::Vehicle(GameWorld* world, Vector2D position, double rotation, Vector2D velocity,
	double mass, double max_force, double max_speed, double max_turn_rate, double scale) :
	MovingEntity(position, scale, velocity, max_speed, Vector2D(sin(rotation), -cos(rotation)), mass, Vector2D(scale, scale), max_turn_rate, max_force),
	m_pWorld(world), m_vSmoothedHeading(Vector2D(0, 0)), m_bSmoothingOn(false), m_dTimeElapsed(0.0), is_Catched(false)
{
	InitializeBuffer();
	m_pSteering = new SteeringBehavior(this);
	m_pHeadingSmoother = new Smoother<Vector2D>(Prm.NumSamplesForSmoothing, Vector2D(0.0, 0.0));
}

Vehicle::~Vehicle()
{
	delete m_pSteering;
	delete m_pHeadingSmoother;
}

void Vehicle::Update(double time_elapsed)
{
	m_dTimeElapsed = time_elapsed;

	Vector2D OldPos = Pos();

	Vector2D SteeringForce = m_pSteering->Calculate();
	Vector2D acceleration = SteeringForce / m_dMass;

	m_vVelocity += acceleration * time_elapsed;
	m_vVelocity.Truncate(m_dMaxSpeed);

	m_vPos += m_vVelocity * time_elapsed;

	if (m_vVelocity.LengthSq() > 0.00000001)
	{
		m_vHeading = Vec2DNormalize(m_vVelocity);
		m_vSide = m_vHeading.Perp();
	}

	//EnforceNonPenetrationConstraint(this, World()->Agents()); // Vehicle 按眉 埃狼 面倒 贸府.

	WrapAround(m_vPos, m_pWorld->cxClient(), m_pWorld->cyClient());

	if (Steering()->isSpacePartitioningOn()) World()->CellSpace()->UpdateEntity(this, OldPos);

	if (isSmoothingOn()) m_vSmoothedHeading = m_pHeadingSmoother->Update(Heading());
}


void Vehicle::Render()
{
	static std::vector<Vector2D> m_vecVehicleVBTrans;

	if (m_pWorld->RenderNeighbors())
	{
		if (ID() == 0) gdi->RedPen();
		else if (IsTagged()) gdi->GreenPen();
		else gdi->BluePen();
	}
	else
	{
		if (m_Leader_Vehicle == this) gdi->PurplePen();
		else if (is_Catched) gdi->OrangePen();
		else gdi->BluePen();
	}

	if (Steering()->isInterposeOn()) gdi->RedPen();
	if (Steering()->isHideOn()) gdi->GreenPen();

	if (isSmoothingOn()) m_vecVehicleVBTrans = WorldTransform(m_vecVehicleVB, Pos(), SmoothedHeading(), SmoothedHeading().Perp(), Scale());
	else m_vecVehicleVBTrans = WorldTransform(m_vecVehicleVB, Pos(), Heading(), Side(), Scale());

	gdi->ClosedShape(m_vecVehicleVBTrans);

	if (m_pWorld->ViewKeys()) Steering()->RenderAids();
}

void Vehicle::InitializeBuffer()
{
	const int NumVehicleVerts = 3;

	Vector2D vehicle[NumVehicleVerts] = { Vector2D(-1.0f, 0.6f), Vector2D(1.0f, 0.0f), Vector2D(-1.0f, -0.6f) };

	for (int vtx = 0; vtx < NumVehicleVerts; ++vtx)
	{
		m_vecVehicleVB.push_back(vehicle[vtx]);
	}
}
