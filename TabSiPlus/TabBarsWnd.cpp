// TabBarsWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TabSiPlus.h"
#include "TabBarsWnd.h"
#include "SIFrameWnd.h"
#include "SiMDIWnd.h"
#include "SiWindow.h"
#include "SourceInsight.h"
#include "WndUIResource.h"

#include "AboutDialog.h"
#include "OptionSheet.h"
#include "Shlobj.h"

#include "PIDL.h"

#include <exception>

#define IDM_SHELLCTXFIRST      1
#define IDM_SHELLCTXLAST       29999

#define CATCH_EXCEPTIONS

#ifdef _DEBUG
#define new DEBUG_NEW
//#undef CATCH_EXCEPTIONS
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLBAR_SIZE_SMALL          40
#define TOOLBAR_SIZE_LARGE          56

#define TABBAR_WND_BORDER    1
#define TABBAR_TOOLBAR_PAD    2
// flags for popup menus
#define TPM_FLAGS   (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD)

LPCTSTR lpFileTypes[] = {_T(""), _T(".c"),_T(".cpp"),_T(".cs"),_T(".*"),_T(".h"),_T(".inf"),_T(".ini"),_T(".txt"),_T(".vb"),_T(".java")};
const int nFileType = 11;

int IndexFromFileType(LPCTSTR lpszExtName)
{
    int nIdx = 4;//default type
    for(int i = 0; i < nFileType; i++)
    {
        if(lstrcmpi(lpFileTypes[i],lpszExtName) == 0)
        {
            nIdx = i;
            break;
        }
    }

    return nIdx;
}

/*
const int nFixToolBtn = 9;
static TBBUTTON Toolbar[nFixToolBtn] = 
{
    { 0,        0,                            TBSTATE_ENABLED,        TBSTYLE_SEP,            0,    0,    NULL,        -1 },
    { 0,        ID_PLUS_CFG,        TBSTATE_ENABLED,        TBSTYLE_BUTTON|TBSTYLE_DROPDOWN ,        0,    0,    NULL,        -1 },
    { 1,        ID_PLUS_BARSH,        TBSTATE_ENABLED,        TBSTYLE_BUTTON,        0,    0,    NULL,        -1 },
    { 2,        ID_CONFIGURATION,        TBSTATE_ENABLED,        TBSTYLE_BUTTON,        0,    0,    NULL,        -1 },
    { 3,        ID_PLUS_MAN,        TBSTATE_ENABLED,        TBSTYLE_BUTTON,        0,    0,    NULL,        -1 },
    { 4,        ID_HELP,    TBSTATE_ENABLED,        TBSTYLE_BUTTON,        0,    0,    NULL,        -1 },
    { 5,        ID_APP_ABOUT,        TBSTATE_ENABLED,        TBSTYLE_BUTTON,        0,    0,    NULL,        -1 },
    { 0,        0,                            TBSTATE_ENABLED,        TBSTYLE_SEP,            0,    0,    NULL,        -1 },
    { 6,        ID_APP_EXIT,        TBSTATE_ENABLED,        TBSTYLE_BUTTON,        0,    0,    NULL,        -1 },
};
*/
const int nFixToolBtn = 1;
static TBBUTTON Toolbar[nFixToolBtn] = 
{
    { 0,        IDC_TOOLBAR_HEADFLIP,        TBSTATE_ENABLED,        TBSTYLE_BUTTON|TBSTYLE_DROPDOWN ,        0,    0,    NULL,        -1 }
};

/////////////////////////////////////////////////////////////////////////////
// CTabBarsWnd

CTabBarsWnd::CTabBarsWnd()
{
    m_nCurRtbTab = -1;
    m_TabImages = NULL;
    m_ToolbarImages = NULL;
    m_ToolbarImagesS = NULL;
    m_hLastActive = NULL;
    m_pTabTheme = NULL;
}

CTabBarsWnd::~CTabBarsWnd()
{
    if(m_pTabTheme != NULL)
    {
        m_pTabTheme->ReleaseConfig();
        delete m_pTabTheme;
    }
}

void CTabBarsWnd::MoveTabCtrl(LPRECT pTabClient)
{
    CRect TabCtrl = *pTabClient;

    DebugTracing(gnDbgLevelHiDebug,_T("CTabBarsWnd::MoveTabCtrl(Client [left = %d,top = %d,right = %d,bottom = %d] )"),
                 pTabClient->left,pTabClient->top,pTabClient->right,pTabClient->bottom);
    TabCtrl.DeflateRect(1, 1);
    m_TabCtrl.MoveWindow(TabCtrl);
}

void CTabBarsWnd::MoveToolbarCtrl(LPRECT pToolbar)
{
    CRect ToolCtrl = *pToolbar;

    DebugTracing(gnDbgLevelHiDebug,_T("CTabBarsWnd::MoveToolbarCtrl(Client [left = %d,top = %d,right = %d,bottom = %d] )"),
                 pToolbar->left,pToolbar->top,pToolbar->right,pToolbar->bottom);
    ToolCtrl.DeflateRect(1, 1);
    
    CToolBarCtrl &tbc = m_wndToolBar.GetToolBarCtrl();
    tbc.SetImageList(CImageList::FromHandle((m_TabCtrl.GetRowCount() >= 2) ? m_ToolbarImages : m_ToolbarImagesS));
    m_wndToolBar.MoveWindow(ToolCtrl);
    m_wndToolBar.ShowWindow(SW_SHOW);
}

