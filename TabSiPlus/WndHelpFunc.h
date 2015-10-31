#ifndef __WNDHELP_FUNCTION_H__
#define __WNDHELP_FUNCTION_H__

UINT GetStyle(HWND hWnd);
UINT GetExStyle(HWND hWnd);
void SwapLeftRight(LPRECT lpRect);
void ScreenToClient(HWND hWnd, LPRECT lpRect);
void ClientToScreen(HWND hWnd, LPRECT lpRect);
void MoveWindow(HWND hWnd, LPRECT lpRect, BOOL bRepaint = TRUE);

void DeflateRect(LPRECT lpRect, int x, int y);
void DeflateRect(LPRECT lpRect, SIZE size);
void DeflateRect(LPRECT lpSrc, LPCRECT lpRect);
void DeflateRect(LPRECT lpSrc, int l, int t, int r, int b);

#endif //__WNDHELP_FUNCTION_H__