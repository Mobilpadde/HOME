var Planet = function(seed){
	var seedrandom = require("seedrandom");
	var SimplexNoise = require("simplex-noise");
	var Colour = require("./colour");
	
	var rng = seedrandom(seed);
	var colour = new Colour(rng);
	var simplex = new SimplexNoise(rng);
	
	var birth = (new Date()).getTime();
	var generation = null;

	return {
		generate: function(size, multiplier, levels){
			var planet = [];
			
			for(var y = 0; y < size; y++){
				var p = "";
				
				var max = size * (59 / multiplier); // 
				for(var x = 0; x < max; x++){
					var n = Math.abs(simplex.noise2D(x / max, y / size));
					
					if(
						Math.sqrt(Math.pow(x - (size / 2 - size / 4), 2) + Math.pow(x - (size / 2 - size / 4), 2)) < ((size * (size / 2))) ||
						x > size
					){ //  * 0.45
						if(n < levels[0])
							p += "0";
						else if(n >= levels[0] && n < levels[1])
							p += "1";
						else
							p += "2";
					}
				}
				
				planet.push(p);
			}
			
			generation = {
				chunks: planet, 
				water: colour.water,
				land: colour.land,
				mountain: colour.mountain,
				text: colour.text
			};
		
			return generation;
		},
		
		get: function(){
			return generation;	
		},
		
		age: function(){
			return (new Date()).getTime() - birth;
		}
	};
};


module.exports = Planet;