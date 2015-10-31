#if !defined(AFX_TABWNDUITHREAD_H__6C890B7A_F068_4A5E_914E_096379B55B5C__INCLUDED_)
#define AFX_TABWNDUITHREAD_H__6C890B7A_F068_4A5E_914E_096379B55B5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabWndUIThread.h : header file
//


class CTabWndUIThread;

extern CTabWndUIThread *g_pTabWndUIThread;

/////////////////////////////////////////////////////////////////////////////
// CTabWndUIThread thread

class CTabWndUIThread : public CWinThread
{
    DECLARE_DYNCREATE(CTabWndUIThread)

public:
    CTabWndUIThread();           // protected constructor used by dynamic creation
    virtual ~CTabWndUIThread();

protected:
//    CTabWndUIThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabWndUIThread)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();
    //}}AFX_VIRTUAL

// Implementation
protected:
//    virtual ~CTabWndUIThread();

    // Generated message map functions
    //{{AFX_MSG(CTabWndUIThread)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABWNDUITHREAD_H__6C890B7A_F068_4A5E_914E_096379B55B5C__INCLUDED_)