int CTabBarsWnd::CalcTabbarWndHeight(LPRECT pMdiPos)
{
    DebugTracing(gnDbgLevelHiDebug,_T("CTabbarWnd::CalcTabbarWndHeight(MdiPos [left = %d, top = %d, right = %d, bottom = %d])"),
                 pMdiPos->left,pMdiPos->top,pMdiPos->right,pMdiPos->bottom);

    CRect rcClient = CRect(0,0,0,0);
    rcClient.right = rcClient.left + (pMdiPos->right - pMdiPos->left);
    rcClient.bottom = rcClient.top + (pMdiPos->bottom - pMdiPos->top);

    CRect rcToolbar, rcTab;
    CalcPosition(rcClient, rcToolbar, rcTab);
    CSize st = m_TabCtrl.ReCalcAllTabsPosAndWndSize(rcTab.Width());
    return st.cy + TABBAR_WND_BORDER * 2;
}

void CTabBarsWnd::MdiClientPosChanging(LPRECT pTabsRect)
{
    DebugTracing(gnDbgLevelHiDebug,_T("CTabbarWnd::MdiClientPosChanging(TabPos [left = %d, top = %d, right = %d, bottom = %d])"),
                 pTabsRect->left,pTabsRect->top,pTabsRect->right,pTabsRect->bottom);
    if(IsPositionChanged(pTabsRect))
    {
        RECT TabsRect;
        ::CopyRect(&TabsRect, pTabsRect);
        GetParent()->ScreenToClient(&TabsRect);
    
        ::SetWindowPos(m_hWnd, HWND_BOTTOM,TabsRect.left,TabsRect.top,TabsRect.right - TabsRect.left,TabsRect.bottom - TabsRect.top, SWP_DRAWFRAME);
        RedrawWindow();
        CRect rcClient,rcToolbar, rcTab;
        GetClientRect(rcClient);
        CalcPosition(rcClient, rcToolbar, rcTab);
        MoveToolbarCtrl(&rcToolbar);
        MoveTabCtrl(&rcTab);
    }
}

BOOL CTabBarsWnd::IsPositionChanged(LPRECT pTabsRect)
{
    RECT r;
    GetWindowRect(&r);

    return !::EqualRect(pTabsRect,&r);
}

void CTabBarsWnd::UpdateTabs(BOOL bForce)
{
    try
    {
        int curTabCount = m_TabCtrl.GetItemCount();
        CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
        ASSERT(pMdiChildMng != NULL);
        CSiMDIWnd *pMDIClientWnd = g_pSiFrameWnd->GetSiMDIWnd();
        ASSERT(pMDIClientWnd != NULL);

        int mdiMgmtcount = pMdiChildMng->GetMdiChildCount();
        if((curTabCount != mdiMgmtcount) || bForce)
        {
            m_TabCtrl.SetRedraw(FALSE);
            m_TabCtrl.DeleteAllItems();
            for(int i = 0; i < mdiMgmtcount; ++i)
            {
                ChildObject *pmdiObj = pMdiChildMng->GetMdiChild(i);
                ASSERT(pmdiObj != NULL);
                int nItem = InternalAddTabItem(pmdiObj);
                if((pmdiObj->hWnd != NULL) && (pmdiObj->hWnd == pMDIClientWnd->MDIGetActive()))
                {
                    m_TabCtrl.SetCurSel(nItem);
                }
            }
            m_TabCtrl.SetRedraw(TRUE);
            /*
            m_TabCtrl.UpdateWindow();
            */
        }
    }
    catch(...)
    {
    }
}

LONG CTabBarsWnd::FindTabItemId(HWND hWnd)
{
    for(int groupIdx = 0; groupIdx < m_TabCtrl.GetGroupCount(); groupIdx++)
    {
        for(int subIdx = 0; subIdx < m_TabCtrl.GetSubItemCount(groupIdx); subIdx++)
        {
            ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemDataByIndex(groupIdx, subIdx);
            ASSERT(pObject != NULL);
            if(pObject->hWnd == hWnd)
            {
                return m_TabCtrl.GetItemIdByIndex(groupIdx, subIdx);
            }
        }
    }

    return -1;
}

void CTabBarsWnd::ActivateTabItem(HWND hWnd)
{
    LONG nItemId = FindTabItemId(hWnd);
    if(nItemId != -1)
    {
        m_TabCtrl.SetCurSel(nItemId);
    }
}

int CTabBarsWnd::InternalAddTabItem(ChildObject *pObj)
{
    ASSERT(pObj);

    CString strExtName = GetFileExtName(pObj->strFileName);

    int nIconIdx = IndexFromFileType(strExtName);
    CString strItemText = MakeTabItemText(pObj->strFileName, pObj->bReadonly, pObj->bModify);
    LONG nItemId = m_TabCtrl.GetCurSel();
    if(cfg_iAddOrder == aoBack)
    {
        nItemId++;
        return m_TabCtrl.InsertItem(nItemId, strItemText, nIconIdx, (LPARAM)pObj);
    }
    else
    {
        return m_TabCtrl.InsertItem(nItemId, strItemText, nIconIdx, (LPARAM)pObj);
    }
}

int CTabBarsWnd::AddTabItem(HWND hWnd)
{
    CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
    ChildObject *pObj = pMdiChildMng->LookupMdiChild(hWnd);
    ASSERT(pObj != NULL);

    int nItem = InternalAddTabItem(pObj);

    return nItem;
}

void CTabBarsWnd::ChangeTabItemText(HWND hWnd, LPCTSTR lpszTitle)
{
    for(int groupIdx = 0; groupIdx < m_TabCtrl.GetGroupCount(); groupIdx++)
    {
        for(int subIdx = 0; subIdx < m_TabCtrl.GetSubItemCount(groupIdx); subIdx++)
        {
            ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemDataByIndex(groupIdx, subIdx);
            ASSERT(pObject != NULL);
            if(pObject->hWnd == hWnd)
            {
                m_TabCtrl.SetItemTextByIndex(groupIdx, subIdx, lpszTitle);
            }
        }
    }
}

void CTabBarsWnd::OnSiWindowTitleChanged(HWND hWnd, LPCTSTR lpszTitle)
{
    ChangeTabItemText(hWnd, lpszTitle);
}

