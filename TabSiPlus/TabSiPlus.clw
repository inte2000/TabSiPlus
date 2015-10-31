; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTabBarPage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tabsiplus.h"
LastPage=0

ClassCount=16
Class1=CAboutDialog
Class2=CBaseTabCtrl
Class3=CColourPicker
Class4=CColourPopup
Class5=CTabCtrlEx
Class6=CFlipPage
Class7=CGeneralPage
Class8=CHyperLink
Class9=COptionSheet
Class10=CSccPage
Class11=CSelectFolderDlg
Class12=CTabBarPage
Class13=CTabBarsWnd
Class14=CTabCheckListBox
Class15=CTabSiPlusApp
Class16=CTabWndUIThread

ResourceCount=23
Resource1=IDD_PROPPAGE_FLIP (English (U.S.))
Resource2=IDD_CHECKIN_DLG
Resource3=IDD_PROPPAGE_GENERAL
Resource4=IDD_CHECKOUT_DLG
Resource5=IDR_MENU_TABRIGHT (English (U.S.))
Resource6=IDD_CHECKOUT_DLG (English (U.S.))
Resource7=IDD_PROPPAGE_TABBAR (English (U.S.))
Resource8=IDD_PROPPAGE_TABBAR
Resource9=IDR_MENU_MAINPLUS
Resource10=IDD_PROPPAGE_FLIP
Resource11=IDD_ADD_DLG
Resource12=IDD_SELECT_FOLDER_DLG
Resource13=IDD_SCCTEST_DIALOG (English (U.S.))
Resource14=IDD_DLG_ABOUT
Resource15=IDD_PROPPAGE_SSC (English (U.S.))
Resource16=IDD_DLG_ABOUT (English (U.S.))
Resource17=IDR_MENU_MAINPLUS (English (U.S.))
Resource18=IDD_PROPPAGE_SSC
Resource19=IDD_PROPPAGE_GENERAL (English (U.S.))
Resource20=IDD_CHECKIN_DLG (English (U.S.))
Resource21=IDR_MENU_TABRIGHT
Resource22=IDD_SELECT_FOLDER_DLG (English (U.S.))
Resource23=IDD_ADD_DLG (English (U.S.))

[CLS:CAboutDialog]
Type=0
BaseClass=CDialog
HeaderFile=AboutDialog.h
ImplementationFile=AboutDialog.cpp
LastObject=CAboutDialog

[CLS:CBaseTabCtrl]
Type=0
BaseClass=CTabCtrl
HeaderFile=BaseTabCtrl.h
ImplementationFile=BaseTabCtrl.cpp

[CLS:CColourPicker]
Type=0
BaseClass=CButton
HeaderFile=ColourPicker.h
ImplementationFile=ColourPicker.cpp

[CLS:CColourPopup]
Type=0
BaseClass=CWnd
HeaderFile=ColourPopup.h
ImplementationFile=ColourPopup.cpp

[CLS:CTabCtrlEx]
Type=0
BaseClass=CTabCtrl
HeaderFile=EnTabCtrl.h
ImplementationFile=tabctrlex.cpp

