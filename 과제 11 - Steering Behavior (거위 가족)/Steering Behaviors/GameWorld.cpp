#include "GameWorld.h"
#include "Vehicle.h"
#include "constants.h"
#include "Obstacle.h"
#include "../Common/2D/Geometry.h"
#include "../Common/2D/Wall2D.h"
#include "../Common/2D/Transformations.h"
#include "SteeringBehaviors.h"
#include "../Common/time/PrecisionTimer.h"
#include "../Common/misc/Smoother.h"
#include "ParamLoader.h"
#include "../Common/misc/WindowUtils.h"
#include "../Common/misc/Stream_Utility_Functions.h"
#include "resource.h"
#include <list>
using std::list;

GameWorld::GameWorld(const int& cx, const int& cy) :
	m_cxClient(cx), m_cyClient(cy),
	m_bPaused(false), m_vCrosshair(Vector2D(cxClient() / 2.0, cxClient() / 2.0)),
	m_bShowWalls(false), m_bShowObstacles(false), m_bShowPath(false),
	m_bShowWanderCircle(false), m_bShowSteeringForce(false), m_bShowFeelers(false), m_bShowDetectionBox(false), m_bShowFPS(true),
	m_dAvFrameTime(0), m_pPath(NULL), m_bRenderNeighbors(false), m_bViewKeys(false), m_bShowCellSpaceInfo(false)
{
	m_pCellSpace = new CellSpacePartition<Vehicle*>((double)cx, (double)cy, Prm.NumCellsX, Prm.NumCellsY, Prm.NumAgents);

	double border = 30;
	m_pPath = new Path(5, border, border, cx - border, cy - border, true);
	
	for (int i = 0; i < Prm.NumAgents; ++i)
	{
		Vector2D SpawnPos = Vector2D(cx / 2.0 + RandomClamped() * cx / 2.0, cy / 2.0 + RandomClamped() * cy / 2.0);
		Vehicle* pVehicle = new Vehicle(this, SpawnPos, RandFloat() * TwoPi, Vector2D(0, 0), Prm.VehicleMass, Prm.MaxSteeringForce, Prm.MaxSpeed, Prm.MaxTurnRatePerSecond, Prm.VehicleScale);
		m_Vehicles.push_back(pVehicle);
		m_pCellSpace->AddEntity(m_Vehicles[i]);
	}

	m_Vehicles[Prm.NumAgents - 1]->SetScale(Vector2D(10, 10));

//#define SHOAL
#ifdef SHOAL // (Default) 집단 행동 + 특정 객체 회피.
	m_Vehicles[Prm.NumAgents - 1]->Steering()->WanderOn();
	m_Vehicles[Prm.NumAgents - 1]->SetMaxSpeed(70);

	for (int i = 0; i < Prm.NumAgents - 1; ++i)
	{
		m_Vehicles[i]->Steering()->FlockingOn();
		m_Vehicles[i]->Steering()->EvadeOn(m_Vehicles[Prm.NumAgents - 1]);
	}

#else // (거위 가족) 모든 객체 배회 + 특정 객체의 범위 안에 들어갈 경우 붙잡혀서 1열로 줄줄이 끌려다님.
	m_Vehicles[Prm.NumAgents - 1]->Steering()->WanderOn();
	m_Vehicles[Prm.NumAgents - 1]->SetMaxSpeed(100);
	m_Vehicles[Prm.NumAgents - 1]->Set_Leader_Vehicle(m_Vehicles[Prm.NumAgents - 1]);
	m_Vehicles[Prm.NumAgents - 1]->Set_Last_Vehicle(m_Vehicles[Prm.NumAgents - 1]);

	for (int i = 0; i < Prm.NumAgents - 1; ++i)
	{
		m_Vehicles[i]->Steering()->WanderOn();
		m_Vehicles[i]->Steering()->CatchingOn(m_Vehicles[Prm.NumAgents - 1]);
	}
#endif
}

GameWorld::~GameWorld()
{
	for (unsigned int i = 0; i < m_Vehicles.size(); ++i) delete m_Vehicles[i];
	for (unsigned int i = 0; i < m_Obstacles.size(); ++i) delete m_Obstacles[i];
	delete m_pCellSpace;
	delete m_pPath;
}

void GameWorld::Update(double time_elapsed)
{
	if (m_bPaused) return;

	const int SampleRate = 10;
	static Smoother<double> FrameRateSmoother(SampleRate, 0.0);
	m_dAvFrameTime = FrameRateSmoother.Update(time_elapsed);

	for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Update(time_elapsed);
}
  