void CTabBarsWnd::OnSiWindowActivated(HWND hActived, HWND hDeactived)
{
    CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();

    m_hLastActive = hDeactived;
    ChildObject *pObj = pMdiChildMng->LookupMdiChild(hActived);
    if(pObj)
    {
        DebugTracing(gnDbgLevelHiDebug,_T("CTabbarWnd::OnSiWindowActivated(hActived = %x, hDeactived = %x)"),
                     hActived,hDeactived);
        ActivateTabItem(hActived);
    }
}

void CTabBarsWnd::OnMdiWindowActivated(HWND hActived)
{
    CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
    pMdiChildMng->AddMdiChildWnd(hActived);

    int curTabCount = m_TabCtrl.GetItemCount();
    int mdiMgmtcount = pMdiChildMng->GetMdiChildCount();
    if(curTabCount != mdiMgmtcount)
    {
        UpdateTabs(TRUE);
    }

    DebugTracing(gnDbgLevelHiDebug,_T("CTabbarWnd::OnMdiWindowActivated(hActived=%x)"),hActived);
    ActivateTabItem(hActived);
}

void CTabBarsWnd::OnWindowClosing(HWND hWnd)
{
    CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
    LONG nItemId = FindTabItemId(hWnd);
    if(nItemId != -1)
    {
        m_TabCtrl.DeleteItem(nItemId);
        pMdiChildMng->RemoveMdiChildWnd(hWnd);
    }
    else
    {
        //MdiChildMngó?m_TabCtrl?é?üò??-2??￥??á?
        pMdiChildMng->RemoveMdiChildWnd(hWnd);
        UpdateTabs(TRUE);
    }
}

void CTabBarsWnd::OnWindowOpened(HWND hWnd)
{
    CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
    ASSERT(pMdiChildMng);

    ChildObject *pObj = pMdiChildMng->LookupMdiChild(hWnd);
    if(pObj != NULL)
    {
        AddTabItem(hWnd);
        DebugTracing(gnDbgLevelHiDebug,_T("CTabBarWnd::OnWindowOpened(hWnd = %x), SetActive it!"), hWnd);
        pObj->pSiWnd->SetActive(TRUE);
    }

    int curTabCount = m_TabCtrl.GetItemCount();
    int mdiMgmtcount = pMdiChildMng->GetMdiChildCount();
    if(curTabCount != mdiMgmtcount)
    {
        UpdateTabs(TRUE);
    }
}

void CTabBarsWnd::CreateTabWndCtrl()
{
    DebugTracing(gnDbgLevelHiDebug,_T("CTabBarWnd::CreateTabWndCtrl()"));

    ASSERT(m_TabImages != NULL);

    SetRedraw(FALSE);

    m_pTabTheme = new CWzTabGraidentTheme();
    if(m_pTabTheme == NULL)
    {
        AfxMessageBox(_T("Failed to create color theme object!"), MB_OK | MB_ICONSTOP);
        return;
    }

    TCHAR szThemeFile[MAX_PATH] = {0};
    GetAppFilePathName(g_szThemeName,szThemeFile,MAX_PATH);
    if(!m_pTabTheme->InitConfig(szThemeFile))
    {
        AfxMessageBox(_T("Fail to load color theme file, invalid file format!"));
        return;
    }
    
    m_pTabTheme->SetImageList(&m_TabImgList);
    
    m_TabCtrl.SetTabTheme(m_pTabTheme, FALSE);
    m_TabCtrl.SetGroupSameName(cfg_bGroupSameTable);
    m_TabCtrl.SetShowItemIcon(cfg_bShowIcons);
    m_TabCtrl.SetShowCloseBtn(cfg_bShowCloseBtn);
    m_TabCtrl.SetNotifyWnd(this);
    if(!m_TabCtrl.CreateTabCtrl(this))
    {
        AfxMessageBox(_T("Failed to create tab control!"), MB_OK | MB_ICONSTOP);
    }

    if(!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT|TBSTYLE_TRANSPARENT,WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC))
    {
        TRACE("Failed to create toolbar\n");
        return;      // fail to create
    }

    CToolBarCtrl &tbc = m_wndToolBar.GetToolBarCtrl();
    tbc.SetImageList(CImageList::FromHandle(m_ToolbarImages));
    tbc.SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);
    tbc.AddButtons(nFixToolBtn,Toolbar);

    tbc.SetImageList(CImageList::FromHandle((m_TabCtrl.GetRowCount() >= 2) ? m_ToolbarImages : m_ToolbarImagesS));

    SetRedraw(TRUE);
}

void CTabBarsWnd::CalcPosition(const CRect& rcClient, CRect& rcToolbarPos, CRect& rcTabPos)
{
    rcToolbarPos = rcClient;
    rcToolbarPos.right = (m_TabCtrl.GetRowCount() >= 2) ? TOOLBAR_SIZE_LARGE : TOOLBAR_SIZE_SMALL;

    rcTabPos = rcClient;
    rcTabPos.left = rcToolbarPos.right + TABBAR_TOOLBAR_PAD;
}

