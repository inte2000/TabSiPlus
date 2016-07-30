#include "stdafx.h"
#include "Options.h"

//////////////////////////////////////////////////////////////////////////

LPCTSTR lpszKeyRoot = _T("Software\\Orbit Studio\\TabSiPlus");
LPCTSTR lpszLoadOncePropName = _T("LoadOnceOnly");
LPCTSTR lpszShowSplashPropName = _T("ShowSplash");
LPCTSTR lpszAutoRunPropName = _T("AutoRun");

LPCTSTR lpszLeftBarWidth = _T("LeftBarWidth");

LPCTSTR lpszShowIcon = _T("ShowIconOnBar");
LPCTSTR lpszNoShellMenu = _T("NoShellMenu");
LPCTSTR lpszSmartLoc = _T("SmartLoc");
LPCTSTR lpszSrhDeep = _T("SearchDeep");
LPCTSTR lpszcfGroupSame = _T("GroupSameTab");
LPCTSTR lpszcfCloseBtn = _T("ShowCloseButton");

LPCTSTR lpszLimitWnd = _T("LimitOpenWnd");
LPCTSTR lpszMaxWnd = _T("MaxOpenWnd");
LPCTSTR lpszOrderBy = _T("BarOrder");
LPCTSTR lpszInitPos = _T("InitPosition");
LPCTSTR lpszFlipPath = _T("msFlipPath");
LPCTSTR lpszHeadType = _T("HeadType");
LPCTSTR lpszCppType = _T("CppType");
LPCTSTR lpszSelectPortion = _T("SelectPortion");
LPCTSTR lpszThemeName = _T("ThemeName");

LPCTSTR lpszSiLocation = _T("SiLocation");
LPCTSTR lpszSmartFlip = _T("SmartFlip");

LPCTSTR lpszCodeBase = _T("WorkingProjectCodeBase");
LPCTSTR lpszUseSysLogon = _T("UseSysLogon");
LPCTSTR lpszUseName = _T("UseName");
LPCTSTR lpszPassword = _T("Password");
LPCTSTR lpszPrjName = _T("CurrentProjectName");

LPCTSTR lpszPowerColorMode = _T("PowerModeColorMode");
LPCTSTR lpszPowerInitialColor = _T("PowerModeInitialColor");
LPCTSTR lpszPowerFadeoutColor = _T("PowerModeFadeoutColor");
LPCTSTR lpszUsePowerMode = _T("UsePowerMode");

static HKEY s_hRootKey = HKEY_CURRENT_USER;

void SetRegistryRootKey(HKEY hRootKey)
{
    s_hRootKey = hRootKey;
}

BOOL GetDwordRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, DWORD& dwValue, DWORD dwDefault)
{
    HKEY hKey = NULL;
    DWORD dwType = REG_DWORD;
    DWORD dwSize = sizeof(DWORD);
    BOOL bSuccess = FALSE;

    dwValue = dwDefault;//‘§…Ë÷µ
    if(::RegCreateKeyEx(s_hRootKey,lpszKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL) == ERROR_SUCCESS)
    {
        LONG lRes = ::RegQueryValueEx(hKey, lpszPropName, NULL, &dwType, (LPBYTE)&dwValue, &dwSize );
        if(lRes == ERROR_SUCCESS)
        {
            bSuccess = TRUE;
        }

        ::RegCloseKey( hKey );
    }

    return bSuccess;
}

BOOL GetIntegerRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, int& nValue, int nDefault)
{
    DWORD dwTmp = 0;
    DWORD dwDefault = (DWORD)nDefault;
    BOOL bSuccess = GetDwordRegPorpValue(lpszKey,lpszPropName,dwTmp,dwDefault);
    nValue = (int)dwTmp;
    return bSuccess;
}

BOOL GetBoolRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, BOOL& bValue, BOOL bDefault)
{
    DWORD dwTmp = 0;
    DWORD dwDefault = bDefault ? 1 : 0;
    BOOL bSuccess = GetDwordRegPorpValue(lpszKey,lpszPropName,dwTmp,dwDefault);
    bValue = (dwTmp != 0);
    return bSuccess;
}

BOOL GetColorRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, COLORREF& crValue, COLORREF crDefault)
{
    DWORD dwTmp = 0;
    DWORD dwDefault = (DWORD)crDefault;
    BOOL bSuccess = GetDwordRegPorpValue(lpszKey,lpszPropName,dwTmp,dwDefault);
    crValue = (COLORREF)dwTmp;
    return bSuccess;
}

BOOL GetStringRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, LPTSTR lpValue, DWORD nBufSize)
{
    HKEY hKey = NULL;
    DWORD dwType = REG_SZ;
    DWORD dwSize = nBufSize;
    BOOL bSuccess = FALSE;

    lpValue[0] = 0;
    if(::RegCreateKeyEx(s_hRootKey,lpszKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL) == ERROR_SUCCESS)
    {
        LONG lRes = ::RegQueryValueEx(hKey, lpszPropName, NULL, &dwType, (LPBYTE)lpValue, &dwSize );
        if(lRes == ERROR_SUCCESS)
        {
            bSuccess = TRUE;
        }

        ::RegCloseKey( hKey );
    }

    return bSuccess;
}

BOOL SetDwordRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, DWORD dwValue)
{
    HKEY hKey = NULL;
    DWORD dwType = REG_DWORD;
    DWORD dwSize = sizeof(DWORD);
    BOOL bSuccess = FALSE;

    if(::RegCreateKeyEx(s_hRootKey,lpszKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL) == ERROR_SUCCESS)
    {
        LONG lRes = ::RegSetValueEx(hKey, lpszPropName, 0, dwType, (LPBYTE)&dwValue, dwSize);
        if(lRes == ERROR_SUCCESS)
        {
            bSuccess = TRUE;
        }

        ::RegCloseKey( hKey );
    }

    return bSuccess;
}

BOOL SetIntegerRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, int nValue)
{
    DWORD dwTmp = (DWORD)nValue;
    return SetDwordRegPorpValue(lpszKey,lpszPropName,dwTmp);
}

BOOL SetBoolRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, BOOL bValue)
{
    DWORD dwTmp = bValue ? 1 : 0;
    return SetDwordRegPorpValue(lpszKey,lpszPropName,dwTmp);
}

BOOL SetColorRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, COLORREF crValue)
{
    DWORD dwTmp = (DWORD)crValue;
    return SetDwordRegPorpValue(lpszKey,lpszPropName,dwTmp);
}

BOOL SetStringRegPorpValue(LPCTSTR lpszKey, LPCTSTR lpszPropName, LPCTSTR lpString)
{
    HKEY hKey = NULL;
    DWORD dwType = REG_SZ;
    DWORD dwSize = lstrlen(lpString) + 1;
    BOOL bSuccess = FALSE;

    if(::RegCreateKeyEx(s_hRootKey,lpszKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL) == ERROR_SUCCESS)
    {
        LONG lRes = ::RegSetValueEx(hKey, lpszPropName, 0, dwType, (LPBYTE)lpString, dwSize);
        if(lRes == ERROR_SUCCESS)
        {
            bSuccess = TRUE;
        }

        ::RegCloseKey( hKey );
    }

    return bSuccess;
}

