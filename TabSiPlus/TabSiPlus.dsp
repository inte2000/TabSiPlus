# Microsoft Developer Studio Project File - Name="TabSiPlus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TabSiPlus - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TabSiPlus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TabSiPlus.mak" CFG="TabSiPlus - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TabSiPlus - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TabSiPlus - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TabSiPlus - Win32 Debug Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TabSiPlus - Win32 Release Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TabSiPlus - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Release\TabSiPlus"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Common" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TABSIPLUSDLL_EXPORTS" /D "TABSIPLUS_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x417 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Shell32.lib Version.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\Release"

!ELSEIF  "$(CFG)" == "TabSiPlus - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\Debug\TabSiPlus"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Common" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TABSIPLUSDLL_EXPORTS" /D "TABSIPLUS_DLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x417 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Shell32.lib Version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\Debug"

!ELSEIF  "$(CFG)" == "TabSiPlus - Win32 Debug Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug Unicode"
# PROP BASE Intermediate_Dir "Debug Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugU"
# PROP Intermediate_Dir "..\DebugU\TabSiPlus"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Common" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TABSIPLUSDLL_EXPORTS" /D "TABSIPLUS_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x417 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x417 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "TabSiPlus - Win32 Release Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release Unicode"
# PROP BASE Intermediate_Dir "Release Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseU"
# PROP Intermediate_Dir "..\ReleaseU\TabSiPlus"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\Common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Common" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TABSIPLUSDLL_EXPORTS" /D "TABSIPLUS_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x417 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x417 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ENDIF 

# Begin Target

# Name "TabSiPlus - Win32 Release"
# Name "TabSiPlus - Win32 Debug"
# Name "TabSiPlus - Win32 Debug Unicode"
# Name "TabSiPlus - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AddDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChechInFilesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckOutFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EnTabDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\FlipPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiChildManagment.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\PIDL.CPP
# End Source File
# Begin Source File

SOURCE=.\SccPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectFolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SiMDIWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SiWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubclassWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TabBarPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TabBarsWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCheckListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\tabctrlex.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSiPlus.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSiPlus.def
# End Source File
# Begin Source File

SOURCE=.\TabSiPlus.rc
# End Source File
# Begin Source File

SOURCE=.\TabWndUIThread.cpp
# End Source File
# Begin Source File

SOURCE=.\WndHelpFunc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\AddDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChechInFilesDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheckOutFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\EnTabDrawer.h
# End Source File
# Begin Source File

SOURCE=.\FlipPage.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\GlobalFileList.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\MdiChildManagment.h
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.h
# End Source File
# Begin Source File

SOURCE=..\Common\PIDL.H
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SccPage.h
# End Source File
# Begin Source File

SOURCE=.\SelectFolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\SIFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\SiMDIWnd.h
# End Source File
# Begin Source File

SOURCE=.\SiWindow.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubclassWnd.h
# End Source File
# Begin Source File

SOURCE=.\TabBarPage.h
# End Source File
# Begin Source File

SOURCE=.\TabBarsWnd.h
# End Source File
# Begin Source File

SOURCE=.\TabCheckListBox.h
# End Source File
# Begin Source File

SOURCE=.\tabctrlex.h
# End Source File
# Begin Source File

SOURCE=.\TabDrawHelper.h
# End Source File
# Begin Source File

SOURCE=.\TabSiPlus.h
# End Source File
# Begin Source File

SOURCE=.\TabWndUIThread.h
# End Source File
# Begin Source File

SOURCE=.\WndHelpFunc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\196.ico
# End Source File
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileType.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Handcur.cur
# End Source File
# Begin Source File

SOURCE=.\res\tabsihos.ico
# End Source File
# Begin Source File

SOURCE=.\res\TabSiHost.ico
# End Source File
# Begin Source File

SOURCE=.\res\TabSiPlus.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarS.bmp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter "*.h;*.cpp"
# Begin Source File

SOURCE=..\Common\ApiHook.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ApiHook.h
# End Source File
# Begin Source File

SOURCE=..\Common\Blowfish.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Blowfish.h
# End Source File
# Begin Source File

SOURCE=..\Common\CmdIconMap.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\CmdIconMap.h
# End Source File
# Begin Source File

SOURCE=..\Common\CmdIconResource.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\CmdIconResource.h
# End Source File
# Begin Source File

SOURCE=..\Common\Debug.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Debug.h
# End Source File
# Begin Source File

SOURCE=..\Common\HelperFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\HelperFunction.h
# End Source File
# Begin Source File

SOURCE=..\Common\IconMapItem.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\IconMapItem.h
# End Source File
# Begin Source File

SOURCE=..\Common\LoadDll.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LoadDll.h
# End Source File
# Begin Source File

SOURCE=..\Common\Localization.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Localization.h
# End Source File
# Begin Source File

SOURCE=..\Common\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LogFile.h
# End Source File
# Begin Source File

SOURCE=..\Common\MenuHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\MenuHelper.h
# End Source File
# Begin Source File

SOURCE=..\Common\ModuleInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ModuleInstance.h
# End Source File
# Begin Source File

SOURCE=..\Common\Options.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Options.h
# End Source File
# Begin Source File

SOURCE=..\Common\OwnerDrawMenuMgmt.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\OwnerDrawMenuMgmt.h
# End Source File
# Begin Source File

SOURCE=..\Common\ProtectFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ProtectFunc.h
# End Source File
# Begin Source File

SOURCE=..\Common\scc.h
# End Source File
# Begin Source File

SOURCE=..\Common\SccDllFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SccDllFunc.h
# End Source File
# Begin Source File

SOURCE=..\Common\SCCProviderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SCCProviderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Common\SCCProviderInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SCCProviderInfo.h
# End Source File
# Begin Source File

SOURCE=..\Common\SCCProviderMgmt.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SCCProviderMgmt.h
# End Source File
# Begin Source File

SOURCE=..\Common\SourceInsight.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SourceInsight.h
# End Source File
# Begin Source File

SOURCE=..\Common\SyncObject.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SyncObject.h
# End Source File
# Begin Source File

SOURCE=..\Common\SysUtils.h
# End Source File
# Begin Source File

SOURCE=..\Common\UIResourceName.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\UIResourceName.h
# End Source File
# Begin Source File

SOURCE=..\Common\WndUIResource.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\WndUIResource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
