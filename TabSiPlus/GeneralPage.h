/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/
#if !defined(AFX_GENERALPAGE_H__942F4AA7_975F_41FE_9F04_7D197BD8E20C__INCLUDED_)
#define AFX_GENERALPAGE_H__942F4AA7_975F_41FE_9F04_7D197BD8E20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPage.h : header file
//
#include "ColourPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
    CGeneralPage();
    ~CGeneralPage();

// Dialog Data
    //{{AFX_DATA(CGeneralPage)
    enum { IDD = IDD_PROPPAGE_GENERAL };
    CColourPicker    m_MenuTextHi;
    CColourPicker    m_MenuText;
    CColourPicker    m_MenuLeftbar;
    CColourPicker    m_MenuFrame;
    CColourPicker    m_MenuBkgndHi;
    CColourPicker    m_MenuBkGnd;
    BOOL    m_bAutoRun;
    BOOL    m_bLoadOnceOnly;
    BOOL    m_bShowSplash;
    BOOL    m_bSmartFlip;
    CString    m_strSiLocation;
    //}}AFX_DATA

    COLORREF m_crMenuTextHi;
    COLORREF m_crMenuText;
    COLORREF m_crMenuLeftbar;
    COLORREF m_crMenuFrame;
    COLORREF m_crMenuBkgndHi;
    COLORREF m_crMenuBkgnd;

// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CGeneralPage)
    public:
    virtual BOOL OnApply();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CGeneralPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnChkAutorun();
    afx_msg void OnChkShoesplash();
    afx_msg void OnChkSmartflip();
    afx_msg void OnChkOnlyonce();
    //}}AFX_MSG
    afx_msg LONG OnColorPickerSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerSelChange(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerCloseUp(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerDropDown(UINT lParam, LONG wParam);
DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__942F4AA7_975F_41FE_9F04_7D197BD8E20C__INCLUDED_)
