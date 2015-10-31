// GlobalFileList.h: interface for the GlobalFileList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALFILELIST_H__A1CC8668_CDC4_4A2E_9BDE_B995FC8EB9F1__INCLUDED_)
#define AFX_GLOBALFILELIST_H__A1CC8668_CDC4_4A2E_9BDE_B995FC8EB9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SyncObject.h"

#define MAX_GLOBAL_OPEN_PATH    128

class GlobalFileList  
{
public:
    GlobalFileList();
    virtual ~GlobalFileList();

    void AddToList(LPCTSTR lpszFilePathName);
    TString GetAt(int nIndex);
    int GetOpenPathCount();
    void SetSearchPathDeep(int nDeep) { m_SrhDeep = nDeep; };
    void ClearPathList();

protected:
    BOOL IsFilePathName(LPCTSTR lpszFilePathName);
    BOOL CalcPath(LPCTSTR lpszFilePathName, TString& strSrhPath);
    BOOL IsFilePathExist(LPCTSTR lpszFilePath);

protected:
    int        m_SrhDeep;
    CSleepEx  *m_pFileEvent;
    vector<TString> m_arOpenPath;
};

#endif // !defined(AFX_GLOBALFILELIST_H__A1CC8668_CDC4_4A2E_9BDE_B995FC8EB9F1__INCLUDED_)
