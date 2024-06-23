SET MapName=Map_1
SET GameMode=Default
SET ExePath=*\UnrealEngine\Engine\Binaries\Win64\UnrealEditor.exe
SET ProjectPath=*\EOSDemo\EOSDemo.uproject

CALL "%ExePath%" "%ProjectPath%" -debug -game -log -AUTH_TYPE="developer" -AUTH_LOGIN="localhost:8081" -AUTH_PASSWORD="Dev1" -epicapp="Client" -ResX=1280 -ResY=720 -windowed