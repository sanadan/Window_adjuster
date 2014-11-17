; -- 64Bit.iss --
; Demonstrates installation of a program built for the x64 (a.k.a. AMD64)
; architecture.
; To successfully run this installation and the program it installs,
; you must have a "x64" edition of Windows.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=Window adjuster
AppVersion=0.5.0
DefaultDirName={pf}\Window_adjuster
DefaultGroupName=Window adjuster
UninstallDisplayIcon={app}\Window_adjuster.exe
Compression=lzma2
SolidCompression=yes
OutputDir=Output
; "ArchitecturesAllowed=x64" specifies that Setup cannot run on
; anything but x64.
ArchitecturesAllowed=x64
; "ArchitecturesInstallIn64BitMode=x64" requests that the install be
; done in "64-bit mode" on x64, meaning it should use the native
; 64-bit Program Files directory and the 64-bit view of the registry.
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "Window_adjuster.exe"; DestDir: "{app}"
Source: "WA_Hook.dll"; DestDir: "{app}"
Source: "Window_adjuster.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{userstartup}\Window_adjuster"; Filename: "{app}\Window_adjuster.exe"

[Languages]
Name: japanese; MessagesFile: compiler:Languages\Japanese.isl 

[Run]
Filename: "{app}\Window_adjuster.exe"; Parameters: "/q"
Filename: "{app}\Window_adjuster.exe"; Description: "Window adjuster‚Ì‹N“®"; Flags: postinstall nowait skipifsilent

[UninstallRun]
Filename: "{app}\Window_adjuster.exe"; Parameters: "/q"
