#include "Pathfinder.h"
#include "../Common/Graph/HandyGraphFunctions.h"
#include "../Common/misc/Cgdi.h"
#include "../Common/Time/PrecisionTimer.h"
#include "constants.h"
#include "../Common/Graph/AStarHeuristicPolicies.h"
#include "../Common/misc/Stream_Utility_Functions.h"

#include <iostream>
using namespace std;

extern HWND g_hwndToolbar;
extern const char*  g_szApplicationName;
extern const char*	g_szWindowClassName;

void Pathfinder::CreateGraph(int CellsUp, int CellsAcross)
{
	RECT rectToolbar;
	GetWindowRect(g_hwndToolbar, &rectToolbar);

	HWND hwndMainWindow = FindWindow(g_szWindowClassName, g_szApplicationName);

	RECT rect;
	GetClientRect(hwndMainWindow, &rect);
	m_icxClient = rect.right;
	m_icyClient = rect.bottom - abs(rectToolbar.bottom - rectToolbar.top) - InfoWindowHeight;

	m_TerrainType.assign(CellsUp * CellsAcross, normal);

	m_iCellsX = CellsAcross;
	m_iCellsY = CellsUp;
	m_dCellWidth = (double)m_icxClient / (double)CellsAcross;
	m_dCellHeight = (double)m_icyClient / (double)CellsUp;

	delete m_pGraph;

	m_pGraph = new NavGraph(false);

	GraphHelper_CreateGrid(*m_pGraph, m_icxClient, m_icyClient, CellsUp, CellsAcross);

	PointToIndex(VectorToPOINTS(Vector2D(m_icxClient / 2, m_dCellHeight * 2)), m_iTargetCell);
	PointToIndex(VectorToPOINTS(Vector2D(m_icxClient / 2, m_icyClient - m_dCellHeight * 2)), m_iSourceCell);

	m_Path.clear();
	m_SubTree.clear();

	m_CurrentAlgorithm = non;
	m_dTimeTaken = 0;
}

bool Pathfinder::PointToIndex(POINTS p, int& NodeIndex)
{
	int x = (int)((double)(p.x) / m_dCellWidth);
	int y = (int)((double)(p.y) / m_dCellHeight);

	if ((x > m_iCellsX) || (y > m_iCellsY))
	{
		NodeIndex = -1;
		return false;
	}
	NodeIndex = y * m_iCellsX + x;
	return true;
}

double Pathfinder::GetTerrainCost(const brush_type brush)
{
	const double cost_normal = 1.0;
	const double cost_water = 2.0;
	const double cost_mud = 1.5;

	switch (brush)
	{
	case normal: return cost_normal;
	case water: return cost_water;
	case mud: return cost_mud;
	default: return MaxDouble;
	};
}

void Pathfinder::PaintTerrain(POINTS p)
{
	int x = (int)((double)(p.x) / m_dCellWidth);
	int y = (int)((double)(p.y) / m_dCellHeight);

	if ((x > m_iCellsX) || (y > (m_iCellsY - 1))) return;

	m_SubTree.clear();
	m_Path.clear();

	if ((m_CurrentTerrainBrush == source) || (m_CurrentTerrainBrush == target))
	{
		switch (m_CurrentTerrainBrush)
		{
		case source: m_iSourceCell = y * m_iCellsX + x; break;

		case target: m_iTargetCell = y * m_iCellsX + x; break;
		}
	}
	else UpdateGraphFromBrush(m_CurrentTerrainBrush, y * m_iCellsX + x);

	UpdateAlgorithm();
}

void Pathfinder::UpdateGraphFromBrush(int brush, int CellIndex)
{
	m_TerrainType[CellIndex] = brush;

	if (brush == 1) m_pGraph->RemoveNode(CellIndex);
	else
	{
		if (!m_pGraph->isNodePresent(CellIndex))
		{
			int y = CellIndex / m_iCellsY;
			int x = CellIndex - (y*m_iCellsY);

			m_pGraph->AddNode(NavGraph::NodeType(CellIndex, Vector2D(x*m_dCellWidth + m_dCellWidth / 2.0,
				y*m_dCellHeight + m_dCellHeight / 2.0)));

			GraphHelper_AddAllNeighboursToGridNode(*m_pGraph, y, x, m_iCellsX, m_iCellsY);
		}

		WeightNavGraphNodeEdges(*m_pGraph, CellIndex, GetTerrainCost((brush_type)brush));
	}
}

