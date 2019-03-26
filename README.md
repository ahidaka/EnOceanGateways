# EnOceanGateways
EnOcean Gateways

## DolphinRide (dpride) -- The basic EnOcean gateway.
Note: Need eep.xml (from DolphinView) in your woring directory.

Verion 1.00 releases notes.
 Fixed to apply radio filter.
 Fixed for ERP1 on 868MHz, 315MHz, 902MHz, and 928MHz.
 Fixed for 1BS, VLD and UTE telegram.
 Use signal SIG_BROKERS (SIGRTMIN + 6) to notify brokers immediately. 
 Receive signal SIGUSR2 to get external command while in working.

Verion 1.01 releases notes.
 Fix 4BS telegram analysys and internal EEP cache management.

You can see how to make brokers at https://github.com/ahidaka/Open62541-Work.

## AzureSupport -- The tools to support Microsoft Azure IoT Hub connection.

Add Microsoft Azure Support tools at [AzureIoTSupport](/AzureIoTSupport/).


### de:code 2019
de:code 2019 サポートソフトウェアは [DolphinRide](/DolphinRide/) と [AzureIoTSupport](/AzureIoTSupport/) 以下にあります。

&copy; 2017-2019 Atomu Hidaka  All rights reserved.
