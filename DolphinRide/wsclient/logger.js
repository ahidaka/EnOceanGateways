'use strict';

module.exports = {
    get: get,
    post: post
};

var fs = require("fs");
//var multer = require('multer');
//var execSync = require('child_process').execSync;
var utils = require('./utils');

function get(req, res, next) {
	utils.print("GET");
	var myAddr = utils.getMyAddr(req);
	console.log("get: myAddr=" + myAddr);
	res.render('logger', {
		wsurl: 'ws://' + myAddr + ':8088/echo',
	});
}

function post(req, res, next) {
	utils.print("POST");
	var myAddr = utils.getMyAddr(req);
	console.log("get: myAddr=" + myAddr);
	res.render('logger', {
		wsurl: 'ws://' + myAddr + ':8088/echo'
	});
}
