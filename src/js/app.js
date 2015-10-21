var utils = require('./utils.js');
var openWeatherMapApi = require('./open-weather-map.js');
var bomApi = require('./bom.js');
var yahooApi = require('./yahoo-weather.js');

// Determine which weather provider to use
var weatherApi = yahooApi;

function getCoordinates(callback) {
  //callback({ latitude: -37.123, longitude: 144.123 });
  navigator.geolocation.getCurrentPosition(
    function(pos) { 
      console.log('Found you. Coordinates ' + pos.coords.latitude + ', ' + pos.coords.longitude);
      callback(pos.coords);
    },
    function(err) {
      console.log('Error requesting location!\n' + err);
    },
    {timeout: 15000, maximumAge: 60000}
  );
}

function sendWeather() {
  if (!weatherApi)
    { console.error('Weather api provider has not been selected'); }

  getCoordinates(function(coords) {
    weatherApi.getCurrentConditions(coords, function(values) {
        // Assemble dictionary using our keys
        var dictionary = {
          'KEY_TEMPERATURE': values.temp,
          'KEY_CONDITIONS': values.conditions
        };

        // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log('Weather info sent to Pebble successfully!');
          },
          function(e) {
            console.log('Error sending weather info to Pebble!\n' + JSON.stringify(e));
          }
        );
    });
  }); 
}

function init() {
  // Get the initial weather
  sendWeather();
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    init();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    sendWeather();
  }                     
);

Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://localhost:8080';

  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log('Configuration page returned: ' + JSON.stringify(configData));

  if (configData.backgroundColor) {
    Pebble.sendAppMessage({
      backgroundColor: parseInt(configData.backgroundColor, 16),
      twentyFourHourFormat: configData.twentyFourHourFormat
    }, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});

exports.setProvider = function(val) {
  switch (val) {
    case 'open-weather-map': 
      weatherApi = openWeatherMapApi;
      break;
    case 'yahoo-weather':
      weatherApi = yahooApi;
      break;
    default:
      weatherApi = bomApi;
  }
};
