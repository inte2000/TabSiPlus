#ifndef __OPTIONS_H__
#define __OPTIONS_H__



extern LPCTSTR lpszKeyRoot;
extern LPCTSTR lpszLoadOncePropName;
extern LPCTSTR lpszShowSplashPropName;
extern LPCTSTR lpszAutoRunPropName;

extern LPCTSTR lpszLeftBarWidth;

extern LPCTSTR lpszShowIcon;
extern LPCTSTR lpszNoShellMenu;
extern LPCTSTR lpszSmartLoc;
extern LPCTSTR lpszSrhDeep;
extern LPCTSTR lpszcfGroupSame;
extern LPCTSTR lpszcfCloseBtn;
extern LPCTSTR lpszLimitWnd;
extern LPCTSTR lpszMaxWnd;
extern LPCTSTR lpszOrderBy;
extern LPCTSTR lpszInitPos;
extern LPCTSTR lpszFlipPath;
extern LPCTSTR lpszHeadType;
extern LPCTSTR lpszCppType;
extern LPCTSTR lpszSelectPortion;
extern LPCTSTR lpszThemeName;

extern LPCTSTR lpszSiLocation;
extern LPCTSTR lpszSmartFlip;

extern LPCTSTR lpszCodeBase;
extern LPCTSTR lpszUseSysLogon;
extern LPCTSTR lpszUseName;
extern LPCTSTR lpszPassword;
extern LPCTSTR lpszPrjName;

void SetRegistryRootKey(HKEY hRootKey);


BOOL GetDwordRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, DWORD& dwValue, DWORD dwDefault = 0);
BOOL GetIntegerRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, int& nValue, int nDefault = 0);
BOOL GetBoolRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, BOOL& bValue, BOOL bDefult = FALSE);
BOOL GetColorRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, COLORREF& crValue, COLORREF crDefault = RGB(0,0,0));
BOOL GetStringRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, LPTSTR lpValue, DWORD nBufSize);

BOOL SetDwordRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, DWORD dwValue);
BOOL SetIntegerRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, int nValue);
BOOL SetBoolRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, BOOL bValue);
BOOL SetColorRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, COLORREF crValue);
BOOL SetStringRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, LPCTSTR lpString);


#endif //__OPTIONS_H__
