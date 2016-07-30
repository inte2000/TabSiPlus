/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/
#include "stdafx.h"
#include "resource.h"
#include "GlobalFileList.h"
#include "CmdIconResource.h"
#include "WndUIResource.h"
#include "PowerModeConfig.h"
#include "Globals.h"

/*
  调整标签栏的位置时，CSiMDIWnd::ReclameTabSpace需要根据老的位置回收原来
  tabwnd占用的空间，然后根据新的位置计算新的占用空间
*/
SnapOrientations cfg_TabWndPosition;
SnapOrientations cfg_LastTabWndPosition;
AddOrder cfg_iAddOrder;// = aoFront;
BOOL cfg_bShowIcons = TRUE;
BOOL cfg_bNoShellContext;// = TRUE;
BOOL cfg_bSmartLoc = FALSE;
int cfg_uSrhDeep  = 2;
BOOL cfg_bGroupSameTable = FALSE;
BOOL cfg_bShowCloseBtn = FALSE;

int cfg_iLeftBarWidth = 46;

BOOL cfg_bSmartFlip = TRUE;
BOOL cfg_bUsePowerMode = FALSE;

TCHAR g_szCurDircetory[MAX_PATH] = {0};
TCHAR g_szCurProjectPath[MAX_PATH] = {0};
TCHAR g_szHeadType[128];// = _T("h;hpp;hxx;hm;inl");
TCHAR g_szCPPType[128];// = _T("cpp;c;cxx;cc");
TCHAR g_szThemeName[128] = _T("不知所措");
CStringArray        g_strAFlipPath;

HINSTANCE g_TSuppDll = NULL;
LPCTSTR lpszTSuppDll = _T("TSupp.dll");
LPCTSTR lpszNull = _T("");

const char *pszPenNameBtnHiLight = "PenNameBtnHiLight";
const char *pszPenNameBtnDarkShadow = "PenNameBtnDarkShadow";

const int nCustomColorCount = 8;
static const char *CustomResourceColorName[nCustomColorCount] = 
{
    pszPenNameBtnHiLight,
    pszPenNameBtnDarkShadow,
    pResMenuTextHilight,
    pResMenuTextNormal,
    pResMebuLeftbar,
    pResCtrlRectFrameHilight,
    pResMenuBkgndHilight,
    pResMenuBkgndNormal
};

CString LoadVersionString(LPCTSTR path)
{
    DWORD   DataLen, Zero;
    PVOID   pData, pValue;
    UINT    ValueLen;
    CString cRetVal;
    
    DataLen = GetFileVersionInfoSize((char *)path, &Zero);
    pData = new char[DataLen];
    GetFileVersionInfo((char *)path, NULL, DataLen, pData);
    VerQueryValue(pData, 
        _T("\\StringFileInfo\\040904b0\\ProductVersion"), &pValue, 
        &ValueLen);

    memcpy(cRetVal.GetBuffer(ValueLen), pValue, ValueLen);
    cRetVal.ReleaseBuffer();

    delete pData;

    return cRetVal;
}

void SplitString(CString & strMultiLine,CStringArray & strAMultistr)
{
    CString strTmp;
    int length = strMultiLine.GetLength();
    if(length == 0)
        return;
    int idx;

    while((idx = strMultiLine.Find(_T("\r\n"),0)) != -1)
    {
        strTmp = strMultiLine.Left(idx);
        strAMultistr.Add(strTmp);
        strMultiLine = strMultiLine.Right(length - idx - 2);
        length = strMultiLine.GetLength();
    }
//    strAMultistr.Add(strMultiLine);
}

void CombineString(const CStringArray & strAMultistr,CString & strMultiLine)
{
    int size = strAMultistr.GetSize();
    if(size == 0)
        return;

    strMultiLine.Empty();

    for(int i = 0; i < size; i++)
    {
        strMultiLine += strAMultistr[i];
//        if(i < (size -1))
        strMultiLine += _T("\r\n");
    }
}

