// WzIniFile.cpp: implementation of the CWzIniFile class.
/*********************************************************************
*    class:        class to operate ini struct file
*    Author:        orbit
*    Date:        2000-11-21
*    Contact us:    Inte2000@163.com
**********************************************************************/
#include "stdafx.h"
#include "WzIniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWzIniFile::CWzIniFile()
{
    m_strFileName = "";
}

CWzIniFile::CWzIniFile(LPCTSTR lpFileName)
{
    m_strFileName = lpFileName;
}

CWzIniFile::~CWzIniFile()
{
    Close();
}

BOOL CWzIniFile::Open()
{
    if(m_strFileName.IsEmpty())
        return FALSE;

    return ReadFrom(m_strFileName);
}

BOOL CWzIniFile::Save()
{
    if(m_strFileName.IsEmpty())
        return FALSE;

    return WriteTo(m_strFileName);
}

void CWzIniFile::Close()
{
    m_csList.RemoveAll();
}

BOOL CWzIniFile::ReadFrom(LPCTSTR lpFileName)
{
    Close();
    char szbuf[512];
    CStdioFile file;
    if(!file.Open(lpFileName, CFile::modeRead|CFile::shareDenyNone))
       return FALSE;
    while(file.ReadString(szbuf,511) != NULL)
    {
        CString cs(szbuf);
        cs = cs.Left(cs.GetLength() - 1);
        m_csList.Add(cs);
    }
    file.Close();
    
    return TRUE;
}

BOOL CWzIniFile::WriteTo(LPCTSTR lpFileName)
{
    CStdioFile file;
    if(!file.Open(lpFileName, CFile::modeWrite|CFile::modeCreate))
       return FALSE;
    int max = m_csList.GetSize();
    for(int i = 0; i < max; i++)
    {
        file.WriteString(m_csList.GetAt(i) + "\n");
    }
    file.Close();

    return TRUE;
}

int CWzIniFile::AddRemarkLine(LPCTSTR lpszRemarkLine,int nPos)
{
    CString strTmp;
    strTmp.Format(";;%s",lpszRemarkLine);
    if(nPos == -1)
        return m_csList.Add(strTmp);
    else
    {
        m_csList.InsertAt(nPos,strTmp);
        return nPos + 1;
    }
}

BOOL CWzIniFile::IsEmpty()
{
    return m_csList.GetSize() > 0 ? FALSE : TRUE;
}

int CWzIniFile::GetLineCount()
{
    return m_csList.GetSize();
}

int CWzIniFile::GetLine(int index ,CString & strLine)
{
    ASSERT((index >= 0) && index < m_csList.GetSize());
    strLine = m_csList[index];
    return index;
}
// **********************************************************************************
int CWzIniFile::FindSection(LPCTSTR cSection)
{
    int t, max = m_csList.GetSize();
    CString csSection;
    csSection.Format("[%s]", cSection);

    for (t = 0; t < max; t++)
        if (m_csList.GetAt(t) == csSection) 
            return t;

    return -1;
}

int CWzIniFile::InsertSection(LPCTSTR cSection)
{
    ASSERT(cSection);
    if (!cSection) return -1;

    int idx = FindSection(cSection);
    if (idx < 0)
    {
        CString csSection;
        csSection.Format("[%s]", cSection);
        int line = GetLineCount();
        if(line > 0)//find the last line of current record
        {//the record not empty
            CString strLine;
            GetLine(line - 1 ,strLine);//get last line
            if(!strLine.IsEmpty())//last line not empty
                m_csList.Add("");//add a empty line between two section
        }
        idx = m_csList.Add(csSection);
    }
    return idx;
}

int CWzIniFile::FindItem(const int iSection, LPCTSTR cItem, CString &csVal)
{
    ASSERT(iSection >= 0);
    ASSERT(cItem);

    int max = m_csList.GetSize(), t;
    CString csItem(cItem), csLook;
    csItem += "=";
    int iLen = csItem.GetLength();

    for (t = iSection; t < max; t++)
    {
        if (!IsSection(t))
        {
            csLook = m_csList.GetAt(t);
            if (csLook.GetLength() >= iLen)
            {
                if (csLook.Left(iLen) == csItem) 
                {
                    if (csLook.GetLength() == iLen) 
                        csVal = "";
                    else 
                        csVal = csLook.Right(csLook.GetLength() - iLen);
                    return t;
                }
            }
        }
        else 
            return -1;
    }
    return -1;
}

