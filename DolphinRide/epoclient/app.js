'use strict';

//var logger = require('./logger');
//var utils = require('./utils');

const SENSOR_PORT = 8000;
const SENSOR_HOST = "127.0.0.1";
const HTTP_PORT = 80;

var net = require("net");
var express = require('express');
var app = express();

//app.set('view options', { layout: false });
//app.set('view engine', 'ejs');

app.use('/', express.static('public'));

function main() {

    //var client = new net.Socket();
    const port = SENSOR_PORT;
    const host = SENSOR_HOST;
    const timeout = 5000;
    let retrying = false;

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
    });

    function memo() {
    client.on("data", function(data) {
    	//console.log(data.toString());
        //socketio.emit('message', data.toString());
        
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
        socketio.emit('message', s);
    });
    }

    // Functions to handle socket events
    function makeConnection () {
        console.log('makeConnection');
        client.connect(port, host);
    }

    function connectEventHandler() {
        console.log('connected');
        retrying = false;
    }

    function dataEventHandler(data) {
        console.log('data');

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
        socketio.emit('message', s);
    }

    function endEventHandler() {
        console.log('end');
    }

    function timeoutEventHandler() {
        console.log('timeout');
    }

    function drainEventHandler() {
        console.log('drain');
    }

    function errorEventHandler() {
        console.log('error');
    }

    function closeEventHandler () {
        console.log('close');
        if (!retrying) {
            retrying = true;
            console.log('Reconnecting...');
        }
        setTimeout(makeConnection, timeout);
    }

    // Create socket and bind callbacks
    let client = new net.Socket();
    client.on('connect', connectEventHandler);
    client.on('data',    (data) => dataEventHandler(data));
    client.on('end',     endEventHandler);
    client.on('timeout', timeoutEventHandler);
    client.on('drain',   drainEventHandler);
    client.on('error',   errorEventHandler);
    client.on('close',   closeEventHandler);

    // Connect
    console.log('Connecting to ' + host + ':' + port + '...');
    makeConnection();

    console.log('OK');
}

if (require.main === module) {
    main();
    console.log('main exit');
}
