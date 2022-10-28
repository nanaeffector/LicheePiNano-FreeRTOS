@echo off

SET CURRENT_PATH=%~dp0

cd ..
call build_config.bat

echo.
echo ***********************************************
echo  build - bootloader
echo ***********************************************
echo.
cd bootloader
call make.bat  %GCCPATH%
cd ..

echo.
echo ***********************************************
echo  build - main
echo ***********************************************
echo.
cd %CURRENT_PATH%
call make.bat  %GCCPATH%

call "%CURRENT_PATH%\flashwrite.bat"