BOOL CWzIniFile::IsSection(const int iSection)
{
    ASSERT(iSection >= 0 && iSection < m_csList.GetSize());
    if (iSection >= 0 && iSection < m_csList.GetSize())
    {
        CString csItem = m_csList.GetAt(iSection);
        if (csItem.GetLength() > 2 && csItem.Left(1) == '[' && csItem.Right(1) == ']') 
            return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::RemoveSection(LPCTSTR cSection)
{
    int idx = FindSection(cSection);
    if (idx >= 0)
    {
        for (;;)
        {
            m_csList.RemoveAt(idx);
            if (idx >= m_csList.GetSize()) 
                return TRUE;
            if (IsSection(idx)) 
                return TRUE;
        }
    }
    return TRUE;
}

int CWzIniFile::GetEndOfSection(int nSectionIndex)
{
    int max = m_csList.GetSize();
    CString strTmp;
    int nEndOfSection = max;
    for (int t = nSectionIndex + 1; t < max; t++)
    {
        strTmp = m_csList.GetAt(t);
        int size = strTmp.GetLength();
        if(size <= 0)
        {
            nEndOfSection = t;
            break;
        }

        if ((size > 0) && IsSection(t))
        {
            nEndOfSection = t;
            break;
        }
    }

    return nEndOfSection;
}
// **********************************************************************************

BOOL CWzIniFile::SetBoolValue(LPCTSTR cSection, LPCTSTR cItem, const BOOL bVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=%s", cItem, bVal ? "TRUE" : "FALSE");
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else
        {
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
//            m_csList.InsertAt(idx+1, csVal);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::SetValue(LPCTSTR cSection, LPCTSTR cItem, LPCTSTR cVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=%s", cItem, cVal);
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else 
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
        return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::SetValue(LPCTSTR cSection, LPCTSTR cItem, const double dbVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=%f", cItem, dbVal);
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else 
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
        return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::SetValue(LPCTSTR cSection, LPCTSTR cItem, const int iVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=%d", cItem, iVal);
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else 
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
        return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::SetValue(LPCTSTR cSection, LPCTSTR cItem, const COLORREF crVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=RGB(%d,%d,%d)", cItem, GetRValue((DWORD)crVal),GetGValue((DWORD)crVal),GetBValue((DWORD)crVal));
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else 
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
        return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::SetValue(LPCTSTR cSection, LPCTSTR cItem, const CRect &rcVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=RECT(%d,%d,%d,%d)", cItem, rcVal.left, rcVal.top, rcVal.right, rcVal.bottom);
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else 
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
        return TRUE;
    }
    return FALSE;
}

BOOL CWzIniFile::SetValue(LPCTSTR cSection, LPCTSTR cItem, const CPoint &ptVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        int iIdx = FindItem(idx+1, cItem, csVal);
        csVal.Format("%s=POINT(%d,%d)", cItem, ptVal.x, ptVal.y);
        if (iIdx >= 0) 
            m_csList.SetAt(iIdx, csVal);
        else 
            m_csList.InsertAt(GetEndOfSection(idx), csVal);
        return TRUE;
    }
    return FALSE;
}

// **********************************************************************************

BOOL CWzIniFile::GetBoolValue(LPCTSTR cSection, LPCTSTR cItem, BOOL &bVal, const BOOL bDefault)
{
    int idx = FindSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        if (FindItem(idx+1, cItem, csVal) > 0)
        {
            if (csVal.Find("TRUE") >= 0) 
                bVal = TRUE; 
            else 
                bVal = FALSE;
            return TRUE;
        }
    }
    bVal = bDefault;
    return FALSE;
}

BOOL CWzIniFile::GetValue(LPCTSTR cSection, LPCTSTR cItem, CString &cVal,LPCTSTR cDefault)
{
    int idx = FindSection(cSection);
    if (idx >= 0)
    {
        if (FindItem(idx+1, cItem, cVal) > 0)
            return TRUE;
    }
    cVal = cDefault == NULL ? "" : cDefault;
    return FALSE;
}

BOOL CWzIniFile::GetValue(LPCTSTR cSection, LPCTSTR cItem, double &dbVal,const double dbDefault)
{
    int idx = FindSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        if (FindItem(idx+1, cItem, csVal) > 0)
        {
            dbVal = (double) atof(csVal);
            return TRUE;
        }
    }
    dbVal = dbDefault;
    return FALSE;
}

BOOL CWzIniFile::GetValue(LPCTSTR cSection, LPCTSTR cItem, int &iVal, const int iDefault)
{
    int idx = FindSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        if (FindItem(idx+1, cItem, csVal) > 0)
        {
            iVal = (int) atoi(csVal);
            return TRUE;
        }
    }
    iVal = iDefault;
    return FALSE;
}

BOOL CWzIniFile::GetValue(LPCTSTR cSection, LPCTSTR cItem, COLORREF &crVal, const COLORREF crDefault)
{
    int idx = FindSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        if (FindItem(idx+1, cItem, csVal) > 0)
        {
//            TCHAR * pt = csVal.LockBuffer();
            int pf;
            int r,g,b;
            pf = sscanf(csVal,"RGB(%d,%d,%d)", &r, &g, &b);
//            csVal.UnlockBuffer();
            if(pf == 3)
            {
                crVal = RGB(r,g,b);
                return TRUE;
            }
        }
    }
    crVal = crDefault;
    return FALSE;
}

BOOL CWzIniFile::GetValue(LPCTSTR cSection, LPCTSTR cItem, CRect &rcVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        if (FindItem(idx+1, cItem, csVal) > 0)
        {
//            TCHAR * pt = csVal.LockBuffer();
            int pf, t = 0, l = 0, r = 0, b = 0;
            pf = sscanf(csVal,"RECT(%d,%d,%d,%d)", &l, &t, &r, &b);
//            csVal.UnlockBuffer();
            if(pf == 4)
            {
                rcVal.SetRect(l, t, r, b);
                return TRUE;
            }
        }
    }
    rcVal.SetRect(0,0,0,0);
    return FALSE;
}

