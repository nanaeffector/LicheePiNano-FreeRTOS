@echo off

SET CURRENT_PATH=%~dp0

echo -----------------------------
echo LicheePi-nano SPI-Flash Write.
echo -----------------------------
echo;
echo LicheePi-nano��FEL���[�h�Ɉڍs���ĉ�����.
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
echo �����𐳂����w�肵�Ă�������. ����1(start address) ����2(�������ݑΏۃt�@�C��)
pause
exit
