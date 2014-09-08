# Microsoft Developer Studio Project File - Name="Perl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Perl - Win32 oldperl
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Perl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Perl.mak" CFG="Perl - Win32 oldperl"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Perl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Perl - Win32 oldperl" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Perl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Perl___Win32_Release"
# PROP BASE Intermediate_Dir "Perl___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\pdflib" /YX /FD /GZ /c /Tp
# ADD CPP /nologo /MT /W3 /O2 /I "c:\programme\Perl5.6\lib\CORE" /I "c:\programme\Perl5.6\lib\CORE\win32" /I "..\..\pdflib" /D "WIN32" /D "_MT" /D "PDFLIB_STATIC" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib perlcore.lib /nologo /dll /debug /machine:I386 /out:"pdflib.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib perl56.lib zlib.lib libpng.lib libtiff.lib pdflib.lib /nologo /base:"0x55330000" /dll /pdb:none /machine:I386 /out:"pdflib_pl.dll" /libpath:"c:\programme\Perl5.6\lib\CORE" /libpath:"..\..\pdflib" /libpath:"..\..\..\zlib" /libpath:"..\..\..\libpng" /libpath:"..\..\..\libtiff"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Perl - Win32 oldperl"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Perl___Win32_oldperl"
# PROP BASE Intermediate_Dir "Perl___Win32_oldperl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "oldperl"
# PROP Intermediate_Dir "oldperl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /I "c:\programme\Perl 5.6.0\lib\CORE" /I "c:\programme\Perl 5.6.0\lib\CORE\win32" /I "..\..\pdflib" /D "WIN32" /D "_MT" /D "PDFLIB_STATIC" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "c:\programme\Perl5.005\lib\CORE" /I "c:\programme\Perl5.005\lib\CORE\win32" /I "..\..\pdflib" /D "WIN32" /D "_MT" /D "PDFLIB_STATIC" /D "PERL_OBJECT" /FR /YX /FD /c /Tp
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib perl56.lib zlib.lib libpng.lib libtiff.lib pdflib.lib /nologo /base:"0x55330000" /dll /pdb:none /machine:I386 /out:"pdflib_pl.dll" /libpath:"c:\programme\Perl 5.6.0\lib\CORE" /libpath:"..\..\pdflib" /libpath:"..\..\..\zlib" /libpath:"..\..\..\libpng" /libpath:"..\..\..\libtiff"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zlib.lib libpng.lib libtiff.lib pdflib.lib  perlcore.lib /nologo /base:"0x55330000" /dll /pdb:none /machine:I386 /out:"oldperl/pdflib_pl.dll" /libpath:"c:\programme\Perl5.005\lib\CORE" /libpath:"..\..\pdflib" /libpath:"..\..\..\zlib" /libpath:"..\..\..\libpng" /libpath:"..\..\..\libtiff"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Perl - Win32 Release"
# Name "Perl - Win32 oldperl"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\pdflib_pl.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
