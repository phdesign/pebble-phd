var weather = require('./weather.js');
var config = require('./config.js');
var appInfo = require('../../appinfo.json');

function noop() {}

function init() {
  config.loadConfig();
  weather.setWeatherService(config.settings.weatherService);
  // Get the initial weather
  weather.sendWeather();
}

// This doesn't need to wait till we've initialised
(function disableLogs() {
  if (appInfo.enableLogs) return;
  console.log = noop;
  console.warn = noop;
  console.info = noop;
  console.error = noop;
  console.debug = noop;
})();

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');
  init();
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e) {
  console.log('AppMessage received!');
  weather.sendWeather();
});

Pebble.addEventListener('showConfiguration', function() {
  config.openConfigPage();
});

Pebble.addEventListener('webviewclosed', function(e) {
  config.receiveConfig(decodeURIComponent(e.response));
});