BEGIN_MESSAGE_MAP(CTabBarsWnd, CWnd)
    //{{AFX_MSG_MAP(CTabBarsWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
    ON_COMMAND(IDC_TOOLBAR_HEADFLIP,OnToolbarHeadFlip)
    ON_COMMAND(IDC_MAIN_HEADFLIP,OnMainHeadFlip)
    ON_COMMAND(IDC_MAIN_OPTION,OnMainOption)
    ON_COMMAND(IDC_MAIN_HELP,OnMainHelp)
    ON_COMMAND(IDC_MAIN_SUPPORT,OnMainSupport)
    ON_COMMAND(IDC_MAIN_SHUTDOWN,OnMainShutdown)
    ON_COMMAND(IDC_MAIN_ABOUT,OnMainAbout)
    ON_COMMAND(IDC_SELECT,OnRbtSelect)
    ON_COMMAND(IDC_CLOSE,OnRbtClose)
    ON_COMMAND(IDC_CLOSEALLEXCEPT,OnRbtCloseAllExcept)
    ON_COMMAND(IDC_CLOSEALL,OnRbtCloseAll)
    ON_COMMAND(IDC_HEADERFLIP,OnRbtHeaderFlip)
    ON_COMMAND(IDC_SETREADONLY, OnSetreadonly)
    ON_COMMAND(IDC_PROPERITY, OnProperity)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
    ON_NOTIFY(TTN_NEEDTEXTA,  0,  OnToolTipTabCtrl)
    ON_NOTIFY(TTN_NEEDTEXTW,  0,  OnToolTipTabCtrlW)
    ON_MESSAGE(WM_OPSHEET2TABBAR_POS_CHANGE,OnOptionTabWndPosChange)
    ON_MESSAGE(WM_OPSHEET2TABBAR_THEME_CHANGE,OnOptionThemeChange)
    ON_MESSAGE(WM_OPSHEET2TABBAR_GROUP_SAME,OnOptionGroupSame)
    ON_MESSAGE(WM_OPSHEET2TABBAR_SHOW_ICON,OnOptionShowIcon)
    ON_MESSAGE(WM_OPSHEET2TABBAR_SHOW_CLOSE,OnOptionShowClose)
    ON_MESSAGE(WZTAB_EVENT_NOTIFY, OnWzTabEventNotify)
//    ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabBarsWnd message handlers
int CTabBarsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;
  
    m_TabImages = ImageList_Create(16,16,ILC_COLOR32|ILC_MASK,0,2);
    HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetResourceHandle(),(LPCTSTR)IDB_FILETYPE,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
    ImageList_AddMasked(m_TabImages,hBmp,RGB(212,208,200));
    ::DeleteObject(hBmp);

    m_TabImgList.Attach(m_TabImages);

    m_ToolbarImages = ImageList_LoadImage(AfxGetResourceHandle(),(LPCTSTR)IDB_TOOLBAR,32,1,RGB(255,255,255),
        IMAGE_BITMAP,LR_CREATEDIBSECTION);
    m_ToolbarImagesS = ImageList_LoadImage(AfxGetResourceHandle(),(LPCTSTR)IDB_TOOLBARS,16,1,RGB(255,255,255),
        IMAGE_BITMAP,LR_CREATEDIBSECTION);

    CreateTabWndCtrl();
    UpdateTabs(TRUE);
    
    return 0;
}

void CTabBarsWnd::OnToolbarDropDown(NMHDR* pnmhdr, LRESULT *plr)
{
    NMTOOLBAR *pnmtb = (NMTOOLBAR *)pnmhdr;
    // Switch on button command id's.
    switch (pnmtb->iItem)
    {
    case IDC_TOOLBAR_HEADFLIP:
    {
        CMenu          menu;
        CMenu *        pPopup;

        if(menu.LoadMenu(IDR_MENU_MAINPLUS))
        {
            pPopup = menu.GetSubMenu(0);

            RECT rc;
            m_wndToolBar.SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
            m_wndToolBar.ClientToScreen(&rc);
            pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,rc.left, rc.bottom, this, &rc);
            PostMessage(NULL,0,0);
            menu.DestroyMenu();
        }
        break;
    }
    default:
        return;
    }
}


// The tool tip handler displays the full path of a document.
// All open documents are scanned, looking for a the respective window's
// document.  Once the document is found, the full path is extracted.
// This function demonstrates enumeration of both documents and windows.
//##ModelId=431C34C800EA
void CTabBarsWnd::OnToolTipTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    LPTOOLTIPTEXT   lpttt = (LPTOOLTIPTEXT)pNMHDR;
    static CString  cStr; // safe - there can be only one tip at a time

    *pResult = 0;
    if (lpttt->hdr.idFrom == IDC_TOOLBAR_HEADFLIP)
    {
        cStr.LoadString(IDS_TOOLBAR_TT);
    }

  // variation on original fix by Christian Rodemeyer 
  // (Christian.Rodemeyer@t-online.de): use lpszText instead of szText so 
  // strings over 80 chars will be handled properly.
  lpttt->lpszText = LPTSTR(LPCTSTR(cStr));//it drive orbit faint!
}

void CTabBarsWnd::OnToolTipTabCtrlW(NMHDR* pNMHDR, LRESULT* pResult) 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    static wchar_t sToolTip[300];

    LPTOOLTIPTEXTW lpttt = (LPTOOLTIPTEXTW)pNMHDR;
    TOOLTIPTEXTA   ttta;

    ttta.hdr    = lpttt->hdr;
    ttta.hinst  = lpttt->hinst;
    ttta.lParam = lpttt->lParam;
    ttta.uFlags = lpttt->uFlags;

    OnToolTipTabCtrl((NMHDR *)&ttta, pResult);

    ::MultiByteToWideChar(CP_ACP, 0, ttta.lpszText, -1, sToolTip, 300) ;
    lpttt->lpszText = (LPWSTR)sToolTip;    
}

/////////////////////////////////////////////////////////////////////////////
// CTabBarsWnd commands
void CTabBarsWnd::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
}

// activates and window and restores it's size (i.e. unminimizes it)
// idea and primary implementation: Alexey N. Kirpa (alexeykirpa@mail.ru)
void CTabBarsWnd::ActivateWindow(CSiWindow*& pWindow, BOOL bActive)
{
    ASSERT(pWindow);
    if(pWindow->IsIconic())
    {
        pWindow->ShowWindow(SW_RESTORE);
    }
    pWindow->SetActive(bActive);
}

