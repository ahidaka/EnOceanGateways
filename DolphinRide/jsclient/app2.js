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
    console.log(obj.dataTelegram);
});

client.on("close", function() {
    console.log("disconnect from: " + host + ":" + port);
})
