module.exports = [
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Planet Seed"
			},
			{
				"type": "input",
				"messageKey": "seed",
				"label": "Seed",
				"attributes": {
					"placeholder": "batman",
					"type": "text"
				}
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Planet Size"
			},
			{
				"type": "slider",
				"messageKey": "planMul",
				"defaultValue": 0.5,
				"label": "Size multiplier",
				"description": "The size of your planet.",
				"min": 0,
				"max": 1,
				"step": 0.01
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Planet Levels"
			},
			{
				"type": "slider",
				"messageKey": "lLvl",
				"defaultValue": 0.3,
				"label": "Lower Level",
				"description": "Everything below this is water.",
				"min": 0,
				"max": 1,
				"step": 0.01
			},
			{
				"type": "slider",
				"messageKey": "uLvl",
				"defaultValue": 0.57,
				"label": "Upper Level",
				"description": "Everything between \"Lower Level\" and this is land, and everyhing above are mountains.",
				"min": 0,
				"max": 1,
				"step": 0.01
			}
		]
	},
	{
		"type": "submit",
		"defaultValue": "Save"
	}
];