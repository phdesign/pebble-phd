var utils = require('./utils.js');
var request = require('request');

var buildRequestUrl = function(lat, lon) {
    return 'http://api.openweathermap.org/data/2.5/weather?lat=' +
        lat + '&lon=' + lon + '&APPID=15664341af304192147d1fb3f38a1f67';
};

var readWeatherValues = function(rawData) {
  return {
    temp: utils.kelvinToCelsius(rawData.main.temp),
    conditions: rawData.weather[0].main 
  };
};

exports.getCurrentConditions = function(coords, done) {
  var url = buildRequestUrl(coords.latitude, coords.longitude);

  request({ url: url, json: true}, function(err, xhr, data) {
    console.log('Received weather data from Open Weather Map');

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
