# Microsoft Developer Studio Project File - Name="TabSiHost" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TabSiHost - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TabSiHost.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TabSiHost.mak" CFG="TabSiHost - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TabSiHost - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TabSiHost - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "TabSiHost - Win32 Release Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "TabSiHost - Win32 Debug Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TabSiHost - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Release\TabSiPlus"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\Common" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "TABSIHOST_EXE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /nologo /entry:"MyCRTStartup" /subsystem:windows /machine:I386 /libpath:"..\Release" /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "TabSiHost - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\Debug\TabSiHost"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\Common" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "TABSIHOST_EXE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"MyCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\Debug" /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TabSiHost - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TabSiHost___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "TabSiHost___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseU"
# PROP Intermediate_Dir "..\ReleaseU\TabSiHost"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /I "..\Common" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\Common" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "TABSIHOST_EXE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TabSiHost.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"MyCRTStartup" /subsystem:windows /machine:I386 /out:"../bin/TabSiHost.exe" /libpath:"..\TaskManagerExDll\Release" /OPT:NOWIN98
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 /nologo /entry:"MyCRTStartup" /subsystem:windows /machine:I386 /libpath:"..\ReleaseU" /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "TabSiHost - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TabSiHost___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "TabSiHost___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugU"
# PROP Intermediate_Dir "..\DebugU\TabSiHost"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\TaskManagerExDll" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\Common" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "TABSIHOST_EXE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TabSiHost.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"MyCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../bin/Debug/TabSiHost.exe" /pdbtype:sept /libpath:"..\TaskManagerExDll\Debug" /OPT:NOWIN98
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /entry:"MyCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\DebugU" /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TabSiHost - Win32 Release"
# Name "TabSiHost - Win32 Debug"
# Name "TabSiHost - Win32 Release Unicode"
# Name "TabSiHost - Win32 Debug Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\crt.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabSiHost.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\SplashWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\TabSiHost.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Group "Other"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\module.h
# End Source File
# Begin Source File

SOURCE=..\Common\product.h
# End Source File
# Begin Source File

SOURCE=..\res\TabSiHost.ico
# End Source File
# Begin Source File

SOURCE=..\Common\VerInfo.ver
# End Source File
# End Group
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\TabSiHost.rc
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter "*.cpp,*.h"
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

SOURCE=..\Common\Options.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Options.h
# End Source File
# Begin Source File

SOURCE=..\Common\SourceInsight.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SourceInsight.h
# End Source File
# Begin Source File

SOURCE=..\Common\TabSiPlusDllExport.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TabSiHost.ico
# End Source File
# End Target
# End Project
