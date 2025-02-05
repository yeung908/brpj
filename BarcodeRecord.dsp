# Microsoft Developer Studio Project File - Name="BarcodeRecord" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BarcodeRecord - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BarcodeRecord.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BarcodeRecord.mak" CFG="BarcodeRecord - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BarcodeRecord - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BarcodeRecord - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BarcodeRecord - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "BarcodeRecord - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "HTTP_ONLY" /D "BUILDING_LIBCURL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Ws2_32.lib winmm.lib wldap32.lib libcurld_imp.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BarcodeRecord - Win32 Release"
# Name "BarcodeRecord - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BarcodeRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\BarcodeRecord.rc
# End Source File
# Begin Source File

SOURCE=.\BarcodeRecordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BSWndContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGNewDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGPlaywin.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGProductInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGVideoList.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGWarnning.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenPannel.cpp
# End Source File
# Begin Source File

SOURCE=.\SignalDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLiteOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\YaanCamera.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BarcodeRecord.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeRecordDlg.h
# End Source File
# Begin Source File

SOURCE=.\BSWndContainer.h
# End Source File
# Begin Source File

SOURCE=.\DLGControl.h
# End Source File
# Begin Source File

SOURCE=.\DLGNewDevice.h
# End Source File
# Begin Source File

SOURCE=.\DLGPlaywin.h
# End Source File
# Begin Source File

SOURCE=.\DLGProductInfo.h
# End Source File
# Begin Source File

SOURCE=.\DLGSettings.h
# End Source File
# Begin Source File

SOURCE=.\DLGVideoList.h
# End Source File
# Begin Source File

SOURCE=.\DLGWarnning.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\PlayWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenPannel.h
# End Source File
# Begin Source File

SOURCE=.\SignalDownload.h
# End Source File
# Begin Source File

SOURCE=.\SQLiteOperate.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\YaanCamera.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BarcodeRecord.ico
# End Source File
# Begin Source File

SOURCE=.\res\BarcodeRecord.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
