#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H
#pragma warning (disable:4786)

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <deque>

#include "../Graph/SparseGraph.h"
#include "../misc/PriorityQueue.h"


//----------------------------- Graph_SearchDFS -------------------------------

template<class graph_type>
class Graph_SearchDFS
{
private:
	enum { visited, unvisited, no_parent_assigned };

	typedef typename graph_type::EdgeType Edge;
	typedef typename graph_type::NodeType Node;

private:
	const graph_type& m_Graph;

	std::vector<int> m_Visited;
	std::vector<int> m_Route; // �� ����� �θ� ��带 ���. (��, ���� '����' ���� ���� ��ΰ� '�ƴ�' ��� '�Դ�' ���� ���� ���)
	std::vector<const Edge*> m_SpanningTree; // ����Ʈ�� (����Ʈ��)

	int m_iSource, m_iTarget;
	bool m_bFound;

	bool Search();

public:
	Graph_SearchDFS(const graph_type& graph, int source, int target = -1) :
		m_Graph(graph),
		m_iSource(source),
		m_iTarget(target),
		m_bFound(false),
		m_Visited(m_Graph.NumNodes(), unvisited),
		m_Route(m_Graph.NumNodes(), no_parent_assigned)
	{
		m_bFound = Search();
	}

	std::vector<const Edge*> GetSearchTree() const { return m_SpanningTree; }

	bool Found() const { return m_bFound; }

	std::list<int> GetPathToTarget() const;
};

template <class graph_type>
bool Graph_SearchDFS<graph_type>::Search()
{
	std::stack<const Edge*> stack; // �������� ����.

	Edge Dummy(m_iSource, m_iSource, 0); // ���� ������ ���ÿ� �ְ� �����Ѵ�.
	stack.push(&Dummy);

	while (!stack.empty())
	{
		const Edge* Next = stack.top(); // ������ ž�� �ִ� ������ ������,
		stack.pop();

		m_Route[Next->To()] = Next->From(); // ��ο� ����Ѵ�.

		if (Next != &Dummy) m_SpanningTree.push_back(Next); // ���� ������ ���� ������ �ƴ϶��, ����Ʈ���� �߰�.

		m_Visited[Next->To()] = visited; // ���� �湮 ��ũ�� �Ѵ�.

		if (Next->To() == m_iTarget) return true; // ���� 'To' ��尡 Ÿ���̶�� ����.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, Next->To()); // 'To' ��忡 ����� ��� �������� ��ȸ�Ѵ�.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			if (m_Visited[pE->To()] == unvisited) stack.push(pE); // ã�� ������ �湮���� ���� �������, ���ÿ� �ִ´�.
		}
	}

	return false;
}

template <class graph_type>
std::list<int> Graph_SearchDFS<graph_type>::GetPathToTarget() const
{
	std::list<int> path;
	if (!m_bFound || m_iTarget < 0) return path; // Ÿ�������� ��θ� ã�� ���ߴٸ�, �� ��θ� ����.

	int nd = m_iTarget; // Ÿ�� ��� ����,
	path.push_front(nd);

	while (nd != m_iSource) // ���� ��� ���� �������� ����.
	{
		nd = m_Route[nd];
		path.push_front(nd);
	}

	return path;
}



//----------------------------- Graph_SearchIterDeep -------------------------------

template<class graph_type>
class Graph_SearchIterDeep
{
private:
	enum { visited, unvisited, no_parent_assigned };

	typedef typename graph_type::EdgeType Edge;
	typedef typename graph_type::NodeType Node;

private:
	const graph_type& m_Graph;

	std::vector<int> m_Visited;
	std::vector<int> m_Route;
	std::vector<const Edge*> m_SpanningTree;
	std::vector<int> m_Depth;

	int m_iSource, m_iTarget;
	bool m_bFound;
	int m_Depth_Limit;

	bool Search();

public:
	Graph_SearchIterDeep(const graph_type& graph, int source, int target = -1) :
		m_Graph(graph),
		m_iSource(source),
		m_iTarget(target),
		m_bFound(false),
		m_Depth_Limit(1),
		m_Visited(m_Graph.NumNodes(), unvisited),
		m_Route(m_Graph.NumNodes(), no_parent_assigned),
		m_Depth(m_Graph.NumNodes(), 0)
	{
		m_bFound = Search();
	}

