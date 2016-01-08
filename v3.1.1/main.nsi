/*
 * by Parent and Childset Accessible Syntax Tree Language Interpreter
 * Copyright (C) 2008-2016  Philippe Choquette
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

!define VERSION       "3.1.1"
!define VERSION_MAJOR "3"
!define VERSION_MINOR "1"
!define VERSION_FULL  "3.1.1.0"
!define PUBLISHER     "Philippe Choquette"
!define WEBSITE_URL   "http://pcosmos.ca/pcastl/"

Name "PCASTL Interpreter"
OutFile "pcastli-v${VERSION}-setup.exe"

InstallDir "$PROGRAMFILES\PCASTL Interpreter"
InstallDirRegKey HKLM "Software\PCASTL Interpreter" ""

RequestExecutionLevel admin

!include "MUI2.nsh"
!include "x64.nsh"

# installer file information
VIProductVersion                   "${VERSION_FULL}"
VIAddVersionKey "CompanyName"      "${PUBLISHER}"
VIAddVersionKey "ProductName"      "PCASTL Interpreter"
VIAddVersionKey "ProductVersion"   "${VERSION}"
VIAddVersionKey "FileDescription"  "PCASTL Command Line Interpreter"
VIAddVersionKey "FileVersion"      "${VERSION}"
VIAddVersionKey "LegalCopyright"   "${PUBLISHER}"
VIAddVersionKey "OriginalFilename" "pcastli-v${VERSION}-setup.exe"

!insertmacro MUI_PAGE_LICENSE "license.rtf"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

; Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\PCASTL Interpreter"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "PCASTL Interpreter"
Var MUI_TEMP
Var STARTMENU_FOLDER
!insertmacro MUI_PAGE_STARTMENU "Application" $STARTMENU_FOLDER

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!insertmacro MUI_LANGUAGE "English"

Section "Binaries" SecMain

   SectionIn RO

   SetOutPath "$INSTDIR"

   File "pcastli64.exe"
   File "pcastli32.exe"
   File "pcastli.ini"
   File "changelog.txt"
   File "changelog-fr.txt"
   File "license.txt"
   
   WriteUninstaller "$INSTDIR\uninstall.exe"

   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
      SetOutPath "$INSTDIR"
      CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
      ${If} ${RunningX64}
         CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\PCASTLI x64.lnk" "%ComSpec%" '/k "$INSTDIR\pcastli64.exe"'
      ${EndIf} 
      CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\PCASTLI x86.lnk" "%ComSpec%" '/k "$INSTDIR\pcastli32.exe"'
      CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
   !insertmacro MUI_STARTMENU_WRITE_END

   Push $R0
   StrCpy $R0 "$STARTMENU_FOLDER" 1
   ${if} $R0 == ">"
      ; Write folder to registry
      WriteRegStr "${MUI_STARTMENUPAGE_Application_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_Application_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_Application_REGISTRY_VALUENAME}" ">"
   ${endif}
   Pop $R0

   WriteRegStr HKLM "Software\PCASTL Interpreter" "" $INSTDIR
   WriteRegStr HKLM "Software\PCASTL Interpreter" "Version" "${VERSION}"

   WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter" "UninstallString" "$INSTDIR\uninstall.exe"
   WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter" "InstallLocation" "$INSTDIR"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "DisplayName"    "PCASTL Interpreter"
   ReadEnvStr $0 COMSPEC
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "DisplayIcon"    "$0"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "DisplayVersion" "${VERSION}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "URLInfoAbout"   "${WEBSITE_URL}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "URLUpdateInfo"  "${WEBSITE_URL}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "HelpLink"       "${WEBSITE_URL}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "Publisher"      "${PUBLISHER}"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "VersionMajor"   "${VERSION_MAJOR}"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "VersionMinor"   "${VERSION_MINOR}"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "NoModify"       "1"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"   "NoRepair"       "1"

SectionEnd

Section "English Documentation" SecDocEn

   SetOutPath "$INSTDIR\pcastl-doc-english"

   File /r "D:\Philippe\dev\C++\pcastl_doc\pcastl-doc-english\*"

   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
      CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\English Documentation.lnk" "$INSTDIR\pcastl-doc-english\index.htm"
   !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section "French Documentation" SecDocFr

   SetOutPath "$INSTDIR\pcastl-doc-francais"

   File /r "D:\Philippe\dev\C++\pcastl_doc\pcastl-doc-francais\*"

   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
      CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\French Documentation.lnk" "$INSTDIR\pcastl-doc-francais\index.htm"
   !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section "Uninstall"

   Delete "$INSTDIR\pcastli64.exe"
   Delete "$INSTDIR\pcastli32.exe"
   Delete "$INSTDIR\pcastli.ini"
   Delete "$INSTDIR\changelog.txt"
   Delete "$INSTDIR\changelog-fr.txt"
   Delete "$INSTDIR\license.txt"
   RMDir /r "$INSTDIR\pcastl-doc-english"
   RMDir /r "$INSTDIR\pcastl-doc-francais"

   !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
   Delete "$SMPROGRAMS\$MUI_TEMP\PCASTLI x64.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\PCASTLI x86.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\English Documentation.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\French Documentation.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
   RMDir "$SMPROGRAMS\$MUI_TEMP"
   
   Delete "$INSTDIR\uninstall.exe"
   
   RMDir "$INSTDIR"

   DeleteRegKey HKLM "Software\PCASTL Interpreter"
   DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter"

SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "Executables, parameter file, change logs and license "
!insertmacro MUI_DESCRIPTION_TEXT ${SecDocEn} "Full HTML English documentation"
!insertmacro MUI_DESCRIPTION_TEXT ${SecDocFr} "Full HTML French documentation"
!insertmacro MUI_FUNCTION_DESCRIPTION_END
