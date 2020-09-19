'use strict';

var express = require('express');
var logger = require('./logger');
var utils = require('./utils');

var app = express();
app.set('view options', { layout: false });
app.set('view engine', 'ejs');
app.use('/', express.static('public'));

if (require.main === module) {
    main()
}

function main() {
    utils.print('Starting...');

    app.get ("/",
    	(req, res, next) => logger.get(req, res, next)
    );
    app.post("/",
	    (req, res, next) => logger.post(req, res, next)
    );

    utils.print('OK');
}

var server = app.listen(80, function() {
    var host = server.address().address;
    var port = server.address().port;
    console.log("Server on http://%s:%s", host, port);
});