[CLS:CFlipPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=FlipPage.h
ImplementationFile=FlipPage.cpp

[CLS:CGeneralPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=GeneralPage.h
ImplementationFile=GeneralPage.cpp

[CLS:CHyperLink]
Type=0
BaseClass=CStatic
HeaderFile=HyperLink.h
ImplementationFile=HyperLink.cpp

[CLS:COptionSheet]
Type=0
BaseClass=CPropertySheet
HeaderFile=OptionSheet.h
ImplementationFile=OptionSheet.cpp

[CLS:CSccPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=SccPage.h
ImplementationFile=SccPage.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_EDIT_SCC_PRJNAME

[CLS:CSelectFolderDlg]
Type=0
BaseClass=CDialog
HeaderFile=SelectFolderDlg.h
ImplementationFile=SelectFolderDlg.cpp

[CLS:CTabBarPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=TabBarPage.h
ImplementationFile=TabBarPage.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_CHK_SMART_LOC

[CLS:CTabBarsWnd]
Type=0
BaseClass=CWnd
HeaderFile=TabBarsWnd.h
ImplementationFile=TabBarsWnd.cpp
LastObject=IDC_SCC_GET
Filter=W
VirtualFilter=WC

[CLS:CTabCheckListBox]
Type=0
BaseClass=CCheckListBox
HeaderFile=TabCheckListBox.h
ImplementationFile=TabCheckListBox.cpp

[CLS:CTabSiPlusApp]
Type=0
BaseClass=CWinApp
HeaderFile=TabSiPlus.h
ImplementationFile=TabSiPlus.cpp

[CLS:CTabWndUIThread]
Type=0
BaseClass=CWinThread
HeaderFile=TabWndUIThread.h
ImplementationFile=TabWndUIThread.cpp

[DLG:IDD_DLG_ABOUT]
Type=1
Class=CAboutDialog
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_STC_TOPBANNER,static,1342181390
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342177296
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STC_ICON,static,1342177283
Control7=IDC_STATIC,static,1342308353
Control8=IDC_STATIC,static,1342308353
Control9=IDC_STC_HOMEPAGE,static,1342308353

[DLG:IDD_PROPPAGE_FLIP]
Type=1
Class=CFlipPage
ControlCount=14
Control1=IDC_STATIC,static,1342308352
Control2=IDC_BTN_ADDFLPPATH,button,1342242816
Control3=IDC_BTN_DELFLPPATH,button,1342242816
Control4=IDC_BTN_UPFLPPATH,button,1342242816
Control5=IDC_BTN_DOWNFLPPATH,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_HEADFILE,edit,1342242944
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_CPPFILE,edit,1342242944
Control11=IDC_LIST_PATH,listbox,1345388881
Control12=IDC_STATIC,static,1342312449
Control13=IDC_STATIC,static,1342312449
Control14=IDC_EDIT_FULLSRHPATH,edit,1342244992

[DLG:IDD_PROPPAGE_GENERAL]
Type=1
Class=CGeneralPage
ControlCount=13
Control1=IDC_CHK_AUTORUN,button,1342242819
Control2=IDC_CHK_SHOESPLASH,button,1342242819
Control3=IDC_CHK_SMARTFLIP,button,1342242819
Control4=IDC_CHK_ONLYONCE,button,1342242819
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_SILOCATION,edit,1350631552
Control7=IDC_STATIC,button,1342177287
Control8=IDC_COLORPICKER_MENUTEXT,button,1342242816
Control9=IDC_COLORPICKER_MENUBKGND,button,1342242816
Control10=IDC_COLORPICKER_MENULEFT,button,1342242816
Control11=IDC_COLORPICKER_MENUTEXTHI,button,1342242816
Control12=IDC_COLORPICKER_MENUBKGNDHI,button,1342242816
Control13=IDC_COLORPICKER_MENUFRAME,button,1342242816

[DLG:IDD_PROPPAGE_SSC]
Type=1
Class=CSccPage
ControlCount=15
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CBO_PROVIDER,combobox,1344339971
Control3=IDC_BTN_PROVIDER_ADV,button,1342242816
Control4=IDC_STATIC,button,1342177287
Control5=IDC_CHK_USE_SYS_SEC,button,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_SCC_USER,edit,1342242944
Control9=IDC_EDIT_SCC_PASSWORD,edit,1342242944
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_PRJ_PATH,edit,1342242944
Control12=IDC_BTN_BOWRSE,button,1342242816
Control13=IDC_EDIT_SCC_PRJNAME,edit,1342242944
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342177283

[DLG:IDD_SELECT_FOLDER_DLG]
Type=1
Class=CSelectFolderDlg
ControlCount=5
Control1=IDC_EDIT_FOLDER,edit,1342242944
Control2=IDC_BTN_BOWRSE,button,1342242816
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_CHK_INCLUDE_SUBFOLDER,button,1476460547

[DLG:IDD_PROPPAGE_TABBAR]
Type=1
Class=CTabBarPage
ControlCount=17
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHK_SHOWICON,button,1342242819
Control3=IDC_CHK_SHELLMENU,button,1342242819
Control4=IDC_STATIC,button,1342177287
Control5=IDC_RAD_ADDF,button,1342308361
Control6=IDC_RAD_ADDB,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RAD_TOP,button,1342308361
Control9=IDC_RAD_BOTTOM,button,1342177289
Control10=IDC_CHK_MULTIROWTAB,button,1342242819
Control11=IDC_CHK_FBTAB,button,1342242819
Control12=IDC_CHK_CUSTCOLOR,button,1342242819
Control13=IDC_BTN_TABTEXT_COLOR,button,1342242816
Control14=IDC_BTN_TABBKGND_COLOR,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_CHK_SMART_LOC,button,1342242819
Control17=IDC_EDIT_SRH_DEEP,edit,1342251136

[MNU:IDR_MENU_MAINPLUS]
Type=1
Class=?
Command1=IDC_MAIN_HEADFLIP
Command2=IDC_MAIN_OPTION
Command3=IDC_MAIN_HELP
Command4=IDC_MAIN_SUPPORT
Command5=IDC_MAIN_SHUTDOWN
Command6=IDC_MAIN_ABOUT
CommandCount=6

[MNU:IDR_MENU_TABRIGHT]
Type=1
Class=?
Command1=IDC_CLOSE
Command2=IDC_CLOSEALL
Command3=IDC_CLOSEALLEXCEPT
Command4=IDC_HEADERFLIP
Command5=IDC_SELECT
Command6=IDC_SETREADONLY
Command7=IDC_SCC_CHECKIN
Command8=IDC_SCC_CHECKOUT
Command9=IDC_SCC_ADD
Command10=IDC_SCC_REMOVE
Command11=IDC_SCC_UNCHECKOUT
Command12=IDC_SCC_DIFF
Command13=IDC_SCC_HISTORY
Command14=IDC_SCC_PROPERTIES
Command15=IDC_SCC_GET
Command16=IDC_SCC_RUNSCC
Command17=IDC_PROPERITY
CommandCount=17

[MNU:IDR_MENU_TABRIGHT (English (U.S.))]
Type=1
Class=?
Command1=IDC_CLOSE
Command2=IDC_CLOSEALL
Command3=IDC_CLOSEALLEXCEPT
Command4=IDC_HEADERFLIP
Command5=IDC_SELECT
Command6=IDC_SETREADONLY
Command7=IDC_SCC_CHECKIN
Command8=IDC_SCC_CHECKOUT
Command9=IDC_SCC_ADD
Command10=IDC_SCC_REMOVE
Command11=IDC_SCC_UNCHECKOUT
Command12=IDC_SCC_DIFF
Command13=IDC_SCC_HISTORY
Command14=IDC_SCC_PROPERTIES
Command15=IDC_SCC_GET
Command16=IDC_SCC_RUNSCC
Command17=IDC_PROPERITY
CommandCount=17

[MNU:IDR_MENU_MAINPLUS (English (U.S.))]
Type=1
Class=?
Command1=IDC_MAIN_HEADFLIP
Command2=IDC_MAIN_OPTION
Command3=IDC_MAIN_HELP
Command4=IDC_MAIN_SUPPORT
Command5=IDC_MAIN_SHUTDOWN
Command6=IDC_MAIN_ABOUT
CommandCount=6

[DLG:IDD_DLG_ABOUT (English (U.S.))]
Type=1
Class=CAboutDialog
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_STC_TOPBANNER,static,1342181390
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342177296
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STC_ICON,static,1342177283
Control7=IDC_STATIC,static,1342308353
Control8=IDC_STATIC,static,1342308353
Control9=IDC_STC_HOMEPAGE,static,1342308353

[DLG:IDD_PROPPAGE_FLIP (English (U.S.))]
Type=1
Class=CFlipPage
ControlCount=14
Control1=IDC_STATIC,static,1342308352
Control2=IDC_BTN_ADDFLPPATH,button,1342242816
Control3=IDC_BTN_DELFLPPATH,button,1342242816
Control4=IDC_BTN_UPFLPPATH,button,1342242816
Control5=IDC_BTN_DOWNFLPPATH,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_HEADFILE,edit,1342242944
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_CPPFILE,edit,1342242944
Control11=IDC_LIST_PATH,listbox,1345388881
Control12=IDC_STATIC,static,1342312449
Control13=IDC_STATIC,static,1342312449
Control14=IDC_EDIT_FULLSRHPATH,edit,1342244992

[DLG:IDD_PROPPAGE_TABBAR (English (U.S.))]
Type=1
Class=CTabBarPage
ControlCount=17
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHK_SHOWICON,button,1342242819
Control3=IDC_CHK_SHELLMENU,button,1342242819
Control4=IDC_STATIC,button,1342177287
Control5=IDC_RAD_ADDF,button,1342308361
Control6=IDC_RAD_ADDB,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RAD_TOP,button,1342308361
Control9=IDC_RAD_BOTTOM,button,1342177289
Control10=IDC_CHK_MULTIROWTAB,button,1342242819
Control11=IDC_CHK_FBTAB,button,1342242819
Control12=IDC_CHK_CUSTCOLOR,button,1342242819
Control13=IDC_BTN_TABTEXT_COLOR,button,1342242816
Control14=IDC_BTN_TABBKGND_COLOR,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_CHK_SMART_LOC,button,1342242819
Control17=IDC_EDIT_SRH_DEEP,edit,1342251136

[DLG:IDD_PROPPAGE_GENERAL (English (U.S.))]
Type=1
Class=CGeneralPage
ControlCount=13
Control1=IDC_CHK_AUTORUN,button,1342242819
Control2=IDC_CHK_SHOESPLASH,button,1342242819
Control3=IDC_CHK_SMARTFLIP,button,1476460547
Control4=IDC_CHK_ONLYONCE,button,1342242819
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_SILOCATION,edit,1350631552
Control7=IDC_STATIC,button,1342177287
Control8=IDC_COLORPICKER_MENUTEXT,button,1342242816
Control9=IDC_COLORPICKER_MENUBKGND,button,1342242816
Control10=IDC_COLORPICKER_MENULEFT,button,1342242816
Control11=IDC_COLORPICKER_MENUTEXTHI,button,1342242816
Control12=IDC_COLORPICKER_MENUBKGNDHI,button,1342242816
Control13=IDC_COLORPICKER_MENUFRAME,button,1342242816

[DLG:IDD_SELECT_FOLDER_DLG (English (U.S.))]
Type=1
Class=CSelectFolderDlg
ControlCount=5
Control1=IDC_EDIT_FOLDER,edit,1342242944
Control2=IDC_BTN_BOWRSE,button,1342242816
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_CHK_INCLUDE_SUBFOLDER,button,1476460547

[DLG:IDD_PROPPAGE_SSC (English (U.S.))]
Type=1
Class=CSccPage
ControlCount=15
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CBO_PROVIDER,combobox,1344339971
Control3=IDC_BTN_PROVIDER_ADV,button,1342242816
Control4=IDC_STATIC,button,1342177287
Control5=IDC_CHK_USE_SYS_SEC,button,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_SCC_USER,edit,1342242944
Control9=IDC_EDIT_SCC_PASSWORD,edit,1342242944
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_PRJ_PATH,edit,1342242944
Control12=IDC_BTN_BOWRSE,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_SCC_PRJNAME,edit,1342242944
Control15=IDC_STATIC,static,1342177283

[DLG:IDD_ADD_DLG]
Type=1
Class=?
ControlCount=9
Control1=IDC_EDIT_COMMENT,edit,1342242948
Control2=IDC_BTN_ADV,button,1342242816
Control3=IDC_CHK_ADD_CHKOUT,button,1342242819
Control4=IDC_CHK_TEXT,button,1342242819
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STC_FILE_NAME,edit,1342244992

[DLG:IDD_CHECKIN_DLG]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_COMMENT,edit,1342242948
Control6=IDC_BTN_ADV,button,1342242816
Control7=IDC_CHK_KEEP,button,1342242819
Control8=IDC_STC_FILE_NAME,edit,1342244992

[DLG:IDD_CHECKOUT_DLG]
Type=1
Class=?
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_COMMENT,edit,1476460676
Control6=IDC_BTN_ADV,button,1342242816
Control7=IDC_STC_FILE_NAME,edit,1342244992

[DLG:IDD_SCCTEST_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_FILE,edit,1350631552
Control4=IDC_BTN_CHECK_IN,button,1342242816
Control5=IDC_BTN_ADD,button,1342242816
Control6=IDC_BTN_REMOVE,button,1342242816
Control7=IDC_BTN_CHECK_OUT,button,1342242816
Control8=IDC_BTN_INIT,button,1342242816
Control9=IDC_BTN_UNINIT,button,1342242816
Control10=IDC_BTN_RUNSCC,button,1342242816
Control11=IDC_BTN_HISTORY,button,1342242816
Control12=IDC_BTN_DIFF,button,1342242816
Control13=IDC_BTN_UNCHECK_OUT,button,1342242816
Control14=IDC_BTN_GET,button,1342242816
Control15=IDC_BTN_PROPERTIS,button,1342242816

[DLG:IDD_CHECKOUT_DLG (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_COMMENT,edit,1476460676
Control6=IDC_BTN_ADV,button,1342242816
Control7=IDC_STC_FILE_NAME,edit,1342244992

[DLG:IDD_CHECKIN_DLG (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_COMMENT,edit,1342242948
Control6=IDC_BTN_ADV,button,1342242816
Control7=IDC_CHK_KEEP,button,1342242819
Control8=IDC_STC_FILE_NAME,edit,1342244992

[DLG:IDD_ADD_DLG (English (U.S.))]
Type=1
Class=?
ControlCount=9
Control1=IDC_EDIT_COMMENT,edit,1342242948
Control2=IDC_BTN_ADV,button,1342242816
Control3=IDC_CHK_ADD_CHKOUT,button,1342242819
Control4=IDC_CHK_TEXT,button,1342242819
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STC_FILE_NAME,edit,1342244992

