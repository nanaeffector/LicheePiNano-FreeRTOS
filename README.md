# Introduction 
LicheePi Nano用のソースコードです。

Linuxではなく、ベアメタルで動かすためのソースになっています。  
LiceePi Nanoでデジタルエフェクタを作ろうとした名残です。
とりあえず、FreeRTOSを動かす土台としては使えるかと思います。


以下の対応をしてあります。
- べアメタルで動かすためのbootloader作成
- 内蔵ペリフェラル用の各種ドライバ作成
- FreeRTOSのポーティング
- JTAGデバッグ用のjlinkマクロ作成

:::note info
lib配下にArduino対応させようとした名残がありますが、未完です。
内蔵のAUDIOCODECが悪く、愛想尽かしました。今後も対応の予定ありません。
:::

:::note info
内蔵のAUDIOCODECは音質が良くないため、外付けAUDIOCODEC推奨です。
※内蔵のAUDIOCODECの音質が悪いのはコードのせいかもしれません。
:::

<br><br>
# 必要な開発環境
- mingw（C:\MinGWにインストールされることを想定）  
  以下、インストール必須なライブラリ
    - mingw32-base-bin
    - mingw32-gcc-g++-bin
    - 環境変数にC:\MinGW\binのPATHを通すこと

- armのgcc（arm-none-uebi-gcc）  
バージョン指定は特にありません。最新のもので大丈夫です。

- zadig  
LicheePI Nanoに汎用USBドライバを適用するために利用。  
usb経由でspiフラッシュにプログラムを書き込むために必要です。

- mksunxi.exe
ビルドしたバイナリファイルをBROM※が認識できる形式に変換するソフト。
bootloaderのソースをビルドする必要がある場合のみ必要です。
※ググれば見つかりますが、野良っぽいのでご利用は自己責任で。

- sunxi-fel.exe
ビルドしたバイナリファイルをSPIフラッシュに書き込むソフト。
ビルドによってSPIフラッシュ内の書き換えを行いますので、
事前にSPIフラッシュのダンプを実行して下さい。
※flash_dump_licheepinano.bat

- vscode  
  必須ではありませんが、j-linkを使ってデバッグするのであれば入れた方が便利です。  
  以下、必要な拡張機能
    - C/C++ Extension Pack
    - Cortex-Debug

<br><br>

# ソースについて

# lib
ペリフェラル用のドライバやFreeRTOSのソースが入っています。

## bootloader
ベアメタルブートするために必要なブートローダです。
SPIフラッシュに書き込まれているメインソースをSDRAM上に展開します。
基本的に変更不要かと思います。

## sample_simple
最も簡単なサンプルです。

CPUが起動すると、FreeRTOSによってタスクが駆動されます。  
FreeRTOSに以下3つのタスクが割り当てられています。
- TestTask1
- TestTask2
- TestTask3

各タスクは所定の時間でReady、Blockedへの遷移を行います。
- TestTask1: 500msecでUARTへログ出力.
- TestTask2: 1000msecでUARTへログ出力.
- TestTask3: 2000msecでUARTへログ出力.

TestTask1はタスクが駆動される度にポートPE3の出力をトグルしています。
これは、LEDのON/OFFを想定しています。

また、タスクとは非同期にTIMER0が500msec周期で動作しており、
タイマ満了のコールバックでUARTへログ出力しています。

※JTAGを使えるようにしてあるため、ポートPF0、PF1、PF3、PF5は利用できません。
もしポートPF0、PF1、PF3、PF5を利用する場合は、USE_JTAGの定義をコメントアウトしてください。

## sample_delaystomp_fpn
ディレイエフェクタを作るために作ったソースです。
LicheeNanoのCPUはFPUを内蔵していないため、FPN（FixedPointNumber）で実装しましたが、
float型を使った場合の速度差との比較はしていません。
もしかしたら、残念な結果になっているかもしれません。知りません。

詳しくは・・・ソースを確認願います・・・

※JTAGを使えるようにしてあるため、ポートPF0、PF1、PF3、PF5は利用できません。
もしポートPF0、PF1、PF3、PF5を利用する場合は、USE_JTAGの定義をコメントアウトしてください。

# ビルド方法
## 事前準備
### SPIフラッシュについて
Licheepiは工場出荷状態では内蔵のSPIフラッシュにLinuxのブートイメージが書き込まれている。
本ソースでは、このSPIフラッシュに書き込みを行うため、必要に応じてSPIフラッシュのダンプが必要になる。

### FELブートについて
Licheepiは内蔵USB経由でSRAMへアクセスしたり、SPIフラッシュにアクセスしたりすることができる。
これを行うためにはFELブートを行う必要がある。

■FELブート実行手順
１．SDカードを抜いて、SPI0のMOSIピンをプルダウンする。（47オーム程度を推奨）
２．licheepinanoとPCをUSB接続する（電源ON）。
３．数秒したら、SPI0のMOSIピンのプルダウンを解除する。
※UART0にUSB-Serialコンバータを接続しておくと、初回であればU-Bootの起動ログが出ないため
　FELブートできているか確認する目安になる。

### usbドライバのインストール
Licheepiにビルドしたバイナリファイルを書き込むために必要。
FELブートした状態で、Licheepiのusb-micro端子とPCをUSBケーブルで接続する。

この時点ではデバイスマネージャーを確認すると「unknown Device」、ドライバがインストールされていない状態。
zadig-2.6.exeを利用してwinUSBドライバ（汎用ドライバ）をインストールする。

以下、インストール方法.
１．zadig-2.6.exeを実行
２．「options」の「List All Devices」にチェックを入れる
３．ドロップダウンリストでunknown Deviceを選択
４．Install Driverをクリックしてドライバーをインストールする。
　　（数分かかる）

### build_config.batの編集
ARM GCCコンパイラが保存されているフォルダをbuild_config.batに指定する必要がある。
build_config.bat内のGCCPATHをご自身のARMコンパイラが保存されているパスに書き換える。

例）ARM GCCコンパイラが「C:\MinGW\lib\gcc-arm-none-eabi-10.3-2021」に保存されている場合
　　set GCCPATH=C:\MinGW\lib\gcc-arm-none-eabi-10.3-2021\bin\

## ビルドの実行
各sampleフォルダ内に以下ビルド用バッチファイルを用意している。
- build.bat 
差分ビルドを実行する。その際、前回ビルドした時点から変更のあったソースのみビルドする。 
ビルド完了後、書き込み実施する。
- build_all.bat 
リビルドを実行する。すべてのソースファイルをビルドする。 
ビルド完了後、書き込み実施する。
- clean.bat 
生成済みのオブジェクトファイルを削除する。
- flashwrite.bat 
ビルド済みのバイナリファイルの書き込みを行う。