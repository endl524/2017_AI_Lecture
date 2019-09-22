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

#define BOARD_SIZE 8 // N 크기 정의
#define GET_ONE_SOLUTION false // 해답을 1개만 찾을 것인가?

int main()
{
	float curr_Time, elapsed_Time, prev_Time;
	bool is_Success = false;
	cout << "[ " << BOARD_SIZE << "-QUEEN PUZZLE! ]\n";

#define ASTAR
#ifdef ASTAR // A* search 수행
	cout << "=> with A*\n\n";

	Astar* p_Astar = new Astar(BOARD_SIZE);
	prev_Time = timeGetTime();
	if (p_Astar->Search(GET_ONE_SOLUTION))
	{
		curr_Time = timeGetTime();
		p_Astar->Draw_Dest_Case();

		elapsed_Time = (curr_Time - prev_Time) / 1000.0f;
		cout << "수행 시간: " << elapsed_Time << " sec\n";
		cout << "Open List Size: " << p_Astar->Get_Open_List_Size() << "\n";
		cout << "Close List Size: " << p_Astar->Get_Close_List_Size() << "\n";
		cout << "찾은 해답 수: " << p_Astar->Get_Dest_List_Size() << "\n\n";
	}
	else cout << "해답이 없습니다.\n\n";
	
	delete p_Astar;

#else // BackTracking DFS search 수행
	cout << "=> with BackTracking DFS\n\n";

	BackTracking* p_BackTracking = new BackTracking(BOARD_SIZE);
	prev_Time = timeGetTime();
	if (p_BackTracking->Search(GET_ONE_SOLUTION))
	{
		curr_Time = timeGetTime();
		p_BackTracking->Draw_Dest_Case();

		elapsed_Time = (curr_Time - prev_Time) / 1000.0f;
		cout << "수행 시간: " << elapsed_Time << " sec\n";
		cout << "처리 횟수: " << p_BackTracking->Get_Process_Count() << "\n";
		cout << "찾은 해답 수: " << p_BackTracking->Get_Dest_List_Size() << "\n\n";
	}
	else cout << "해답이 없습니다.\n\n";

	delete p_BackTracking;

#endif
}