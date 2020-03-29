[Setup]
AppName=Ernies Adventure
AppVerName=Ernies Adventure version 1.0
DefaultDirName={pf}\Ernies Adventure
DefaultGroupName=Ernies Adventure
UninstallDisplayIcon={app}\Ernies Adventure.exe
OutputDir=userdocs:Installers

[Files]
Source: "Character Images\*"; DestDir: "{app}\Character Images"; Flags: recursesubdirs
Source: "Common Sprites\*"; DestDir: "{app}\Common Sprites"; Flags: recursesubdirs
Source: "Inventory Images\*"; DestDir: "{app}\Inventory Images"; Flags: recursesubdirs
Source: "Music\*"; DestDir: "{app}\Music"; Flags: recursesubdirs
Source: "PathBrainSequences\*"; DestDir: "{app}\PathBrainSequences"; Flags: recursesubdirs
Source: "Saved Games\*"; DestDir: "{app}\Saved Games"; Flags: recursesubdirs
Source: "Scenes\*"; DestDir: "{app}\Scenes"; Flags: recursesubdirs
Source: "alleg42.dll"; DestDir: "{app}"
Source: "Ernies Adventure.exe"; DestDir: "{app}"
Source: "GameSettings.stg"; DestDir: "{app}"
Source: "LoadScreen.bmp"; DestDir: "{app}"
Source: "SkyChange.scf"; DestDir: "{app}"

[Icons]
Name: "{group}\Ernies Adventure"; Filename: "{app}\Ernies Adventure.exe"; WorkingDir: "{app}"
Name: "{group}\Uninstall Ernies Adventure"; Filename: "{uninstallexe}"

