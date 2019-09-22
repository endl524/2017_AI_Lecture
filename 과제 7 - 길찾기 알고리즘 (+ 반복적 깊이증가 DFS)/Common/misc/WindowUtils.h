#ifndef WINDOW_UTILS_H
#define WINDOW_UTILS_H
#pragma warning (disable:4786)

#include <windows.h>
#include <string>

struct Vector2D;

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

#define WAS_KEY_PRESSED(vk_code) ((GetKeyState(vk_code) & 0x8000) != 0)
#define IS_KEY_PRESSED(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) != 0)

inline void RedrawWindow(HWND hwnd, bool RedrawBackGround = true)
{
	InvalidateRect(hwnd, NULL, RedrawBackGround);
	UpdateWindow(hwnd);
}

inline void RedrawWindowRect(HWND hwnd, bool RedrawBackGround, RECT& RedrawArea)
{
	InvalidateRect(hwnd, &RedrawArea, RedrawBackGround);
	UpdateWindow(hwnd);
}

void ChangeMenuState(HWND hwnd, UINT MenuItem, UINT state);

void CheckMenuItemAppropriately(HWND hwnd, UINT MenuItem, bool b);

bool CheckBufferLength(char* buff, int MaxLength, unsigned int& BufferLength);

void ErrorBox(std::string& msg);
void ErrorBox(char* msg);

Vector2D GetClientCursorPosition();
Vector2D GetClientCursorPosition(HWND hwnd);

void FileInitialize (HWND hwnd,
                     OPENFILENAME& ofn,
                     const std::string& defaultFileTypeDescription,
                     const std::string& defaultFileExtension);

BOOL FileOpenDlg (HWND               hwnd,
                  PTSTR              pstrFileName,
                  PTSTR              pstrTitleName,
                  const std::string& defaultFileTypeDescription,
                  const std::string& defaultFileExtension);

BOOL FileSaveDlg (HWND hwnd,
                  PTSTR pstrFileName,
                  PTSTR pstrTitleName,
                  const std::string& defaultFileTypeDescription,
                  const std::string& defaultFileExtension);

void ResizeWindow(HWND hwnd, int cx, int cy);

int  GetWindowHeight(HWND hwnd);
int  GetWindowWidth(HWND hwnd);

#endif