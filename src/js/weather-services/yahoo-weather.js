var utils = require('../utils.js');
var request = require('request');

function getShortConditionText(conditionCode) {
  var remap = {
    '0': 'tornado', //tornado
    '1': 'storms', //tropical storm
    '2': 'storms', //hurricane
    '3': 'storms', //severe thunderstorms
    '4': 'storms', //thunderstorms
    '5': 'snow', //mixed rain and snow
    '6': 'sleet', //mixed rain and sleet
    '7': 'sleet', //mixed snow and sleet
    '8': 'drizzle', //freezing drizzle
    '9': 'drizzle', //drizzle
    '10': 'rain', //freezing rain
    '11': 'showers', //showers
    '12': 'showers', //showers
    '13': 'snow', //snow flurries
    '14': 'snow', //light snow showers
    '15': 'snow', //blowing snow
    '16': 'snow', //snow
    '17': 'hail', //hail
    '18': 'sleet', //sleet
    '19': 'dust', //dust
    '20': 'foggy', //foggy
    '21': 'haze', //haze
    '22': 'smoky', //smoky
    '23': 'blustery', //blustery
    '24': 'windy', //windy
    '25': 'cold', //cold
    '26': 'cloudy', //cloudy
    '27': 'cloudy', //mostly cloudy (night)
    '28': 'cloudy', //mostly cloudy (day)
    '29': 'cloudy', //partly cloudy (night)
    '30': 'cloudy', //partly cloudy (day)
    '31': 'clear', //clear (night)
    '32': 'sunny', //sunny
    '33': 'fair', //fair (night)
    '34': 'fair', //fair (day)
    '35': 'hail', //mixed rain and hail
    '36': 'hot', //hot
    '37': 'storms', //isolated thunderstorms
    '38': 'storms', //scattered thunderstorms
    '39': 'storms', //scattered thunderstorms
    '40': 'storms', //scattered showers
    '41': 'snow', //heavy snow
    '42': 'snow', //scattered snow showers
    '43': 'snow', //heavy snow
    '44': 'cloudy', //partly cloudy
    '45': 'showers', //thundershowers
    '46': 'snow', //snow showers
    '47': 'storms', //isolated thundershowers
    '3200': 'unknown' // not available
  };

  return remap[conditionCode] || 'unknown';
}

function buildRequestOptions(lat, lon) {
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
}

function readWeatherValues(rawData) {
  var celsius = utils.fahreheitToCelsius(rawData.query.results.channel.item.condition.temp);
  return {
    temp: Math.round(celsius),
    conditions: getShortConditionText(rawData.query.results.channel.item.condition.code)
  };
}

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