void Pathfinder::UpdateAlgorithm()
{
	switch (m_CurrentAlgorithm)
	{
	case non: break;
	case search_dfs: CreatePathDFS(); break;
	case search_iterdeep: CreatePathIterDeep(); break;
	case search_bfs: CreatePathBFS(); break;
	case search_dijkstra: CreatePathDijkstra(); break;
	case search_astar: CreatePathAStar(); break;
	default: break;
	}
}

void Pathfinder::CreatePathDFS()
{
	m_CurrentAlgorithm = search_dfs;

	m_Path.clear();
	m_SubTree.clear();

	PrecisionTimer timer; timer.Start();

	Graph_SearchDFS<NavGraph> DFS(*m_pGraph, m_iSourceCell, m_iTargetCell);

	m_dTimeTaken = timer.TimeElapsed();

	if (DFS.Found()) m_Path = DFS.GetPathToTarget();

	m_SubTree = DFS.GetSearchTree();

	m_dCostToTarget = 0.0;
}

void Pathfinder::CreatePathIterDeep()
{
	m_CurrentAlgorithm = search_iterdeep;

	m_Path.clear();
	m_SubTree.clear();

	PrecisionTimer timer; timer.Start();

	Graph_SearchIterDeep<NavGraph> IterDeep(*m_pGraph, m_iSourceCell, m_iTargetCell);

	m_dTimeTaken = timer.TimeElapsed();

	if (IterDeep.Found()) m_Path = IterDeep.GetPathToTarget();

	m_SubTree = IterDeep.GetSearchTree();

	m_dCostToTarget = 0.0;
}

void Pathfinder::CreatePathBFS()
{
	m_CurrentAlgorithm = search_bfs;

	m_Path.clear();
	m_SubTree.clear();

	PrecisionTimer timer; timer.Start();

	Graph_SearchBFS<NavGraph> BFS(*m_pGraph, m_iSourceCell, m_iTargetCell);

	m_dTimeTaken = timer.TimeElapsed();

	if (BFS.Found()) m_Path = BFS.GetPathToTarget();

	m_SubTree = BFS.GetSearchTree();

	m_dCostToTarget = 0.0;
}

void Pathfinder::CreatePathDijkstra()
{
	m_CurrentAlgorithm = search_dijkstra;

	PrecisionTimer timer; timer.Start();

	Graph_SearchDijkstra<NavGraph> djk(*m_pGraph, m_iSourceCell, m_iTargetCell);

	m_dTimeTaken = timer.TimeElapsed();

	m_Path = djk.GetPathToTarget();

	m_SubTree = djk.GetSPT();

	m_dCostToTarget = djk.GetCostToTarget();
}

void Pathfinder::CreatePathAStar()
{
	m_CurrentAlgorithm = search_astar;

	PrecisionTimer timer; timer.Start();

	typedef Graph_SearchAStar<NavGraph, Heuristic_Euclid> AStarSearch;

	AStarSearch AStar(*m_pGraph, m_iSourceCell, m_iTargetCell);

	m_dTimeTaken = timer.TimeElapsed();

	m_Path = AStar.GetPathToTarget();

	m_SubTree = AStar.GetSPT();

	m_dCostToTarget = AStar.GetCostToTarget();
}

void Pathfinder::Save(char* FileName)
{
	ofstream save(FileName);
	assert(save && "Pathfinder::Save< bad file >");

	save << m_iCellsX << endl;
	save << m_iCellsY << endl;

	for (unsigned int t = 0; t < m_TerrainType.size(); ++t)
	{
		if (t == m_iSourceCell) save << source << endl;
		else if (t == m_iTargetCell) save << target << endl;
		else save << m_TerrainType[t] << endl;
	}
}

void Pathfinder::Load(char* FileName)
{
	ifstream load(FileName);
	assert(load && "Pathfinder::Save< bad file >");

	load >> m_iCellsX;
	load >> m_iCellsY;

	CreateGraph(m_iCellsY, m_iCellsX);

	int terrain;

	for (int t = 0; t < m_iCellsX*m_iCellsY; ++t)
	{
		load >> terrain;

		if (terrain == source) m_iSourceCell = t;
		else if (terrain == target) m_iTargetCell = t;
		else
		{
			m_TerrainType[t] = terrain;
			UpdateGraphFromBrush(terrain, t);
		}
	}
}

