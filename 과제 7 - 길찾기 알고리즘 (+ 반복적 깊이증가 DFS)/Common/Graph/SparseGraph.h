#ifndef SPARSEGRAPH_H
#define SPARSEGRAPH_H
#pragma warning (disable:4786)

#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <iostream>

#include "../2D/Vector2D.h"
#include "../misc/utils.h" 
#include "../Graph/NodeTypeEnumerations.h"

template <class node_type, class edge_type>   
class SparseGraph
{
public:
	typedef edge_type EdgeType;
	typedef node_type NodeType;

	typedef std::vector<node_type> NodeVector;
	typedef std::list<edge_type> EdgeList;
	typedef std::vector<EdgeList> EdgeListVector;

private:
	NodeVector m_Nodes;
	EdgeListVector m_Edges;
	bool m_bDigraph;
	int m_iNextNodeIndex;
	bool UniqueEdge(int from, int to) const;
	void CullInvalidEdges();

public:
	SparseGraph(bool digraph) : m_iNextNodeIndex(0), m_bDigraph(digraph) {}

	const NodeType&  GetNode(int idx)const;
	NodeType&  GetNode(int idx);

	const EdgeType& GetEdge(int from, int to) const;
	EdgeType& GetEdge(int from, int to);

	int GetNextFreeNodeIndex() const { return m_iNextNodeIndex; }

	int AddNode(node_type node);
	void RemoveNode(int node);

	void AddEdge(EdgeType edge);
	void RemoveEdge(int from, int to);

	void SetEdgeCost(int from, int to, double cost);

	int NumNodes() const { return m_Nodes.size(); }

	int NumActiveNodes() const
	{
		int count = 0;

		for (unsigned int n = 0; n < m_Nodes.size(); ++n) if (m_Nodes[n].Index() != invalid_node_index) ++count;

		return count;
	}

	int NumEdges() const
	{
		int tot = 0;

		for (EdgeListVector::const_iterator curEdge = m_Edges.begin(); curEdge != m_Edges.end(); ++curEdge)
		{
			tot += curEdge->size();
		}

		return tot;
	}

	bool isDigraph() const { return m_bDigraph; }
	bool isEmpty() const { return m_Nodes.empty(); }
	bool isNodePresent(int nd) const;
	bool isEdgePresent(int from, int to) const;

	bool  Save(const char* FileName) const;
	bool  Save(std::ofstream& stream) const;

	bool  Load(const char* FileName);
	bool  Load(std::ifstream& stream);

	void Clear() { m_iNextNodeIndex = 0; m_Nodes.clear(); m_Edges.clear(); }

	void RemoveEdges()
	{
		for (EdgeListVector::iterator it = m_Edges.begin(); it != m_Edges.end(); ++it)
		{
			it->clear();
		}
	}

	class EdgeIterator // 특정 노드에 연결된 모든 엣지를 순회하는 클래스
	{
	private:
		typename EdgeList::iterator curEdge;
		SparseGraph<node_type, edge_type>& G;
		const int NodeIndex;

	public:
		EdgeIterator(SparseGraph<node_type, edge_type>& graph, int node) : G(graph), NodeIndex(node)
		{
			curEdge = G.m_Edges[NodeIndex].begin();
		}

		EdgeType* begin()
		{
			curEdge = G.m_Edges[NodeIndex].begin();
			return &(*curEdge);
		}

		EdgeType* next()
		{
			++curEdge;
			if (end()) return NULL;
			return &(*curEdge);
		}

		bool end()
		{
			return (curEdge == G.m_Edges[NodeIndex].end());
		}
	};
	friend class EdgeIterator;

	class ConstEdgeIterator // 특정 노드에 연결된 모든 엣지를 순회하는 const 클래스
	{
	private:
		typename EdgeList::const_iterator curEdge;
		const SparseGraph<node_type, edge_type>& G;
		const int NodeIndex;

	public:
		ConstEdgeIterator(const SparseGraph<node_type, edge_type>& graph, int node) : G(graph), NodeIndex(node)
		{
			curEdge = G.m_Edges[NodeIndex].begin();
		}

		const EdgeType* begin()
		{
			curEdge = G.m_Edges[NodeIndex].begin();
			if (end()) return 0;
			return &(*curEdge);
		}

		const EdgeType* next()
		{
			++curEdge;
			if (end()) return NULL;
			else return &(*curEdge);
		}

		bool end()
		{
			return (curEdge == G.m_Edges[NodeIndex].end());
		}
	};
	friend class ConstEdgeIterator;

	class NodeIterator // 그래프의 노드들을 순회하는데 사용하는 클래스.
	{
	private:
		typename NodeVector::iterator curNode;
		SparseGraph<node_type, edge_type>& G;

		void GetNextValidNode(typename NodeVector::iterator& it)
		{
			if (curNode == G.m_Nodes.end() || it->Index() != invalid_node_index) return;

			while ((it->Index() == invalid_node_index))
			{
				++it;
				if (curNode == G.m_Nodes.end()) break;
			}
		}

