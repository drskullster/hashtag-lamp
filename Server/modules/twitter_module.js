var twitter = require('ntwitter'),
	util = require('util');

module.exports.twit = new twitter({
	consumer_key: '',
    consumer_secret: '',
    access_token_key: '',
    access_token_secret:''
});

module.exports.consumeStream =  function(stream, socket, params){
    var pins = params.pins;
    stream.on('data', function(data){
        // console.log(data);
        if(data != 'undefined'){
            try{
                //console.log("sending tweet "+data.text);
                var htlength = data.entities.hashtags.length;
                for(var i = 0; i< htlength; i++){
                    var pin = 0;
                    var ht = data.entities.hashtags[i].text;

                    var pinlgt = pins.length;
                    for(var j = 0; j < pinlgt; j++){
                        if(pins[j].hashtag == ht.toLowerCase()){
                            pin = pins[j].pin;
                        }
                    }
                    if(pin > 0){
                        console.log('sending '+data.text);
                        socket.write(pin+'|'+data.text.length+'<');
                        socket.pipe(socket);
                    }
                }

            }catch(err){
                console.log("error "+err );
            }

        }
    });
};