int SplitFileTypeString(LPCTSTR lpszTypeString,CStringArray& arTypes,LPCTSTR lpszSepMark)
{
    int nPart = 0;
    arTypes.RemoveAll();
    if(_tcslen(lpszTypeString) > 0)
    {
        TCHAR szTemp[256];
        _tcscpy(szTemp,lpszTypeString);//copy to a buffer
        _tcscat(szTemp,lpszSepMark);//add a end mark

        LPTSTR lpStart = szTemp;
        LPTSTR lpPoint;
        int nCharSetLength = _tcslen(lpszSepMark);
        while((*lpStart != 0) && (lpPoint = _tcsstr(lpStart,lpszSepMark)) != NULL)
        {
            *lpPoint = 0;
            arTypes.Add(lpStart);
            lpStart = lpPoint + nCharSetLength;
            nPart++;
        }
    }
    return nPart;
}

LPCTSTR StrRight(LPCTSTR lpszStr, int n)
{
    if(!lpszStr)
        return NULL;

    int nLength = lstrlen(lpszStr);
    if(nLength <= 2)
        return lpszStr;

    LPCTSTR lpszRight = lpszStr + (nLength - 2);
    return lpszRight;
}

/*********************************************************************
 * Function Name : InitGlobalVar
 * Explain : call this function when start up
 * Parameters :
 * Return : 
 * void  -- 
 * Author : orbit 
 * Time : 2001-03-15 17:28:02 
*********************************************************************/
void InitGlobalVar()
{
    SetRegistryRootKey(HKEY_CURRENT_USER);
    
    int nTmp;
    GetIntegerRegPorpValue(lpszKeyRoot,lpszInitPos,nTmp,(int)soBottom);
    cfg_TabWndPosition = (SnapOrientations)nTmp;
    cfg_LastTabWndPosition = cfg_TabWndPosition;

    GetIntegerRegPorpValue(lpszKeyRoot,lpszOrderBy,nTmp,(int)aoFront);
    cfg_iAddOrder = (AddOrder)nTmp;

    GetBoolRegPorpValue(lpszKeyRoot,lpszShowIcon,cfg_bShowIcons,TRUE);
    GetBoolRegPorpValue(lpszKeyRoot,lpszNoShellMenu,cfg_bNoShellContext,TRUE);

    GetBoolRegPorpValue(lpszKeyRoot,lpszSmartLoc,cfg_bSmartLoc,FALSE);
    GetIntegerRegPorpValue(lpszKeyRoot,lpszSrhDeep,cfg_uSrhDeep,2);
    GetBoolRegPorpValue(lpszKeyRoot,lpszcfGroupSame,cfg_bGroupSameTable,FALSE);
    GetBoolRegPorpValue(lpszKeyRoot,lpszcfCloseBtn,cfg_bShowCloseBtn,FALSE);

    g_pOpenFileList->SetSearchPathDeep(cfg_uSrhDeep);
    GetIntegerRegPorpValue(lpszKeyRoot,lpszLeftBarWidth,cfg_iLeftBarWidth,46);
    
    GetBoolRegPorpValue(lpszKeyRoot,lpszSmartFlip,cfg_bSmartFlip,FALSE);

    CString strMultiLine;
    TCHAR szSelect[64];
    BOOL bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszFlipPath,strMultiLine.GetBuffer(8192),8192);
    strMultiLine.ReleaseBuffer();

    GetStringRegPorpValue(lpszKeyRoot,lpszSelectPortion,&(szSelect[1]),62);

    g_strAFlipPath.RemoveAll();
    SplitString(strMultiLine,g_strAFlipPath);

    TCHAR szDefaultPath[MAX_PATH];
    DWORD dwBufSize = MAX_PATH;//SIZEOF_ARRAY(szDefaultPath);
    LocLoadString(IDS_DEFAULT_FLIP_PATH, szDefaultPath, &dwBufSize );
    g_strAFlipPath.InsertAt(0,szDefaultPath,1);
    szSelect[0] = _T('1');

    int max = g_strAFlipPath.GetSize();
    CUIntArray nASelect;
    int i;
    for(i = 0; i < max; i++)
        nASelect.Add((szSelect[i] == _T('1')) ? 1 : 0);

    for(i = 0; i < max; i++)
    {
        if(nASelect[i] != 1)
        {
            g_strAFlipPath.RemoveAt(i);
            nASelect.RemoveAt(i);
            i--;
            max--;
        }
    }
    nASelect.RemoveAll();

    bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszThemeName,g_szThemeName,128);
    if(!bSuccess)
        _tcscpy(g_szThemeName, _T("base_theme.ini"));
        
    bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszHeadType,g_szHeadType,128);
    if(!bSuccess)
        _tcscpy(g_szHeadType, _T(".h;.hpp"));//_T(".h;.hpp;.hxx;.hm;.inl"));
    bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszCppType,g_szCPPType,128);
    if(!bSuccess)
        _tcscpy(g_szCPPType,_T(".cpp;.c")); //_T(".cpp;.c;.cxx;.cc"));

    CWndUIResource *pUIRes = GlobalGetUIRes();

    COLORREF crTmp;
    for(i = 0; i < nCustomColorCount; i++)
    {
        if(GetColorRegPorpValue(lpszKeyRoot,CustomResourceColorName[i],crTmp,RGB(0,0,0)))
        {
            pUIRes->SetColor(CustomResourceColorName[i],crTmp);
        }
    }

    int powerColorMode;
    COLORREF crInitial, crFadeout;
    GetIntegerRegPorpValue(lpszKeyRoot, lpszPowerColorMode, powerColorMode, 0);
    GetColorRegPorpValue(lpszKeyRoot, lpszPowerInitialColor, crInitial, RGB(255,255,255));
    GetColorRegPorpValue(lpszKeyRoot, lpszPowerFadeoutColor, crFadeout, RGB(166, 202, 240));
    GetBoolRegPorpValue(lpszKeyRoot, lpszUsePowerMode, cfg_bUsePowerMode, FALSE);

    CPowerModeConfig *pmCfg = GlobalPowerModeConfig();
    pmCfg->SetInitialColor(crInitial);
    pmCfg->SetFadeoutColor(crFadeout);
    pmCfg->SetColorMode(powerColorMode);
}

