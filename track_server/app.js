var express = require('express');
var bodyParser = require('body-parser')

var app = express();
app.use(express.static('static'));
app.set('view engine', 'ejs');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
	extended: true
}));

var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/trackerBottle');
var WM = mongoose.model('WeightMeasurement',
	{
		time: { type: Date, default: Date.now },
		weight: Number
	});

app.get('/dashboard', function (req, res) {

	WM.find({}, 'weight time', function (err, measurements) {
		if (err) {
			console.log(err);
		}
		console.log(measurements);

		var consumption = 0;
		for (var i=0; i < measurements.length; i++) {
			consumption += measurements[i].weight;
		}

		var data = {
			goal: 100,
			consumption: consumption,
			events: measurements
		}

		res.render('dashboard', data);
	});

	// var data = {
	// 	goal: 100,
	// 	consumption: 43,
	// 	events: [
	// 		{
	// 			time: "10:03:34 01/04/2012",
	// 			weight: 456
	// 		},
	// 		{
	// 			time: "11:33:56 01/04/2012",
	// 			weight: 345
	// 		},
	// 		{
	// 			time: "12:01:00 01/04/2012",
	// 			weight: 234
	// 		}
	// 	]
	// }
});

app.get('/setWeight', function (req, res) {
	var weight_value = req.query.weight;
	console.log("Got weight " + weight_value);

	var wm = new WM({ weight: weight_value });
		wm.save(function (err) {
		  if (err) {
		    console.log(err);
		  } else {
		    console.log('Saved weight: ' + weight_value);
		  }
		});

	res.send("ok");
});

app.listen(3000, function () {
	console.log('Tracker Server listening on port 3000!');
});
