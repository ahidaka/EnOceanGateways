# EnOcean Gateways -- de:code 2019 用サンプルコード解説
### EnOceanGateways / README_ja.md 

## 概要

ここで紹介するサンプルソースコード群は、[de:code 2018 シアターセッション(DA61)](https://www.microsoft.com/ja-jp/events/decode/2018/sessions.aspx#DA61) の進化形で、Raspberry pi等の一般的なLinux上で動作する、次のプログラムを含みます。また昨年発売された、[EnOcean IoT スターターキットや E-Kit EnOceanゲートウェイ](http://e-kit.jp/products/EnOcean/index.htm#EO-IOTKIT) でもこれらのツールを使用しています。

### Dolphin Ride (dpride)
    - EnOceanデバイス上のボタンを押すだけでIoT Hubにデバイス登録するEnOcean IoTデータ通信ゲートウェイ
    - 各種フォーマットのEnOceanセンサーのデータをAzure IoT Hub に転送します。
    
 詳細は [Dolphin Ride](/DolphinRide/) 参照。

### simulated_device_cloud_upload_sample (IoT Edge V1)
    - Azure IoT Hub / Azure IoT Central 接続をサポートします。
    - [Azure IoT Edge V1](https://github.com/Azure/iot-edge-v1) のsimulated_device_cloud_upload_sampleをEnOcean / Dolphin Ride に対応する様に改造したものです。
    - 以前は２秒おきのポーリングでデータファイルを読み出していましたが、現在は POSIX Signal を使用して、ほぼリアルタイムでデータ転送します。
    
詳細は [iot-edge-hacks](https://github.com/ahidaka/iot-edge-hacks/) 参照。  

### iothub_registrymanager_sample
    - [IoTHub-Explorer](https://www.npmjs.com/package/iothub-explorer) や Azure IoT Toolkit (Python) に代わるコマンドライン型の高速 IoT Hub デバイス・メンテナンス・ツール。
    - [Azure-IoT-SDK-C](https://github.com/Azure/azure-iot-sdk-c) のサンプルを改造して、REST で直接 IoT Hub にアクセスするため、高速です。使い勝手は、IoTHub-Explorer に合わせています。
    
詳細は [改造版 azure-iot-sdk-c の iothub_registrymanager_sample](https://github.com/ahidaka/azure-iot-sdk-c/tree/registrymanager_sample/iothub_service_client/samples/iothub_registrymanager_sample) 参照。  

### Azure IoT Support ツール群
    - IoT Edge では JSON で設定ファイルを基準するため、メンテナンスが面倒です。前述のDolphin Ride (dpriode) / simulated_device_cloud_upload_sample (IoT Edge V1) 使用時にこの JSON ファイルを自動メンテナンスするツールを作りました。これにより、デバイスの追加・変更・削除時に Azure Portal, Device Exploror, 設定用 JSON ファイルのいずれも触らずに Azure IoT Hub の登録デバイスを自動メンテナンス
    
詳細は [AzureIoTSupport](https://github.com/ahidaka/EnOceanGateways/tree/master/AzureIoTSupport) 参照。

## 解説：何をするものか

これらのツールをLinux上にインストールして使用することで、各種の [EnOcean IoT デバイス（センサー）](http://enocean.jp/)のデータを受信して、Microsoft Azure IoT Hub にデータ転送します。さらに Azure IoT Hub で EnOcean のデバイスを使い易くするため、EnOcean デバイス固有の機能を活用した下記の特長を持ちます。

* LEARN ボタン・サポート
    - EnOcean 各センサーが持つLEARNボタンをサポートし、ボタンを押すだけで Azure IoT Hub に各センサーを自動登録します。
    - ロッカースイッチには個別のLEARN ボタンがありません。スイッチ自体が LEARN ボタンとして機能します。
    
* プロファイル
    - EEP (EnOcean Equipment Profile) に対応しているため、登録したセンサーがどの様なセンサーなのかを知ることができ、センサーデータ実データへの変換を自動的に行います。

* IDの自動命名
    - EEP データベースが持つ「Shorcut Name」の名前（2～4文字の英数字）を流用して自動的に連番を振り、自動登録するセンサーの ID を命名します。
    - この機能により温度センサーは TMP, TMP1,... 湿度センサーは HUM, HUM1,... のIDが付加されるため、IoT Hub で扱い易くなります。

* ドメイン名
    - 他のプロトコル利用や、異なったロケーションでの運用を想定して、各センサーが使用する ID には、'.' (ピリオド)で区切る「ドメイン名」を付加します。

## 必要機材

使用（動作確認）するためには次の機材が必要です。

* インターネット接続済のLinuxマシン
    - Raspberry Pi 3 と Ubuntu 18.04 搭載のPCで動作確認しています。

* 前述のLinuxマシンに接続するEnOcean USB 受信ドングル
    - [USB400J](http://e-kit.jp/products/EnOcean/index.htm#USB400J) または同等品
  
* テスト用 EnOcean センサーまたはスイッチ
    - [STM431J](http://e-kit.jp/products/EnOcean/index.htm#STM431J) など多数市販済
  

&copy; 2017-2019 Atomu Hidaka  All rights reserved.