BOOL SetWindowsAutoRunKey(BOOL bEnable)
{
    HKEY hRun;
    DWORD dwPosion;
    LONG lRtn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,NULL,0,KEY_ALL_ACCESS,NULL,&hRun,&dwPosion);
    if(lRtn == ERROR_SUCCESS)
    {
        char szPath[MAX_PATH];
        GetAppFilePathName(_T("TabSiHost.exe"), szPath, MAX_PATH);
        if(bEnable)
        {
            ::RegSetValueEx(hRun,_T("TabSiPlus"),0,REG_SZ,(const BYTE *)szPath,lstrlen(szPath)+1);
        }
        else
        {
            ::RegDeleteValue(hRun,_T("TabSiPlus"));
        }

        ::RegCloseKey(hRun);

        return TRUE;
    }

    return FALSE;
}

BOOL SIOpenFile(LPCTSTR lpszSiProgram, LPCTSTR lpszFileName)
{
    WZ_ASSERT(lpszSiProgram);
    WZ_ASSERT(lpszFileName);

    //判断文件是否存在
    if(::GetFileAttributes(lpszFileName) == 0xFFFFFFFF)
        return FALSE;

    TCHAR szCmdLine[300];
    wsprintf(szCmdLine,_T(" -i \"%s\""),lpszFileName);

    SHELLEXECUTEINFO si;
    ZeroMemory(&si,sizeof(SHELLEXECUTEINFO));
    si.cbSize = sizeof(SHELLEXECUTEINFO);
    si.fMask = SEE_MASK_NOCLOSEPROCESS;
    si.hwnd = ::GetFocus();
    si.lpVerb = NULL;
    si.lpFile = lpszSiProgram;
    si.lpParameters = szCmdLine;
    si.nShow = SW_SHOWNORMAL;
    BOOL bOpen = ::ShellExecuteEx(&si);

    return bOpen;
/*
    STARTUPINFO su;
    PROCESS_INFORMATION pi;

    memset(&pi,0,sizeof(PROCESS_INFORMATION));
    memset(&su,0,sizeof(STARTUPINFO));
    su.cb = sizeof(STARTUPINFO);
    su.wShowWindow = SW_SHOW;

    return ::CreateProcess(lpszSiProgram,szCmdLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&su,&pi);
*/
}