void CTabBarsWnd::CloseSIWindow(CSiWindow*& pWindow)
{
    ASSERT(pWindow);
    
    HWND hThisWnd = pWindow->GetSafeHwnd();
//    pWindow->SendMessage(WM_CLOSE, 0, 0);//now close it!
    pWindow->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);//换成WM_SYSCOMMAND消息，修补关闭窗口时没有处理已经修改的文件的问题

    CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
    ASSERT(pMdiChildMng);

    ChildObject *pObj = pMdiChildMng->LookupMdiChild(hThisWnd);
    if(pObj == NULL) /*确实已经关闭了*/
    {
        // need to reactivate the previously active window?
        if((m_hLastActive != NULL) && ::IsWindow(m_hLastActive))
        {
            ::PostMessage(::GetParent(m_hLastActive), WM_MDIACTIVATE,(WPARAM)m_hLastActive, 0);
        }
    }
}

BOOL CTabBarsWnd::DestroyWindow() 
{
    return CWnd::DestroyWindow();
}

LRESULT CTabBarsWnd::OnOptionTabWndPosChange(WPARAM wParam, LPARAM lParam)
{
    SnapOrientations oldPos = (SnapOrientations)wParam;
    SnapOrientations newPos = (SnapOrientations)lParam;

    CSiMDIWnd *m_pSiMDIClientWnd = g_pSiFrameWnd->GetSiMDIWnd();
    if(m_pSiMDIClientWnd && ::IsWindow(m_pSiMDIClientWnd->GetSafeHwnd()))
    {
        m_pSiMDIClientWnd->SetManaging(false, oldPos);
        m_pSiMDIClientWnd->SetManaging(true, newPos);
    }

    return 0;
}

LRESULT CTabBarsWnd::OnOptionThemeChange(WPARAM wParam, LPARAM lParam)
{
    TCHAR szThemePath[MAX_PATH] = {0};
    GetAppFilePathName((LPCTSTR)wParam, szThemePath, MAX_PATH);

    CWzTabGraidentTheme *pTabTheme = new CWzTabGraidentTheme();
    if(pTabTheme == NULL)
    {
        AfxMessageBox(_T("Failed to create color theme object!"), MB_OK | MB_ICONSTOP);
        return 0;
    }
    pTabTheme->SetImageList(&m_TabImgList);
    
    if(!pTabTheme->InitConfig(szThemePath))
    {
        AfxMessageBox(_T("Fail to load color theme file, invalid file format!"));
        delete pTabTheme;
        return 0;
    }
    CWzTabGraidentTheme *pOldTheme = (CWzTabGraidentTheme *)m_TabCtrl.SetTabTheme(pTabTheme, TRUE);
    if(pOldTheme != NULL)
    {
        pOldTheme->ReleaseConfig();
        delete pOldTheme;
    }
    m_pTabTheme = pTabTheme;
    
    return 1;
}

LRESULT CTabBarsWnd::OnOptionShowIcon(WPARAM wParam, LPARAM lParam)
{
    if(m_TabCtrl.GetSafeHwnd() != NULL)
    {
        m_TabCtrl.SetShowItemIcon((BOOL)wParam);
    }
    return 0;
}

LRESULT CTabBarsWnd::OnOptionShowClose(WPARAM wParam, LPARAM lParam)
{
    if(m_TabCtrl.GetSafeHwnd() != NULL)
    {
        m_TabCtrl.SetShowCloseBtn((BOOL)wParam);
    }
    return 0;
}
LRESULT CTabBarsWnd::OnOptionGroupSame(WPARAM wParam, LPARAM lParam)
{
    if(m_TabCtrl.GetSafeHwnd() != NULL)
    {
        m_TabCtrl.SetGroupSameName((BOOL)wParam);
    }
    return 0;
}

LRESULT CTabBarsWnd::OnWzTabEventNotify(WPARAM wParam, LPARAM lParam)
{
    switch(wParam)
    {
        case EVENT_ITEM_DELETE:
            OnWaTabItemDelete((WZTAB_EVENT_PARA *)lParam);
            break;
        case EVENT_ITEM_L_DBCLICK:
            OnWaTabItemLDbClick((WZTAB_EVENT_PARA *)lParam);
            break;
        case EVENT_ITEM_R_CLICK:
            OnWaTabItemRClick((WZTAB_EVENT_PARA *)lParam);
            break;
        case EVENT_ROW_COUNT_CHANGE:
            OnWaTabRowCountChange((WZTAB_EVENT_PARA *)lParam);
            break;
        case EVENT_ITEM_STATE_CHANGE:
            OnWaTabItemStateChange((WZTAB_EVENT_PARA *)lParam);
            break;
        default:
            break;
    }

    return 0;
}

void CTabBarsWnd::OnDestroy() 
{
    DebugTracing(gnDbgLevelNormalDebug,_T("CTabBarsWnd::OnDestroy"));

    m_TabCtrl.SetNotifyWnd(NULL);
    m_TabCtrl.DestroyWindow();
/*    
    if(m_pTabTheme != NULL)
    {
        m_pTabTheme->ReleaseConfig();
        delete m_pTabTheme;
    }
*/
    m_TabImgList.Detach();
    ImageList_Destroy(m_TabImages);

    CToolBarCtrl &tbc = m_wndToolBar.GetToolBarCtrl();
    tbc.SetImageList(NULL);
    m_wndToolBar.DestroyWindow();
    
    ImageList_Destroy(m_ToolbarImages);
    ImageList_Destroy(m_ToolbarImagesS);
    
    ::PostQuitMessage(0);
    
    CWnd::OnDestroy();
}

void CTabBarsWnd::OnToolbarHeadFlip()
{
    OnMainHeadFlip();
}

