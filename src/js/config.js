var utils = require('./utils.js');
var extend = require('extend');
var weather = require('./weather.js');

module.exports = {

  settings: {
    configVersion: 1,
    showWeather: false,
    weatherService: 'yahoo-weather'
  },

  isRunningInEmulator: function() {
    return Pebble.getActiveWatchInfo && /^qemu/.test(Pebble.getActiveWatchInfo().model);
  },

  loadConfig: function() {
    if (localStorage.config) {
      try {
        this.settings = JSON.parse(localStorage.config);
      } catch (e) { } 
    }

    weather.setWeatherService(this.settings.weatherService);
    console.log('Config loaded, using ' + weather.activeService.name + ' service');
  },

  saveConfig: function() {
    localStorage.config = JSON.stringify(this.settings);
  },

  openConfigPage: function() {
    // Send current config to the config page. Why do the tutorials recommend saving config on the config page? 
    // That's dumb, the app should be the source of truth for current configuration.
    var url = this.isRunningInEmulator() ? 'http://localhost:8080' : 'https://phdesign.com.au/pebble-phd/';
    url += '?cfg=' + encodeURIComponent(JSON.stringify(this.settings));
    console.log('Showing configuration page: ' + url);
    Pebble.openURL(url);
  },

  sendConfig: function() {
    var dictionary = {
      'KEY_SHOW_WEATHER': !!this.settings.showWeather
    };
    Pebble.sendAppMessage(dictionary, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
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
    extend(this.settings, newConfig);
    this.saveConfig();
    this.sendConfig();

    weather.setWeatherService(this.settings.weatherService);
    if (this.settings.showWeather)
      weather.sendWeather();
  },

};
