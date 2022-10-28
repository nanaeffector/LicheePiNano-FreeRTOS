@echo off

SET CURRENT_PATH=%~dp0
cd..

echo.
echo ***********************************************
echo targetÇ÷ÇÃèëÇ´çûÇ›ÇäJénÇµÇ‹Ç∑ÅB
echo Start write to the target.
echo ***********************************************
pause
echo.

set TOOLPATH=tools\sunxi-tools-win32_f1_flash\sunxi-fel.exe
set BLADDR=0x0
set BLPATH=bootloader\build
set MAINADDR=0x10000
set MAINPATH=%CURRENT_PATH%\build

echo "%TOOLPATH%" spiflash-write %BLADDR% "%BLPATH%bootloader.bin"
"%TOOLPATH%" spiflash-write %BLADDR% "%BLPATH%bootloader.bin"
echo.
echo "%TOOLPATH%" spiflash-write %MAINADDR% "%MAINPATH%licheenanomain.bin"
"%TOOLPATH%" spiflash-write %MAINADDR% "%MAINPATH%licheenanomain.bin"

echo.
pause