void CTabBarsWnd::OnMainHeadFlip()
{
    TCHAR szSiProgramLocation[MAX_PATH];
    if(!GetSiProgramLocation(szSiProgramLocation,MAX_PATH))
    {
        //提示输入si程序的位置
    }

    if(::IsWindow(m_TabCtrl.GetSafeHwnd()))
    {
        int nCurId = m_TabCtrl.GetCurSel();
        if(nCurId != -1)
        {
            ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(nCurId);
            ASSERT(pObject != NULL);
            if(pObject->bIsFileWindow)
            {
                if(pObject->strFullName.IsEmpty())
                {
                    pObject->strFullName = GetAssocFileName(pObject);
                }
                if(!pObject->strFullName.IsEmpty())
                    FlipFile(pObject->strFullName,szSiProgramLocation);
            }
        }
    }
}

void CTabBarsWnd::OnMainOption()
{
    CString strOptionSheetTitle;
    strOptionSheetTitle.LoadString(IDS_OPTION_SHEET_TITLE);
    COptionSheet dlg(strOptionSheetTitle,this);
    if(dlg.DoModal() == IDOK)
    {

    }
}

void CTabBarsWnd::OnMainHelp()
{
    CString strHtmlFile;
    strHtmlFile.LoadString(IDS_HELP_FILE_NAME);
    TCHAR szHelpFile[MAX_PATH] = {0};
    GetAppFilePathName(strHtmlFile,szHelpFile,MAX_PATH);
    HINSTANCE hInst = CHyperLink::GotoURL(szHelpFile);
    if(32 >= (int)hInst)
    {
        CString strErrorMsg;
        strErrorMsg.LoadString(IDS_FAIL_OPEN_HELP);
        AfxMessageBox(strErrorMsg,MB_OK | MB_ICONSTOP);
    }
}

void CTabBarsWnd::OnMainSupport()
{
    CString strSupportUrl;
    strSupportUrl.LoadString(IDS_TECH_SUPPORT);
    HINSTANCE hInst = CHyperLink::GotoURL(strSupportUrl);
}

void CTabBarsWnd::OnMainShutdown()
{
    g_pSiFrameWnd->ShutDownTabSiPlus();
}

void CTabBarsWnd::OnMainAbout()
{
    CAboutDialog dlg;
    dlg.DoModal();
}

void CTabBarsWnd::OnRbtSelect()
{
    if(m_nCurRtbTab != -1)
    {
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        ASSERT(pObject->pSiWnd);
        ActivateWindow(pObject->pSiWnd,TRUE);
     }
    
    m_nCurRtbTab = -1;
}

void CTabBarsWnd::OnRbtClose()
{
    if(m_nCurRtbTab != -1)
    {
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        ASSERT(pObject->pSiWnd);

        CloseSIWindow(pObject->pSiWnd);
     }
    
    m_nCurRtbTab = -1;
}


void CTabBarsWnd::OnRbtCloseAllExcept()
{
    if(m_nCurRtbTab != -1)
    {
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        ASSERT(pObject->pSiWnd);
        CSiMDIWnd *pSiMDIClientWnd = g_pSiFrameWnd->GetSiMDIWnd();
        if(pSiMDIClientWnd)
        {
            HWND hWndExcept = pObject->pSiWnd->GetSafeHwnd();
            HWND hWnd = pSiMDIClientWnd->GetTopWindow();
            while (hWnd)
            {
                HWND hWndNext = ::GetNextWindow(hWnd, GW_HWNDNEXT);
                if(hWnd != hWndExcept)
                {
                    ::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);//换成WM_SYSCOMMAND消息，修补关闭窗口时没有处理已经修改的文件的问题
                    //::PostMessage(hWnd, WM_CLOSE, 0, 0);
                }
                hWnd = hWndNext;
            }
            ActivateWindow(pObject->pSiWnd,FALSE);
            ActivateWindow(pObject->pSiWnd,TRUE);
        }
     }
    
    m_nCurRtbTab = -1;
}

void CTabBarsWnd::OnRbtCloseAll()
{
    if(m_nCurRtbTab != -1)
    {
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        ASSERT(pObject->pSiWnd);
        CSiMDIWnd *pSiMDIClientWnd = g_pSiFrameWnd->GetSiMDIWnd();
        if(pSiMDIClientWnd)
        {
            HWND hWnd = pSiMDIClientWnd->GetTopWindow();
            while (hWnd)
            {
                HWND hWndNext = ::GetNextWindow(hWnd, GW_HWNDNEXT);
                ::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);//换成WM_SYSCOMMAND消息，修补关闭窗口时没有处理已经修改的文件的问题
                //::PostMessage(hWnd, WM_CLOSE, 0, 0);
                hWnd = hWndNext;
            }
        }
     }
    
    m_nCurRtbTab = -1;
}

void CTabBarsWnd::OnRbtHeaderFlip()
{
    if(m_nCurRtbTab != -1)
    {
        TCHAR szSiProgramLocation[MAX_PATH];
        if(!GetSiProgramLocation(szSiProgramLocation,MAX_PATH))
        {
            //提示输入si程序的位置
        }
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        if(pObject->bIsFileWindow)
        {
            if(pObject->strFullName.IsEmpty())
            {
                pObject->strFullName = GetAssocFileName(pObject);
            }
            if(!pObject->strFullName.IsEmpty())
                FlipFile(pObject->strFullName,szSiProgramLocation);
        }
     }
    
    m_nCurRtbTab = -1;
}

