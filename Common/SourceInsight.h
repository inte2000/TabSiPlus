// Information about Source Insight (insight3.exe)

#pragma once

extern LPCTSTR splashWndText;
extern LPCTSTR splashWndClassName;

extern LPCTSTR sourceInsightTitle;
extern LPCTSTR lpszSiFrameWndClass;
extern LPCTSTR lpszSi4FrameWndClass;
extern LPCTSTR lpszChindFrameWndClass;
extern LPCTSTR lpszMdiClientWndClass;

extern LPCTSTR lpszDllFileName;
extern LPCTSTR splashBmpFileName;
extern LPCTSTR lpszTextMark;

BOOL IsSourceInsightFrameWnd(HWND hWnd);
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);
HWND FindSourceInsightFrameWindow();
BOOL HookSourceInsightWindow(HWND hSiWnd, LPCTSTR lpszHookDll);
BOOL FindAndHookSourceInsightWindow(LPCTSTR lpszHookDll);
BOOL GetFileNameFromSiCaption(LPCTSTR lpszSiCaption, LPTSTR lpszBuffer, int nBufSize);