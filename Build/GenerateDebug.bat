@ECHO OFF
ECHO Generate a debug solution to test Sharpmake config files for ZenEngine
ECHO .
ECHO Create output directory : {DebugSharpmake}
mkdir {DebugSharpmake}
cd {DebugSharpmake}
ECHO .
ECHO Create link to Sharmapmake config files
mklink /H "zenShared.sharpmake.cs" "../Sharpmake/zenShared.sharpmake.cs"
mklink /H "zenProjectAll.sharpmake.cs" "../Sharpmake/zenProjectAll.sharpmake.cs"
ECHO .
ECHO Generates debug solution
..\Sharpmake\Bin\Sharpmake.Application.exe /sources(@"zenProjectAll.sharpmake.cs") /verbose /generateDebugSolution
pause