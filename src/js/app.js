var utils = require('./utils.js');
var openWeatherMapApi = require('./open-weather-map.js');
var bomApi = require('./bom.js');
var yahooApi = require('./yahoo-weather.js');

var config;
// Determine which weather provider to use
var activeWeatherService = yahooApi;

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
  if (!activeWeatherService)
    { console.error('Weather api provider has not been selected'); }

  getCoordinates(function(coords) {
    activeWeatherService.getCurrentConditions(coords, function(values) {
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

function setWeatherService(serviceKey) {
  if (serviceKey === activeWeatherService.key)
    return;

  switch (serviceKey) {
    case 'open-weather-map': 
      activeWeatherService = openWeatherMapApi;
      break;
    case 'bom':
      activeWeatherService = bomApi;
      break;
    default:
      activeWeatherService = yahooApi;
  }
}

function loadConfig() {
 try {
    if (localStorage.config) {
      config = JSON.parse(localStorage.config);
      setWeatherService(config.weatherService);
      console.log('Config loaded, using ' + activeWeatherService.name + ' service');
    }
  } catch (e) { } 
}

function init() {
  // Load config data
  loadConfig();
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
  var url = 'http://phdesign.com.au/pebble-phd';

  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  try {
    config = JSON.parse(decodeURIComponent(e.response));
    localStorage.config = JSON.stringify(config);
    console.log('Configuration page returned: ' + localStorage.config);
  } catch (err) {
    console.warn('Invalid config data returned', e.response, err);
  }

  var dictionary = {};
  if (config.weatherService) {
    setWeatherService(config.weatherService);
    sendWeather();
    dictionary['KEY_WEATHER_SERVICE'] = activeWeatherService.name;
  }
  if (config.showWeather) {
    dictionary['KEY_SHOW_WEATHER'] = !!config.showWeather;
  }

  if (config.showWeather || config.weatherService) {
    Pebble.sendAppMessage(dictionary, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});

exports.setWeatherService = setWeatherService;