	public:
		NodeIterator(SparseGraph<node_type, edge_type> &graph) : G(graph)
		{
			curNode = G.m_Nodes.begin();
		}

		node_type* begin()
		{
			curNode = G.m_Nodes.begin();
			GetNextValidNode(curNode);
			return &(*curNode);
		}

		node_type* next()
		{
			++curNode;
			if (end()) return NULL;
			GetNextValidNode(curNode);
			return &(*curNode);
		}

		bool end()
		{
			return (curNode == G.m_Nodes.end());
		}
	};
	friend class NodeIterator;

	class ConstNodeIterator // 그래프의 노드들을 순회하는데 사용하는 const 클래스.
	{
	private:
		typename NodeVector::const_iterator	curNode;
		const SparseGraph<node_type, edge_type>& G;

		void GetNextValidNode(typename NodeVector::const_iterator& it)
		{
			if (curNode == G.m_Nodes.end() || it->Index() != invalid_node_index) return;

			while ((it->Index() == invalid_node_index))
			{
				++it;
				if (curNode == G.m_Nodes.end()) break;
			}
		}

	public:
		ConstNodeIterator(const SparseGraph<node_type, edge_type> &graph) : G(graph)
		{
			curNode = G.m_Nodes.begin();
		}

		const node_type* begin()
		{
			curNode = G.m_Nodes.begin();
			GetNextValidNode(curNode);
			return &(*curNode);
		}

		const node_type* next()
		{
			++curNode;
			if (end()) return NULL;
			else
			{
				GetNextValidNode(curNode);
				return &(*curNode);
			}
		}

