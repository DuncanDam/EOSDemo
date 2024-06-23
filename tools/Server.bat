SET MapName=Map_1
SET GameMode=Default
SET ExePath=*\UnrealEngine\Engine\Binaries\Win64\UnrealEditor.exe
SET ProjectPath=*\EOSDemo\EOSDemo.uproject

CALL "%ExePath%" "%ProjectPath%" %MapName%?game=%GameMode% -server -log -epicapp="Server" -MAPNAME=%MapName% -GAMEMODE=%GameMode%