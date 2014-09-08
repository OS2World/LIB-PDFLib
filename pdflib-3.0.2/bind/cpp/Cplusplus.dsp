# Microsoft Developer Studio Project File - Name="Cplusplus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Cplusplus - Win32 no auxiliary libs
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Cplusplus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cplusplus.mak" CFG="Cplusplus - Win32 no auxiliary libs"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cplusplus - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Cplusplus - Win32 no auxiliary libs" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "Cplusplus - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Cplusplus___Win32_Release"
# PROP BASE Intermediate_Dir "Cplusplus___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../pdflib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../pdflib" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_MT" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:3.02 /subsystem:console /debug /machine:I386 /out:"pdfclock.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zlib.lib libpng.lib libtiff.lib pdflib.lib /nologo /version:3.02 /subsystem:console /pdb:none /machine:I386 /out:"hello.exe" /libpath:"..\..\pdflib" /libpath:"..\..\..\zlib" /libpath:"..\..\..\libpng" /libpath:"..\..\..\libtiff"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Cplusplus - Win32 no auxiliary libs"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Cplusplus___Win32_no_auxiliary_libs"
# PROP BASE Intermediate_Dir "Cplusplus___Win32_no_auxiliary_libs"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Cplusplus___Win32_no_auxiliary_libs"
# PROP Intermediate_Dir "Cplusplus___Win32_no_auxiliary_libs"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../../pdflib" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../pdflib" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_MT" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zlib.lib libpng.lib libtiff.lib pdflib.lib /nologo /version:3.02 /subsystem:console /pdb:none /machine:I386 /out:"hello.exe" /libpath:"..\..\pdflib" /libpath:"..\..\..\zlib" /libpath:"..\..\..\libpng" /libpath:"..\..\..\libtiff"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib pdflib.lib /nologo /version:3.02 /subsystem:console /pdb:none /machine:I386 /out:"hello.exe" /libpath:"..\..\pdflib"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Cplusplus - Win32 Release"
# Name "Cplusplus - Win32 no auxiliary libs"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\hello.cpp
# End Source File
# Begin Source File

SOURCE=.\pdflib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\pdflib.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