void GameWorld::CreateWalls()
{
	double bordersize = 20.0;
	double CornerSize = 0.2;
	double vDist = m_cyClient - 2 * bordersize;
	double hDist = m_cxClient - 2 * bordersize;
	const int NumWallVerts = 8;
	Vector2D walls[NumWallVerts] =
	{
		Vector2D(hDist * CornerSize + bordersize, bordersize),
		Vector2D(m_cxClient - bordersize - hDist * CornerSize, bordersize),
		Vector2D(m_cxClient - bordersize, bordersize + vDist * CornerSize),
		Vector2D(m_cxClient - bordersize, m_cyClient - bordersize - vDist * CornerSize),
		Vector2D(m_cxClient - bordersize - hDist * CornerSize, m_cyClient - bordersize),
		Vector2D(hDist * CornerSize + bordersize, m_cyClient - bordersize),
		Vector2D(bordersize, m_cyClient - bordersize - vDist * CornerSize),
		Vector2D(bordersize, bordersize + vDist * CornerSize)
	};

	for (int i = 0; i < NumWallVerts - 1; ++i) m_Walls.push_back(Wall2D(walls[i], walls[i + 1]));
	m_Walls.push_back(Wall2D(walls[NumWallVerts - 1], walls[0]));
}

void GameWorld::CreateObstacles()
{
	for (int i = 0; i < Prm.NumObstacles; ++i)
	{
		bool bOverlapped = true;
		int NumTrys = 0; int NumAllowableTrys = 2000;

		while (bOverlapped)
		{
			++NumTrys;
			if (NumTrys > NumAllowableTrys) return;

			int radius = RandInt((int)Prm.MinObstacleRadius, (int)Prm.MaxObstacleRadius);
			const int border = 10, MinGapBetweenObstacles = 20;

			Obstacle* ob = new Obstacle(RandInt(radius + border, m_cxClient - radius - border), RandInt(radius + border, m_cyClient - radius - 30 - border), radius);

			if (!Overlapped(ob, m_Obstacles, MinGapBetweenObstacles)) // 생성한 장애물이 중복되지 않는다면 추가.
			{
				m_Obstacles.push_back(ob);
				bOverlapped = false;
			}
			else delete ob; // 중복된다면 지우고 다시 만든다.
		}
	}
}

void GameWorld::SetCrosshair(POINTS p)
{
	Vector2D ProposedPosition((double)p.x, (double)p.y);
	
	// 크로스헤어가 장애물에 찍히지 않도록 해야한다.
	for (ObIt curOb = m_Obstacles.begin(); curOb != m_Obstacles.end(); ++curOb)
	{
		if (PointInCircle((*curOb)->Pos(), (*curOb)->BRadius(), ProposedPosition)) return;
	}
	m_vCrosshair.x = (double)p.x;
	m_vCrosshair.y = (double)p.y;
}

void GameWorld::HandleKeyPresses(WPARAM wParam)
{
	switch(wParam)
	{
	case 'U':
	{
		delete m_pPath;
		double border = 60;
		m_pPath = new Path(RandInt(3, 7), border, border, cxClient() - border, cyClient() - border, true);
		m_bShowPath = true;
		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->SetPath(m_pPath->GetPath());
	}
		break;

    case 'P': TogglePause(); break;

    case 'O': ToggleRenderNeighbors(); break;

    case 'I':
		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->ToggleSmoothing();
        break;

    case 'Y':
		m_bShowObstacles = !m_bShowObstacles;
		if (!m_bShowObstacles)
        {
			m_Obstacles.clear();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->ObstacleAvoidanceOff();
        }
        else
        {
			CreateObstacles();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->ObstacleAvoidanceOn();
        }
        break;
	}
}

