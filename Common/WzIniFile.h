// WzIniFile.h: interface for the CWzIniFile class.
/*********************************************************************
*    class:        class to operate ini struct file
*    Author:        orbit
*    Date:        2000-11-21
*    Contact us:    Inte2000@163.com
**********************************************************************/
#if !defined(AFX_WZINIFILE_H__4A8E7C07_902F_11D1_8B70_0000B43382FE__INCLUDED_)
#define AFX_WZINIFILE_H__4A8E7C07_902F_11D1_8B70_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxcoll.h>

class CWzIniFile  
{
public:
    CWzIniFile();
    CWzIniFile(LPCTSTR lpFileName);
    ~CWzIniFile();

    BOOL Open();
    BOOL Save();
    void Close();
    BOOL ReadFrom(LPCTSTR lpFileName);
    BOOL WriteTo(LPCTSTR lpFileName);

    void SetAttachFile(LPCTSTR lpFileName) { m_strFileName = lpFileName;};
    CString GetAttachFile() {return m_strFileName;};
    BOOL IsEmpty();
    int GetLineCount();
    int GetLine(int index ,CString & strLine);

    BOOL RemoveSection(LPCTSTR cSection);
    BOOL RemoveItem(LPCTSTR lpszSection, LPCTSTR lpszItem);
    BOOL RenameSection(LPCTSTR lpszSection, LPCTSTR lpszNewSection);
    BOOL RenameItem(LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpszNewItem);
    int InsertSection(LPCTSTR cSection);
    int AddRemarkLine(LPCTSTR lpszRemarkLine,int nPos = -1);

    void AddLineToSection(LPCTSTR lpszSection,LPCTSTR lpszLine);//add to the last line of Section
    void InsertLineToSection(LPCTSTR lpszSection,LPCTSTR lpszLine);//insert to the first line of Section
    int  GetSectionItemCount(LPCTSTR lpszSection);
    CString GetSectionItem(LPCTSTR lpszSection,int index);
    int FindSection(LPCTSTR cSection);

    BOOL SetBoolValue(LPCTSTR cSection, LPCTSTR cItem, const BOOL bVal);
    BOOL SetValue(LPCTSTR cSection, LPCTSTR cItem, LPCTSTR cVal);
    BOOL SetValue(LPCTSTR cSection, LPCTSTR cItem, const double dbVal);
    BOOL SetValue(LPCTSTR cSection, LPCTSTR cItem, const int iVal);
    BOOL SetValue(LPCTSTR cSection, LPCTSTR cItem, const COLORREF crVal);
    BOOL SetValue(LPCTSTR cSection, LPCTSTR cItem, const CRect &rcVal);
    BOOL SetValue(LPCTSTR cSection, LPCTSTR cItem, const CPoint &ptVal);

    BOOL GetBoolValue(LPCTSTR cSection, LPCTSTR cItem, BOOL &bVal, const BOOL bDefault = FALSE);
    BOOL GetValue(LPCTSTR cSection, LPCTSTR cItem, CString &cVal, LPCTSTR cDefault = NULL);
    BOOL GetValue(LPCTSTR cSection, LPCTSTR cItem, double &dbVal, const double dbDefault = 0.0);
    BOOL GetValue(LPCTSTR cSection, LPCTSTR cItem, int &iVal, const int iDefault = 0);
    BOOL GetValue(LPCTSTR cSection, LPCTSTR cItem, COLORREF &crVal, const COLORREF crDefault = RGB(255,255,255));
    BOOL GetValue(LPCTSTR cSection, LPCTSTR cItem, CRect &rcVal);
    BOOL GetValue(LPCTSTR cSection, LPCTSTR cItem, CPoint &ptVal);
protected:
    int FindItem(const int iSection, LPCTSTR cItem, CString &csVal);
    BOOL IsSection(const int iSection);
    int GetEndOfSection(int nSectionIndex);

private:
    CStringArray    m_csList;
    CString m_strFileName;
};

#endif // !defined(AFX_WZINIFILE_H__4A8E7C07_902F_11D1_8B70_0000B43382FE__INCLUDED_)
