'use strict';

var issuepath="/etc/issue";

module.exports = {
    get: get,
    post: post,
    value: value,
	print: print,
	isExist: isExist,
	isNumber: isNumber,
	isValidChar: isValidChar,	
	getMyName: getMyName,
	getMyAddr: getMyAddr
};

var express = require('express');
var fs = require("fs");
var ip = require('ip');
var execSync = require('child_process').execSync;

const checkedMark = 'checked="checked"';
const valueLeading = 'value="';
const valuetrailing = '"';
var myname;
var myAddr;

function value(str) {
    return(valueLeading + str + valuetrailing);
}

function get(req, res) {
    res.send("utils.get");
}

function post(req, res) {
    res.send("utils.post");
}

function print(req, res) {
    console.log(req);
}

function isExist(filePath) {
	try {
		fs.statSync(filePath);
	}
	catch(err) {
		return false;
	}
	return true;
}

function isNumber(target) {
	const numbers = /^\d+$/;
	return target.match(numbers);
}

function isValidChar(target) {
	const valids = /^[-\+=\.\w;]+$/;
	return target.match(valids);
}

function getMyAddr(req) {
	var targetAddr = "0.0.0.0";
	var lines = [];
	var ipRoute;
	var ipRouteLines;
	var i = 0;

	if (myAddr != null && myAddr.length >= 7 && myAddr != "0.0.0.0") {
		console.log("MYADDR:" + myAddr);
		return myAddr;
	}

	if (req == null || req.headers == null) {
		console.log("MYADDR: req==null");
		return null;
	}

	if (req.connection && req.connection.remoteAddress) {
		console.log("MYADDR: req.connection.remote=" + req.connection.remoteAddress);
		targetAddr = req.connection.remoteAddress;
	}
	if (req.connection.socket && req.connection.socket.remoteAddress) {
		console.log("MYADDR: req.connection.socket.remote=" + req.connection.socket.remoteAddress);
		targetAddr = req.connection.socket.remoteAddress;
	}
	if (req.socket && req.socket.remoteAddress) {
		console.log("MYADDR: req.socket.remote=" + req.socket.remoteAddress);
		targetAddr = req.socket.remoteAddress;
	}
	console.log("target=<%s>", targetAddr);
	if (targetAddr.indexOf(':') != -1) 
	{
		var temps = String(targetAddr).split(':');
		console.log("MYADDR: temps<%s>", JSON.stringify(temps));
		targetAddr = String(temps[3]);
	}
	console.log("newTarget=<%s>", targetAddr);

	ipRoute = execSync("ip route");
	console.log("route=<%s>", ipRoute);
	ipRouteLines = String(ipRoute).split('\n');
	for(var line of ipRouteLines) {
		var temps;
		var networkCidr;
		var targetCidr;
		var items = line.split(/ +/);

		console.log("MYADDR(%d): line<%s>", i, line);
	
		if (items[0] == "default")
			continue;
		temps = String(items[0]).split('/');
		lines[i] = {};
		lines[i].cidr = String(items[0]);
		//lines[i].maskPtn = String(temps[0]);
		lines[i].maskBit = Number(temps[1]);
		//lines[i].intf = String(items[2]);
		console.log("MYADDR(%d): lines<%s>temps<%s>", i, JSON.stringify(lines[i]), JSON.stringify(temps));
		if (items[7] == "src") {
			lines[i].addr = String(items[8]);
			//console.log("line[%d].mask=%s bit=%d intf=%s addr=%s",
			//          i, lines[i].maskPtn, lines[i].maskBit, lines[i].intf, lines[i].addr);
			networkCidr = ip.cidr(lines[i].cidr);
			console.log("NET(%d): cidr=%s", i, networkCidr);
			//console.log("MY : cidr=%s", ip.cidr(lines[i].addr + '/' + lines[i].maskBit));
			targetCidr = ip.cidr(targetAddr + '/' + lines[i].maskBit);
			console.log("TGT(%d): cidr=%s", i, targetCidr);
			if (networkCidr == targetCidr) {
				myAddr = lines[i].addr;
				console.log("TGT(%d) matched: myaddr=%s", i, myAddr);
				break;
			}
		}
		i++;
	}
	return myAddr;
}

function getMyName() {
    const grep_a_At = "grep -a '^@ .* $'";
	var name;
	var dotName;
	var words;
	var ipAddr;
	var rtn;

	console.log("getMyName: myname=%s", myname);
	if (myname != null && myname != "") {
		return myname;
	}
    name = execSync("hostname");
	console.log("getMyName:hostname=%s", name);
	if (name.indexOf('.') == -1) {
		console.log("hostname doesn't include '.'");
	}
	else {
		dotName = name;
	}
	rtn = execSync("ip route");
	//rtn = execSync("ls");
	console.log("<%s>", rtn);
	//words = rtn.split(/[ \n\r]+/);
	words = (String(rtn)).split(/\s+/);
	for(var i=0; i < words.length; i++) {
		if (words[i] == "src") {
			ipAddr = words[i + 1];
			if (ipAddr.indexOf('.') == -1) {
				console.log("ipAddr doesn't include '.'");
				ipAddr = null;
			}
		}
	}

	if (dotName != null) {
		name = dotName;
		console.log("name: dotname");		
	}
	else if (ipAddr != null) {
		name = ipAddr;
		console.log("name: ipaddr");
	}
	else if (name != null) {
		;//name = name;
		console.log("other");		
	}

	console.log("name: <%s>(%s)[%d]", name, typeof(name), name.length);

	myname = String(name).replace(/\n/,'');
	console.log("getMyName: %s", myname);
	return myname;
}
