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
	std::vector<int> m_Route; // 각 노드의 부모 노드를 기록. (즉, 어디로 '가는' 지에 대한 경로가 '아닌' 어디서 '왔는' 지에 대한 경로)
	std::vector<const Edge*> m_SpanningTree; // 서브트리 (신장트리)

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
	std::stack<const Edge*> stack; // 스택으로 구현.

	Edge Dummy(m_iSource, m_iSource, 0); // 더미 엣지를 스택에 넣고 시작한다.
	stack.push(&Dummy);

	while (!stack.empty())
	{
		const Edge* Next = stack.top(); // 스택의 탑에 있는 엣지를 꺼내서,
		stack.pop();

		m_Route[Next->To()] = Next->From(); // 경로에 기록한다.

		if (Next != &Dummy) m_SpanningTree.push_back(Next); // 꺼낸 엣지가 더미 엣지가 아니라면, 서브트리에 추가.

		m_Visited[Next->To()] = visited; // 또한 방문 마크를 한다.

		if (Next->To() == m_iTarget) return true; // 만약 'To' 노드가 타겟이라면 종료.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, Next->To()); // 'To' 노드에 연결된 모든 엣지들을 순회한다.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			if (m_Visited[pE->To()] == unvisited) stack.push(pE); // 찾은 엣지가 방문되지 않은 엣지라면, 스택에 넣는다.
		}
	}

	return false;
}

template <class graph_type>
std::list<int> Graph_SearchDFS<graph_type>::GetPathToTarget() const
{
	std::list<int> path;
	if (!m_bFound || m_iTarget < 0) return path; // 타겟으로의 경로를 찾지 못했다면, 빈 경로를 리턴.

	int nd = m_iTarget; // 타겟 노드 부터,
	path.push_front(nd);

	while (nd != m_iSource) // 시작 노드 까지 역순으로 추적.
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
		std::stack<const Edge*> stack; // 탐색 스택

		Edge Dummy(m_iSource, m_iSource, 0); // 더미 엣지를 탐색 스택에 넣고 시작한다.
		stack.push(&Dummy);

		while (!stack.empty())
		{
			const Edge* Next = stack.top(); // 스택의 탑에 있는 엣지를 꺼내서,
			stack.pop();

			m_Route[Next->To()] = Next->From(); // 경로에 기록한다.

			if (Next != &Dummy) m_SpanningTree.push_back(Next); // 꺼낸 엣지가 더미 엣지가 아니라면, 서브트리에 추가.

			m_Visited[Next->To()] = visited; // 또한 방문 마크를 한다.

			if (Next->To() == m_iTarget) return true; // 만약 'To' 노드가 타겟이라면 종료.

			int next_Depth = m_Depth[Next->To()] + 1;
			graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, Next->To()); // 'To' 노드에 연결된 모든 엣지들을 순회한다.
			for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
			{
				if (m_Visited[pE->To()] == unvisited && next_Depth < m_Depth_Limit)
				{
					// 찾은 엣지가 방문되지 않은 엣지이고, 깊이가 제한 깊이를 넘지 않았다면
					m_Depth[pE->To()] = next_Depth; // 깊이 값을 기록.
					stack.push(pE); // 스택에 엣지 삽입.
				}
			}
		}
		++m_Depth_Limit; // 제한 깊이 증가.
		m_Visited = vector<int>(m_Graph.NumNodes(), unvisited); // 방문 여부 초기화.
		m_Route = vector<int>(m_Graph.NumNodes(), no_parent_assigned); // 경로 초기화.
		m_SpanningTree.clear(); // 서브트리 초기화.
	}
	return false;
}

