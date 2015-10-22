var extend = require('extend');
var weather = require('./weather.js');

var config = {};

module.exports = {

  loadConfig: function() {
    try {
      if (localStorage.config) {
        config = JSON.parse(localStorage.config);
        weather.setWeatherService(config.weatherService);
        console.log('Config loaded, using ' + weather.activeService.name + ' service');
      }
    } catch (e) { } 
  },

  saveConfig: function() {
    localStorage.config = JSON.stringify(config);
  },

  openConfigPage: function() {
    // TODO: switch between localhost and prod config page if running emulator or phone 
    var url = 'http://phdesign.com.au/pebble-phd';
    console.log('Showing configuration page: ' + url);
    Pebble.openURL(url);
  },

  receiveConfig: function(configString) {
    var newConfig;

    try {
      newConfig = JSON.parse(configString);
      console.log('Configuration page returned: ' + JSON.stringify(newConfig));
    } catch (err) {
      console.warn('Invalid config data returned', configString, err);
    }
    if (!newConfig) return false;

    // Copy received config properties onto our exisitng config
    extend(config, newConfig);
    this.saveConfig();

    var dictionary = {};
    if (config.weatherService) {
      if (weather.setWeatherService(config.weatherService))
        weather.sendWeather();
      dictionary['KEY_WEATHER_SERVICE'] = weather.activeService.name;
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
  },

};