	std::vector<const Edge*> GetSearchTree() const { return m_SpanningTree; }

	bool Found() const { return m_bFound; }

	std::list<int> GetPathToTarget() const;
};

template <class graph_type>
bool Graph_SearchIterDeep<graph_type>::Search()
{
	int limit = abs(m_iSource - m_iTarget);
	while (m_Depth_Limit <= limit)
	{
		std::stack<const Edge*> stack; // Ž�� ����

		Edge Dummy(m_iSource, m_iSource, 0); // ���� ������ Ž�� ���ÿ� �ְ� �����Ѵ�.
		stack.push(&Dummy);

		while (!stack.empty())
		{
			const Edge* Next = stack.top(); // ������ ž�� �ִ� ������ ������,
			stack.pop();

			m_Route[Next->To()] = Next->From(); // ��ο� ����Ѵ�.

			if (Next != &Dummy) m_SpanningTree.push_back(Next); // ���� ������ ���� ������ �ƴ϶��, ����Ʈ���� �߰�.

			m_Visited[Next->To()] = visited; // ���� �湮 ��ũ�� �Ѵ�.

			if (Next->To() == m_iTarget) return true; // ���� 'To' ��尡 Ÿ���̶�� ����.

			int next_Depth = m_Depth[Next->To()] + 1;
			graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, Next->To()); // 'To' ��忡 ����� ��� �������� ��ȸ�Ѵ�.
			for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
			{
				if (m_Visited[pE->To()] == unvisited && next_Depth < m_Depth_Limit)
				{
					// ã�� ������ �湮���� ���� �����̰�, ���̰� ���� ���̸� ���� �ʾҴٸ�
					m_Depth[pE->To()] = next_Depth; // ���� ���� ���.
					stack.push(pE); // ���ÿ� ���� ����.
				}
			}
		}
		++m_Depth_Limit; // ���� ���� ����.
		m_Visited = vector<int>(m_Graph.NumNodes(), unvisited); // �湮 ���� �ʱ�ȭ.
		m_Route = vector<int>(m_Graph.NumNodes(), no_parent_assigned); // ��� �ʱ�ȭ.
		m_SpanningTree.clear(); // ����Ʈ�� �ʱ�ȭ.
	}
	return false;
}

template <class graph_type>
std::list<int> Graph_SearchIterDeep<graph_type>::GetPathToTarget() const
{
	std::list<int> path;
	if (!m_bFound || m_iTarget < 0) return path; // Ÿ�������� ��θ� ã�� ���ߴٸ�, �� ��θ� ����.

	int nd = m_iTarget; // Ÿ�� ��� ����,
	path.push_front(nd);

	while (nd != m_iSource) // ���� ��� ���� �������� ����.
	{
		nd = m_Route[nd];
		path.push_front(nd);
	}

	return path;
}



//----------------------------- Graph_SearchBFS -------------------------------

template<class graph_type>
class Graph_SearchBFS
{
private:
	enum { visited, unvisited, no_parent_assigned };
	typedef typename graph_type::EdgeType Edge;

private: // DFS�� ������ ����.
	const graph_type& m_Graph;

	std::vector<int> m_Visited;
	std::vector<int> m_Route;
	std::vector<const Edge*> m_SpanningTree;

	int m_iSource, m_iTarget;
	bool m_bFound;

	bool Search();

public:
	Graph_SearchBFS(const graph_type& graph, int source, int target = -1) :
		m_Graph(graph),
		m_iSource(source),
		m_iTarget(target),
		m_bFound(false),
		m_Visited(m_Graph.NumNodes(), unvisited),
		m_Route(m_Graph.NumNodes(), no_parent_assigned)
	{
		m_bFound = Search();
	}

	bool Found() const { return m_bFound; }

	std::vector<const Edge*> GetSearchTree() const { return m_SpanningTree; }

	std::list<int> GetPathToTarget() const;
};

