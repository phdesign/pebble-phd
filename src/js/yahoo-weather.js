var utils = require('./utils.js');
var request = require('request');

var buildRequestOptions = function(lat, lon) {
  return {
    url: 'https://query.yahooapis.com/v1/public/yql',
    //url: 'http://localhost:8080/yahoo-weather.json',
    qs: {
      q: 'select * from weather.forecast where woeid in (select woeid from geo.placefinder(1) where text="' + lat + ',' + lon + '" and gflags="R")',
      format: 'json'
    },
    json: true,
    verbose: true
  };
};

var readWeatherValues = function(rawData) {
  var celsius = utils.fahreheitToCelsius(rawData.query.results.channel.item.condition.temp);
  return {
    temp: Math.round(celsius),
    conditions: rawData.query.results.channel.item.condition.text
  };
};

module.exports = {

  key: 'yahoo-weather',
  name: 'Yahoo! Weather',

  getCurrentConditions: function(coords, done) {
    var options = buildRequestOptions(coords.latitude, coords.longitude);

    request(options, function(err, xhr, data) {
      console.log('Received weather data from Yahoo Weather');
      console.log('First 255 bytes:', JSON.stringify(data).substr(0, 255));

      var values = readWeatherValues(data);
     
      if (!values || !values.temp || !values.conditions) {
        console.log('Unable to process weather data, aborting...');
        return;
      }
      console.log('Temperature is ' + values.temp);
      console.log('Conditions are ' + values.conditions);

      done(values);
    });
  }

};
