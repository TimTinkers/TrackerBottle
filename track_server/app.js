var express = require('express');
var bodyParser = require('body-parser')

var app = express();
app.use(express.static('static'));
app.set('view engine', 'ejs');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
	extended: true
}));

app.get('/dashboard', function (req, res) {

	var data = {
		goal: 100,
		consumption: 43,
		events: [
			{
				time: "10:03:34 01/04/2012",
				weight: 456
			},
			{
				time: "11:33:56 01/04/2012",
				weight: 345
			},
			{
				time: "12:01:00 01/04/2012",
				weight: 234
			}
		]
	}

	res.render('dashboard', data);
});

app.get('/setWeight', function (req, res) {
	var weight = req.query.weight;
	console.log("Got weight " + weight);
	res.send("ok");
});

app.listen(3000, function () {
	console.log('Tracker Server listening on port 3000!');
});
