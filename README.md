# EnOceanGateways
EnOcean Gateways

DolphinRide (dpride) -- The basic EnOcean gateway.
Note: Need eep.xml (from DolphinView) in your woring directory.

Verion 1.00 releases notes.
 Fixed to apply radio filter.
 Fixed for ERP1 on 868MHz, 315MHz, 902MHz, and 928MHz.
 Fixed for 1BS, VLD and UTE telegram.
 Use signal SIG_BROKERS (SIGRTMIN + 6) to notify brokers immediately. 
 Receive signal SIGUSR2 to get external command while in working.

You can see how to make brokers at https://github.com/ahidaka/Open62541-Work.


