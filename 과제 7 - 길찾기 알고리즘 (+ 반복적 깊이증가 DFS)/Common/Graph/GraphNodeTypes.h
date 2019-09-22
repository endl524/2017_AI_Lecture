#ifndef GRAPH_NODE_TYPES_H
#define GRAPH_NODE_TYPES_H

#include <list>
#include <ostream>
#include <fstream>
#include "../2D/Vector2D.h"
#include "../Graph/NodeTypeEnumerations.h"

class GraphNode
{
protected:
	int m_iIndex; // ³ëµå ÀÎµ¦½º

public:
	GraphNode() : m_iIndex(invalid_node_index) {}
	GraphNode(int idx) : m_iIndex(idx) {}
	GraphNode(std::ifstream& stream) { char buffer[50]; stream >> buffer >> m_iIndex; }
	virtual ~GraphNode() {}

	int Index() const { return m_iIndex; }
	void SetIndex(int NewIndex) { m_iIndex = NewIndex; }

	friend std::ostream& operator<<(std::ostream& os, const GraphNode& n)
	{
		os << "Index: " << n.m_iIndex << std::endl; return os;
	}
};

template <class extra_info = void*>
class NavGraphNode : public GraphNode
{
protected:
	Vector2D m_vPosition; // ³ëµå ÁÂÇ¥
	extra_info  m_ExtraInfo;

public:
	NavGraphNode() : m_ExtraInfo(extra_info()) {}

	NavGraphNode(int idx, Vector2D pos) : GraphNode(idx), m_vPosition(pos), m_ExtraInfo(extra_info()) {}

	NavGraphNode(std::ifstream& stream) :m_ExtraInfo(extra_info())
	{
		char buffer[50];
		stream >> buffer >> m_iIndex >> buffer >> m_vPosition.x >> buffer >> m_vPosition.y;
	}
	virtual ~NavGraphNode() {}

	Vector2D Pos() const { return m_vPosition; }
	void SetPos(Vector2D NewPosition) { m_vPosition = NewPosition; }

	extra_info ExtraInfo() const { return m_ExtraInfo; }
	void SetExtraInfo(extra_info info) { m_ExtraInfo = info; }

	friend std::ostream& operator<<(std::ostream& os, const NavGraphNode& n)
	{
		os << "Index: " << n.m_iIndex << " PosX: " << n.m_vPosition.x << " PosY: " << n.m_vPosition.y << std::endl;
		return os;
	}

};

#endif