template <class graph_type>
std::list<int> Graph_SearchIterDeep<graph_type>::GetPathToTarget() const
{
	std::list<int> path;
	if (!m_bFound || m_iTarget < 0) return path; // 타겟으로의 경로를 찾지 못했다면, 빈 경로를 리턴.

	int nd = m_iTarget; // 타겟 노드 부터,
	path.push_front(nd);

	while (nd != m_iSource) // 시작 노드 까지 역순으로 추적.
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

private: // DFS와 동일한 구성.
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
	std::queue<const Edge*> Q; // 큐로 구현

	const Edge Dummy(m_iSource, m_iSource, 0); // 더미 엣지를 큐에 넣고 시작한다.
	Q.push(&Dummy);

	m_Visited[m_iSource] = visited; // 시작점 방문 마크.

	while (!Q.empty())
	{
		const Edge* Next = Q.front(); // 큐의 맨 앞에 있는 노드를 꺼내서,
		Q.pop();

		m_Route[Next->To()] = Next->From(); // 경로에 기록한다.

		if (Next != &Dummy) m_SpanningTree.push_back(Next); // 꺼낸 엣지가 더미 엣지가 아니라면, 서브트리에 추가.

		if (Next->To() == m_iTarget) return true; // 만약 'To' 노드가 타겟이라면 종료.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, Next->To()); // 'To' 노드에 연결된 모든 엣지들을 순회한다.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			if (m_Visited[pE->To()] == unvisited) // 찾은 엣지가 방문되지 않은 엣지라면, 큐에 넣고 방문 마크.
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
	// DFS와 동일
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
	std::vector<const Edge*> m_ShortestPathTree; // 최단 경로로 선정된 엣지들.
	std::vector<double> m_CostToThisNode; // 검색된 모든 노드의 비용들을 저장.
	std::vector<const Edge*> m_SearchFrontier; // 검색된 모든 엣지들을 저장.

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
		int NextClosestNode = pq.Pop(); // 우선순위 큐의 노드를 pop한다.
		
		m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode]; // 최단 경로의 해당 노드에 프론티어에 기록된 해당 노드의 '엣지를 기록'.

		if (NextClosestNode == m_iTarget) return; // 꺼낸 노드가 타겟이라면 종료.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, NextClosestNode); // 현재 노드에 붙은 모든 엣지들을 순회.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			double NewCost = m_CostToThisNode[NextClosestNode] + pE->Cost(); // 현재 노드 까지의 비용 + 현재 노드에서 다음 노드로의 비용.

			if (m_SearchFrontier[pE->To()] == 0) // 프론티어의 해당 노드를 처음 방문한다면, (아무것도 기록되어있지 않다면)
			{
				m_CostToThisNode[pE->To()] = NewCost; // 해당 노드에 '새로 구한 비용'을 기록.
				pq.insert(pE->To()); // 우선순위 큐에 해당 노드 삽입.
				m_SearchFrontier[pE->To()] = pE; // 프론티어의 해당 노드에 해당 엣지를 기록.
			}
			// 프론티어에 해당 엣지가 이미 기록되어 있다면, 비용이 더 작은 엣지를 기록하도록 교체 해야한다.
			else if ((NewCost < m_CostToThisNode[pE->To()]) && (m_ShortestPathTree[pE->To()] == 0))
			{
				m_CostToThisNode[pE->To()] = NewCost; // 해당 노드 까지의 비용을 '새로 구한 비용'으로 교체.
				pq.ChangePriority(pE->To()); // 해당 노드 까지의 비용이 변했으므로, 우선 순위 큐 안에 있는 해당 노드의 우선순위도 변동되어야 할것.
				m_SearchFrontier[pE->To()] = pE; // 프론티어에 기록된 엣지를 교체.
			}
		}
	}
}

template <class graph_type>
std::list<int> Graph_SearchDijkstra<graph_type>::GetPathToTarget() const
{
	std::list<int> path;
	if (m_iTarget < 0) return path; // 타겟까지의 경로를 찾지 못했다면, 빈 경로를 리턴.

	int nd = m_iTarget; // 타겟 노드 부터,
	path.push_front(nd);

	while ((nd != m_iSource) && (m_ShortestPathTree[nd] != 0)) // 역순으로 순회.
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

	std::vector<double> m_GCosts; // 각 노드의 G 비용 (누적 기본 비용)
	std::vector<double> m_FCosts; // 각 노드의 F 비용 (G + 휴리스틱 비용)

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
	IndexedPriorityQLow<double> pq(m_FCosts, m_Graph.NumNodes()); // F 비용을 기준으로 하는 우선순위 큐 생성.
	pq.insert(m_iSource); // 시작 노드를 우선순위 큐에 삽입. 

	while (!pq.empty())
	{
		int NextClosestNode = pq.Pop(); // 우선순위 큐에서 F 비용이 가장 낮은 노드를 pop.
		
		m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode]; // 꺼낸 노드로 가는 경로를 구성 (최단 경로)

		if (NextClosestNode == m_iTarget) return; // 꺼낸 노드가 타겟이면 종료.

		graph_type::ConstEdgeIterator ConstEdgeItr(m_Graph, NextClosestNode); // 꺼낸 노드에 붙은 모든 엣지를 순회.
		for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
		{
			double HCost = heuristic::Calculate(m_Graph, m_iTarget, pE->To()); // 다음 노드에서 타겟으로 가는 휴리스틱 비용 계산.
			double GCost = m_GCosts[NextClosestNode] + pE->Cost(); // 현재 노드 까지 오는 누적된 비용 + 다음 노드로 가는 비용 계산.
			
			// 프론티어의 다음 노드에 엣지가 기록돼있지 않다면,
			if (m_SearchFrontier[pE->To()] == NULL)
			{
				m_FCosts[pE->To()] = GCost + HCost; // F 비용을 기록.
				m_GCosts[pE->To()] = GCost; // G 비용을 기록.
				pq.insert(pE->To()); // 다음 노드를 우선순위 큐에 삽입.
				m_SearchFrontier[pE->To()] = pE; // 프론티어의 다음 노드에 해당 엣지를 기록.
			}
			// 프론티어의 다음 노드에 엣지가 기록돼있다면, 비용이 더 작은 경로를 선택하도록 해야한다.
			else if ((GCost < m_GCosts[pE->To()]) && (m_ShortestPathTree[pE->To()] == NULL))
			{
				m_FCosts[pE->To()] = GCost + HCost; // F 비용을 기록.
				m_GCosts[pE->To()] = GCost; // G 비용을 기록.
				pq.ChangePriority(pE->To()); // 우선순위 큐 안에 있는 다음 노드의 우선순위의 조정이 필요하다. (비용 변경)
				m_SearchFrontier[pE->To()] = pE; // 프론티어의 다음 노드에 해당 엣지를 기록.
			}
			// 기존 경로의 비용이 더 작다면 아무것도 안해야 한다.
		}
	}
}

template <class graph_type, class heuristic>
std::list<int> Graph_SearchAStar<graph_type, heuristic>::GetPathToTarget() const
{
	// 다익스트라와 동일.
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