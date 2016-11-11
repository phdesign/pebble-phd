var weather = require('./weather.js');
var config = require('./config.js');

function noop() {}

function init() {
  config.loadConfig();
  config.sendConfig();
  if (config.settings.showWeather)
    weather.sendWeather();
}

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