void CTabBarsWnd::OnSetreadonly() 
{
    if(m_nCurRtbTab != -1)
    {
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        if(pObject->bIsFileWindow)
        {
            if(pObject->strFullName.IsEmpty())
            {
                pObject->strFullName = GetAssocFileName(pObject);
            }

            DWORD dwFileAttr = ::GetFileAttributes(pObject->strFullName);
            if(dwFileAttr != 0xFFFFFFFF)//文件存在
            {
                if((dwFileAttr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)//有只读属性，要去掉
                {
                    dwFileAttr &= ~FILE_ATTRIBUTE_READONLY;
                }
                else//没有只读属性，要添加
                {
                    dwFileAttr |= FILE_ATTRIBUTE_READONLY;
                }

                ::SetFileAttributes(pObject->strFullName,dwFileAttr);

                SHChangeNotify(SHCNE_ATTRIBUTES,SHCNF_PATH,pObject->strFullName,NULL);
                
                CSiMDIWnd *pSiMDIClientWnd = g_pSiFrameWnd->GetSiMDIWnd();
                if(pSiMDIClientWnd)
                {
                    UINT uMessage = WM_USER + 19;
                    ::PostMessage(pSiMDIClientWnd->GetSafeHwnd(),uMessage,0,0);
                }
                
                ActivateWindow(pObject->pSiWnd, FALSE);
                ActivateWindow(pObject->pSiWnd, TRUE);
            }
        }
     }
    
    m_nCurRtbTab = -1;
}

void CTabBarsWnd::OnProperity() 
{
    HRESULT        hr;
    CPIDL          pidl;
    LPCONTEXTMENU  lpcm = NULL;
    
    if(m_nCurRtbTab != -1)
    {
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
        ASSERT(pObject != NULL);
        if(pObject->bIsFileWindow)
        {
            if(pObject->strFullName.IsEmpty())
            {
                pObject->strFullName = GetAssocFileName(pObject);
            }
            DWORD dwFileAttr = ::GetFileAttributes(pObject->strFullName);
            if(dwFileAttr != 0xFFFFFFFF)//文件存在
            {
                hr = pidl.Set(pObject->strFullName);
                if(SUCCEEDED(hr))
                {
                    hr = pidl.GetUIObjectOf(IID_IContextMenu, (LPVOID *)&lpcm, m_hWnd);
                    if(SUCCEEDED(hr) && lpcm)
                    {
                        HMENU hMenu = ::CreateMenu();
                        if(hMenu)
                        {
                            hr = lpcm->QueryContextMenu(hMenu, 0, IDM_SHELLCTXFIRST, IDM_SHELLCTXLAST, CMF_NODEFAULT | CMF_EXPLORE);
                            if(SUCCEEDED(hr))
                            {
                                int nCount = ::GetMenuItemCount(hMenu);
                                if(nCount > 0)
                                {
                                    UINT iCmd = GetMenuItemID(hMenu,nCount - 1);
                                    CMINVOKECOMMANDINFO cmi;
                                    cmi.cbSize       = sizeof(cmi);
                                    cmi.fMask        = 0;
                                    cmi.hwnd         = m_hWnd;
                                    cmi.lpVerb       = MAKEINTRESOURCE(iCmd - 1);
                                    cmi.lpParameters = NULL;
                                    cmi.lpDirectory  = NULL;
                                    cmi.nShow        = SW_SHOWNORMAL;
                                    cmi.dwHotKey     = 0;
                                    cmi.hIcon        = NULL;
                                    lpcm->InvokeCommand(&cmi);
                                }
                            }
                            ::DestroyMenu(hMenu);
                        }
                        lpcm->Release();
                    }
                }
            }
        }
    }
}

LRESULT CTabBarsWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch(message)
    {
        case WM_UNINITMENUPOPUP://WM_EXITMENULOOP:
        {
            m_ActiveMenus.OnUninitMenuPopup(wParam, lParam);
            break;
        }
        case WM_INITMENUPOPUP/*WM_INITMENU*/:
        {
            m_ActiveMenus.OnInitMenuPopup(wParam, lParam);
            break;
        }
        case WM_MEASUREITEM:
        {
            int nRtn = m_ActiveMenus.OnMeasureItem(wParam, lParam);
            if(nRtn == 1)
                return 1;
            else
                return CWnd::WindowProc(message, wParam, lParam);
        }
        case WM_DRAWITEM:
        {
            int nRtn = m_ActiveMenus.OnDrawItem(wParam, lParam);
            if(nRtn == 1)
                return 1;
            else
                return CWnd::WindowProc(message, wParam, lParam);
        }
        default:
            break;
    }
    return CWnd::WindowProc(message, wParam, lParam);
}

//
BOOL CTabBarsWnd::FlipFile(LPCTSTR lpszFileName, LPCTSTR lpszSiProgram)
{
    ASSERT(lpszFileName);
    ASSERT(lpszSiProgram);

    TCHAR       cDrive[_MAX_DRIVE], cDir[_MAX_DIR];
    TCHAR       cActFName[_MAX_FNAME], cExt[_MAX_EXT];

    if(_tcslen(lpszFileName) == 0)
        return FALSE;

    _splitpath(lpszFileName, cDrive, cDir, cActFName, cExt);

    CString strExts,cFullName;
    strExts.Format(_T("%s%s"),cDrive,cDir);
    g_strAFlipPath.SetAt(0,strExts);//这个位置总是被替换
    CStringArray    strAExts;
    _tcslwr(cExt);
    if(_tcsstr(g_szHeadType,cExt) != NULL)
    {
        strExts = g_szCPPType;
    }
    else if(_tcsstr(g_szCPPType,cExt) != NULL)
    {
        strExts = g_szHeadType;
    }
    else
    {
        return FALSE;
    }
    
    strAExts.RemoveAll();
    SplitFileTypeString(strExts, strAExts, _T(";"));

    int i = 0;
    BOOL bOpen = FALSE;
    while(i < strAExts.GetSize() && !bOpen)
    {
        CString strFileName = cActFName;
        strFileName += strAExts[i];
        bOpen = MyGetFullPathName(strFileName, cFullName.GetBuffer(MAX_PATH), MAX_PATH);
        cFullName.ReleaseBuffer();
        if(bOpen)
        {
            CMdiChildManagment *pMdiChildMng = g_pSiFrameWnd->GetMdiChildMgmt();
            ASSERT(pMdiChildMng);
            ChildObject *pObj = pMdiChildMng->LookupMdiChildByFileName(cFullName);
            if(pObj)//文件已经打开了？
            {
                ActivateWindow(pObj->pSiWnd,TRUE);
            }
            else
            {
                //bOpen = SIOpenFile(lpszSiProgram,cFullName);/*如果打开失败则继续循环*/
                /*原来是打开失败则继续循环,现在看来没有这个需要*/
                SIOpenFile(lpszSiProgram,cFullName);
            }
        }
        i++;
    }

    strAExts.RemoveAll();

    return bOpen;
}

