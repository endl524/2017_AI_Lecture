#pragma once

#include <vector>
#include <algorithm>
#include <random>
using namespace std;

#define HEURISTICS_WEIGHT 10

struct Case // ** 모든 Case는 Unique하다. **
{
	int m_F = 0; // 총 비용 (f = g + h)
	int m_G = 0; // (비용) == 배치 해야 할 '남은 퀸의 수'.
	int m_New_Queen_Row_idx = 0; // 다음 퀸을 배치 해야 할 행의 '색인' 번호.
	Case* m_Parent_Case = nullptr; // A*의 경로 추적을 위한 부모 객체.
	vector<int> m_Queen_List; // 현재 객체가 나타내는 퀸 배치 상태.

	Case(const int& f, const int& g, const int& row, Case* parent, const vector<int>& queen_list)
		: m_F(f), m_G(g), m_New_Queen_Row_idx(row), m_Parent_Case(parent), m_Queen_List(queen_list) {};
};

bool Compare(const Case* a, const Case* b) { return a->m_F < b->m_F; }

class Astar
{
private:
	vector<Case*> m_PQueue_Open; // 열린 목록.
	vector<Case*> m_PQueue_Close; // 닫힌 목록.
	vector<Case*> m_Entire_Case; // 모든 Case 객체를 기록.
	vector<Case*> m_Dest_Cases; // 찾은 목표 상태의 Case 객체를 기록.
	int m_Board_Length; // 보드의 크기 N.

	random_device m_Random_Device;
	mt19937_64 m_Random_Seed_64;
	

public:
	Astar(const int& board_length) : m_Board_Length(board_length)
	{
		mt19937_64 random_seed_64(m_Random_Device());
		m_Random_Seed_64 = random_seed_64;
	};
	~Astar()
	{
		if (!m_Entire_Case.empty())
		{
			for (auto& cb : m_Entire_Case) { if (cb != nullptr) { delete cb; cb = nullptr; } }
			m_PQueue_Open.clear();
		}
	};

	int Get_Open_List_Size() const { return m_PQueue_Open.size(); }
	int Get_Close_List_Size() const { return m_PQueue_Close.size(); }
	int Get_Dest_List_Size() const { return m_Dest_Cases.size(); }

