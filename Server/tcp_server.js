var express = require('express'),
    routes = require('routes'),
    dgram = require('net'),
    twitter = require('./ntwitter/lib/twitter.js'),
    twitterModule = require('./modules/twitter_module.js'),
    util = require('util')
    net = require('net');

var PORT = 33333;
var HOST = '0.0.0.0';
var filters =
    [
        '#happy', '#fablab',
        '#3dprinting', '#relab',
        '#liege', '#sad',
        '#processing', '#lasercutting',
        'fabjamliege', '#sex'
    ];
var params = {};
var pins = [
    {hashtag:'happy', pin: 1},{hashtag:'fablab', pin: 2},{hashtag:'3dprinting', pin : 3},
    {hashtag:'relab', pin:4},{hashtag:'liege', pin: 5},{hashtag:'sad', pin: 6},
    {hashtag:'processing', pin : 7},{hashtag:'lasercutting', pin : 8},{hashtag:'fabjamliege', pin :9},
    {hashtag: 'sex', pin: 10}
];

params.pins = pins;

// var server = dgram.createSocket('udp4');

var server = net.createServer(function(c){
    console.log('client connected');
    c.on('end', function(){
       console.log('client disconnected');
    });
    var twit = twitterModule.twit;

    twit.stream('statuses/filter',{'track': filters}, function(stream){twitterModule.consumeStream(stream, c, params)});
});
server.listen(PORT, function(){
   console.log('server created');
});
server.on('error', function(e){
    switch(e.code){
        case 'EADDRINUSE':{
            console.log('Address in use. Retrying...');
            setTimeout(function(){
                server.close();
                server.listen(PORT)
            },1000);
            break;
        }
        case 'EHOSTUNREACH':{
            console.log("Restarting server");
            setTimeout(function(){
               server.close();
                server.listen(PORT);
            }, 1000);
            break;
        }
        case 'ECONNRESET':{
            console.log('Restarting server');
            setTimeout(function(){
               server.close();
                server.listen(PORT);
            }, 1000);
            break;
        }
        default :{
            console.log('Error '+ e.code);
        }
    }
});

server.maxConnections = 7000;