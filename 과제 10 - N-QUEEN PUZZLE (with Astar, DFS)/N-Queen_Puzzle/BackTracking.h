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
	vector<vector<int>> m_Dest_Cases; // ã�� ��ǥ ������ Case ��ü�� ���.
	int m_Board_Length; // ������ ũ�� N.
	long long m_Process_Count = 0; // DFS Ž�� Ƚ��.

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

	// ��� �ش��� ã�� ���� BackTracking.
	void BackTracking_Process(const int& row, const int& column, const int& N, vector<bool>& column_check, vector<int>& queen_info)
	{
		++m_Process_Count;
		queen_info[row] = column;
		if (row + 1 >= N) { m_Dest_Cases.push_back(queen_info); return; } // ������ ���� �Ϳ� �����ߴ°�?
		column_check[column] = true;
		
		bool cross_check;
		for (int i = 0; i < N; ++i)
		{
			if (column_check[i]) continue; // ���� �ٸ� ����� ���� �ߺ��Ǵ��� �˻�.
			cross_check = false;
			for (int r = 0; r <= row; ++r) // ���� �ٸ� ������ �밢���� ���ԵǴ��� �˻�.
			{
				if (abs(queen_info[r] - i) == ((row + 1) - r)) // (�ɸ��°� �ϳ��� �־ �ȵȴ�.)
				{
					cross_check = true; break;
				}
			}
			if (cross_check) continue;
			BackTracking_Process(row + 1, i, N, column_check, queen_info); // �ƴ϶�� ��� ����
		}

		queen_info[row] = -1;
		column_check[column] = false;
	}

	// �ش��� 1���� ������ ã�� ���� BackTracking.
	void BackTracking_Get_One_Process(const int& row, const int& column, const int& N, vector<bool>& column_check, vector<int>& queen_info, bool& is_found)
	{
		++m_Process_Count;
		queen_info[row] = column;
		if (row + 1 >= N) { m_Dest_Cases.push_back(queen_info); is_found = true; return; } // ������ ���� �Ϳ� �����ߴ°�?
		column_check[column] = true;

		bool cross_check;
		for (int i = 0; i < N; ++i)
		{
			if (is_found) return;
			if (column_check[i]) continue; // ���� �ٸ� ����� ���� �ߺ��Ǵ��� �˻�.
			cross_check = false;
			for (int r = 0; r <= row; ++r) // ���� �ٸ� ������ �밢���� ���ԵǴ��� �˻�.
			{
				if (abs(queen_info[r] - i) == ((row + 1) - r)) // (�ɸ��°� �ϳ��� �־ �ȵȴ�.)
				{
					cross_check = true; break;
				}
			}
			if (cross_check) continue;
			BackTracking_Get_One_Process(row + 1, i, N, column_check, queen_info, is_found); // �ƴ϶�� ��� ����
		}

		queen_info[row] = -1;
		column_check[column] = false;
	}

	bool Search(const bool& is_just_get_one_solution)
	{
		vector<bool> column_check(m_Board_Length, false);
		vector<int> queen_info(m_Board_Length, -1); // �� ���� ���� ���� �� ��ȣ�� ���.
		for (int i = 0; i < m_Board_Length; ++i)
		{
			if (is_just_get_one_solution) { bool temp = false; BackTracking_Get_One_Process(0, i, m_Board_Length, column_check, queen_info, temp); }
			else BackTracking_Process(0, i, m_Board_Length, column_check, queen_info);
		}
		return !m_Dest_Cases.empty();
	}

	void Draw_Dest_Case() // ã�� '��ǥ ����' �� �������� �ϳ��� �׷��ش�.
	{
		if (m_Dest_Cases.empty()) { cout << "ã�� �ش��� �����ϴ�.\n"; return; }

		uniform_int_distribution<int> random_index(0, m_Dest_Cases.size() - 1);
		int index = random_index(m_Random_Seed_64);
		vector<int> dest_case = m_Dest_Cases[index];

		cout << "< �ش� " << index + 1 << "�� >\n\n";

		for (int i = 0; i < m_Board_Length; ++i)
		{
			for (int j = 0; j < m_Board_Length; ++j)
			{
				if (dest_case[i] == j) cout << "��";
				else cout << "��";
			}
			cout << endl;
		}
		cout << endl;
	}
};