LPCTSTR lpszPathsPath = _T("SOFTWARE\\Source Dynamics\\Source Insight\\3.0\\Paths");
LPCTSTR lpszInitDir = _T("InitDir");
LPCTSTR lpszSiProgram = _T("insight3.exe");

BOOL GetSiProgramLocation(LPTSTR lpBuffer, int nBufSize)
{
    WZ_ASSERT(lpBuffer);
    HKEY hKey = NULL;
    DWORD dwType = REG_SZ;
    DWORD dwSize = nBufSize;
    BOOL bSuccess = FALSE;

    lpBuffer[0] = 0;

    SetRegistryRootKey(HKEY_CURRENT_USER);
    bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszSiLocation,lpBuffer,nBufSize);
    if(bSuccess && (::GetFileAttributes(lpBuffer) != 0xFFFFFFFF))
    {
        return bSuccess;
    }
    //尝试从source insight的安装目录寻找

    if(::RegCreateKeyEx(HKEY_LOCAL_MACHINE,lpszPathsPath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL) == ERROR_SUCCESS)
    {
        LONG lRes = ::RegQueryValueEx(hKey, lpszInitDir, NULL, &dwType, (LPBYTE)lpBuffer, &dwSize );
        if(lRes == ERROR_SUCCESS)
        {
            _tcscat(lpBuffer,_T("\\"));
            _tcscat(lpBuffer,lpszSiProgram);
            bSuccess = TRUE;
        }

        ::RegCloseKey( hKey );
    }

    return bSuccess;
}

BOOL IsSiRetainFile(LPCTSTR lpszFileName)
{
    WZ_ASSERT(lpszFileName);
    LPCTSTR lpDot = _tcsrchr(lpszFileName,_T('.'));
    if(lpDot)
    {
        if( (_tcsicmp(lpDot, _T(".iab")) == 0) || (_tcsicmp(lpDot, _T(".iad")) == 0) 
            || (_tcsicmp(lpDot, _T(".imb")) == 0) || (_tcsicmp(lpDot, _T(".imd")) == 0)
            || (_tcsicmp(lpDot, _T(".pfi")) == 0) || (_tcsicmp(lpDot, _T(".po")) == 0)
            || (_tcsicmp(lpDot, _T(".pri")) == 0) || (_tcsicmp(lpDot, _T(".ps")) == 0)
            || (_tcsicmp(lpDot, _T(".wk3")) == 0) || (_tcsicmp(lpDot, _T(".tmp")) == 0)
            || (_tcsicmp(lpDot, _T(".pr")) == 0) || (_tcsicmp(lpDot, _T(".pr")) == 0)
            || (_tcsicmp(lpDot, _T(".db3")) == 0) || (_tcsicmp(lpDot, _T(".cf3")) == 0)    )
            return TRUE;

        return FALSE;
    }

    return FALSE;
}

BOOL IsSourceCodeFile(LPCTSTR lpszFileName)
{
    WZ_ASSERT(lpszFileName);
    LPCTSTR lpDot = _tcsrchr(lpszFileName,_T('.'));
    if(lpDot)
    {
        if( (_tcsicmp(lpDot, _T(".c")) == 0) || (_tcsicmp(lpDot, _T(".cpp")) == 0) 
            || (_tcsicmp(lpDot, _T(".cxx")) == 0) || (_tcsicmp(lpDot, _T(".h")) == 0)
            || (_tcsicmp(lpDot, _T(".hxx")) == 0) || (_tcsicmp(lpDot, _T(".cs")) == 0)
            || (_tcsicmp(lpDot, _T(".vb")) == 0) || (_tcsicmp(lpDot, _T(".java")) == 0) )
            return TRUE;

        return FALSE;
    }

    return FALSE;
}

