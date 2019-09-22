#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <random>
using namespace std;

class BackTracking
{
private:
	vector<vector<int>> m_Dest_Cases; // 찾은 목표 상태의 Case 객체를 기록.
	int m_Board_Length; // 보드의 크기 N.
	long long m_Process_Count = 0; // DFS 탐색 횟수.

	random_device m_Random_Device;
	mt19937_64 m_Random_Seed_64;

public:
	BackTracking(const int& board_length) : m_Board_Length(board_length)
	{
		mt19937_64 random_seed_64(m_Random_Device());
		m_Random_Seed_64 = random_seed_64;
	}
	~BackTracking(){}

	long long Get_Process_Count() const { return m_Process_Count; }
	int Get_Dest_List_Size() const { return m_Dest_Cases.size(); }

	// 모든 해답을 찾기 위한 BackTracking.
	void BackTracking_Process(const int& row, const int& column, const int& N, vector<bool>& column_check, vector<int>& queen_info)
	{
		++m_Process_Count;
		queen_info[row] = column;
		if (row + 1 >= N) { m_Dest_Cases.push_back(queen_info); return; } // 끝까지 놓는 것에 성공했는가?
		column_check[column] = true;
		
		bool cross_check;
		for (int i = 0; i < N; ++i)
		{
			if (column_check[i]) continue; // 앞의 다른 퀸들과 열이 중복되는지 검사.
			cross_check = false;
			for (int r = 0; r <= row; ++r) // 앞의 다른 퀸들의 대각선에 포함되는지 검사.
			{
				if (abs(queen_info[r] - i) == ((row + 1) - r)) // (걸리는게 하나라도 있어선 안된다.)
				{
					cross_check = true; break;
				}
			}
			if (cross_check) continue;
			BackTracking_Process(row + 1, i, N, column_check, queen_info); // 아니라면 계속 수행
		}

		queen_info[row] = -1;
		column_check[column] = false;
	}

	// 해답을 1개만 빠르게 찾기 위한 BackTracking.
	void BackTracking_Get_One_Process(const int& row, const int& column, const int& N, vector<bool>& column_check, vector<int>& queen_info, bool& is_found)
	{
		++m_Process_Count;
		queen_info[row] = column;
		if (row + 1 >= N) { m_Dest_Cases.push_back(queen_info); is_found = true; return; } // 끝까지 놓는 것에 성공했는가?
		column_check[column] = true;

		bool cross_check;
		for (int i = 0; i < N; ++i)
		{
			if (is_found) return;
			if (column_check[i]) continue; // 앞의 다른 퀸들과 열이 중복되는지 검사.
			cross_check = false;
			for (int r = 0; r <= row; ++r) // 앞의 다른 퀸들의 대각선에 포함되는지 검사.
			{
				if (abs(queen_info[r] - i) == ((row + 1) - r)) // (걸리는게 하나라도 있어선 안된다.)
				{
					cross_check = true; break;
				}
			}
			if (cross_check) continue;
			BackTracking_Get_One_Process(row + 1, i, N, column_check, queen_info, is_found); // 아니라면 계속 수행
		}

		queen_info[row] = -1;
		column_check[column] = false;
	}

	bool Search(const bool& is_just_get_one_solution)
	{
		vector<bool> column_check(m_Board_Length, false);
		vector<int> queen_info(m_Board_Length, -1); // 각 행의 퀸이 놓인 열 번호를 기록.
		for (int i = 0; i < m_Board_Length; ++i)
		{
			if (is_just_get_one_solution) { bool temp = false; BackTracking_Get_One_Process(0, i, m_Board_Length, column_check, queen_info, temp); }
			else BackTracking_Process(0, i, m_Board_Length, column_check, queen_info);
		}
		return !m_Dest_Cases.empty();
	}

	void Draw_Dest_Case() // 찾은 '목표 상태' 중 무작위로 하나를 그려준다.
	{
		if (m_Dest_Cases.empty()) { cout << "찾은 해답이 없습니다.\n"; return; }

		uniform_int_distribution<int> random_index(0, m_Dest_Cases.size() - 1);
		int index = random_index(m_Random_Seed_64);
		vector<int> dest_case = m_Dest_Cases[index];

		cout << "< 해답 " << index + 1 << "번 >\n\n";

		for (int i = 0; i < m_Board_Length; ++i)
		{
			for (int j = 0; j < m_Board_Length; ++j)
			{
				if (dest_case[i] == j) cout << "★";
				else cout << "□";
			}
			cout << endl;
		}
		cout << endl;
	}
};

