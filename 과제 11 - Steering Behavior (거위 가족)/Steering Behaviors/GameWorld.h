#ifndef GameWorld_H
#define GameWorld_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   GameWorld.h
//
//  Desc:   All the environment data and methods for the Steering
//          Behavior projects. This class is the root of the project's
//          update and render calls (excluding main of course)
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <windows.h>
#include <vector>
#include "../Common/2D/Vector2D.h"
#include "../Common/time/PrecisionTimer.h"
#include "../Common/misc/CellSpacePartition.h"
#include "../Common/Game/BaseGameEntity.h"
#include "../Common/Game/EntityFunctionTemplates.h"
#include "vehicle.h"

class Obstacle;
class Wall2D;
class Path;

typedef std::vector<BaseGameEntity*>::iterator  ObIt;

class GameWorld
{
private:
	std::vector<Vehicle*> m_Vehicles; // �����̴� ��ü��

	std::vector<BaseGameEntity*> m_Obstacles; // ��ֹ���

	std::vector<Wall2D> m_Walls; // �� ������ '����� ��' ��ü��

	CellSpacePartition<Vehicle*>* m_pCellSpace; // �� ���� ��ü

	Path* m_pPath; // ��� ��ü

	bool m_bPaused; // �Ͻ����� ���

	int m_cxClient, m_cyClient; // ������ ������ x, y

	Vector2D m_vCrosshair; // ũ�ν���� ��ü

	double m_dAvFrameTime; // FPS ��

	// ������Ʈ �� ��ü�� �� �������� ������ ���� ���
	bool  m_bShowWalls;
	bool  m_bShowObstacles;
	bool  m_bShowPath;
	bool  m_bShowDetectionBox;
	bool  m_bShowWanderCircle;
	bool  m_bShowFeelers;
	bool  m_bShowSteeringForce;
	bool  m_bShowFPS;
	bool  m_bRenderNeighbors;
	bool  m_bViewKeys;
	bool  m_bShowCellSpaceInfo;

private:
	void CreateObstacles();
	void CreateWalls();

public:
	GameWorld(const int& cx, const int& cy);
	~GameWorld();

	void  Update(double time_elapsed);

	void  Render();

	void  NonPenetrationContraint(Vehicle* v) { EnforceNonPenetrationConstraint(v, m_Vehicles); }

	void  TagVehiclesWithinViewRange(BaseGameEntity* pVehicle, double range)
	{
		TagNeighbors(pVehicle, m_Vehicles, range);
	}

	void  TagObstaclesWithinViewRange(BaseGameEntity* pVehicle, double range)
	{
		TagNeighbors(pVehicle, m_Obstacles, range);
	}

	const std::vector<Wall2D>& Walls() { return m_Walls; }
	CellSpacePartition<Vehicle*>* CellSpace() { return m_pCellSpace; }
	const std::vector<BaseGameEntity*>& Obstacles()const { return m_Obstacles; }
	const std::vector<Vehicle*>& Agents() { return m_Vehicles; }

	void HandleKeyPresses(WPARAM wParam);
	void HandleMenuItems(WPARAM wParam, HWND hwnd);

	void TogglePause() { m_bPaused = !m_bPaused; }
	bool Paused()const { return m_bPaused; }

	Vector2D Crosshair()const { return m_vCrosshair; }
	void SetCrosshair(POINTS p);
	void SetCrosshair(Vector2D v) { m_vCrosshair = v; }

	int cxClient()const { return m_cxClient; }
	int cyClient()const { return m_cyClient; }

	bool RenderWalls()const { return m_bShowWalls; }
	bool RenderObstacles()const { return m_bShowObstacles; }
	bool RenderPath()const { return m_bShowPath; }
	bool RenderDetectionBox()const { return m_bShowDetectionBox; }
	bool RenderWanderCircle()const { return m_bShowWanderCircle; }
	bool RenderFeelers()const { return m_bShowFeelers; }
	bool RenderSteeringForce()const { return m_bShowSteeringForce; }

	bool RenderFPS()const { return m_bShowFPS; }
	void ToggleShowFPS() { m_bShowFPS = !m_bShowFPS; }

	void ToggleRenderNeighbors() { m_bRenderNeighbors = !m_bRenderNeighbors; }
	bool RenderNeighbors()const { return m_bRenderNeighbors; }

	void ToggleViewKeys() { m_bViewKeys = !m_bViewKeys; }
	bool ViewKeys()const { return m_bViewKeys; }
};

#endif