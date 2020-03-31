[Setup]
AppName=Ernies Adventure
AppVerName=Ernies Adventure version 1.0
DefaultDirName={pf}\Ernies Adventure
DefaultGroupName=Ernies Adventure
UninstallDisplayIcon={app}\Ernies Adventure.exe
OutputDir=..\..\Installers
OutputBaseFilename=ErniesAdventureInstall_1.0

[Files]
Source: "Character Images\*"; DestDir: "{app}\Character Images"; Flags: recursesubdirs
Source: "Common Sprites\*"; DestDir: "{app}\Common Sprites"; Flags: recursesubdirs
Source: "Inventory Images\*"; DestDir: "{app}\Inventory Images"; Flags: recursesubdirs
Source: "Music\*.wav"; DestDir: "{app}\Music"; Flags: recursesubdirs
Source: "PathBrainSequences\*"; DestDir: "{app}\PathBrainSequences"; Flags: recursesubdirs
Source: "Saved Games\Start.psg"; DestDir: "{app}\Saved Games"
Source: "Scenes\*"; DestDir: "{app}\Scenes"; Flags: recursesubdirs
Source: "allegro-4.4.2-monolith-mt.dll"; DestDir: "{app}"
Source: "Ernies Adventure.exe"; DestDir: "{app}"
Source: "GameSettings.stg"; DestDir: "{app}"
Source: "LoadScreen.bmp"; DestDir: "{app}"
Source: "SkyChange.scf"; DestDir: "{app}"

[Icons]
Name: "{group}\Ernies Adventure"; Filename: "{app}\Ernies Adventure.exe"; WorkingDir: "{app}"
Name: "{group}\Uninstall Ernies Adventure"; Filename: "{uninstallexe}"