std::string Pathfinder::GetNameOfCurrentSearchAlgorithm()const
{
	switch (m_CurrentAlgorithm)
	{
	case non: return "";
	case search_astar: return "A Star";
	case search_bfs: return "Breadth First";
	case search_dfs: return "Depth First";
	case search_dijkstra: return "Dijkstras";
	case search_iterdeep: return "Iterative Deepening Depth First";
	default: return "UNKNOWN!";
	}
}

void Pathfinder::Render()
{
	gdi->TransparentText();

	for (int nd = 0; nd < m_pGraph->NumNodes(); ++nd)
	{
		int left = (int)(m_pGraph->GetNode(nd).Pos().x - m_dCellWidth / 2.0);
		int top = (int)(m_pGraph->GetNode(nd).Pos().y - m_dCellHeight / 2.0);
		int right = (int)(1 + m_pGraph->GetNode(nd).Pos().x + m_dCellWidth / 2.0);
		int bottom = (int)(1 + m_pGraph->GetNode(nd).Pos().y + m_dCellHeight / 2.0);

		gdi->GreyPen();

		switch (m_TerrainType[nd])
		{
		case 0:
			gdi->WhiteBrush();
			if (!m_bShowTiles)gdi->WhitePen();
			break;

		case 1:
			gdi->BlackBrush();
			if (!m_bShowTiles)gdi->BlackPen();
			break;

		case 2:
			gdi->LightBlueBrush();
			if (!m_bShowTiles)gdi->LightBluePen();
			break;

		case 3:
			gdi->BrownBrush();
			if (!m_bShowTiles)gdi->BrownPen();
			break;

		default:
			gdi->WhiteBrush();
			if (!m_bShowTiles)gdi->WhitePen();
			break;
		}

		if (nd == m_iTargetCell)
		{
			gdi->RedBrush();
			if (!m_bShowTiles)gdi->RedPen();
		}
		if (nd == m_iSourceCell)
		{
			gdi->GreenBrush();
			if (!m_bShowTiles)gdi->GreenPen();
		}

		gdi->Rect(left, top, right, bottom);

		if (nd == m_iTargetCell)
		{
			gdi->ThickBlackPen();
			gdi->Cross(Vector2D(m_pGraph->GetNode(nd).Pos().x - 1, m_pGraph->GetNode(nd).Pos().y - 1),
				(int)((m_dCellWidth*0.6) / 2.0));
		}
		if (nd == m_iSourceCell)
		{
			gdi->ThickBlackPen();
			gdi->HollowBrush();
			gdi->Rect(left + 7, top + 7, right - 6, bottom - 6);
		}

		gdi->DrawDot(left, top, RGB(0, 0, 0));
		gdi->DrawDot(right - 1, top, RGB(0, 0, 0));
		gdi->DrawDot(left, bottom - 1, RGB(0, 0, 0));
		gdi->DrawDot(right - 1, bottom - 1, RGB(0, 0, 0));
	}

	if (m_bShowGraph) // 그래프 렌더링
		GraphHelper_DrawUsingGDI<NavGraph>(*m_pGraph, Cgdi::light_grey, false);  //false = don't draw node IDs

	gdi->RedPen();
	for (unsigned int e = 0; e < m_SubTree.size(); ++e) // 경로 서브트리 렌더링
	{
		if (m_SubTree[e])
		{
			Vector2D from = m_pGraph->GetNode(m_SubTree[e]->From()).Pos();
			Vector2D to = m_pGraph->GetNode(m_SubTree[e]->To()).Pos();
			gdi->Line(from, to);
		}
	}

	if (m_Path.size() > 0) // 경로 렌더링
	{
		gdi->ThickBluePen();

		std::list<int>::iterator it = m_Path.begin();
		std::list<int>::iterator nxt = it; ++nxt;

		for (it; nxt != m_Path.end(); ++it, ++nxt)
		{
			gdi->Line(m_pGraph->GetNode(*it).Pos(), m_pGraph->GetNode(*nxt).Pos());
		}
	}

	if (m_dTimeTaken) // 수행 시간 렌더링
	{
		string time = ttos(m_dTimeTaken, 8);
		string s = "Time Elapsed for " + GetNameOfCurrentSearchAlgorithm() + " is " + time;
		gdi->TextAtPos(1, m_icyClient + 3, s);
	}

	if (m_CurrentAlgorithm == search_astar || m_CurrentAlgorithm == search_dijkstra) // 소요 비용 렌더링
		gdi->TextAtPos(m_icxClient - 110, m_icyClient + 3, "Cost is " + ttos(m_dCostToTarget));
}