template <class graph_type>
bool Graph_SearchBFS<graph_type>::Search()
{
	std::queue<const Edge*> Q; // ť�� ����

	const Edge Dummy(m_iSource, m_iSource, 0); // ���� ������ ť�� �ְ� �����Ѵ�.
	Q.push(&Dummy);

	m_Visited[m_iSource] = visited; // ������ �湮 ��ũ.

	while (!Q.empty())
	{
		const Edge* Next = Q.front(); // ť�� �� �տ� �ִ� ��带 ������,
		Q.pop();

		m_Route[Next->To()] = Next->From(); // ��ο� ����Ѵ�.

		if (Next != &Dummy) m_SpanningTree.push_back(Next); // ���� ������ ���� ������ �ƴ϶��, ����Ʈ���� �߰�.

		if (Next->To() == m_iTarget) return true; // ���� 'To' ��尡 Ÿ���̶�� ����.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, Next->To()); // 'To' ��忡 ����� ��� �������� ��ȸ�Ѵ�.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			if (m_Visited[pE->To()] == unvisited) // ã�� ������ �湮���� ���� �������, ť�� �ְ� �湮 ��ũ.
			{
				Q.push(pE);
				m_Visited[pE->To()] = visited;
			}
		}
	}

	return false;
}

template <class graph_type>
std::list<int> Graph_SearchBFS<graph_type>::GetPathToTarget() const
{
	// DFS�� ����
	std::list<int> path;
	if (!m_bFound || m_iTarget < 0) return path;

	int nd = m_iTarget;
	path.push_front(nd);

	while (nd != m_iSource)
	{
		nd = m_Route[nd];
		path.push_front(nd);
	}

	return path;
}



//----------------------- Graph_SearchDijkstra --------------------------------
template <class graph_type>
class Graph_SearchDijkstra
{
private:
	typedef typename graph_type::EdgeType Edge;

private:
	const graph_type& m_Graph;
	std::vector<const Edge*> m_ShortestPathTree; // �ִ� ��η� ������ ������.
	std::vector<double> m_CostToThisNode; // �˻��� ��� ����� ������ ����.
	std::vector<const Edge*> m_SearchFrontier; // �˻��� ��� �������� ����.

	int m_iSource, m_iTarget;

	void Search();

public:
	Graph_SearchDijkstra(const graph_type &graph, int source, int target = -1) :
		m_Graph(graph),
		m_ShortestPathTree(graph.NumNodes()),
		m_SearchFrontier(graph.NumNodes()),
		m_CostToThisNode(graph.NumNodes()),
		m_iSource(source),
		m_iTarget(target)
	{
		Search();
	}

	std::vector<const Edge*> GetSPT() const { return m_ShortestPathTree; }

	std::list<int> GetPathToTarget() const;

	double GetCostToTarget() const { return m_CostToThisNode[m_iTarget]; }
	double GetCostToNode(unsigned int nd) const { return m_CostToThisNode[nd]; }
};

template <class graph_type>
void Graph_SearchDijkstra<graph_type>::Search()
{
	IndexedPriorityQLow<double> pq(m_CostToThisNode, m_Graph.NumNodes());
	pq.insert(m_iSource);

	while (!pq.empty())
	{
		int NextClosestNode = pq.Pop(); // �켱���� ť�� ��带 pop�Ѵ�.
		
		m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode]; // �ִ� ����� �ش� ��忡 ����Ƽ� ��ϵ� �ش� ����� '������ ���'.

		if (NextClosestNode == m_iTarget) return; // ���� ��尡 Ÿ���̶�� ����.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, NextClosestNode); // ���� ��忡 ���� ��� �������� ��ȸ.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			double NewCost = m_CostToThisNode[NextClosestNode] + pE->Cost(); // ���� ��� ������ ��� + ���� ��忡�� ���� ������ ���.

			if (m_SearchFrontier[pE->To()] == 0) // ����Ƽ���� �ش� ��带 ó�� �湮�Ѵٸ�, (�ƹ��͵� ��ϵǾ����� �ʴٸ�)
			{
				m_CostToThisNode[pE->To()] = NewCost; // �ش� ��忡 '���� ���� ���'�� ���.
				pq.insert(pE->To()); // �켱���� ť�� �ش� ��� ����.
				m_SearchFrontier[pE->To()] = pE; // ����Ƽ���� �ش� ��忡 �ش� ������ ���.
			}
			// ����Ƽ� �ش� ������ �̹� ��ϵǾ� �ִٸ�, ����� �� ���� ������ ����ϵ��� ��ü �ؾ��Ѵ�.
			else if ((NewCost < m_CostToThisNode[pE->To()]) && (m_ShortestPathTree[pE->To()] == 0))
			{
				m_CostToThisNode[pE->To()] = NewCost; // �ش� ��� ������ ����� '���� ���� ���'���� ��ü.
				pq.ChangePriority(pE->To()); // �ش� ��� ������ ����� �������Ƿ�, �켱 ���� ť �ȿ� �ִ� �ش� ����� �켱������ �����Ǿ�� �Ұ�.
				m_SearchFrontier[pE->To()] = pE; // ����Ƽ� ��ϵ� ������ ��ü.
			}
		}
	}
}

