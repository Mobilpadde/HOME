var DEBUG = false, K_DEBUG = false;

var KiezelPay = require('kiezelpay-core');
var kiezelpay = new KiezelPay(K_DEBUG);

var statuses = require("./statuses.json");
var keys = require("message_keys");

var Clay = require("pebble-clay");
var config = require("./config");
var clay =  new Clay(config);

var Planet = require("./planet");

var settings = {  
	"seed":{  
		"value":"HOME"
	},
	"planMul":{  
		"value":0.5,
		"precision":2
	},
	"lLvl":{  
		"value":0.3,
		"precision":2
	},
	"uLvl":{  
		"value":0.57,
		"precision":2
	},
	"background":{  
		"value":0
	},
	"star":{  
		"value":16777215
	}
};

var planet = new Planet(settings.seed.value);

function messageSuccess(e){
	if(DEBUG)
		console.log("Message send succeeded: " + JSON.stringify(e));  
}

function messageFailure(e){
	if(DEBUG)
		console.log("Message send failed: " + JSON.stringify(e));
}

function send(payload){
	if(DEBUG)
		console.log("Sending: " + JSON.stringify(payload));
	Pebble.sendAppMessage(payload, messageSuccess, messageFailure);
}

function sendChunk(chunks, idx){
	var key = keys.chunk;
	var dict = { status: statuses.CHUNK, malloc: chunks[idx].length };
	dict[key] = chunks[idx];
	Pebble.sendAppMessage(dict, messageSuccess, function(){
		console.log("Failed at idx: " + idx);
	});
	
	if(++idx < chunks.length){
		sendChunk(chunks, idx);
	}else
		console.log("Transfer complete");
}

function planeteer(e){
	var plan = {};
	//if(planet.get() === null || planet.age() > 60000)
		plan = planet.generate(e.size, e.multiplier, [settings.lLvl.value, settings.uLvl.value]);
	//else 
	//	plan = planet.get();

	var p = {
		status: statuses.COLOURS,
		water: plan.water,
		land: plan.land,
		mountain: plan.mountain,
		text: plan.text
	};
	
	return p;
}

Pebble.addEventListener("ready", function(e){
	send({ status: statuses.READY });
});

Pebble.addEventListener("appmessage", function(e){
	e = e.payload;
	
	if(DEBUG)
		console.log("Received Message: " + JSON.stringify(e));
	
	if(e.status == 1)
		send(planeteer(e));
	else if(e.status == 2)
		sendChunk(planet.get().chunks, 0);
});

Pebble.addEventListener("webviewclosed", function(e){
	var oldSeed = settings.seed.value;
	
	settings = clay.getSettings(e.response, true);
	
	if(oldSeed !== settings.seed.value)
		planet = new Planet(settings.seed.value);
	
	send(planeteer(e));
	
	console.log(JSON.stringify(settings));
});