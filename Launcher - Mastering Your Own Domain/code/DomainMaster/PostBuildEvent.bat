@echo off
call "C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\..\Tools\vsvars32.bat"
sn -Vr D:\Gigi\Dev\C#\Launcher\DomainMaster\bin\Debug\DomainMaster.dll
gacutil -u DomainMaster
gacutil -i D:\Gigi\Dev\C#\Launcher\DomainMaster\bin\Debug\DomainMaster.dll

if errorlevel 1 goto CSharpReportError
goto CSharpEnd
:CSharpReportError
echo Project error: A tool returned an error code from the build event
exit 1
:CSharpEnd