@echo off

SET CURRENT_PATH=%~dp0

echo -----------------------------
echo LicheePi-nano SPI-Flash Write.
echo -----------------------------
echo;
echo LicheePi-nanoをFELモードに移行して下さい.
echo;
pause
echo;
echo write start addr:%1
echo write file:%2
"%CURRENT_PATH%sunxi-fel" spiflash-write %1 %2

echo;
pause
exit

:err
echo 引数を正しく指定してください. 引数1(start address) 引数2(書き込み対象ファイル)
pause
exit