BOOL ModifyFileName(LPTSTR lpszFileNameBuf, int nMaxBufSize, BOOL bReadOnlyFile)
{
    if(!lpszFileNameBuf)
        return FALSE;

    TCHAR szFileName[MAX_PATH];
    int nSrcLength = _tcslen(lpszFileNameBuf);
    TCHAR *pSlash = _tcsrchr(lpszFileNameBuf,_T('\\'));
    if(!pSlash)
        return FALSE;

    _tcscpy(szFileName, pSlash + 1);//skip '\'

    if(bReadOnlyFile)
    {
        if((nSrcLength + 1) > nMaxBufSize)
            return FALSE;

        if(szFileName[0] == _T('!'))//已经有!
            return TRUE;

        _tcscpy(pSlash,_T("\\!"));
        _tcscat(pSlash,szFileName);
    }
    else
    {
        if(szFileName[0] == _T('!'))//有!
        {
            _tcscpy(pSlash + 1, &(szFileName[1]));
        }
    }

    return TRUE;
}

BOOL MyGetFullPathName(LPCTSTR lpszFileName, LPTSTR lpBuffer, int nBufSize)
{
    WZ_ASSERT(lpBuffer);
    BOOL bFind = FALSE;

    int nCount, i;
    if(cfg_bSmartLoc)
    {
        //先从API钩子获取的文件全名匹配
        nCount = g_pOpenFileList->GetOpenPathCount();
        for(i = 0; i < nCount; i++)
        {
            TString strPath = g_pOpenFileList->GetAt(i);
            if(!strPath.empty())
            {
                bFind = FindFileInFolder2(strPath.c_str(),lpszFileName,lpBuffer,nBufSize);
                if(bFind)
                    return (::GetFileAttributes(lpBuffer) != 0xFFFFFFFF);//文件确实存在
            }
        }
    }

    //最后，搜索用户设置的切换搜索目录
    nCount = g_strAFlipPath.GetSize();
    for(i = 0; i < nCount; i++)
    {
        bFind = FindFileInFolder2(g_strAFlipPath[i],lpszFileName,lpBuffer,nBufSize);
        if(bFind)
            return (::GetFileAttributes(lpBuffer) != 0xFFFFFFFF);//文件确实存在
    }

    DebugTracing(gnDbgLevelNormalDebug,_T("Not Get File FullName(%s)"),lpszFileName);
    return FALSE;
}


BOOL FindFileInFolder(LPCTSTR lpszRootFolder, LPCTSTR lpszName, LPCTSTR lpszExt, CString& strFullName)
{
    CString strCurName = lpszRootFolder;
    strCurName += lpszName;
    strCurName += lpszExt;
    if(GetFileAttributes(strCurName) != 0xFFFFFFFF)/*已经找到文件*/
    {
        strFullName = strCurName;
        return TRUE;
    }

    BOOL bSuccess = FALSE;
    TCHAR szRootPath[_MAX_PATH];
    _tcscpy(szRootPath,lpszRootFolder);
    _tcscat(szRootPath,_T("*.*"));

    WIN32_FIND_DATA data;
    HANDLE hFindFile = FindFirstFile(szRootPath,&data);
    if(hFindFile != INVALID_HANDLE_VALUE)
    {
        TCHAR szSubRootName[_MAX_PATH];
        do
        {
            if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
            {
                if((_tcscmp(data.cFileName,_T(".")) == 0) || (_tcscmp(data.cFileName,_T("..")) == 0))
                    continue;

                _tcscpy(szSubRootName,lpszRootFolder);
                _tcscat(szSubRootName,data.cFileName);
                _tcscat(szSubRootName,_T("\\"));
                bSuccess = FindFileInFolder(szSubRootName,lpszName,lpszExt,strFullName);
                if(bSuccess)/*找到*/
                {
                    break;
                }
            }
        }while(FindNextFile(hFindFile,&data));

        FindClose(hFindFile);
    }

    return bSuccess;
}

