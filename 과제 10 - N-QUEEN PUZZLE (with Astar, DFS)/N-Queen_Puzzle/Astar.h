#pragma once

#include <vector>
#include <algorithm>
#include <random>
using namespace std;

#define HEURISTICS_WEIGHT 10

struct Case // ** ��� Case�� Unique�ϴ�. **
{
	int m_F = 0; // �� ��� (f = g + h)
	int m_G = 0; // (���) == ��ġ �ؾ� �� '���� ���� ��'.
	int m_New_Queen_Row_idx = 0; // ���� ���� ��ġ �ؾ� �� ���� '����' ��ȣ.
	Case* m_Parent_Case = nullptr; // A*�� ��� ������ ���� �θ� ��ü.
	vector<int> m_Queen_List; // ���� ��ü�� ��Ÿ���� �� ��ġ ����.

	Case(const int& f, const int& g, const int& row, Case* parent, const vector<int>& queen_list)
		: m_F(f), m_G(g), m_New_Queen_Row_idx(row), m_Parent_Case(parent), m_Queen_List(queen_list) {};
};

bool Compare(const Case* a, const Case* b) { return a->m_F < b->m_F; }

class Astar
{
private:
	vector<Case*> m_PQueue_Open; // ���� ���.
	vector<Case*> m_PQueue_Close; // ���� ���.
	vector<Case*> m_Entire_Case; // ��� Case ��ü�� ���.
	vector<Case*> m_Dest_Cases; // ã�� ��ǥ ������ Case ��ü�� ���.
	int m_Board_Length; // ������ ũ�� N.

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
		for (int i = 0; i < m_Board_Length; ++i) // N���� '���� ����'���� ����.
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
			sort(m_PQueue_Open.begin(), m_PQueue_Open.end(), Compare); // F�� ���� �������� ����.
			curr_Case = m_PQueue_Open.front(); // F�� ���� ���� Case�� ����.
			m_PQueue_Open.erase(m_PQueue_Open.begin()); // Open list���� �ش� Case�� pop.
			m_PQueue_Close.push_back(curr_Case); // Close list�� �ش� Case�� push.

			if (curr_Case->m_New_Queen_Row_idx == m_Board_Length) // �� ��ġ�� ���� Case�� ��,
			{
				if (curr_Case->m_F == 0) // �ش��̸� �����Ѵ�.
				{
					m_Dest_Cases.push_back(curr_Case);
					if (is_just_get_one_solution) return true;
				}
				continue; // ���� Case�� ���캻��.
			}

			queen_list = curr_Case->m_Queen_List; // ���� Case�� �� ��ġ���� �����ؿ´�.
			for (int col = 0; col < m_Board_Length; ++col) // ��� ���� ��ȸ�Ѵ�.
			{
				bool is_valid = true;
				
				// * ���õ� Case���� ������ ���� Case�� �����Ѵ�. *
				// ������ ��ΰ� ��ġ�� �κ��� �ִ��� Ȯ�� �ؾ��Ѵ�.
				queen_list[curr_Case->m_New_Queen_Row_idx] = col; // �� ���� ��ġ�Ѵ�.

				for (int row = 0; row < curr_Case->m_New_Queen_Row_idx; ++row) // 'ù ��' ���� ���� '���� ��ġ�� �� ����' ���� ��ȸ.
				{
					// ���� ��ġ�� ���� �����ϰ�, ������ ���� ������ ���� ��ġ�� ���� ��ġ���� �˻�.
					int diff_row = curr_Case->m_New_Queen_Row_idx - row;
					int diff_col = abs(queen_list[row] - col);
					if ((queen_list[row] == col || diff_row == diff_col)) { is_valid = false; break; } // ��ģ�ٸ� col�� ��ġ�ϴ� ���� ��ȿ���� ����.
				}
				if (!is_valid) continue; // ��ȿ���� �ʴٸ� ����ġ��.
				/*
				int heuristics = 0; // heuristics = ��ȿ�ϰ� ���� �� �ִ� ���� ĭ�� ��.
				vector<bool> temp(m_Board_Length, true);
				vector<vector<bool>> check(m_Board_Length - curr_Case->m_New_Queen_Row_idx - 1, temp);
				for (int row = 0; row <= curr_Case->m_New_Queen_Row_idx; ++row) // �̹����� '���� ��ġ�� ��' ���� ��ȸ.
				{
					// ���� ���� ���� ���� ��ġ�� ���� ��ġ�� �ʴ´ٸ�, ������ ���� ĭ�� �� ��ȿ���� ���� ĭ�� üũ�Ѵ�.
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
				for (int i = 0; i < check.size(); ++i) // �޸���ƽ�� ����Ѵ�. (��ȿ�� ĭ�� ������ ����.)
				{
					for (int j = 0; j < m_Board_Length; ++j) if (check[i][j]) ++heuristics;
				}
				if (heuristics < m_Board_Length - curr_Case->m_New_Queen_Row_idx - 1) continue; // '��ġ�ؾ� �� ���� ����' ���� '��ȿ�� ĭ�� ��'�� �� ���ٸ� '��ȿ'.
				*/
				p_New_Case = new Case(curr_Case->m_G - 1, curr_Case->m_G - 1, curr_Case->m_New_Queen_Row_idx + 1, curr_Case, queen_list);
				m_PQueue_Open.push_back(p_New_Case);
				m_Entire_Case.push_back(p_New_Case);
			}
		}

		if (!m_Dest_Cases.empty()) return true;
		return false;
	}

	void Draw_Dest_Case() // ã�� '��ǥ ����' �� �������� �ϳ��� �׷��ش�.
	{
		if (m_Dest_Cases.empty()) { cout << "ã�� �ش��� �����ϴ�.\n"; return; }

		uniform_int_distribution<int> random_index(0, m_Dest_Cases.size() - 1);
		int index = random_index(m_Random_Seed_64);
		Case* dest_case = m_Dest_Cases[index];

		// ���⼭�� ��� ��ü�� �ƴ� '��ǥ ����'�� �׷��ش�. (N�� �׷��ָ� �����ϹǷ�..)
		cout << "< �ش� " << index + 1 << "�� >\n\n";

		for (int i = 0; i < m_Board_Length; ++i)
		{
			for (int j = 0; j < m_Board_Length; ++j)
			{
				if (dest_case->m_Queen_List[i] == j) cout << "��";
				else cout << "��";
			}
			cout << endl;
		}
		cout << endl;
	}
};