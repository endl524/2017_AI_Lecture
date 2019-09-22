#ifndef ASTAR_HEURISTIC_POLICIES_H
#define ASTAR_HEURISTIC_POLICIES_H

#include "../misc/utils.h"

class Heuristic_Euclid // �⺻���� �޸���ƽ ���
{
public:
	Heuristic_Euclid() {}

	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return Vec2DDistance(G.GetNode(nd1).Pos(), G.GetNode(nd2).Pos());
	} // H = ��� 1���� ��� 2�� ���� ���� �Ÿ�.
};

class Heuristic_Noisy_Euclidian // �޸���ƽ�� ������ ����� �ɾ��ش�.
{
public:
	Heuristic_Noisy_Euclidian() {}

	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return Vec2DDistance(G.GetNode(nd1).Pos(), G.GetNode(nd2).Pos()) * RandInRange(0.9f, 1.1f);
	} // H = ��� 1���� ��� 2�� ���� ���� �Ÿ� * (0.9 ~ 1.1)�� ���� ��.
};

class Heuristic_Dijkstra // A* �� ���ͽ�Ʈ�� �˰������� ��ȯ�ϴµ� ���.
{
public:
	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return 0;
	} // H = 0. (���ͽ�Ʈ�� �˰��򿡼��� �޸���ƽ�� ������� �ʴ´�.)
};

#endif