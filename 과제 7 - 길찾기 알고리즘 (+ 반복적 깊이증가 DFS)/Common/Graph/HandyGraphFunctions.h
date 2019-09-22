#ifndef GRAPH_FUNCS
#define GRAPH_FUNCS

#include <iostream>

#include "../misc/Cgdi.h"
#include "../misc/utils.h"
#include "../misc/Stream_Utility_Functions.h"
#include "../Graph/GraphAlgorithms.h"
#include "../Graph/AStarHeuristicPolicies.h"

bool ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY)
{
	return !((x < 0) || (x >= NumCellsX) || (y < 0) || (y >= NumCellsY));
}

template <class graph_type>
void GraphHelper_AddAllNeighboursToGridNode(graph_type& graph, int row, int col, int NumCellsX, int NumCellsY)
{
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			int nodeX = col + j;
			int nodeY = row + i;

			if ((i == 0) && (j == 0)) continue;

			if (ValidNeighbour(nodeX, nodeY, NumCellsX, NumCellsY))
			{
				Vector2D PosNode = graph.GetNode(row*NumCellsX + col).Pos();
				Vector2D PosNeighbour = graph.GetNode(nodeY*NumCellsX + nodeX).Pos();

				double dist = PosNode.Distance(PosNeighbour);

				graph_type::EdgeType NewEdge(row*NumCellsX + col, nodeY*NumCellsX + nodeX, dist);
				graph.AddEdge(NewEdge);

				if (!graph.isDigraph())
				{
					graph_type::EdgeType NewEdge(nodeY*NumCellsX + nodeX, row*NumCellsX + col, dist);
					graph.AddEdge(NewEdge);
				}
			}
		}
	}
}

template <class graph_type>
void GraphHelper_CreateGrid(graph_type& graph, int cySize, int cxSize, int NumCellsY, int NumCellsX)
{
	double CellWidth = (double)cySize / (double)NumCellsX;
	double CellHeight = (double)cxSize / (double)NumCellsY;

	double midX = CellWidth / 2;
	double midY = CellHeight / 2;

	for (int row = 0; row < NumCellsY; ++row)
	{
		for (int col = 0; col < NumCellsX; ++col)
		{
			graph.AddNode(NavGraphNode<>(graph.GetNextFreeNodeIndex(), Vector2D(midX + (col*CellWidth), midY + (row*CellHeight))));
		}
	}

	for (int row = 0; row < NumCellsY; ++row)
	{
		for (int col = 0; col < NumCellsX; ++col)
		{
			GraphHelper_AddAllNeighboursToGridNode(graph, row, col, NumCellsX, NumCellsY);
		}
	}
}

template <class graph_type>
void GraphHelper_DrawUsingGDI(const graph_type& graph, int color, bool DrawNodeIDs = false)
{
	if (graph.NumNodes() == 0) return;

	gdi->SetPenColor(color);

	graph_type::ConstNodeIterator NodeItr(graph);
	for (const graph_type::NodeType* pN = NodeItr.begin(); !NodeItr.end(); pN = NodeItr.next())
	{
		gdi->Circle(pN->Pos(), 2);

		if (DrawNodeIDs)
		{
			gdi->TextColor(200, 200, 200);
			gdi->TextAtPos((int)pN->Pos().x + 5, (int)pN->Pos().y - 5, ttos(pN->Index()));
		}

		graph_type::ConstEdgeIterator EdgeItr(graph, pN->Index());
		for (const graph_type::EdgeType* pE = EdgeItr.begin();
			!EdgeItr.end();
			pE = EdgeItr.next())
		{
			gdi->Line(pN->Pos(), graph.GetNode(pE->To()).Pos());
		}
	}
}

template <class graph_type>
void WeightNavGraphNodeEdges(graph_type& graph, int node, double weight)
{
	assert(node < graph.NumNodes());

	graph_type::ConstEdgeIterator ConstEdgeItr(graph, node);
	for (const graph_type::EdgeType* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
	{
		double dist = Vec2DDistance(graph.GetNode(pE->From()).Pos(), graph.GetNode(pE->To()).Pos());

		graph.SetEdgeCost(pE->From(), pE->To(), dist * weight);

		if (!graph.isDigraph()) graph.SetEdgeCost(pE->To(), pE->From(), dist * weight);
	}
}

template <class graph_type>
std::vector<std::vector<int>> CreateAllPairsTable(const graph_type& G)
{
	enum { no_path = -1 };

	std::vector<int> row(G.NumNodes(), no_path);

	std::vector<std::vector<int> > ShortestPaths(G.NumNodes(), row);

	for (int source = 0; source < G.NumNodes(); ++source)
	{
		Graph_SearchDijkstra<graph_type> search(G, source);

		std::vector<const graph_type::EdgeType*> spt = search.GetSPT();

		for (int target = 0; target < G.NumNodes(); ++target)
		{
			if (source == target) ShortestPaths[source][target] = target;
			else
			{
				int nd = target;
				while ((nd != source) && (spt[nd] != 0))
				{
					ShortestPaths[spt[nd]->From][target] = nd;
					nd = spt[nd]->From;
				}
			}
		}
	}

	return ShortestPaths;
}

template <class graph_type>
std::vector<std::vector<double>> CreateAllPairsCostsTable(const graph_type& G)
{
	std::vector<double> row(G.NumNodes(), 0.0);
	std::vector<std::vector<double> > PathCosts(G.NumNodes(), row);

	for (int source = 0; source < G.NumNodes(); ++source)
	{
		Graph_SearchDijkstra<graph_type> search(G, source);

		for (int target = 0; target < G.NumNodes(); ++target)
		{
			if (source != target) PathCosts[source][target] = search.GetCostToNode(target);
		}
	}

	return PathCosts;
}

template <class graph_type>
double CalculateAverageGraphEdgeLength(const graph_type& G)
{
	double TotalLength = 0;
	int NumEdgesCounted = 0;

	graph_type::ConstNodeIterator NodeItr(G);
	const graph_type::NodeType* pN;
	for (pN = NodeItr.begin(); !NodeItr.end(); pN = NodeItr.next())
	{
		graph_type::ConstEdgeIterator EdgeItr(G, pN->Index());
		for (const graph_type::EdgeType* pE = EdgeItr.begin(); !EdgeItr.end(); pE = EdgeItr.next())
		{
			++NumEdgesCounted;
			TotalLength += Vec2DDistance(G.GetNode(pE->From()).Pos(), G.GetNode(pE->To()).Pos());
		}
	}

	return TotalLength / (double)NumEdgesCounted;
}

template <class graph_type>
double GetCostliestGraphEdge(const graph_type& G)
{
	double greatest = MinDouble;

	graph_type::ConstNodeIterator NodeItr(G);
	const graph_type::NodeType* pN;
	for (pN = NodeItr.begin(); !NodeItr.end(); pN = NodeItr.next())
	{
		graph_type::ConstEdgeIterator EdgeItr(G, pN->Index());
		for (const graph_type::EdgeType* pE = EdgeItr.begin(); !EdgeItr.end(); pE = EdgeItr.next())
		{
			if (pE->Cost() > greatest)greatest = pE->Cost();
		}
	}

	return greatest;
}

#endif
