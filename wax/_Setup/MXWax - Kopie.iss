

[Setup]
SourceDir=.\..\..\bin\Wax\wax_fuer_3te_Release32
SetupIconFile=..\..\..\wax\wax\res\wax.ico
OutputDir=..\..\..\wax\_Setup
OutputManifestFile=istool-de.lng
AppCopyright=2012 by Minimax GmbH & Co Kg
AppName=Wax3
AppVerName=Wax3  Version 3.4.71
ExtraDiskSpaceRequired=2000
DefaultDirName={pf}\waxplus
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
LanguageDetectionMethod=locale
WizardImageBackColor=clOlive
DisableStartupPrompt=false
DefaultGroupName=Minimax
Compression=none
DiskSpanning=false
DiskClusterSize=4096
DiskSliceSize=30000000
OutputBaseFilename=waxplus_setup


AppID={{098A3A88-563C-418A-B6DB-6B53F0281391}
PrivilegesRequired=admin
ChangesAssociations=true
UserInfoPage=true
DefaultUserInfoSerial=Distributor Serial No. if available;
VersionInfoVersion=3.4.7
VersionInfoCompany=Minimax GmbH & Co KG
VersionInfoCopyright=2012 by Minimax GmbH & Co KG
VersionInfoProductName=waxplus
VersionInfoProductVersion=3.4.7


[Languages]
Name: de; MessagesFile: compiler:Languages\German.isl; LicenseFile: ..\..\..\wax\_Setup\Lizenzvertrag-deutsch.rtf


[Files]
Source: *; DestDir: {app}; Flags: overwritereadonly replacesameversion; Permissions: users-readexec
Source: ..\..\..\wax\_Setup\WaxUser.mdb; DestDir: {app}; Flags: onlyifdoesntexist uninsneveruninstall; Permissions: users-readexec
Source: ..\..\..\wax\_Setup\SVGView.exe; DestDir: {app}; Permissions: users-readexec; Flags: deleteafterinstall
Source: ..\..\..\wax\_Setup\SetupDesignReview2011.msi; DestDir: {app}; Permissions: users-readexec; Flags: deleteafterinstall
Source: ..\..\..\wax\_Setup\vcredist_x86.exe; DestDir: {app}; Permissions: users-readexec; Flags: deleteafterinstall
Source: ..\..\..\wax\_Setup\2005vcredist_x86.exe; DestDir: {app}; Permissions: users-readexec; Flags: deleteafterinstall
Source: ..\..\..\wax\_Setup\wax3deutsch.pdf; DestDir: {app}\doc; Flags: overwritereadonly replacesameversion; Permissions: users-readexec

[Dirs]
Name: "{app}\projekte"
Name: "{app}\excel"
Name: "{app}\komp"
Name: "{app}\doc"

[Run]
Filename: {app}\vcredist_x86.exe; Parameters: /qb
Filename: {app}\2005vcredist_x86.exe; Parameters:
Filename: {app}\SVGView.exe;  Parameters: /passive;  Check: ShouldInstallComCtlUpdate
Filename: "msiexec.exe"; Parameters: "/i ""{app}\SetupDesignReview2011"""
Filename: {app}\Wax3.exe; Flags: postinstall nowait



[Icons]
Name: {group}\Wax3; Filename: {app}\Wax3.exe; WorkingDir: {app}; IconFilename: {app}\Wax3.exe; IconIndex: 0; Components: ; Languages: 
Name: {commondesktop}\Wax3; Filename: {app}\Wax3.exe; WorkingDir: {app}; IconFilename: {app}\Wax3.exe; IconIndex: 0; Components: ; Languages: 



[Registry]
Root: HKLM; Subkey: Software\Clients\{{5DC93A51-D210-35D0-9A9A-00C236FC30340}; ValueType: string; Flags: createvalueifdoesntexist; Permissions: authusers-full


[Code]
var
  DefaultTop,
  DefaultLeft,
  DefaultHeight,
  DefaultWidth,
  DefaultBackTop,
  DefaultNextTop,
  DefaultCancelTop,
  DefaultBevelTop,
  DefaultBevelWidth,
  DefaultOuterHeight,
  WizardForm_InnerPage_Height,
  WizardForm_InnerPage_Width,
  WizardForm_InnerNotebook_Height,
  WizardForm_InnerNotebook_Width,
  WizardForm_Top,
  WizardForm_Height,
  WizardForm_Width,
  WizardForm_OuterNotebook_Height,
  WizardForm_OuterNotebook_Width,
  WizardForm_CancelButton_Top,
  WizardForm_NextButton_Top,
  WizardForm_BackButton_Top,
  WizardForm_Bevel_Top,
  WizardForm_Bevel_Width,
  WizardForm_LicensePage_Height,
  WizardForm_LicenseMemo_Height,
  WizardForm_LicensePage_Width,
  WizardForm_LicenseMemo_Width,
  WizardForm_LicenseNotAcceptedRadio_Top,
  WizardForm_LicenseAcceptedRadio_Top  : Integer;

const
  LicenseHeight = 500;
  LicenseWidth = 500;

procedure InitializeWizard();
begin
  DefaultTop := WizardForm.Top;
  DefaultLeft := WizardForm.Left;
  DefaultHeight := WizardForm.Height;
  DefaultWidth := WizardForm.Width;
  DefaultBackTop := WizardForm.BackButton.Top;
  DefaultNextTop := WizardForm.NextButton.Top;
  DefaultCancelTop := WizardForm.CancelButton.Top;
  DefaultBevelTop := WizardForm.Bevel.Top;
  DefaultBevelWidth := WizardForm.Bevel.Width;
  DefaultOuterHeight := WizardForm.OuterNotebook.Height;

 	WizardForm_InnerPage_Height := WizardForm.InnerPage.Height;
	WizardForm_InnerPage_Width :=	WizardForm.InnerPage.Width;
	WizardForm_InnerNotebook_Height :=	WizardForm.InnerNotebook.Height;
	WizardForm_InnerNotebook_Width :=	WizardForm.InnerNotebook.Width;
	WizardForm_Top :=	WizardForm.Top;
	WizardForm_Height :=	WizardForm.Height;
	WizardForm_Width :=	WizardForm.Width;
	WizardForm_OuterNotebook_Height :=	WizardForm.OuterNotebook.Height;
	WizardForm_OuterNotebook_Width :=	WizardForm.OuterNotebook.Width;
	WizardForm_CancelButton_Top :=	WizardForm.CancelButton.Top;
	WizardForm_NextButton_Top :=	WizardForm.NextButton.Top;
	WizardForm_BackButton_Top :=	WizardForm.BackButton.Top;
	WizardForm_Bevel_Top :=	WizardForm.Bevel.Top;
	WizardForm_Bevel_Width := WizardForm.Bevel.Width;

	WizardForm_LicensePage_Height := WizardForm.LicensePage.Height;
	WizardForm_LicenseMemo_Height := WizardForm.LicenseMemo.Height;
	WizardForm_LicensePage_Width := WizardForm.LicensePage.Width;
	WizardForm_LicenseMemo_Width := WizardForm.LicenseMemo.Width;
	WizardForm_LicenseNotAcceptedRadio_Top := WizardForm.LicenseNotAcceptedRadio.Top;
	WizardForm_LicenseAcceptedRadio_Top := WizardForm.LicenseAcceptedRadio.Top;


end;

function ShouldInstallComCtlUpdate: Boolean;
var
  Version: TWindowsVersion;
  S: String;
begin
  GetWindowsVersionEx(Version);

   Result := False;

  // Disallow installation on Home edition of Windows
  if Version.SuiteMask and VER_SUITE_PERSONAL <> 0 then
  begin
    SuppressibleMsgBox('This program cannot be installed on a Home edition of Windows.',
      mbCriticalError, MB_OK, MB_OK);
    Result := False;
  end;
 
  // On Windows 2000, check for SP4
  if Version.NTPlatform and
     (Version.Major = 5) and
     (Version.Minor = 0) then
  begin
    Result := True;
  end;

  // On Windows XP, check for SP2
  if Version.NTPlatform and
     (Version.Major = 5) and
     (Version.Minor = 1) then
  begin
    Result := True;
  end;
 
end;

procedure PrintButtonClickLicense(Sender: TObject);

var
Text: AnsiString;
Text2 : AnsiString;
RetCode: Integer;


begin

	if  ActiveLanguage = 'de' then begin
		Text := 'Möchten Sie die Lizenz jetzt Drucken?';
		Text2 := 'Fehler beim drucken der Lizenz: '
	end
	else begin
		Text := 'Do you really want to print the License?';
		Text2 := 'Error while printing: ';
	end;

	if MsgBox( Text , mbConfirmation, MB_YESNO ) = IDYES then
	begin

		ExtractTemporaryFile(ExpandConstant('LizenzvertragMXDesignManager-englisch.rtf') );

		if not ShellExec('print', ExpandConstant('{tmp}\LizenzvertragMXDesignManager-englisch.rtf'),  '', '', SW_SHOW, ewNoWait, RetCode) then
			MsgBox(Text2 + IntToStr(RetCode), mbError, mb_Ok)
	end

end;


/////////////////////////////////////////////////////////////////////
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;


/////////////////////////////////////////////////////////////////////
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;


/////////////////////////////////////////////////////////////////////
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
// Return Values:
// 1 - uninstall string is empty
// 2 - error executing the UnInstallString
// 3 - successfully executed the UnInstallString

  // default return value
  Result := 0;

  // get the uninstall string of the old app
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
      sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

/////////////////////////////////////////////////////////////////////
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
    end;
  end;
end;





procedure CurPageChanged(CurPageID: Integer);



begin
	if CurPageId = wpLicense then
	begin

		

		WizardForm.InnerPage.Height := WizardForm.InnerPage.Height + (LicenseHeight - DefaultHeight);
		WizardForm.InnerPage.Width := WizardForm.InnerPage.Width + (LicenseWidth - DefaultWidth);
		WizardForm.InnerNotebook.Height :=  WizardForm.InnerNotebook.Height + (LicenseHeight - DefaultHeight);
		WizardForm.InnerNotebook.Width :=  WizardForm.InnerNotebook.Width + (LicenseWidth - DefaultWidth);
		WizardForm.Top := DefaultTop - (LicenseHeight - DefaultHeight) div 2;
		WizardForm.Left := DefaultLeft - (LicenseWidth - DefaultWidth) div 2;
		WizardForm.Height := LicenseHeight;
		WizardForm.Width := LicenseWidth;
		WizardForm.OuterNotebook.Height := WizardForm.OuterNotebook.Height + (LicenseHeight - DefaultHeight);
		WizardForm.OuterNotebook.Width := WizardForm.OuterNotebook.Width + (LicenseWidth - DefaultWidth);
		WizardForm.CancelButton.Top := DefaultCancelTop + (LicenseHeight - DefaultHeight);
		WizardForm.NextButton.Top := DefaultNextTop + (LicenseHeight - DefaultHeight);
		WizardForm.BackButton.Top := DefaultBackTop + (LicenseHeight - DefaultHeight);
		WizardForm.Bevel.Top := DefaultBevelTop + (LicenseHeight - DefaultHeight);
		WizardForm.Bevel.Width := DefaultBevelWidth + (LicenseWidth - DefaultWidth);

		WizardForm.LicensePage.Height := WizardForm_LicensePage_Height + (LicenseHeight - DefaultHeight);
		WizardForm.LicenseMemo.Height := WizardForm_LicenseMemo_Height + (LicenseHeight - DefaultHeight);
		WizardForm.LicensePage.Width := WizardForm_LicensePage_Width + (LicenseWidth - DefaultWidth);
		WizardForm.LicenseMemo.Width := WizardForm_LicenseMemo_Width + (LicenseWidth - DefaultWidth);
		WizardForm.LicenseNotAcceptedRadio.Top := WizardForm_LicenseNotAcceptedRadio_Top + (LicenseHeight - DefaultHeight);
		WizardForm.LicenseAcceptedRadio.Top := WizardForm_LicenseAcceptedRadio_Top + (LicenseHeight - DefaultHeight);

		

  	end
  	else begin
		WizardForm.InnerPage.Height := WizardForm_InnerPage_Height;
		WizardForm.InnerPage.Width := WizardForm_InnerPage_Width;
		WizardForm.InnerNotebook.Height :=  WizardForm_InnerNotebook_Height;
		WizardForm.InnerNotebook.Width :=  WizardForm_InnerNotebook_Width;
		WizardForm.Top := WizardForm_Top;
		WizardForm.Left := DefaultLeft;
		WizardForm.Height := WizardForm_Height;
		WizardForm.Width := WizardForm_Width;
		WizardForm.OuterNotebook.Height := WizardForm_OuterNotebook_Height
		WizardForm.OuterNotebook.Width := WizardForm_OuterNotebook_Width
		WizardForm.CancelButton.Top := WizardForm_CancelButton_Top
		WizardForm.NextButton.Top := WizardForm_NextButton_Top
		WizardForm.BackButton.Top := WizardForm_BackButton_Top
		WizardForm.Bevel.Top := WizardForm_Bevel_Top
		WizardForm.Bevel.Width := WizardForm_Bevel_Width
  	end;

end;




