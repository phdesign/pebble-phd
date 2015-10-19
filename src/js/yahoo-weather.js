var utils = require('./utils.js');
var request = require('request');

var buildRequestUrl = function(lat, lon) {
  return 'https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.placefinder(1)%20where%20text%3D%22' + lat + '%2C' + lon + '%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&callback=';
};

var readWeatherValues = function(rawData) {
  var celsius = utils.fahreheitToCelsius(rawData.query.results.channel.item.condition.temp);
  return {
    temp: Math.round(celsius),
    conditions: rawData.query.results.channel.item.condition.text
  };
};

exports.getCurrentConditions = function(coords, done) {
  var url = buildRequestUrl(coords.latitude, coords.longitude);

  request({ url: url, json: true}, function(err, xhr, data) {
    console.log('Received weather data from Yahoo Weather');

    var values = readWeatherValues(data);
   
    if (!values || !values.temp || !values.conditions) {
      console.log('Unable to process weather data, aborting...');
      return;
    }
    console.log('Temperature is ' + values.temp);
    console.log('Conditions are ' + values.conditions);

    done(values);
  });
};
