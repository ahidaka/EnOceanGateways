'use strict';
var net = require("net");
var host = "127.0.0.1";
var port = 8000;
var client = new net.Socket();

client.connect(port, host, function() {
    console.log("connect to: " + host + ":" + port);
});

client.on("data", function(data) {
    //console.log(data.toString());
    var s = data.toString();

    // Clear non printable character and omit error
    s = s.replace(/\\n/g, "\\n")
	.replace(/\\'/g, "\\'")
	.replace(/\\"/g, '\\"')
	.replace(/\\&/g, "\\&")
	.replace(/\\r/g, "\\r")
	.replace(/\\t/g, "\\t")
	.replace(/\\b/g, "\\b")
	.replace(/\\f/g, "\\f");
    s = s.replace(/[\u0000-\u0019]+/g,"");

    var obj = JSON.parse(s);
    //console.log(obj.dataTelegram);

// format samples
//    { deviceId: '0400200F',
//      friendlyId: 'Temperature and Humidity Sensor',
//      direction: 'from',
//      security: '0',
//      profile: 'A5-04-01',
//      timestamp: '09/18/20 22:48:41',
//      functions:
//      [ { key: 'HUM', value: '34.00', unit: '%' },
//	{ key: 'TMP', value: '36.16', unit: '°C' },
//      telegramInfo: { dbm: -80, rorg: 'A5' } }
    
    var dataTelegram = obj.dataTelegram;
    var dataArray = dataTelegram.functions;
    console.log(dataTelegram.deviceId, dataTelegram.profile);
    dataArray.forEach(function(dataNode) {
	console.log(" ", 
	    "key=" + dataNode.key,
	    "value=" + dataNode.value,
	    "unit=" + dataNode.unit);
    });
});

client.on("close", function() {
    console.log("disconnect from: " + host + ":" + port);
})
