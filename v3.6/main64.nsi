/*
 * by Parent and Childset Accessible Syntax Tree Language Interpreter
 * Copyright (C) 2008-2018  Philippe Choquette
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

!define VERSION       "3.5"
!define VERSION_MAJOR "3"
!define VERSION_MINOR "5"
!define VERSION_FULL  "3.5.0.0"
!define PUBLISHER     "Philippe Choquette"
!define WEBSITE_URL   "http://pcosmos.ca/pcastl/"

Name "PCASTL Interpreter 64-bit"
OutFile "pcastli-v${VERSION}-64-bit-setup.exe"

InstallDir "$PROGRAMFILES64\PCASTL Interpreter 64-bit"
InstallDirRegKey HKLM "Software\PCASTL Interpreter 64-bit" ""

RequestExecutionLevel admin

!include "MUI2.nsh"
!include "x64.nsh"

# installer file information
VIProductVersion                   "${VERSION_FULL}"
VIAddVersionKey "CompanyName"      "${PUBLISHER}"
VIAddVersionKey "ProductName"      "PCASTL Interpreter 64-bit"
VIAddVersionKey "ProductVersion"   "${VERSION}"
VIAddVersionKey "FileDescription"  "PCASTL Command Line Interpreter"
VIAddVersionKey "FileVersion"      "${VERSION}"
VIAddVersionKey "LegalCopyright"   "${PUBLISHER}"
VIAddVersionKey "OriginalFilename" "pcastli-v${VERSION}-64-bit-setup.exe"

!define MUI_PAGE_CUSTOMFUNCTION_PRE PageLicensePre
!insertmacro MUI_PAGE_LICENSE "license.rtf"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

; Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\PCASTL Interpreter 64-bit"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "PCASTL Interpreter 64-bit"
Var MUI_TEMP
Var STARTMENU_FOLDER
!insertmacro MUI_PAGE_STARTMENU "Application" $STARTMENU_FOLDER

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!insertmacro MUI_LANGUAGE "English"

Function PageLicensePre

   ReadRegStr $0 HKLM "Software\PCASTL Interpreter 64-bit" ""

   ${If} $0 != ""
      MessageBox MB_OK "Please uninstall before reinstalling."
      Quit
   ${EndIf}

FunctionEnd

Section "Core Files" SecMain

   SectionIn RO

   SetOutPath "$INSTDIR"

   File "pcastli64.exe"
   File "pcastli.ini"
   File "changelog.txt"
   File "changelog-fr.txt"
   File "COPYING"
   File "COPYING.LESSER"
   
   WriteUninstaller "$INSTDIR\uninstall.exe"

   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
      SetOutPath "$INSTDIR"
      CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
      ${If} ${RunningX64}
         CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\PCASTLI x64.lnk" "%ComSpec%" '/k "$INSTDIR\pcastli64.exe"'
      ${EndIf} 
      CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
   !insertmacro MUI_STARTMENU_WRITE_END

   Push $R0
   StrCpy $R0 "$STARTMENU_FOLDER" 1
   ${if} $R0 == ">"
      ; Write folder to registry
      WriteRegStr "${MUI_STARTMENUPAGE_Application_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_Application_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_Application_REGISTRY_VALUENAME}" ">"
   ${endif}
   Pop $R0

   WriteRegStr HKLM "Software\PCASTL Interpreter 64-bit" "" $INSTDIR
   WriteRegStr HKLM "Software\PCASTL Interpreter 64-bit" "Version" "${VERSION}"

   WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit" "UninstallString" "$INSTDIR\uninstall.exe"
   WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit" "InstallLocation" "$INSTDIR"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "DisplayName"    "PCASTL Interpreter 64-bit"
   ReadEnvStr $0 COMSPEC
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "DisplayIcon"    "$0"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "DisplayVersion" "${VERSION}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "URLInfoAbout"   "${WEBSITE_URL}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "URLUpdateInfo"  "${WEBSITE_URL}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "HelpLink"       "${WEBSITE_URL}"
   WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "Publisher"      "${PUBLISHER}"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "VersionMajor"   "${VERSION_MAJOR}"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "VersionMinor"   "${VERSION_MINOR}"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "NoModify"       "1"
   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"   "NoRepair"       "1"

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
   Delete "$INSTDIR\pcastli.ini"
   Delete "$INSTDIR\changelog.txt"
   Delete "$INSTDIR\changelog-fr.txt"
   Delete "$INSTDIR\COPYING"
   Delete "$INSTDIR\COPYING.LESSER"
   RMDir /r "$INSTDIR\pcastl-doc-english"
   RMDir /r "$INSTDIR\pcastl-doc-francais"

   !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
   Delete "$SMPROGRAMS\$MUI_TEMP\PCASTLI x64.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\English Documentation.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\French Documentation.lnk"
   Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
   RMDir "$SMPROGRAMS\$MUI_TEMP"
   
   Delete "$INSTDIR\uninstall.exe"
   
   RMDir "$INSTDIR"

   DeleteRegKey HKLM "Software\PCASTL Interpreter 64-bit"
   DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PCASTL Interpreter 64-bit"

SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "Executables, parameter file, change logs and license "
!insertmacro MUI_DESCRIPTION_TEXT ${SecDocEn} "Full HTML English documentation"
!insertmacro MUI_DESCRIPTION_TEXT ${SecDocFr} "Full HTML French documentation"
!insertmacro MUI_FUNCTION_DESCRIPTION_END