BOOL FindFileInFolder2(LPCTSTR lpszRootFolder, LPCTSTR lpszFileName, LPTSTR lpBuf, int nSize)
{
    CString strRootPath = lpszRootFolder;
    if(strRootPath.IsEmpty())
        return FALSE;
    if(strRootPath[strRootPath.GetLength() - 1] != _T('\\'))
        strRootPath += _T("\\");

    CString strFullName;
    BOOL bFind;
    CString strName = lpszFileName;
    int nDot = strName.ReverseFind(_T('.'));
    if(nDot > 0)
    {
        CString strExt = strName.Mid(nDot);
        strName = strName.Left(nDot);
        bFind = FindFileInFolder(strRootPath,strName, strExt,strFullName);
    }
    else
        bFind = FindFileInFolder(strRootPath,strName,_T(""),strFullName);

    if(bFind)
    {
        if(strFullName.GetLength() < nSize)
            _tcscpy(lpBuf,strFullName);
        else
        {
            _tcsncpy(lpBuf,strFullName,nSize - 1);
            lpBuf[nSize - 1] = _T('\0');
        }
    }
    return bFind;
}


LPCTSTR GetAppFilePathName(LPCTSTR lpszFileName, LPTSTR lpBuffer, int nBufferSize)
{
    lpBuffer[0] = 0;
    GetModuleFileName((HMODULE)AfxGetInstanceHandle(),lpBuffer,nBufferSize);
    LPTSTR lpChar = lpBuffer + lstrlen(lpBuffer) - 1;
    while(*lpChar != _T('\\'))
        lpChar--;

    lstrcpy(lpChar + 1,lpszFileName);

    return lpBuffer;
}

BOOL HackFileNameFromTitle(LPCTSTR lpszWndTitle, CString& strFileName, BOOL& bReadOnly, BOOL& bModify)
{
    bReadOnly = FALSE;
    bModify   = FALSE;
    LPCTSTR lpTitle = lpszWndTitle;
    
    if(lpTitle[0] == _T('!'))
    {
        bReadOnly = TRUE;
        lpTitle++;
    }
    CString strTitle = lpTitle;
    if(strTitle[strTitle.GetLength() - 1] == _T('*'))
    {
        bModify = TRUE;
        strTitle.TrimRight(_T('*'));
    }

    int nPos = strTitle.Find(_T(" ("), 0);
    if(nPos < 0)
    {
        strFileName = strTitle;
    }
    else
    {
        strFileName = strTitle.Left(nPos);
    }

    return (strFileName.Find(_T('.'), 0) > 0);
}

BOOL GetFileNameFromWindowTitle(HWND hWnd, CString& strFileName, BOOL& bReadOnly, BOOL& bModify)
{
    TCHAR szTitle[260];
    GetWindowText(hWnd, szTitle, 260);
    return HackFileNameFromTitle(szTitle, strFileName, bReadOnly, bModify);
}

CString MakeTabItemText(const CString& strFileName, BOOL bReadOnly, BOOL bModify)
{
    CString strItemText;
    if(bReadOnly)
        strItemText += _T('!');

    strItemText += strFileName;
    if(bModify)
        strItemText += _T(" *");

    return strItemText;
}

CString GetFileExtName(CString& fileName)
{
    CString extName;
    int pos = fileName.ReverseFind(_T('.'));
    if(pos >= 0)
    {
        extName = fileName.Mid(pos);
    }

    return extName;
}

#ifdef INSTALL_SI_MENU_THEME
static IconStateDate sc_close[4] = 
{
    {nIconStateNormal,FALSE,RGB(0,0,0), {0,0,15,15}, {0,0,0,0} },
    {nIconStateHover,FALSE,RGB(0,0,0), {48,0,63,15}, {0,0,0,0} },
    {nIconStateSelected,FALSE,RGB(0,0,0), {32,0,47,15}, {0,0,0,0} },
    {nIconStateDisabled,FALSE,RGB(0,0,0), {16,0,31,15}, {0,0,0,0} }
};

