// ===================================================================================================================
//												[N-QUEEN PUZZLE]
//												  with A*, DFS.
//													for x64.
// ===================================================================================================================

#include <iostream>
#include <vector>
#include "Astar.h"
#include "BackTracking.h"
#include <windows.h>
using namespace std;

#define BOARD_SIZE 8 // N ũ�� ����
#define GET_ONE_SOLUTION false // �ش��� 1���� ã�� ���ΰ�?

int main()
{
	float curr_Time, elapsed_Time, prev_Time;
	bool is_Success = false;
	cout << "[ " << BOARD_SIZE << "-QUEEN PUZZLE! ]\n";

#define ASTAR
#ifdef ASTAR // A* search ����
	cout << "=> with A*\n\n";

	Astar* p_Astar = new Astar(BOARD_SIZE);
	prev_Time = timeGetTime();
	if (p_Astar->Search(GET_ONE_SOLUTION))
	{
		curr_Time = timeGetTime();
		p_Astar->Draw_Dest_Case();

		elapsed_Time = (curr_Time - prev_Time) / 1000.0f;
		cout << "���� �ð�: " << elapsed_Time << " sec\n";
		cout << "Open List Size: " << p_Astar->Get_Open_List_Size() << "\n";
		cout << "Close List Size: " << p_Astar->Get_Close_List_Size() << "\n";
		cout << "ã�� �ش� ��: " << p_Astar->Get_Dest_List_Size() << "\n\n";
	}
	else cout << "�ش��� �����ϴ�.\n\n";
	
	delete p_Astar;

#else // BackTracking DFS search ����
	cout << "=> with BackTracking DFS\n\n";

	BackTracking* p_BackTracking = new BackTracking(BOARD_SIZE);
	prev_Time = timeGetTime();
	if (p_BackTracking->Search(GET_ONE_SOLUTION))
	{
		curr_Time = timeGetTime();
		p_BackTracking->Draw_Dest_Case();

		elapsed_Time = (curr_Time - prev_Time) / 1000.0f;
		cout << "���� �ð�: " << elapsed_Time << " sec\n";
		cout << "ó�� Ƚ��: " << p_BackTracking->Get_Process_Count() << "\n";
		cout << "ã�� �ش� ��: " << p_BackTracking->Get_Dest_List_Size() << "\n\n";
	}
	else cout << "�ش��� �����ϴ�.\n\n";

	delete p_BackTracking;

#endif
}