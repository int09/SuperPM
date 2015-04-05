# Microsoft Developer Studio Project File - Name="SuperProcessManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SuperProcessManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SuperProcessManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SuperProcessManager.mak" CFG="SuperProcessManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SuperProcessManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SuperProcessManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SuperProcessManager - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "Common" /I "ProcessTool" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../Bin/SuperProcessManager.exe"

!ELSEIF  "$(CFG)" == "SuperProcessManager - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "Common" /I "ProcessTool" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../Bin/SuperProcessManager.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SuperProcessManager - Win32 Release"
# Name "SuperProcessManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CMListCtrlFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorNetworkIoSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessFileIODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessNetworkIODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessRegIODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PublicTextBarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RunlogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RunlogDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManager.rc
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManagerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManagerView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CMListCtrlFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MonitorFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\MonitorNetworkIoSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessFileIODlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessManager.h
# End Source File
# Begin Source File

SOURCE=.\ProcessNetworkIODlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessRegIODlg.h
# End Source File
# Begin Source File

SOURCE=..\Public\Public.h
# End Source File
# Begin Source File

SOURCE=.\PublicTextBarDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManager.h
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManagerDoc.h
# End Source File
# Begin Source File

SOURCE=.\SuperProcessManagerView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AbBg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\SuperProcessManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\SuperProcessManager.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SuperProcessManagerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Common\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Common.h
# End Source File
# Begin Source File

SOURCE=.\Common\DataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\DataManager.h
# End Source File
# Begin Source File

SOURCE=.\Common\FileVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\FileVersion.h
# End Source File
# Begin Source File

SOURCE=.\Common\HookManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\HookManager.h
# End Source File
# Begin Source File

SOURCE=.\Common\MSortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\MSortListCtrl.h
# End Source File
# End Group
# Begin Group "ProcessTool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ProcessTool\HandleTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\HandleTool.h
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\ModuleToo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\ModuleToo.h
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\PerformanceTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\PerformanceTool.h
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\ProcessTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\ProcessTool.h
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\ThreadTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\ThreadTool.h
# End Source File
# Begin Source File

SOURCE=.\ProcessTool\WDK.h
# End Source File
# End Group
# Begin Group "UICommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UICommon\CreditStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\UICommon\CreditStatic.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
