// GlobalFileList.cpp: implementation of the GlobalFileList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalFileList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GlobalFileList::GlobalFileList()
{
    m_pFileEvent = new CSleepEx(TRUE);
    m_SrhDeep    = 3;
}

GlobalFileList::~GlobalFileList()
{
    ClearPathList();
    delete m_pFileEvent;
}

void GlobalFileList::AddToList(LPCTSTR lpszFilePathName)
{
    CEventLock lock(m_pFileEvent);

    if(IsFilePathName(lpszFilePathName))
    {
        TString strPath;
        if(CalcPath(lpszFilePathName, strPath) && !IsFilePathExist(strPath.c_str()))
        {
            if(m_arOpenPath.size() >= MAX_GLOBAL_OPEN_PATH)
            {
                m_arOpenPath.erase(m_arOpenPath.begin()); //删除第一个,也就是说最老的一个
            }

            m_arOpenPath.push_back(strPath);
        }
    }
}

TString GlobalFileList::GetAt(int nIndex)
{
    TString strRtn;
    CEventLock lock(m_pFileEvent);
    if(nIndex >= 0 && nIndex < (int)m_arOpenPath.size())
    {
        strRtn = m_arOpenPath[nIndex];
    }

    return strRtn;
}

int GlobalFileList::GetOpenPathCount()
{
    CEventLock lock(m_pFileEvent);
    int nCount = m_arOpenPath.size();
    return nCount;
}

void GlobalFileList::ClearPathList()
{
    CEventLock lock(m_pFileEvent);

    m_arOpenPath.erase(m_arOpenPath.begin(), m_arOpenPath.end());
}

BOOL GlobalFileList::IsFilePathName(LPCTSTR lpszFilePathName)
{
    if(lstrlen(lpszFilePathName) >= 10)
    {
        if((lpszFilePathName[1] == _T(':')) && (lpszFilePathName[2] == _T('\\')))
        {
            if(_tcsrchr(lpszFilePathName, _T('.')) != NULL)
                return TRUE;
        }
    }

    return FALSE;
}

BOOL GlobalFileList::CalcPath(LPCTSTR lpszFilePathName, TString& strSrhPath)
{
    strSrhPath = lpszFilePathName;

    int RightSlash = strSrhPath.rfind(_T('\\'));
    if((RightSlash < 0) || (RightSlash == 2)) /*同样不希望根目录下的文件c:\sss_log.txt*/
        return FALSE;

    strSrhPath = strSrhPath.substr(0, RightSlash); 
    int nDeepCount = m_SrhDeep - 1;
    while(nDeepCount > 0)
    {
        RightSlash = strSrhPath.rfind(_T('\\'));
        if(RightSlash == 2) //不希望出现C:\这样的结果
            break;

        strSrhPath = strSrhPath.substr(0, RightSlash); 
        nDeepCount--;
    }

    return TRUE;
}

BOOL GlobalFileList::IsFilePathExist(LPCTSTR lpszFilePath)
{
    for(int nIndex = 0; nIndex < (int)m_arOpenPath.size(); nIndex++)
    {
        if(_tcsicmp(m_arOpenPath[nIndex].c_str(), lpszFilePath) == 0)
            return TRUE;
    }

    return FALSE;
}
