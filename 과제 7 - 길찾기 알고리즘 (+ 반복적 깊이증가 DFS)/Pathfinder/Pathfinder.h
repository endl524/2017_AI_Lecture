#ifndef Pathfinder_H
#define Pathfinder_H
#pragma warning (disable:4786)

#include <windows.h>
#include <vector>
#include <fstream>
#include <string>
#include <list>

#include "../Common/2D/Vector2d.h"
#include "../Common/Graph/SparseGraph.h"
#include "../Common/Graph/GraphAlgorithms.h"
#include "../Common/misc/utils.h"
#include "../Common/Graph/GraphEdgeTypes.h"
#include "../Common/Graph/GraphNodeTypes.h"

class Pathfinder
{
public:
	enum brush_type
	{
		normal = 0,
		obstacle = 1,
		water = 2,
		mud = 3,
		source = 4,
		target = 5
	};

	enum algorithm_type
	{
		non,
		search_astar,
		search_bfs,
		search_dfs,
		search_dijkstra,
		search_iterdeep
	};

private:
	std::vector<int> m_TerrainType; // 각 셀의 터레인 타입을 저장
	std::list<int> m_Path; // 그래프 탐색을 통해 반환된 경로를 저장

	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph; // 그래프 타입
	NavGraph* m_pGraph;

	std::vector<const GraphEdge*> m_SubTree; // 그래프 알고리즘을 통해 반환된 서브트리를 저장

	double m_dCostToTarget; // 원점에서 목표 까지의 총 비용

	algorithm_type m_CurrentAlgorithm; // 현재 선택된 알고리즘

	brush_type m_CurrentTerrainBrush; // 현재 선택된 터레인 브러쉬 타입.

	double m_dCellWidth, m_dCellHeight; // 셀 사이즈
	int m_iCellsX, m_iCellsY;

	int m_icxClient, m_icyClient; // 윈도우 크기

	int m_iSourceCell, m_iTargetCell; // 셀 인덱스

	bool m_bStart, m_bFinish; // 시작 지점과 목표 지점이 설정되었는가에 대한 플래그
	bool m_bShowGraph; // 그래프 렌더링 여부
	bool m_bShowTiles; // 타일 렌더링 여부

	double m_dTimeTaken; // 알고리즘 수행 시간

	void  UpdateAlgorithm();
	void  UpdateGraphFromBrush(int brush, int CellIndex);
	std::string GetNameOfCurrentSearchAlgorithm() const;

public:

	Pathfinder() : m_bStart(false), m_bFinish(false), m_bShowGraph(false), m_bShowTiles(true),
		m_dCellWidth(0), m_dCellHeight(0), m_iCellsX(0), m_iCellsY(0),
		m_dTimeTaken(0.0), m_CurrentTerrainBrush(normal), m_iSourceCell(0), m_iTargetCell(0),
		m_icxClient(0), m_icyClient(0), m_dCostToTarget(0.0), m_pGraph(NULL)
	{
		m_pGraph = new NavGraph(false);
	}
	~Pathfinder() { delete m_pGraph; }

	void CreateGraph(int CellsUp, int CellsAcross);

	void Render();

	void PaintTerrain(POINTS p);

	void CreatePathDFS();
	void CreatePathBFS();
	void CreatePathDijkstra();
	void CreatePathAStar();
	void CreatePathIterDeep();
	void MinSpanningTree();

	void ToggleShowGraph() { m_bShowGraph = !m_bShowGraph; }
	void SwitchGraphOn() { m_bShowGraph = true; }
	void SwitchGraphOff() { m_bShowGraph = false; }
	bool isShowGraphOn() const { return m_bShowGraph; }

	void ToggleShowTiles() { m_bShowTiles = !m_bShowTiles; }
	void SwitchTilesOn() { m_bShowTiles = true; }
	void SwitchTilesOff() { m_bShowTiles = false; }
	bool isShowTilesOn() const { return m_bShowTiles; }

	void ChangeBrush(const brush_type NewBrush) { m_CurrentTerrainBrush = NewBrush; }

	void ChangeSource(const int cell) { m_iSourceCell = cell; }
	void ChangeTarget(const int cell) { m_iTargetCell = cell; }

	bool PointToIndex(POINTS p, int& NodeIndex);

	double GetTerrainCost(brush_type brush);

	void Save(char* FileName);
	void Load(char* FileName);
};

#endif