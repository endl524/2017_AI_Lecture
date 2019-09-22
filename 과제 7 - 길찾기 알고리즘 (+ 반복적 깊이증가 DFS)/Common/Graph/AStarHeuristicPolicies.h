#ifndef ASTAR_HEURISTIC_POLICIES_H
#define ASTAR_HEURISTIC_POLICIES_H

#include "../misc/utils.h"

class Heuristic_Euclid // 기본적인 휴리스틱 계산
{
public:
	Heuristic_Euclid() {}

	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return Vec2DDistance(G.GetNode(nd1).Pos(), G.GetNode(nd2).Pos());
	} // H = 노드 1에서 노드 2로 가는 직선 거리.
};

class Heuristic_Noisy_Euclidian // 휴리스틱에 랜덤한 노이즈를 걸어준다.
{
public:
	Heuristic_Noisy_Euclidian() {}

	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return Vec2DDistance(G.GetNode(nd1).Pos(), G.GetNode(nd2).Pos()) * RandInRange(0.9f, 1.1f);
	} // H = 노드 1에서 노드 2로 가는 직선 거리 * (0.9 ~ 1.1)의 랜덤 값.
};

class Heuristic_Dijkstra // A* 를 다익스트라 알고리즘으로 전환하는데 사용.
{
public:
	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return 0;
	} // H = 0. (다익스트라 알고리즘에서는 휴리스틱을 사용하지 않는다.)
};

#endif