@echo off

set date_now=%date:~0,4%%date:~5,2%%date:~8,2%

set time_tmp=%time: =0%
set time_now=%time_tmp:~0,2%%time_tmp:~3,2%%time_tmp:~6,2%

echo -----------------------------
echo LicheePi-nano SPI-Flash Dump.
echo -----------------------------
echo;
echo LicheePi-nano‚ğFELƒ‚[ƒh‚ÉˆÚs‚µ‚Ä‰º‚³‚¢.
echo;
pause
echo;
echo read addr:0x00000000.
echo read size:16,777,216bytes.
sunxi-fel spiflash-read 0 16777216 licheepinano_spiflash_dump_%date_now%%time_now%.bin

echo;
pause