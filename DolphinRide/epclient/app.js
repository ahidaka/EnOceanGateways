'use strict';

//var logger = require('./logger');
//var utils = require('./utils');


const SENSOR_PORT = 8000;
const SENSOR_HOST = "127.0.0.1";
const HTTP_PORT = 80;

var net = require("net");
var client = new net.Socket();

var express = require('express');
var app = express();

//app.set('view options', { layout: false });
//app.set('view engine', 'ejs');

app.use('/', express.static('public'));

function main() {
    console.log('Starting...');

    var http_server = require('http').Server(app);
    var socketio = require('socket.io')(http_server);

    http_server.listen(HTTP_PORT, () => {
    	console.log('listening on *:' + HTTP_PORT);
    });

    socketio.on('connection', (socket) => {
	console.log('sensor connected');
	socket.on('disconnect', () => {
	    console.log('sensor disconnected');
    });
    
    client.connect(SENSOR_PORT, SENSOR_HOST, function() {
    	console.log("connect to: " + SENSOR_HOST + ":" + SENSOR_PORT);
    });

    client.on("data", function(data) {
    	//console.log(data.toString());
	    //socketio.emit('message', data.toString());
	    socketio.emit('message', function(d) {
            var s = d.toString();
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
            return s;
        });
    });

    client.on("close", function() {
	    console.log("disconnect from: " + SENSOR_HOST + ":" + SENSOR_PORT);
    })

    //app.get('/', (req, res) => {
    //  res.sendFile(__dirname + 'index.html');
    //});

    console.log('OK');
}

if (require.main === module) {
    main()
}
