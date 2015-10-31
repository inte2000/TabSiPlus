/*********************************************************************
*    Author         : orbit
*    Date           : October 6, 2000
* Last-modified  : February 12, 2005 
*    Contact us     : inte2000@163.com,support@winmsg.com
*    Web Page       : http://www.winmsg.com/cn/orbit.htm (for Chinese version)
*                               http://www.winmsg.com/orbit.htm (for English version)
**********************************************************************/

#ifndef __GLOBALS_H
#define __GLOBALS_H

// size constants
#define TAB_HEIGHT              10   // slack (non text part)
#define TAB_PADDING_X           6
#define TAB_PADDING_Y           4
#define FB_PADDING_X            3
#define FB_PADDING_Y            2

#define TAB_BORDER_HEIGHT       3   // slack (non text part)

#define MAC_ADDR_LEN          6

// user messages
#define WM_OPSHEET2TABBAR_POS_CHANGE           (WM_USER + 0x1C)
#define WM_OPSHEET2TABBAR_THEME_CHANGE         (WM_USER + 0x1D)
#define WM_OPSHEET2TABBAR_GROUP_SAME           (WM_USER + 0x1E)
#define WM_OPSHEET2TABBAR_SHOW_ICON            (WM_USER + 0x1F)
#define WM_OPSHEET2TABBAR_SHOW_CLOSE           (WM_USER + 0x20)


// enumerations
enum SnapOrientations           { soTop = 0, soBottom };
enum AddOrder                   { aoFront = 0, aoBack };

extern SnapOrientations cfg_TabWndPosition;
extern SnapOrientations cfg_LastTabWndPosition;
extern AddOrder cfg_iAddOrder;
extern BOOL cfg_bShowIcons;
extern BOOL cfg_bNoShellContext;
extern BOOL cfg_bSmartLoc;
extern int cfg_uSrhDeep;
extern BOOL cfg_bGroupSameTable;
extern BOOL cfg_bShowCloseBtn;

extern int cfg_iLeftBarWidth;

extern BOOL cfg_bSmartFlip;

extern HINSTANCE g_TSuppDll;
extern LPCTSTR lpszTSuppDll;
extern LPCTSTR lpszNull;

#define IS_HORIZONTAL(o) ((o) == soTop  ||  (o) == soBottom)

// class declarations
class CSiWindow;
class CTabBarsWnd;
class CSIFrameWnd;
class CSiMDIWnd;

class GlobalFileList;

// global variables
extern CSIFrameWnd            *g_pSiFrameWnd;  // Source insight main window
extern GlobalFileList         *g_pOpenFileList;


extern TCHAR                    g_szCurDircetory[MAX_PATH];
extern TCHAR                    g_szHeadType[128];
extern TCHAR                    g_szCPPType[128];
extern TCHAR                    g_szThemeName[128];
extern CStringArray        g_strAFlipPath;

extern const char *pszPenNameBtnHiLight;
extern const char *pszPenNameBtnDarkShadow;

// misc
//#define PACKVERSION(major, minor) MAKELONG(minor, major)
#define countof(arr)  (sizeof(arr)/sizeof(arr[0]))

// returns version information about a specified file
extern CString LoadVersionString(LPCTSTR path);

extern void SplitString(CString & strMultiLine,CStringArray & strAMultistr);
extern void CombineString(const CStringArray & strAMultistr,CString & strMultiLine);
extern void InitGlobalVar();
extern int  SplitFileTypeString(LPCTSTR lpszTypeString,CStringArray & arTypes,LPCTSTR lpszSepMark = _T(";"));
extern LPCTSTR StrRight(LPCTSTR lpszStr, int n);
/*
// VC5 compatible replace function
inline void CString_Replace(CString& cStr, LPCTSTR cToReplace, 
                            LPCTSTR cWithWhat)
{
    int nPos = cStr.Find(cToReplace);
    if (nPos >= 0)
    {
        CString cRight = cStr.Mid(nPos + _tcslen(cToReplace));
        cStr = cStr.Left(nPos);
        cStr += cWithWhat + cRight;
    }
}
*/
LPCTSTR GetAppFilePathName(LPCTSTR lpszFileName, LPTSTR lpBuffer, int nBufferSize = MAX_PATH);

BOOL SetWindowsAutoRunKey(BOOL bEnable);
BOOL SIOpenFile(LPCTSTR lpszSiProgram, LPCTSTR lpszFileName);
BOOL GetSiProgramLocation(LPTSTR lpBuffer, int nBufSize);
BOOL IsSiRetainFile(LPCTSTR lpszFileName);
BOOL IsSourceCodeFile(LPCTSTR lpszFileName);
BOOL ModifyFileName(LPTSTR lpszFileNameBuf, int nMaxBufSize, BOOL bReadOnlyFile);
BOOL MyGetFullPathName(LPCTSTR lpszFileName, LPTSTR lpBuffer, int nBufSize = MAX_PATH);
BOOL FindFileInFolder(LPCTSTR lpszRootFolder, LPCTSTR lpszName, LPCTSTR lpszExt, CString& strFullName);
BOOL FindFileInFolder2(LPCTSTR lpszRootFolder, LPCTSTR lpszFileName, LPTSTR lpBuf, int nSize);
BOOL PrepareResource(HINSTANCE hResDll);

BOOL HackFileNameFromTitle(LPCTSTR lpszWndTitle, CString& strFileName, BOOL& bReadOnly, BOOL& bModify);
BOOL GetFileNameFromWindowTitle(HWND hWnd, CString& strFileName, BOOL& bReadOnly, BOOL& bModify);
CString MakeTabItemText(const CString& strFileName, BOOL bReadOnly, BOOL bModify);
CString GetFileExtName(CString& fileName);

#endif  // __GLOBALS_H