template <class graph_type>
std::list<int> Graph_SearchDijkstra<graph_type>::GetPathToTarget() const
{
	std::list<int> path;
	if (m_iTarget < 0) return path; // Ÿ�ٱ����� ��θ� ã�� ���ߴٸ�, �� ��θ� ����.

	int nd = m_iTarget; // Ÿ�� ��� ����,
	path.push_front(nd);

	while ((nd != m_iSource) && (m_ShortestPathTree[nd] != 0)) // �������� ��ȸ.
	{
		nd = m_ShortestPathTree[nd]->From();
		path.push_front(nd);
	}

	return path;
}


//------------------------------- Graph_SearchAStar --------------------------
template <class graph_type, class heuristic>
class Graph_SearchAStar
{
private:
	typedef typename graph_type::EdgeType Edge;

private:
	const graph_type& m_Graph;

	std::vector<double> m_GCosts; // �� ����� G ��� (���� �⺻ ���)
	std::vector<double> m_FCosts; // �� ����� F ��� (G + �޸���ƽ ���)

	std::vector<const Edge*> m_ShortestPathTree;
	std::vector<const Edge*> m_SearchFrontier;

	int m_iSource, m_iTarget;

	void Search();

public:
	Graph_SearchAStar(graph_type &graph, int source, int target) :
		m_Graph(graph),
		m_ShortestPathTree(graph.NumNodes()),
		m_SearchFrontier(graph.NumNodes()),
		m_GCosts(graph.NumNodes(), 0.0),
		m_FCosts(graph.NumNodes(), 0.0),
		m_iSource(source),
		m_iTarget(target)
	{
		Search();
	}

	std::vector<const Edge*> GetSPT() const { return m_ShortestPathTree; }

	std::list<int> GetPathToTarget() const;

	double GetCostToTarget() const { return m_GCosts[m_iTarget]; }
};

template <class graph_type, class heuristic>
void Graph_SearchAStar<graph_type, heuristic>::Search()
{
	IndexedPriorityQLow<double> pq(m_FCosts, m_Graph.NumNodes()); // F ����� �������� �ϴ� �켱���� ť ����.
	pq.insert(m_iSource); // ���� ��带 �켱���� ť�� ����. 

	while (!pq.empty())
	{
		int NextClosestNode = pq.Pop(); // �켱���� ť���� F ����� ���� ���� ��带 pop.
		
		m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode]; // ���� ���� ���� ��θ� ���� (�ִ� ���)

		if (NextClosestNode == m_iTarget) return; // ���� ��尡 Ÿ���̸� ����.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, NextClosestNode); // ���� ��忡 ���� ��� ������ ��ȸ.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			double HCost = heuristic::Calculate(m_Graph, m_iTarget, pE->To()); // ���� ��忡�� Ÿ������ ���� �޸���ƽ ��� ���.
			double GCost = m_GCosts[NextClosestNode] + pE->Cost(); // ���� ��� ���� ���� ������ ��� + ���� ���� ���� ��� ���.
			
			// ����Ƽ���� ���� ��忡 ������ ��ϵ����� �ʴٸ�,
			if (m_SearchFrontier[pE->To()] == NULL)
			{
				m_FCosts[pE->To()] = GCost + HCost; // F ����� ���.
				m_GCosts[pE->To()] = GCost; // G ����� ���.
				pq.insert(pE->To()); // ���� ��带 �켱���� ť�� ����.
				m_SearchFrontier[pE->To()] = pE; // ����Ƽ���� ���� ��忡 �ش� ������ ���.
			}
			// ����Ƽ���� ���� ��忡 ������ ��ϵ��ִٸ�, ����� �� ���� ��θ� �����ϵ��� �ؾ��Ѵ�.
			else if ((GCost < m_GCosts[pE->To()]) && (m_ShortestPathTree[pE->To()] == NULL))
			{
				m_FCosts[pE->To()] = GCost + HCost; // F ����� ���.
				m_GCosts[pE->To()] = GCost; // G ����� ���.
				pq.ChangePriority(pE->To()); // �켱���� ť �ȿ� �ִ� ���� ����� �켱������ ������ �ʿ��ϴ�. (��� ����)
				m_SearchFrontier[pE->To()] = pE; // ����Ƽ���� ���� ��忡 �ش� ������ ���.
			}
			// ���� ����� ����� �� �۴ٸ� �ƹ��͵� ���ؾ� �Ѵ�.
		}
	}
}