static IconStateDate sc_max[4] = 
{
    {nIconStateNormal,FALSE,RGB(0,0,0), {0,16,15,31}, {0,0,0,0} },
    {nIconStateHover,FALSE,RGB(0,0,0), {48,16,63,31}, {0,0,0,0} },
    {nIconStateSelected,FALSE,RGB(0,0,0), {32,16,47,31}, {0,0,0,0} },
    {nIconStateDisabled,FALSE,RGB(0,0,0), {16,16,31,31}, {0,0,0,0} }
};

static IconStateDate sc_restore[4] = 
{
    {nIconStateNormal,FALSE,RGB(0,0,0), {0,32,15,47}, {0,0,0,0} },
    {nIconStateHover,FALSE,RGB(0,0,0), {48,32,63,47}, {0,0,0,0} },
    {nIconStateSelected,FALSE,RGB(0,0,0), {32,32,47,47}, {0,0,0,0} },
    {nIconStateDisabled,FALSE,RGB(0,0,0), {16,32,31,47}, {0,0,0,0} }
};

static IconStateDate sc_min[4] = 
{
    {nIconStateNormal,FALSE,RGB(0,0,0), {0,48,15,63}, {0,0,0,0} },
    {nIconStateHover,FALSE,RGB(0,0,0), {48,48,63,63}, {0,0,0,0} },
    {nIconStateSelected,FALSE,RGB(0,0,0), {32,48,47,63}, {0,0,0,0} },
    {nIconStateDisabled,FALSE,RGB(0,0,0), {16,48,31,63}, {0,0,0,0} }
};

static IconStateDate file_new[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {64,16,79,31}, {0,0,0,0} }
};

static IconStateDate edit_redo[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {80,64,95,79}, {0,0,0,0} }
};

static IconStateDate edit_cut[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {96,64,111,79}, {0,0,0,0} }
};

static IconStateDate file_saveas[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {64,64,79,79}, {0,0,0,0} }
};

static IconStateDate file_saveall[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {64,48,79,63}, {0,0,0,0} }
};

static IconStateDate edit_refresh[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {80,48,95,63}, {0,0,0,0} }
};

static IconStateDate search_prj[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {96,48,111,63}, {0,0,0,0} }
};

static IconStateDate file_save[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {64,32,79,47}, {0,0,0,0} }
};

static IconStateDate edit_undo[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {80,32,95,47}, {0,0,0,0} }
};

static IconStateDate search_next[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {96,32,111,47}, {0,0,0,0} }
};

static IconStateDate edit_copy[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {64,0,79,15}, {0,0,0,0} }
};

static IconStateDate search_srh[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {80,0,95,15}, {0,0,0,0} }
};

static IconStateDate file_open[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {96,0,111,15}, {0,0,0,0} }
};

static IconStateDate edit_paste[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {112,0,127,15}, {0,0,0,0} }
};

static IconStateDate window_titleh[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {128,0,143,15}, {0,0,0,0} }
};

static IconStateDate window_titlev[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {128,16,143,31}, {0,0,0,0} }
};

static IconStateDate option_perf[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {128,32,143,47}, {0,0,0,0} }
};

static IconStateDate edit_select[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {80,16,95,31}, {0,0,0,0} }
};

static IconStateDate search_prev[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {96,16,111,31}, {0,0,0,0} }
};

static IconStateDate edit_insert[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {112,16,127,31}, {0,0,0,0} }
};

static IconStateDate window_new[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {112,48,127,63}, {0,0,0,0} }
};

static IconStateDate window_cascade[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {112,64,127,79}, {0,0,0,0} }
};
#endif

static IconStateDate om_option[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {144,0,159,15}, {0,0,0,0} }
};

static IconStateDate om_home[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {128,48,143,63}, {0,0,0,0} }
};

static IconStateDate om_flip[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {128,64,143,79}, {0,0,0,0} }
};