BOOL CWzIniFile::GetValue(LPCTSTR cSection, LPCTSTR cItem, CPoint &ptVal)
{
    int idx = InsertSection(cSection);
    if (idx >= 0)
    {
        CString csVal;
        if (FindItem(idx+1, cItem, csVal) > 0)
        {
//            char * pt = csVal.LockBuffer();
            int pf, x = 0, y = 0;
            pf = sscanf(csVal,"POINT(%d,%d)", &x, &y);
//            csVal.UnlockBuffer();
            if(pf == 2)
            {
                ptVal.x = x;
                ptVal.y = y;
                return TRUE;
            }
        }
    }
    ptVal = CPoint(0,0);
    return FALSE;
}

/*********************************************************************************************************/
void CWzIniFile::AddLineToSection(LPCTSTR lpszSection,LPCTSTR lpszLine)
{
    int nSection = InsertSection(lpszSection);
    int count = GetSectionItemCount(lpszSection);
    m_csList.InsertAt(nSection + count + 1,lpszLine,1);
}

void CWzIniFile::InsertLineToSection(LPCTSTR lpszSection,LPCTSTR lpszLine)
{
    int nSection = InsertSection(lpszSection);
    m_csList.InsertAt(nSection + 1,lpszLine,1);
}

int CWzIniFile::GetSectionItemCount(LPCTSTR lpszSection)
{
    int nSection = FindSection(lpszSection);
    if(nSection < 0)
        return -1;

    int max = m_csList.GetSize();
    if((max - 1) == nSection)
        return 0;

    int count = 0;
    for(int i = nSection + 1; i < max; i++)
    {
        if(IsSection(i))
            break;
        else
            count++;
    }

    return count;
}

CString CWzIniFile::GetSectionItem(LPCTSTR lpszSection,int index)
{
    int nSection = FindSection(lpszSection);
    if(nSection < 0)
        return _T("");

    return m_csList[nSection + index + 1];
}


BOOL CWzIniFile::RemoveItem(LPCTSTR lpszSection, LPCTSTR lpszItem)
{
    BOOL bRtn = FALSE;
    int iSection = FindSection(lpszSection);
    if (iSection >= 0)
    {
        CString csVal;
        int idxItem = FindItem(iSection+1,lpszItem,csVal);
        if(idxItem != -1)
        {
            m_csList.RemoveAt(idxItem);
            bRtn = TRUE;
        }
    }

    return bRtn;
}

BOOL CWzIniFile::RenameSection(LPCTSTR lpszSection, LPCTSTR lpszNewSection)
{
    BOOL bRtn = FALSE;
    int iSection = FindSection(lpszSection);
    if (iSection >= 0)
    {
        CString csNewSection;
        csNewSection.Format("[%s]",lpszNewSection);
        m_csList.SetAt(iSection,csNewSection);
        bRtn = TRUE;
    }

    return bRtn;
}

BOOL CWzIniFile::RenameItem(LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpszNewItem)
{
    BOOL bRtn = FALSE;
    int iSection = FindSection(lpszSection);
    if (iSection >= 0)
    {
        CString csVal;
        int idxItem = FindItem(iSection+1,lpszItem,csVal);
        if(idxItem != -1)
        {
            CString newString;
            newString.Format("%s=%s",lpszNewItem,csVal);
            m_csList.SetAt(idxItem,newString);
            bRtn = TRUE;
        }
    }

    return bRtn;
}