void GameWorld::HandleMenuItems(WPARAM wParam, HWND hwnd)
{
	switch (wParam)
	{
	case ID_OB_OBSTACLES:
		m_bShowObstacles = !m_bShowObstacles;

		if (!m_bShowObstacles)
		{
			m_Obstacles.clear();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->ObstacleAvoidanceOff();
			ChangeMenuState(hwnd, ID_OB_OBSTACLES, MFS_UNCHECKED);
		}
		else
		{
			CreateObstacles();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->ObstacleAvoidanceOn();
			ChangeMenuState(hwnd, ID_OB_OBSTACLES, MFS_CHECKED);
		}
		break;

	case ID_OB_WALLS:
		m_bShowWalls = !m_bShowWalls;

		if (m_bShowWalls)
		{
			CreateWalls();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->WallAvoidanceOn();
			ChangeMenuState(hwnd, ID_OB_WALLS, MFS_CHECKED);
		}
		else
		{
			m_Walls.clear();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->WallAvoidanceOff();
			ChangeMenuState(hwnd, ID_OB_WALLS, MFS_UNCHECKED);
		}
		break;


	case IDR_PARTITIONING:
		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->ToggleSpacePartitioningOnOff();

		if (m_Vehicles[0]->Steering()->isSpacePartitioningOn())
		{
			m_pCellSpace->EmptyCells();
			for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_pCellSpace->AddEntity(m_Vehicles[i]);
			ChangeMenuState(hwnd, IDR_PARTITIONING, MFS_CHECKED);
		}
		else
		{
			ChangeMenuState(hwnd, IDR_PARTITIONING, MFS_UNCHECKED);
			ChangeMenuState(hwnd, IDM_PARTITION_VIEW_NEIGHBORS, MFS_UNCHECKED);
			m_bShowCellSpaceInfo = false;
		}
		break;

	case IDM_PARTITION_VIEW_NEIGHBORS:
		m_bShowCellSpaceInfo = !m_bShowCellSpaceInfo;

		if (m_bShowCellSpaceInfo)
		{
			ChangeMenuState(hwnd, IDM_PARTITION_VIEW_NEIGHBORS, MFS_CHECKED);

			if (!m_Vehicles[0]->Steering()->isSpacePartitioningOn()) SendMessage(hwnd, WM_COMMAND, IDR_PARTITIONING, NULL);
		}
		else ChangeMenuState(hwnd, IDM_PARTITION_VIEW_NEIGHBORS, MFS_UNCHECKED);
		break;


	case IDR_WEIGHTED_SUM:
		ChangeMenuState(hwnd, IDR_WEIGHTED_SUM, MFS_CHECKED);
		ChangeMenuState(hwnd, IDR_PRIORITIZED, MFS_UNCHECKED);
		ChangeMenuState(hwnd, IDR_DITHERED, MFS_UNCHECKED);

		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->SetSummingMethod(SteeringBehavior::weighted_average);
		break;

	case IDR_PRIORITIZED:
		ChangeMenuState(hwnd, IDR_WEIGHTED_SUM, MFS_UNCHECKED);
		ChangeMenuState(hwnd, IDR_PRIORITIZED, MFS_CHECKED);
		ChangeMenuState(hwnd, IDR_DITHERED, MFS_UNCHECKED);

		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->SetSummingMethod(SteeringBehavior::prioritized);
		break;

	case IDR_DITHERED:
		ChangeMenuState(hwnd, IDR_WEIGHTED_SUM, MFS_UNCHECKED);
		ChangeMenuState(hwnd, IDR_PRIORITIZED, MFS_UNCHECKED);
		ChangeMenuState(hwnd, IDR_DITHERED, MFS_CHECKED);

		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->Steering()->SetSummingMethod(SteeringBehavior::dithered);
		break;


	case ID_VIEW_KEYS:
		ToggleViewKeys();
		CheckMenuItemAppropriately(hwnd, ID_VIEW_KEYS, m_bViewKeys);
		break;

	case ID_VIEW_FPS:
		ToggleShowFPS();
		CheckMenuItemAppropriately(hwnd, ID_VIEW_FPS, RenderFPS());
		break;

	case ID_MENU_SMOOTHING:
		for (unsigned int i = 0; i < m_Vehicles.size(); ++i) m_Vehicles[i]->ToggleSmoothing();
		CheckMenuItemAppropriately(hwnd, ID_MENU_SMOOTHING, m_Vehicles[m_Vehicles.size() - 1]->isSmoothingOn());
		break;
	}
}

void GameWorld::Render()
{
	gdi->TransparentText();

	gdi->BlackPen();
	for (unsigned int w = 0; w < m_Walls.size(); ++w) m_Walls[w].Render(true);

	gdi->BlackPen();
	for (unsigned int ob = 0; ob < m_Obstacles.size(); ++ob) gdi->Circle(m_Obstacles[ob]->Pos(), m_Obstacles[ob]->BRadius());

	for (unsigned int i = 0; i < m_Vehicles.size(); ++i)
	{
		m_Vehicles[i]->Render();

		if (m_bShowCellSpaceInfo && i == m_Vehicles.size() - 1)
		{
			gdi->HollowBrush();
			InvertedAABBox2D box(m_Vehicles[i]->Pos() - Vector2D(Prm.ViewDistance, Prm.ViewDistance), m_Vehicles[i]->Pos() + Vector2D(Prm.ViewDistance, Prm.ViewDistance));
			box.Render();

			gdi->RedPen();
			CellSpace()->CalculateNeighbors(m_Vehicles[i]->Pos(), Prm.ViewDistance);
			for (BaseGameEntity* pV = CellSpace()->begin(); !CellSpace()->end(); pV = CellSpace()->next())
				gdi->Circle(pV->Pos(), pV->BRadius());

			gdi->GreenPen();
			gdi->Circle(m_Vehicles[i]->Pos(), Prm.ViewDistance);
		}
	}

//#define CROSSHAIR
#ifdef CROSSHAIR
	gdi->RedPen();
	gdi->Circle(m_vCrosshair, 4);
	gdi->Line(m_vCrosshair.x - 8, m_vCrosshair.y, m_vCrosshair.x + 8, m_vCrosshair.y);
	gdi->Line(m_vCrosshair.x, m_vCrosshair.y - 8, m_vCrosshair.x, m_vCrosshair.y + 8);
	gdi->TextAtPos(5, cyClient() - 20, "Click to move crosshair");
#endif

	gdi->TextAtPos(cxClient() - 120, cyClient() - 20, "Press R to reset");
	gdi->TextColor(Cgdi::grey);

	if (RenderPath())
	{
		gdi->TextAtPos((int)(cxClient() / 2.0f - 80), cyClient() - 20, "Press 'U' for random path");
		m_pPath->Render();
	}
	if (RenderFPS())
	{
		gdi->TextColor(Cgdi::grey);
		gdi->TextAtPos(cxClient() - 50, 5, ttos(1.0 / m_dAvFrameTime));
	}
	if (m_bShowCellSpaceInfo)
	{
		m_pCellSpace->RenderCells();
	}
}