	bool Search(const bool& is_just_get_one_solution)
	{
		Case* p_New_Case = nullptr;
		vector<int> queen_list(m_Board_Length, -1);
		for (int i = 0; i < m_Board_Length; ++i) // N개의 '시작 상태'들을 구성.
		{
			queen_list[0] = i;
			//int heuristics = m_Board_Length * m_Board_Length - 3 * (m_Board_Length - 1) - 1;
			p_New_Case = new Case(m_Board_Length - 1, m_Board_Length - 1, 1, nullptr, queen_list);
			m_PQueue_Open.push_back(p_New_Case);
			m_Entire_Case.push_back(p_New_Case);
		}

		Case* curr_Case = nullptr;
		while (!m_PQueue_Open.empty())
		{
			sort(m_PQueue_Open.begin(), m_PQueue_Open.end(), Compare); // F에 대해 오름차순 정렬.
			curr_Case = m_PQueue_Open.front(); // F가 가장 작은 Case를 선택.
			m_PQueue_Open.erase(m_PQueue_Open.begin()); // Open list에서 해당 Case를 pop.
			m_PQueue_Close.push_back(curr_Case); // Close list에 해당 Case를 push.

			if (curr_Case->m_New_Queen_Row_idx == m_Board_Length) // 퀸 배치가 끝난 Case일 때,
			{
				if (curr_Case->m_F == 0) // 해답이면 저장한다.
				{
					m_Dest_Cases.push_back(curr_Case);
					if (is_just_get_one_solution) return true;
				}
				continue; // 다음 Case를 살펴본다.
			}

			queen_list = curr_Case->m_Queen_List; // 현재 Case의 퀸 배치도를 복사해온다.
			for (int col = 0; col < m_Board_Length; ++col) // 모든 열을 순회한다.
			{
				bool is_valid = true;
				
				// * 선택된 Case에서 진행할 다음 Case를 생성한다. *
				// 퀸들의 경로가 겹치는 부분이 있는지 확인 해야한다.
				queen_list[curr_Case->m_New_Queen_Row_idx] = col; // 새 퀸을 배치한다.

				for (int row = 0; row < curr_Case->m_New_Queen_Row_idx; ++row) // '첫 행' 부터 퀸을 '새로 배치한 행 이전' 까지 순회.
				{
					// 새로 배치한 행을 제외하고, 나머지 행의 퀸들이 새로 배치한 퀸과 겹치는지 검사.
					int diff_row = curr_Case->m_New_Queen_Row_idx - row;
					int diff_col = abs(queen_list[row] - col);
					if ((queen_list[row] == col || diff_row == diff_col)) { is_valid = false; break; } // 겹친다면 col에 배치하는 것이 유효하지 않음.
				}
				if (!is_valid) continue; // 유효하지 않다면 가지치기.
				/*
				int heuristics = 0; // heuristics = 유효하게 놓을 수 있는 남은 칸의 수.
				vector<bool> temp(m_Board_Length, true);
				vector<vector<bool>> check(m_Board_Length - curr_Case->m_New_Queen_Row_idx - 1, temp);
				for (int row = 0; row <= curr_Case->m_New_Queen_Row_idx; ++row) // 이번에는 '새로 배치한 행' 까지 순회.
				{
					// 현재 행의 퀸이 새로 배치한 퀸과 겹치지 않는다면, 보드의 남은 칸들 중 유효하지 않은 칸을 체크한다.
					int start_index = curr_Case->m_New_Queen_Row_idx + 1;
					for (int rem_row = start_index; rem_row < m_Board_Length; ++rem_row)
					{
						check[rem_row - start_index][queen_list[row]] = false;
						int cross_col_left = queen_list[row] - (rem_row - row);
						int cross_col_right = queen_list[row] + (rem_row - row);
						if (cross_col_left >= 0) check[rem_row - start_index][cross_col_left] = false;
						if (cross_col_right < m_Board_Length) check[rem_row - start_index][cross_col_right] = false;
					}
				}
				for (int i = 0; i < check.size(); ++i) // 휴리스틱을 계산한다. (유효한 칸의 개수를 센다.)
				{
					for (int j = 0; j < m_Board_Length; ++j) if (check[i][j]) ++heuristics;
				}
				if (heuristics < m_Board_Length - curr_Case->m_New_Queen_Row_idx - 1) continue; // '배치해야 할 퀸의 개수' 보다 '유효한 칸의 수'가 더 적다면 '무효'.
				*/
				p_New_Case = new Case(curr_Case->m_G - 1, curr_Case->m_G - 1, curr_Case->m_New_Queen_Row_idx + 1, curr_Case, queen_list);
				m_PQueue_Open.push_back(p_New_Case);
				m_Entire_Case.push_back(p_New_Case);
			}
		}

		if (!m_Dest_Cases.empty()) return true;
		return false;
	}

	void Draw_Dest_Case() // 찾은 '목표 상태' 중 무작위로 하나를 그려준다.
	{
		if (m_Dest_Cases.empty()) { cout << "찾은 해답이 없습니다.\n"; return; }

		uniform_int_distribution<int> random_index(0, m_Dest_Cases.size() - 1);
		int index = random_index(m_Random_Seed_64);
		Case* dest_case = m_Dest_Cases[index];

		// 여기서는 경로 전체가 아닌 '목표 상태'만 그려준다. (N번 그려주면 복잡하므로..)
		cout << "< 해답 " << index + 1 << "번 >\n\n";

		for (int i = 0; i < m_Board_Length; ++i)
		{
			for (int j = 0; j < m_Board_Length; ++j)
			{
				if (dest_case->m_Queen_List[i] == j) cout << "★";
				else cout << "□";
			}
			cout << endl;
		}
		cout << endl;
	}
};