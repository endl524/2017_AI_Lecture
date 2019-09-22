#include "WindowUtils.h"
#include <windows.h>
#include "../2D/Vector2D.h"
#include "utils.h"
#include "Stream_Utility_Functions.h"

void ChangeMenuState(HWND hwnd, UINT MenuItem, UINT state)
{
	MENUITEMINFO mi;

	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STATE;
	mi.fState = state;

	SetMenuItemInfo(GetMenu(hwnd), MenuItem, false, &mi);
	DrawMenuBar(hwnd);
}

void CheckMenuItemAppropriately(HWND hwnd, UINT MenuItem, bool b)
{
	if (b) ChangeMenuState(hwnd, MenuItem, MFS_CHECKED);
	else ChangeMenuState(hwnd, MenuItem, MFS_UNCHECKED);
}

bool CheckBufferLength(char* buff, int MaxLength, int& BufferLength)
{
	std::string s = ttos(buff);

	BufferLength = s.length();

	if (BufferLength > MaxLength)
	{
		BufferLength = 0; return false;
	}

	return true;
}

void ErrorBox(std::string& msg)
{
	MessageBox(NULL, msg.c_str(), "Error", MB_OK);
}

void ErrorBox(char* msg)
{
	MessageBox(NULL, msg, "Error", MB_OK);
}

Vector2D GetClientCursorPosition()
{
	POINT MousePos;

	GetCursorPos(&MousePos);

	ScreenToClient(GetActiveWindow(), &MousePos);

	return POINTtoVector(MousePos);
}

 
Vector2D GetClientCursorPosition(HWND hwnd)
{
	POINT MousePos;

	GetCursorPos(&MousePos);

	ScreenToClient(hwnd, &MousePos);

	return POINTtoVector(MousePos);
}

void FileInitialize(HWND hwnd, OPENFILENAME& ofn, const std::string& defaultFileTypeDescription, const std::string& defaultFileExtension)
{
	std::string filter = defaultFileTypeDescription + '\0' + "*." + defaultFileExtension + '\0' +
		"All Files (*.*)" + '\0' + "*.*" + '\0' + '\0';

	static TCHAR szFilter[255];

	for (unsigned int i = 0; i < filter.size(); ++i) szFilter[i] = filter.at(i);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = NULL;          // Set in Open and Close functions
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;          // Set in Open and Close functions
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0;             // Set in Open and Close functions
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = defaultFileExtension.c_str();
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
}

BOOL FileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName, const std::string& defaultFileTypeDescription, const std::string& defaultFileExtension)
{
	OPENFILENAME ofn;

	FileInitialize(hwnd, ofn, defaultFileTypeDescription, defaultFileExtension);

	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	return GetOpenFileName(&ofn);
}

BOOL FileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName, const std::string& defaultFileTypeDescription, const std::string& defaultFileExtension)
{
	OPENFILENAME ofn; FileInitialize(hwnd, ofn, defaultFileTypeDescription, defaultFileExtension);

	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	return GetSaveFileName(&ofn);
}

void ResizeWindow(HWND hwnd, int cx, int cy)
{
	HMENU hwndMenu = GetMenu(hwnd);
	bool bMenu = false;
	if (hwndMenu) bMenu = true;

	RECT DesiredSize;
	DesiredSize.left = 0;
	DesiredSize.top = 0;
	DesiredSize.right = cx;
	DesiredSize.bottom = cy;

	AdjustWindowRectEx(&DesiredSize, WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU, bMenu, NULL);

	SetWindowPos(hwnd, NULL, GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2,
		DesiredSize.right, DesiredSize.bottom, SWP_NOZORDER);
}

int GetWindowHeight(HWND hwnd)
{
	if (hwnd == 0) return 0;

	RECT windowRect;

	GetWindowRect(hwnd, &windowRect);

	return windowRect.bottom - windowRect.top;
}

int GetWindowWidth(HWND hwnd)
{
	if (hwnd == 0) return 0;

	RECT windowRect;

	GetWindowRect(hwnd, &windowRect);

	return windowRect.right - windowRect.left;
}