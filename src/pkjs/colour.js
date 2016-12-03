// TODO: Made seedable (alea)
var Colour = function(rng){
	/*var colours = require("./colours.json");
	
	return {
		//base: colours[~~(rng() * colours.length)],
		text: colours[~~(rng() * colours.length)],
		water: colours[~~(rng() * colours.length)],
		land: colours[~~(rng() * colours.length)],
		mountain: colours[~~(rng() * colours.length)]
	};*/
	
	// https://gist.github.com/matthewtole/49da38377b0b0a423407
	function GColorFromHex(hex) {
		var hexNum = parseInt(hex, 16);
		var a = 192;
		var r = (((hexNum >> 16) & 0xFF) >> 6) << 4;
		var g = (((hexNum >>  8) & 0xFF) >> 6) << 2;
		var b = (((hexNum >>  0) & 0xFF) >> 6) << 0;
		return a + r + g + b;
	}
	
	var _base = null, _water = null, _land = null, _mountain = null, _text = null;
	
	var fixer = function(num){
		var str = Math.abs(num).toString(16);
		
		while(str.length < 2) str = "f" + str;
 		if(str.length > 2) str = str.slice(0, 2);
		
		return str;
	};
	
	var rnd = function(min, max){
		return ~~(rng() * (max - min)) + min;
	};
	
	var base = function(r, g, b){
		var c = {
			r: r ? r : ~~(rng() * 256),
			g: g ? g : ~~(rng() * 256),
			b: b ? b : ~~(rng() * 256)
		};

		return c;
	};
	
	_base = base();
	var gen = function(mix){
		var r = ~~(Math.abs(_base.r + mix.r) / 2);
		var g = ~~(Math.abs(_base.g + mix.g) / 2);
		var b = ~~(Math.abs(_base.b + mix.b) / 2);
		
		console.log("Base", r, g, b);
		return GColorFromHex(fixer(r) + fixer(g) + fixer(b));
	};
	
	_water = gen(base(256, 200, 100));
	_land = gen(base(100, 200, 256));
	_mountain = gen(base(200, 100, 256));
	
	 (function(){
		var r = Math.min((256 - parseInt(_base.r)) + rnd(0, 255), 255);
		var g = Math.min((256 - parseInt(_base.g)) + rnd(0, 255), 255);
		var b = Math.min((256 - parseInt(_base.b)) + rnd(0, 255), 255);

		_text = GColorFromHex(fixer("ff") + fixer("ff") + fixer("ff"));
	})();
	
	return {
		base: _base,
		text: _text,
		water: _water,
		land: _land,
		mountain: _mountain
	};
};

module.exports = Colour;