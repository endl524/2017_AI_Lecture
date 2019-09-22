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
	std::vector<int> m_TerrainType; // �� ���� �ͷ��� Ÿ���� ����
	std::list<int> m_Path; // �׷��� Ž���� ���� ��ȯ�� ��θ� ����

	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph; // �׷��� Ÿ��
	NavGraph* m_pGraph;

	std::vector<const GraphEdge*> m_SubTree; // �׷��� �˰����� ���� ��ȯ�� ����Ʈ���� ����

	double m_dCostToTarget; // �������� ��ǥ ������ �� ���

	algorithm_type m_CurrentAlgorithm; // ���� ���õ� �˰���

	brush_type m_CurrentTerrainBrush; // ���� ���õ� �ͷ��� �귯�� Ÿ��.

	double m_dCellWidth, m_dCellHeight; // �� ������
	int m_iCellsX, m_iCellsY;

	int m_icxClient, m_icyClient; // ������ ũ��

	int m_iSourceCell, m_iTargetCell; // �� �ε���

	bool m_bStart, m_bFinish; // ���� ������ ��ǥ ������ �����Ǿ��°��� ���� �÷���
	bool m_bShowGraph; // �׷��� ������ ����
	bool m_bShowTiles; // Ÿ�� ������ ����

	double m_dTimeTaken; // �˰��� ���� �ð�

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