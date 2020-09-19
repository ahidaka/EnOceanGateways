'use strict';
var net = require("net");
var host = "127.0.0.1";
var port = 8000;
var client = new net.Socket();

client.connect(port, host, function() {
    console.log("connect to: " + host + ":" + port);
});

client.on("data", function(data) {
    console.log(data.toString());
});

client.on("close", function() {
    console.log("disconnect from: " + host + ":" + port);
})
