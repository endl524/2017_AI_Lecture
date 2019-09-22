#pragma warning (disable:4786)

#include <windows.h>
#include <time.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include "constants.h"
#include "../Common/misc/utils.h"
#include "../Common/Time/PrecisionTimer.h"
#include "Pathfinder.h"
#include "resource.h"
#include "../Common/misc/Cgdi.h"
#include "../Common/misc/WindowUtils.h"

#define UM_TOOLBAR_HAS_BEEN_CREATED (WM_USER + 33)

const char* g_szApplicationName = "PathFinder";
const char*	g_szWindowClassName = "MyWindowClass";

Pathfinder* g_Pathfinder;
HWND g_hwndToolbar;

void RedrawDisplay(HWND hwnd)
{
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void ResizeToCorrectClientArea(HWND hwnd, int AccumulativeToolbarHeight, RECT ClientArea)
{
	AdjustWindowRectEx(&ClientArea, WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU, true, NULL);
	SetWindowPos(hwnd, NULL, 0, 0, ClientArea.right, ClientArea.bottom + AccumulativeToolbarHeight, SWP_NOMOVE | SWP_NOZORDER);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;

	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap = NULL;
	static HBITMAP	hOldBitmap = NULL;

	static int ToolBarHeight;

	static TCHAR szFileName[MAX_PATH], szTitleName[MAX_PATH];

	static RECT rectClientWindow;

	static int CurrentSearchButton = 0;

	switch (msg)
	{
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));
		hdcBackBuffer = CreateCompatibleDC(NULL);

		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		ReleaseDC(hwnd, hdc);

		g_Pathfinder = new Pathfinder();

		CheckMenuItemAppropriately(hwnd, IDM_VIEW_TILES, g_Pathfinder->isShowTilesOn());
		CheckMenuItemAppropriately(hwnd, IDM_VIEW_GRAPH, g_Pathfinder->isShowGraphOn());
	}
	break;

	case UM_TOOLBAR_HAS_BEEN_CREATED:
	{
		RECT rectToolbar;
		GetWindowRect(g_hwndToolbar, &rectToolbar);
		ToolBarHeight = abs(rectToolbar.bottom - rectToolbar.top);

		rectClientWindow.left = 0;
		rectClientWindow.right = ClientWidth;
		rectClientWindow.top = 0;
		rectClientWindow.bottom = ClientHeight + InfoWindowHeight;

		ResizeToCorrectClientArea(hwnd, ToolBarHeight, rectClientWindow);

		SendMessage(g_hwndToolbar, WM_SIZE, wParam, lParam);

		GetClientRect(hwnd, &rectClientWindow);
		rectClientWindow.bottom = ClientHeight - ToolBarHeight - 1;

		hdcBackBuffer = CreateCompatibleDC(NULL);

		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc, rectClientWindow.right, rectClientWindow.bottom);
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		ReleaseDC(hwnd, hdc);
	}
	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_ESCAPE: SendMessage(hwnd, WM_DESTROY, NULL, NULL); break;
		case 'G': g_Pathfinder->ToggleShowGraph(); break;
		case 'T': g_Pathfinder->ToggleShowTiles(); break;
		}

		RedrawWindowRect(hwnd, false, rectClientWindow);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		g_Pathfinder->PaintTerrain(MAKEPOINTS(lParam));
		RedrawWindowRect(hwnd, false, rectClientWindow);
	}
	break;

	case WM_MOUSEMOVE:
	{
		switch (wParam)
		{
		case MK_LBUTTON:
		{
			g_Pathfinder->PaintTerrain(MAKEPOINTS(lParam));
			RedrawWindowRect(hwnd, false, rectClientWindow);
		}
		break;
		}
	}
	break;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_BUTTON_STOP: g_Pathfinder->ChangeBrush(Pathfinder::target); break;

		case ID_BUTTON_START: g_Pathfinder->ChangeBrush(Pathfinder::source); break;

		case ID_BUTTON_OBSTACLE: g_Pathfinder->ChangeBrush(Pathfinder::obstacle); break;

		case ID_BUTTON_WATER: g_Pathfinder->ChangeBrush(Pathfinder::water); break;

		case ID_BUTTON_MUD: g_Pathfinder->ChangeBrush(Pathfinder::mud); break;

		case ID_BUTTON_NORMAL: g_Pathfinder->ChangeBrush(Pathfinder::normal); break;

		case ID_BUTTON_DFS: g_Pathfinder->CreatePathDFS(); CurrentSearchButton = ID_BUTTON_DFS; break;

		case ID_BUTTON_ITERDEEP: g_Pathfinder->CreatePathIterDeep(); CurrentSearchButton = ID_BUTTON_ITERDEEP; break;

		case ID_BUTTON_BFS: g_Pathfinder->CreatePathBFS(); CurrentSearchButton = ID_BUTTON_BFS; break;

		case ID_BUTTON_DIJKSTRA: g_Pathfinder->CreatePathDijkstra(); CurrentSearchButton = ID_BUTTON_DIJKSTRA; break;

		case ID_BUTTON_ASTAR: g_Pathfinder->CreatePathAStar(); CurrentSearchButton = ID_BUTTON_ASTAR; break;		

		case ID_MENU_LOAD:
		{
			FileOpenDlg(hwnd, szFileName, szTitleName, "pathfinder files (*.map)", "map");
			if (strlen(szTitleName) > 0) g_Pathfinder->Load(szTitleName);
			SendMessage(g_hwndToolbar, TB_CHECKBUTTON, (WPARAM)CurrentSearchButton, (LPARAM)false);
		}
		break;

		case ID_MENU_SAVEAS:
		{
			FileSaveDlg(hwnd, szFileName, szTitleName, "pathfinder files (*.map)", "map");
			if (strlen(szTitleName) > 0) g_Pathfinder->Save(szTitleName);
		}
		break;

		case ID_MENU_NEW:
		{
			g_Pathfinder->CreateGraph(NumCellsX, NumCellsY);
			SendMessage(g_hwndToolbar, TB_CHECKBUTTON, (WPARAM)CurrentSearchButton, (LPARAM)false);
		}
		break;

		case IDM_VIEW_GRAPH:
		{
			if (GetMenuState(GetMenu(hwnd), IDM_VIEW_GRAPH, MFS_CHECKED) && MF_CHECKED)
			{
				ChangeMenuState(hwnd, IDM_VIEW_GRAPH, MFS_UNCHECKED);
				g_Pathfinder->SwitchGraphOff();
			}
			else
			{
				ChangeMenuState(hwnd, IDM_VIEW_GRAPH, MFS_CHECKED);
				g_Pathfinder->SwitchGraphOn();
			}
		}
		break;

		case IDM_VIEW_TILES:
		{
			if (GetMenuState(GetMenu(hwnd), IDM_VIEW_TILES, MFS_CHECKED) && MF_CHECKED)
			{
				ChangeMenuState(hwnd, IDM_VIEW_TILES, MFS_UNCHECKED);
				g_Pathfinder->SwitchTilesOff();
			}
			else
			{
				ChangeMenuState(hwnd, IDM_VIEW_TILES, MFS_CHECKED);
				g_Pathfinder->SwitchTilesOn();
			}
		}
		break;
		}

		RedrawWindowRect(hwnd, false, rectClientWindow);
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		BeginPaint(hwnd, &ps);

		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);

		gdi->StartDrawing(hdcBackBuffer);

		g_Pathfinder->Render();

		gdi->StopDrawing(hdcBackBuffer);

		BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_SIZE:
	{
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		SelectObject(hdcBackBuffer, hOldBitmap);

		DeleteObject(hBitmap);

		HDC hdc = GetDC(hwnd);

		hBitmap = CreateCompatibleBitmap(hdc, rectClientWindow.right, rectClientWindow.bottom);

		ReleaseDC(hwnd, hdc);

		SelectObject(hdcBackBuffer, hBitmap);
	}
	break;

	case WM_DESTROY:
	{
		SelectObject(hdcBackBuffer, hOldBitmap);

		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);
		DeleteObject(hOldBitmap);
 
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND CreateToolBar(HWND hwndParent, HINSTANCE hinstMain)
{
	const int NumButtons = 12;

	INITCOMMONCONTROLSEX cc;
	cc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	cc.dwICC = ICC_BAR_CLASSES;

	if (!InitCommonControlsEx(&cc))
	{
		MessageBox(NULL, "Failed to load common ctrls!", "Error!", MB_OK);
		return 0;
	}

	HWND hwndToolBar = CreateWindowEx(NULL, TOOLBARCLASSNAME, (LPSTR)NULL, WS_CHILD | WS_VISIBLE | CCS_BOTTOM,
		0, 0, 0, 0, hwndParent, (HMENU)IDR_TOOLBAR1, hinstMain, NULL);

	if (!hwndToolBar) MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);

	SendMessage(hwndToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	TBADDBITMAP tb;
	tb.hInst = NULL;
	tb.nID = (UINT_PTR)LoadBitmap((HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_TOOLBAR1));

	int idx = SendMessage(hwndToolBar, TB_ADDBITMAP, NumButtons, (LPARAM)&tb);

	TBBUTTON button[NumButtons];

	button[0].iBitmap = 0;
	button[0].idCommand = ID_BUTTON_STOP;
	button[0].fsState = TBSTATE_ENABLED;
	button[0].fsStyle = TBSTYLE_CHECKGROUP;
	button[0].dwData = NULL;
	button[0].iString = NULL;

	button[1].iBitmap = 1;
	button[1].idCommand = ID_BUTTON_START;
	button[1].fsState = TBSTATE_ENABLED;
	button[1].fsStyle = TBSTYLE_CHECKGROUP;
	button[1].dwData = NULL;
	button[1].iString = NULL;

	button[2].iBitmap = 2;
	button[2].idCommand = ID_BUTTON_OBSTACLE;
	button[2].fsState = TBSTATE_ENABLED;
	button[2].fsStyle = TBSTYLE_CHECKGROUP;
	button[2].dwData = NULL;
	button[2].iString = NULL;

	button[3].iBitmap = 3;
	button[3].idCommand = ID_BUTTON_MUD;
	button[3].fsState = TBSTATE_ENABLED;
	button[3].fsStyle = TBSTYLE_CHECKGROUP;
	button[3].dwData = NULL;
	button[3].iString = NULL;

	button[4].iBitmap = 4;
	button[4].idCommand = ID_BUTTON_WATER;
	button[4].fsState = TBSTATE_ENABLED;
	button[4].fsStyle = TBSTYLE_CHECKGROUP;
	button[4].dwData = NULL;
	button[4].iString = NULL;

	button[5].iBitmap = 5;
	button[5].idCommand = ID_BUTTON_NORMAL;
	button[5].fsState = TBSTATE_ENABLED;
	button[5].fsStyle = TBSTYLE_CHECKGROUP;
	button[5].dwData = NULL;
	button[5].iString = NULL;

	button[6].iBitmap = 238;
	button[6].idCommand = 0;
	button[6].fsState = NULL;
	button[6].fsStyle = TBSTYLE_SEP;
	button[6].dwData = NULL;
	button[6].iString = NULL;

	button[7].iBitmap = 6;
	button[7].idCommand = ID_BUTTON_DFS;
	button[7].fsState = TBSTATE_ENABLED;
	button[7].fsStyle = TBSTYLE_CHECKGROUP;
	button[7].dwData = NULL;
	button[7].iString = NULL;

	button[8].iBitmap = 7;
	button[8].idCommand = ID_BUTTON_ITERDEEP;
	button[8].fsState = TBSTATE_ENABLED;
	button[8].fsStyle = TBSTYLE_CHECKGROUP;
	button[8].dwData = NULL;
	button[8].iString = NULL;

	button[9].iBitmap = 8;
	button[9].idCommand = ID_BUTTON_BFS;
	button[9].fsState = TBSTATE_ENABLED;
	button[9].fsStyle = TBSTYLE_CHECKGROUP;
	button[9].dwData = NULL;
	button[9].iString = NULL;

	button[10].iBitmap = 9;
	button[10].idCommand = ID_BUTTON_DIJKSTRA;
	button[10].fsState = TBSTATE_ENABLED;
	button[10].fsStyle = TBSTYLE_CHECKGROUP;
	button[10].dwData = NULL;
	button[10].iString = NULL;

	button[11].iBitmap = 10;
	button[11].idCommand = ID_BUTTON_ASTAR;
	button[11].fsState = TBSTATE_ENABLED;
	button[11].fsStyle = TBSTYLE_CHECKGROUP;
	button[11].dwData = NULL;
	button[11].iString = NULL;

	SendMessage(hwndToolBar, TB_ADDBUTTONS, (WPARAM)NumButtons, (LPARAM)(LPTBBUTTON)&button);

	return hwndToolBar;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	HWND hWnd;
	WNDCLASSEX winclass;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = NULL;
	winclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName = g_szWindowClassName;
	winclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, "Registration Failed!", "Error", 0);
		return 0;
	}

	hWnd = CreateWindowEx(NULL, // extended style
		g_szWindowClassName, // window class name
		g_szApplicationName, // window caption
		WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU, // window style
		GetSystemMetrics(SM_CXSCREEN) / 2 - WindowWidth / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - WindowHeight / 2,
		WindowWidth, // initial x size
		WindowHeight, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		hInstance, // program instance handle
		NULL); // creation parameters

	if (!hWnd) MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);

	g_hwndToolbar = CreateToolBar(hWnd, hInstance);
	SendMessage(hWnd, UM_TOOLBAR_HAS_BEEN_CREATED, NULL, NULL);
	SendMessage(hWnd, WM_COMMAND, ID_MENU_NEW, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete g_Pathfinder;
	UnregisterClass(g_szWindowClassName, winclass.hInstance);
	return msg.wParam;
}