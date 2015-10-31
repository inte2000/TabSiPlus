// HyperLink.cpp : implementation file

#include "stdafx.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink()
{
    m_hLinkCursor       = NULL;                 // No cursor as yet
    m_crLinkColour      = RGB(0,0,238);   // Blue
    m_crHoverColour     = RGB(196,128,64);
    m_bOverControl      = false;                // Cursor not yet over control
    m_strURL.Empty();
}

CHyperLink::~CHyperLink()
{
    if(m_hLinkCursor)
        ::DestroyCursor(m_hLinkCursor);
    m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
    //{{AFX_MSG_MAP(CHyperLink)
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLink message handlers
void CHyperLink::OnClicked()
{
    int result = (int)GotoURL(m_strURL, SW_SHOW);
    if(result < HINSTANCE_ERROR) 
        ::MessageBeep(MB_ICONEXCLAMATION);
}

void CHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
  CStatic::OnMouseMove(nFlags, point);
  if(m_bOverControl)
  {
    CRect rect;
    GetClientRect(rect);
    if(!rect.PtInRect(point))
    {
      m_bOverControl = false;
      ReleaseCapture();
      RedrawWindow();
      return;
    }
  }
  else
  {
    m_bOverControl = true;
    RedrawWindow();
    SetCapture();
  }
}

BOOL CHyperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

void CHyperLink::PreSubclassWindow() 
{
  // We want to get mouse clicks via STN_CLICKED
  DWORD dwStyle = GetStyle();
  ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);
  
  // Set the URL as the window text
  if(m_strURL.IsEmpty())
      GetWindowText(m_strURL);

  // Create the font
  LOGFONT lf;
  GetFont()->GetLogFont(&lf);
  lf.lfUnderline = 1;
  m_Font.CreateFontIndirect(&lf);
  SetFont(&m_Font);

  CStatic::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink operations

void CHyperLink::SetURL(LPCTSTR lpszURL)
{
    m_strURL = lpszURL;
}

void CHyperLink::SetColours(COLORREF crLinkColour, COLORREF crHoverColour)
{ 
  m_crLinkColour = crLinkColour; 
    crHoverColour = crHoverColour;

  if (::IsWindow(m_hWnd))
      Invalidate(); 
}

void CHyperLink::SetLinkCursor(HINSTANCE hResInst, UINT uResID)
{ 
    m_hLinkCursor = ::LoadCursor(hResInst,(LPCTSTR)uResID);
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink implementation
LONG CHyperLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = ::RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
    if (retval == ERROR_SUCCESS) 
    {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        ::RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        ::RegCloseKey(hkey);
    }

    return retval;
}

HINSTANCE CHyperLink::GotoURL(LPCTSTR url, int showcmd)
{
  TCHAR key[MAX_PATH + MAX_PATH];
  // First try ShellExecute()
  HINSTANCE result = ::ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);
  // If it failed, get the .htm regkey and lookup the program
  if((UINT)result <= HINSTANCE_ERROR) 
    {
    if(GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
        {
      lstrcat(key, _T("\\shell\\open\\command"));
      if(GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
            {
        TCHAR *pos;
        pos = _tcsstr(key, _T("\"%1\""));
        if (pos == NULL) 
                {                     // No quotes found
          pos = strstr(key, _T("%1"));       // Check for %1, without quotes 
          if (pos == NULL)                   // No parameter at all...
              pos = key+lstrlen(key) - 1;
          else
              *pos = '\0';                   // Remove the parameter
        }
        else
          *pos = '\0';                       // Remove the parameter

        lstrcat(pos, _T(" "));
        lstrcat(pos, url);
        result = (HINSTANCE)::WinExec(key,showcmd);
      }
    }
  }

  return result;
}

void CHyperLink::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    
    DWORD dwStyle = GetStyle();
    if((dwStyle & SS_ICON) != SS_ICON)
    {
        RECT rcClient;
        GetClientRect(&rcClient);
        TCHAR szText[256];
        GetWindowText(szText,256);
        HFONT hFont = (HFONT)::SendMessage(m_hWnd,WM_GETFONT,0,0); 
        HFONT Of = (HFONT)SelectObject(dc.m_hDC,hFont);
        if(m_bOverControl)
            SetTextColor(dc.m_hDC,m_crHoverColour);
        else
            SetTextColor(dc.m_hDC,m_crLinkColour);

        SetBkMode(dc.m_hDC,TRANSPARENT);
        UINT uFmt = DT_SINGLELINE|DT_VCENTER;
        if((dwStyle & SS_RIGHT) == SS_RIGHT)
            uFmt |= DT_RIGHT;
        else if((dwStyle & SS_CENTER) == SS_CENTER)
            uFmt |= DT_CENTER;
        else
            uFmt |= DT_LEFT;

        DrawText(dc.m_hDC,szText,-1,&rcClient,uFmt);
    }
    else
        Default();
}