template <class graph_type, class heuristic>
std::list<int> Graph_SearchAStar<graph_type, heuristic>::GetPathToTarget() const
{
	// ���ͽ�Ʈ��� ����.
	std::list<int> path;
	if (m_iTarget < 0) return path;

	int nd = m_iTarget;
	path.push_front(nd);

	while ((nd != m_iSource) && (m_ShortestPathTree[nd] != 0))
	{
		nd = m_ShortestPathTree[nd]->From();
		path.push_front(nd);
	}

	return path;
}



//---------------------- Graph_MinSpanningTree --------------------------------
//
//  given a graph and a source node you can use this class to calculate
//  the minimum spanning tree. If no source node is specified then the 
//  algorithm will calculate a spanning forest starting from node 1 
//
//  It uses a priority first queue implementation of Prims algorithm
//------------------------------------------------------------------------
template <class graph_type>
class Graph_MinSpanningTree
{
private:
	typedef typename graph_type::EdgeType Edge;

	const graph_type& m_Graph;

	std::vector<double> m_CostToThisNode;

	std::vector<const Edge*> m_SpanningTree;
	std::vector<const Edge*> m_Fringe;

	void Search(const int source)
	{
		//create a priority queue
		IndexedPriorityQLow<double> pq(m_CostToThisNode, m_Graph.NumNodes());

		//put the source node on the queue
		pq.insert(source);

		//while the queue is not empty
		while (!pq.empty())
		{
			//get lowest cost edge from the queue
			int best = pq.Pop();

			//move this edge from the fringe to the spanning tree
			m_SpanningTree[best] = m_Fringe[best];

			//now to test the edges attached to this node
			graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, best);

			for (const Edge* pE = ConstEdgeItr.beg(); !ConstEdgeItr.end(); pE = ConstEdgeItr.nxt())
			{
				double Priority = pE->Cost;

				if (m_Fringe[pE->To()] == 0)
				{
					m_CostToThisNode[pE->To()] = Priority;
					pq.insert(pE->To());
					m_Fringe[pE->To()] = pE;
				}
				else if ((Priority < m_CostToThisNode[pE->To()]) && (m_SpanningTree[pE->To()] == 0))
				{
					m_CostToThisNode[pE->To()] = Priority;
					pq.ChangePriority(pE->To());
					m_Fringe[pE->To()] = pE;
				}
			}
		}
	}

public:
	Graph_MinSpanningTree(graph_type &G, int source = -1) :
		m_Graph(G),
		m_SpanningTree(G.NumNodes()),
		m_Fringe(G.NumNodes()),
		m_CostToThisNode(G.NumNodes(), -1)
	{
		if (source < 0)
		{
			for (int nd = 0; nd < G.NumNodes(); ++nd)
			{
				if (m_SpanningTree[nd] == 0) Search(nd);
			}
		}
		else Search(source);
	}

	std::vector<const Edge*> GetSpanningTree() const { return m_SpanningTree; }
};

#endif