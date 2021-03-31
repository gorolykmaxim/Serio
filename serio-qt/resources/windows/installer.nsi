!include "MUI2.nsh"

Name "Serio"
Outfile "$%NSIS_OUTPUT_DIR%\install.exe"
Unicode True

InstallDir "$APPDATA\Serio"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\Serio.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Run Serio"

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE English

Section
    SetOutPath "$INSTDIR"
    File /r "$%NSIS_INPUT_DIR%\*"
    File "icon.ico"
    CreateShortcut "$DESKTOP\Serio.lnk" "$INSTDIR\Serio.exe" "" "$INSTDIR\icon.ico" 0
    WriteUninstaller "$INSTDIR\uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Serio" "DisplayName" "Serio"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Serio" "DisplayIcon" "$INSTDIR\icon.ico"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Serio" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    ExecWait "$INSTDIR\vc_redist.x64.exe"
SectionEnd

Section "Uninstall"
    SetOutPath $INSTDIR
    Delete "$INSTDIR\uninstall.exe"
    Delete "$DESKTOP\Serio.lnk"
    RMDir /r "$INSTDIR"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Serio"
SectionEnd