		bool end()
		{
			return (curNode == G.m_Nodes.end());
		}
	};
	friend class ConstNodeIterator;
};

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::isNodePresent(int nd)const
{
	if ((nd >= (int)m_Nodes.size() || (m_Nodes[nd].Index() == invalid_node_index)))
	{
		return false;
	}
	else return true;
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::isEdgePresent(int from, int to)const
{
	if (isNodePresent(from) && isNodePresent(from))
	{
		for (EdgeList::const_iterator curEdge = m_Edges[from].begin();
			curEdge != m_Edges[from].end();
			++curEdge)
		{
			if (curEdge->To() == to) return true;
		}

		return false;
	}
	else return false;
}

template <class node_type, class edge_type>
const node_type&  SparseGraph<node_type, edge_type>::GetNode(int idx)const
{
	assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<SparseGraph::GetNode>: invalid index");

	return m_Nodes[idx];
}

template <class node_type, class edge_type>
node_type&  SparseGraph<node_type, edge_type>::GetNode(int idx)
{
	assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<SparseGraph::GetNode>: invalid index");
	return m_Nodes[idx];
}

template <class node_type, class edge_type>
const edge_type& SparseGraph<node_type, edge_type>::GetEdge(int from, int to)const
{
	assert((from < m_Nodes.size()) && (from >= 0) && m_Nodes[from].Index() != invalid_node_index && "<SparseGraph::GetEdge>: invalid 'from' index");
	assert((to < m_Nodes.size()) && (to >= 0) && m_Nodes[to].Index() != invalid_node_index && "<SparseGraph::GetEdge>: invalid 'to' index");

	for (EdgeList::const_iterator curEdge = m_Edges[from].begin(); curEdge != m_Edges[from].end(); ++curEdge)
	{
		if (curEdge->To() == to) return *curEdge;
	}

	assert(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

template <class node_type, class edge_type>
edge_type& SparseGraph<node_type, edge_type>::GetEdge(int from, int to)
{
	assert((from < m_Nodes.size()) && (from >= 0) && m_Nodes[from].Index() != invalid_node_index && "<SparseGraph::GetEdge>: invalid 'from' index");
	assert((to < m_Nodes.size()) && (to >= 0) && m_Nodes[to].Index() != invalid_node_index && "<SparseGraph::GetEdge>: invalid 'to' index");

	for (EdgeList::iterator curEdge = m_Edges[from].begin(); curEdge != m_Edges[from].end(); ++curEdge)
	{
		if (curEdge->To() == to) return *curEdge;
	}

	assert(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::AddEdge(EdgeType edge)
{
	assert((edge.From() < m_iNextNodeIndex) && (edge.To() < m_iNextNodeIndex) && "<SparseGraph::AddEdge>: invalid node index");

	if ((m_Nodes[edge.To()].Index() != invalid_node_index) && (m_Nodes[edge.From()].Index() != invalid_node_index))
	{
		if (UniqueEdge(edge.From(), edge.To())) m_Edges[edge.From()].push_back(edge);

		if (!m_bDigraph)
		{
			if (UniqueEdge(edge.To(), edge.From()))
			{
				EdgeType NewEdge = edge;

				NewEdge.SetTo(edge.From());
				NewEdge.SetFrom(edge.To());

				m_Edges[edge.To()].push_back(NewEdge);
			}
		}
	}
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveEdge(int from, int to)
{
	assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) && "<SparseGraph::RemoveEdge>:invalid node index");

	EdgeList::iterator curEdge;

	if (!m_bDigraph)
	{
		for (curEdge = m_Edges[to].begin(); curEdge != m_Edges[to].end(); ++curEdge)
		{
			if (curEdge->To() == from) { curEdge = m_Edges[to].erase(curEdge); break; }
		}
	}

	for (curEdge = m_Edges[from].begin(); curEdge != m_Edges[from].end(); ++curEdge)
	{
		if (curEdge->To() == to) { curEdge = m_Edges[from].erase(curEdge); break; }
	}
}

template <class node_type, class edge_type>
int SparseGraph<node_type, edge_type>::AddNode(node_type node)
{
	if (node.Index() < (int)m_Nodes.size())
	{
		assert(m_Nodes[node.Index()].Index() == invalid_node_index && "<SparseGraph::AddNode>: Attempting to add a node with a duplicate ID");
		m_Nodes[node.Index()] = node;
		return m_iNextNodeIndex;
	}

	else
	{
		assert(node.Index() == m_iNextNodeIndex && "<SparseGraph::AddNode>:invalid index");

		m_Nodes.push_back(node);
		m_Edges.push_back(EdgeList());

		return m_iNextNodeIndex++;
	}
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::CullInvalidEdges()
{
	for (EdgeListVector::iterator curEdgeList = m_Edges.begin(); curEdgeList != m_Edges.end(); ++curEdgeList)
	{
		for (EdgeList::iterator curEdge = (*curEdgeList).begin(); curEdge != (*curEdgeList).end(); ++curEdge)
		{
			if (m_Nodes[curEdge->To()].Index() == invalid_node_index || m_Nodes[curEdge->From()].Index() == invalid_node_index)
			{
				curEdge = (*curEdgeList).erase(curEdge);
			}
		}
	}
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveNode(int node)
{
	assert(node < (int)m_Nodes.size() && "<SparseGraph::RemoveNode>: invalid node index");

	m_Nodes[node].SetIndex(invalid_node_index);

	if (!m_bDigraph)
	{
		for (EdgeList::iterator curEdge = m_Edges[node].begin(); curEdge != m_Edges[node].end(); ++curEdge)
		{
			for (EdgeList::iterator curE = m_Edges[curEdge->To()].begin(); curE != m_Edges[curEdge->To()].end(); ++curE)
			{
				if (curE->To() == node)
				{
					curE = m_Edges[curEdge->To()].erase(curE);
					break;
				}
			}
		}

		m_Edges[node].clear();
	}

	else CullInvalidEdges();
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::SetEdgeCost(int from, int to, double NewCost)
{
	assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) && "<SparseGraph::SetEdgeCost>: invalid index");

	for (EdgeList::iterator curEdge = m_Edges[from].begin(); curEdge != m_Edges[from].end(); ++curEdge)
	{
		if (curEdge->To() == to)
		{
			curEdge->SetCost(NewCost);
			break;
		}
	}
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::UniqueEdge(int from, int to) const
{
	for (EdgeList::const_iterator curEdge = m_Edges[from].begin(); curEdge != m_Edges[from].end(); ++curEdge)
	{
		if (curEdge->To() == to) return false;
	}
	return true;
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Save(const char* FileName) const
{
	std::ofstream out(FileName);

	if (!out)
	{
		throw std::runtime_error("Cannot open file: " + std::string(FileName));
		return false;
	}

	return Save(out);
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Save(std::ofstream& stream)const
{
	stream << m_Nodes.size() << std::endl;

	NodeVector::const_iterator curNode = m_Nodes.begin();
	for (curNode; curNode != m_Nodes.end(); ++curNode)
	{
		stream << *curNode;
	}

	stream << NumEdges() << std::endl;

	for (unsigned int nodeIdx = 0; nodeIdx < m_Nodes.size(); ++nodeIdx)
	{
		for (EdgeList::const_iterator curEdge = m_Edges[nodeIdx].begin(); curEdge != m_Edges[nodeIdx].end(); ++curEdge)
		{
			stream << *curEdge;
		}
	}

	return true;
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Load(const char* FileName)
{
	std::ifstream in(FileName);

	if (!in)
	{
		throw std::runtime_error("Cannot open file: " + std::string(FileName));
		return false;
	}

	return Load(in);
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Load(std::ifstream& stream)
{
	Clear();

	int NumNodes, NumEdges;

	stream >> NumNodes;

	for (int n = 0; n < NumNodes; ++n)
	{
		NodeType NewNode(stream);

		if (NewNode.Index() != invalid_node_index) AddNode(NewNode);
		else
		{
			m_Nodes.push_back(NewNode);
			m_Edges.push_back(EdgeList());
			++m_iNextNodeIndex;
		}
	}

	stream >> NumEdges;
	for (int e = 0; e < NumEdges; ++e)
	{
		EdgeType NextEdge(stream);

		AddEdge(NextEdge);
	}

	return true;
}

#endif