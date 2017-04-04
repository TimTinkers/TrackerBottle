var express = require('express');
var bodyParser = require('body-parser')

var app = express();
app.set('view engine', 'ejs');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
	extended: true
}));

app.get('/track', function (req, res) {
	res.render('track_test');
});

app.get('/setWeight', function (req, res) {
	var weight = req.query.weight;
	console.log("Got weight " + weight);
	res.send("ok");
});

app.listen(3000, function () {
	console.log('Tracker Server listening on port 3000!');
});