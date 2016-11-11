var utils = require('./utils.js');
var extend = require('extend');
var weather = require('./weather.js');

var CELSIUS = 1;
var FAHRENHEIT = 2;
var currentConfigVersion = 2;

module.exports = {

  settings: {
    configVersion: currentConfigVersion,
    showWeather: false,
    weatherService: 'yahoo-weather',
    temperatureUnit: CELSIUS
  },

  isRunningInEmulator: function() {
    return Pebble.getActiveWatchInfo && /^qemu/.test(Pebble.getActiveWatchInfo().model);
  },

  migrateV1SettingsToV2: function(settingsV1) {
    settingsV1.temperatureUnit = CELSIUS;
    return settingsV1;
  },

  loadConfig: function() {
    if (localStorage.config) {
      try {
        var savedSettings = JSON.parse(localStorage.config);
        switch (savedSettings.configVersion) {
          case 1:
            this.settings = this.migrateV1SettingsToV2(savedSettings);
            break;
          case currentConfigVersion:
            this.settings = savedSettings;
            break;
        }
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
      console.log('Send config successful!');
    }, function() {
      console.log('Send config failed!');
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

  CELSIUS: CELSIUS,
  FAHRENHEIT: FAHRENHEIT
};