void CTabBarsWnd::OnWaTabItemDelete(WZTAB_EVENT_PARA *pPara)
{
    int nItemId = pPara->para1;
    
    ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(nItemId);
    ASSERT(pObject != NULL);
    ASSERT(pObject->pSiWnd);
    if(pObject != NULL)
    {
        CloseSIWindow(pObject->pSiWnd);
    }
}

void CTabBarsWnd::OnWaTabItemLDbClick(WZTAB_EVENT_PARA *pPara)
{
    POINT pt;

    pt.x = LOWORD(pPara->para1);
    pt.y = HIWORD(pPara->para1);
    int nItemId = pPara->para2;
    
    ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(nItemId);
    ASSERT(pObject != NULL);
    ASSERT(pObject->pSiWnd);
    if(pObject != NULL)
    {
        CloseSIWindow(pObject->pSiWnd);
    }
}

void CTabBarsWnd::OnWaTabItemRClick(WZTAB_EVENT_PARA *pPara)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CMenu          menu;
    CMenu *        pPopup;
    POINT           point;

    point.x = LOWORD(pPara->para1);
    point.y = HIWORD(pPara->para1);
    m_nCurRtbTab = pPara->para2;
    if(menu.LoadMenu(IDR_MENU_TABRIGHT))
    {
        pPopup = menu.GetSubMenu(0);
        GetCursorPos(&point);
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            //iCmd = pPopup->GetSubMenu(iShellContextPos)->TrackPopupMenu(TPM_FLAGS, point.x, point.y, this);
        }
        else
        {
            if(m_nCurRtbTab != -1)
            {
                ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(m_nCurRtbTab);
                ASSERT(pObject != NULL);
                if(pObject->bIsFileWindow && !pObject->strFullName.IsEmpty())
                {
                    DWORD dwFileAttr = ::GetFileAttributes(pObject->strFullName);
                    if(dwFileAttr != 0xFFFFFFFF)//文件存在
                    {
                        if((dwFileAttr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
                        {
                            pPopup->CheckMenuItem(IDC_SETREADONLY,MF_CHECKED | MF_BYCOMMAND);
                        }
                        else
                        {
                            pPopup->CheckMenuItem(IDC_SETREADONLY,MF_UNCHECKED | MF_BYCOMMAND);
                        }
                    }
                }
            }

            pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, point.x, point.y, this);
            PostMessage(NULL,0,0);
        }

        menu.DestroyMenu();
    }
}

void CTabBarsWnd::OnWaTabRowCountChange(WZTAB_EVENT_PARA *pPara)
{
    int nCx = LOWORD(pPara->para1);
    int nCy = HIWORD(pPara->para1);
    int nRowCount = (int)pPara->para2;

    CSiMDIWnd *pMDIClientWnd = g_pSiFrameWnd->GetSiMDIWnd();
    ASSERT(pMDIClientWnd != NULL);
    if(pMDIClientWnd && ::IsWindow(pMDIClientWnd->GetSafeHwnd()))
    {
        DebugTracing(gnDbgLevelHiDebug,_T("CTabbarWnd::OnWaTabRowCountChange(nRowCount = %d)"),nRowCount);
        pMDIClientWnd->OnTabCtrlRowCountChanged(nCy + TABBAR_WND_BORDER * 2, nRowCount);
    }
}

void CTabBarsWnd::OnWaTabItemStateChange(WZTAB_EVENT_PARA *pPara)
{
    if(pPara->para2 != 0) /*只处理激活事件*/
    {
        int nItem = m_TabCtrl.GetCurSel();
        ASSERT(nItem == pPara->para1);
        ChildObject *pObject = (ChildObject *)m_TabCtrl.GetItemData(nItem);
        ASSERT(pObject != NULL);
        ASSERT(pObject->pSiWnd);
        DebugTracing(gnDbgLevelHiDebug,_T("CTabbarWnd::OnWaTabItemStateChange(nItem = %d)"),nItem);
        ActivateWindow(pObject->pSiWnd, TRUE);
    }
}

BOOL CTabBarsWnd::OnEraseBkgnd(CDC* pDC) 
{
    CWndUIResource *pUIRes = GlobalGetUIRes();
    WZ_ASSERT(pUIRes);

    CRect rcClient, rcToolbar, rcTabctrl;
    GetClientRect(rcClient);
    CalcPosition(rcClient, rcToolbar, rcTabctrl);
    rcToolbar.right += TABBAR_TOOLBAR_PAD + 1;
    if(m_pTabTheme != NULL)
    {
        pDC->FillSolidRect(rcToolbar, m_pTabTheme->GetTabWndBkgndColor());
    }
    else
    {
        pDC->FillSolidRect(rcToolbar, RGB(255,238,194));
    }
    pDC->DrawEdge((LPRECT)&rcClient,BDR_RAISEDINNER,BF_TOPLEFT);
    pDC->DrawEdge((LPRECT)&rcClient,BDR_RAISEDOUTER,BF_BOTTOMRIGHT);

    return TRUE;
    //return CWnd::OnEraseBkgnd(pDC);
}
