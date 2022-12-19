@echo off
echo --------------------------------------------------
echo Building Debug
echo --------------------------------------------------
echo[
call "tools\Setup.bat"
MsBuild CLEOAddon.sln /property:Configuration=Debug /t:CLEOAddon /property:Platform=Win32
del %OUT_DIR%"\cleo\CLEO_PLUGINS\CLEOAddon.cleo" /Q
del %OUT_DIR%"\cleo\CLEO_PLUGINS\CLEOAddon.pdb" /Q
xcopy /s "bin\CLEOAddon.cleo" %OUT_DIR%"\cleo\CLEO_PLUGINS\" /K /D /H /Y 
xcopy /s "bin\CLEOAddon.pdb" %OUT_DIR%"\cleo\CLEO_PLUGINS\" /K /D /H /Y 
