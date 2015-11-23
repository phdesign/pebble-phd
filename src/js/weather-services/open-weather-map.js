var utils = require('../utils.js');
var request = require('request');
var keys = require('../../../keys.json');

function getShortConditionText(conditionCode) {
  var remap = {
    '200': 'storms', // thunderstorm with light rain
    '201': 'storms', // thunderstorm with rain
    '202': 'storms', // thunderstorm with heavy rain
    '210': 'storms', // light thunderstorm
    '211': 'storms', // thunderstorm
    '212': 'storms', // heavy thunderstorm
    '221': 'storms', // ragged thunderstorm
    '230': 'storms', // thunderstorm with light drizzle
    '231': 'storms', // thunderstorm with drizzle
    '232': 'storms', // thunderstorm with heavy drizzle
    '300': 'drizzle', // light intensity drizzle
    '301': 'drizzle', // drizzle
    '302': 'drizzle', // heavy intensity drizzle
    '310': 'drizzle', // light intensity drizzle rain
    '311': 'drizzle', // drizzle rain
    '312': 'drizzle', // heavy intensity drizzle rain
    '313': 'drizzle', // shower rain and drizzle
    '314': 'drizzle', // heavy shower rain and drizzle
    '321': 'drizzle', // shower drizzle
    '500': 'rain', // light rain
    '501': 'rain', // moderate rain
    '502': 'rain', // heavy intensity rain
    '503': 'rain', // very heavy rain
    '504': 'rain', // extreme rain
    '511': 'rain', // freezing rain
    '520': 'rain', // light intensity shower rain
    '521': 'rain', // shower rain
    '522': 'rain', // heavy intensity shower rain
    '531': 'rain', // ragged shower rain
    '600': 'snow', // light snow
    '601': 'snow', // snow
    '602': 'snow', // heavy snow
    '611': 'sleet', // sleet
    '612': 'sleet', // shower sleet
    '615': 'snow', // light rain and snow
    '616': 'snow', // rain and snow
    '620': 'snow', // light shower snow
    '621': 'snow', // shower snow
    '622': 'snow', // heavy shower snow
    '701': 'mist', // mist
    '711': 'smoke', // smoke
    '721': 'haze', // haze
    '731': 'dust', // Sand/Dust Whirls
    '741': 'fog', // Fog
    '751': 'sand', // sand
    '761': 'dust', // dust
    '762': 'ash', // VOLCANIC ASH
    '771': 'squalls', // SQUALLS
    '781': 'tornado', // TORNADO
    '800': 'clear', // sky is clear
    '801': 'cloudy', // few clouds
    '802': 'cloudy', // scattered clouds
    '803': 'cloudy', // broken clouds
    '804': 'cloudy', // overcast clouds
    '900': 'tornado', // tornado
    '901': 'storms', // tropical storm
    '902': 'storms', // hurricane
    '903': 'cold', // cold
    '904': 'hot', // hot
    '905': 'windy', // windy
    '906': 'hail', // hail
    '950': 'setting', // Setting
    '951': 'calm', // Calm
    '952': 'breezy', // Light breeze
    '953': 'breezy', // Gentle Breeze
    '954': 'breezy', // Moderate breeze
    '955': 'breezy', // Fresh Breeze
    '956': 'breezy', // Strong breeze
    '957': 'gale', // High wind, near gale
    '958': 'gale', // Gale
    '959': 'gale', // Severe Gale
    '960': 'storms', // Storm
    '961': 'storms', // Violent Storm
    '962': 'storms' // Hurricane
  };

  return remap[conditionCode] || 'unknown';
}

function buildRequestUrl(lat, lon) {
    return 'http://api.openweathermap.org/data/2.5/weather?lat=' +
        lat + '&lon=' + lon + '&APPID=' + keys.openWeatherMapAppId;
}

function readWeatherValues(rawData) {
  return {
    temp: utils.kelvinToCelsius(rawData.main.temp),
    conditions: getShortConditionText(rawData.weather[0].id)
  };
}

module.exports = {

  key: 'open-weather-map',
  name: 'Open Weather Map',

  getCurrentConditions: function(coords, done) {
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
  }

};
