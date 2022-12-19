@echo off
echo --------------------------------------------------
echo Building Release
echo --------------------------------------------------
echo[
call "tools\Setup.bat"
MsBuild CLEOAddon.sln /property:Configuration=Release /t:CLEOAddon /property:Platform=Win32   
del %OUT_DIR%"\cleo\CLEO_PLUGINS\CLEOAddon.cleo" /Q
xcopy /s "bin\CLEOAddon.cleo" %OUT_DIR%"\cleo\CLEO_PLUGINS\" /K /D /H /Y 