static IconStateDate help_help[1] = 
{
    {nIconStateNormal,TRUE,RGB(255,0,255), {112,32,127,47}, {0,0,0,0} }
};


BOOL PrepareResource(HINSTANCE hResDll)
{
    CCmdIconResource *pCmdIconRes = GlobalGetCmdIconResource();
    WZ_ASSERT(pCmdIconRes);

    if(!pCmdIconRes->LoadIconBitmap(hResDll,101))
        return FALSE;

    CCmdIconMap *pCmdMap = NULL;
#ifdef INSTALL_SI_MENU_THEME
    pCmdMap = new CCmdIconMap(SC_CLOSE);
    pCmdMap->AddStateItems(sc_close,4);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(SC_MAXIMIZE);
    pCmdMap->AddStateItems(sc_max,4);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(SC_RESTORE);
    pCmdMap->AddStateItems(sc_restore,4);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(SC_MINIMIZE);
    pCmdMap->AddStateItems(sc_min,4);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(56);//file_new
    pCmdMap->AddStateItems(file_new,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(57);//file_new
    pCmdMap->AddStateItems(file_open,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(59);//
    pCmdMap->AddStateItems(file_save,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(60);//
    pCmdMap->AddStateItems(file_saveas,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(89);//
    pCmdMap->AddStateItems(file_saveall,1);
    pCmdIconRes->AddIconMap(pCmdMap);


    pCmdMap = new CCmdIconMap(44);//
    pCmdMap->AddStateItems(edit_select,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(87);//
    pCmdMap->AddStateItems(edit_undo,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(88);//
    pCmdMap->AddStateItems(edit_redo,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(90);//
    pCmdMap->AddStateItems(edit_cut,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(91);//
    pCmdMap->AddStateItems(edit_copy,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(92);//
    pCmdMap->AddStateItems(edit_paste,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(100);//
    pCmdMap->AddStateItems(edit_refresh,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(251);//
    pCmdMap->AddStateItems(edit_insert,1);
    pCmdIconRes->AddIconMap(pCmdMap);


    pCmdMap = new CCmdIconMap(62);//
    pCmdMap->AddStateItems(search_srh,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(464);//
    pCmdMap->AddStateItems(search_prj,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(43);//
    pCmdMap->AddStateItems(search_prev,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(106);//
    pCmdMap->AddStateItems(search_next,1);
    pCmdIconRes->AddIconMap(pCmdMap);


    pCmdMap = new CCmdIconMap(71);//
    pCmdMap->AddStateItems(option_perf,1);
    pCmdIconRes->AddIconMap(pCmdMap);


    pCmdMap = new CCmdIconMap(110);//
    pCmdMap->AddStateItems(window_new,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(280);//
    pCmdMap->AddStateItems(window_cascade,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(281);//
    pCmdMap->AddStateItems(window_titleh,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(287);//
    pCmdMap->AddStateItems(window_titlev,1);
    pCmdIconRes->AddIconMap(pCmdMap);


    pCmdMap = new CCmdIconMap(33);//
    pCmdMap->AddStateItems(help_help,1);
    pCmdIconRes->AddIconMap(pCmdMap);
#endif

    pCmdMap = new CCmdIconMap(IDC_MAIN_OPTION);
    pCmdMap->AddStateItems(om_option,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(IDC_MAIN_HEADFLIP);
    pCmdMap->AddStateItems(om_flip,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(IDC_HEADERFLIP);
    pCmdMap->AddStateItems(om_flip,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(IDC_MAIN_HELP);
    pCmdMap->AddStateItems(help_help,1);
    pCmdIconRes->AddIconMap(pCmdMap);

    pCmdMap = new CCmdIconMap(IDC_MAIN_SUPPORT);
    pCmdMap->AddStateItems(om_home,1);
    pCmdIconRes->AddIconMap(pCmdMap);


    CWndUIResource *pUIRes = GlobalGetUIRes();
    pUIRes->PrepareDefaultColors();

    return TRUE;
}
