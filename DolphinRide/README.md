# Dolphin Ride
### EnOceanGateways/DolphinRide

### [日本語はこちら](README_ja.md)

## Dolphin Ride (dpride) -- The open source basic EnOcean gateway.

Note: Need eep.xml from [DolphinView](https://www.enocean.com/download/) in your working directory.

### How to use
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

### History
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

### Broker

You can see how to make brokers at https://github.com/ahidaka/Open62541-Work.

&copy; 2017-2019 Atomu Hidaka  All rights reserved.