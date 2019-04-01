# Dolphin Ride
### EnOceanGateways/DolphinRide

## Dolphin Ride (dpride) -- オープンソースの基本的な EnOcean ゲートウェイ.

EnOcean デバイスやセンサーからデータを受信して、ブローカーに振り分け、通知します。

注意: 実行時は **eep.xml** ファイルが作業ディレクトリに必要。[DolphinView](https://www.enocean.com/download/) のインストールで入手可能。

### 使い方
```sh 
Usage: dpride [-m|-r|-o][-c][-v]
  [-d Directory][-f Controlfile][-e EEPfile][-b BrokerFile]
  [-s SeriaPort][-z CommandFile]
    -m    Monitor mode
    -r    Register mode
    -o    Operation mode
    -c    Clear settings before register
    -v    View working status
    -l    Output websocket log for logger client
    -d    Bridge file directrory
    -f    Control file
```

### リリース履歴
```sh 
Verion 1.00 releases notes.
 Fixed to apply radio filter.
 Fixed for ERP1 on 868MHz, 315MHz, 902MHz, and 928MHz.
 Fixed for 1BS, VLD and UTE telegram.
 Use signal SIG_BROKERS (SIGRTMIN + 6) to notify brokers immediately. 
 Receive signal SIGUSR2 to get external command while in working.

Verion 1.01 releases notes.
 Fix 4BS telegram analysys and internal EEP cache management.
```

### ブローカー

対応するブローカーの作り方は https://github.com/ahidaka/Open62541-Work を参照。

&copy; 2017-2019 Atomu Hidaka  All rights reserved.
