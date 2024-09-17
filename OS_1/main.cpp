#undef UNICODE
#include <windows.h>
#include <stdio.h>
#include "resource.h"

#pragma warning(disable : 4996)

BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, DLGPROC(MainWndProc));
	return 0;
}

void ShowInfo(HWND wnd) {
	int DIV = 1048576; // 2^20
	char buf[100];
	DWORD size = 100;

	if (!GetComputerName(buf, &size)) strcpy_s(buf, "error");
	SetDlgItemText(wnd, IDC_COMPNAME, buf);
	size = 100;

	OSVERSIONINFO ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ver);
	sprintf_s(buf, "%d.%d (build %d) %s", ver.dwMajorVersion, ver.dwMinorVersion, ver.dwBuildNumber, ver.szCSDVersion);
	SetDlgItemText(wnd, IDC_VERSION, buf);

	if (!GetUserName(buf, &size)) strcpy_s(buf, "error");
	SetDlgItemText(wnd, IDC_USERNAME, buf);

	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	GlobalMemoryStatusEx(&mem);

	sprintf_s(buf, "%u MB", mem.ullTotalPhys / DIV);
	SetDlgItemText(wnd, IDC_TOTALRAM, buf);

	sprintf_s(buf, "%u MB", mem.ullAvailPhys / DIV);
	SetDlgItemText(wnd, IDC_FREERAM, buf);

	sprintf_s(buf, "%u%%", mem.dwMemoryLoad);
	SetDlgItemText(wnd, IDC_PERCENTRAM, buf);

	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf_s(buf, "%u.%u.%u %02d:%02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
	SetDlgItemText(wnd, IDC_DATETIME, buf);

	DWORD tick = GetTickCount();
	sprintf_s(buf, "%llu", tick);
	SetDlgItemText(wnd, IDC_TIMEON, buf);
}

BOOL CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_INITDIALOG:
		ShowInfo(hWnd);
		SetTimer(hWnd, NULL, 250, NULL);
		return TRUE;
	case WM_TIMER:
		ShowInfo(hWnd);
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DestroyWindow(hWnd);
			return TRUE;
		}
		return FALSE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;
}