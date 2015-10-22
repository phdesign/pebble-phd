var extend = require('extend');
var weather = require('./weather.js');

module.exports = {

  settings: {
    configVersion: 1,
    showWeather: true,
    weatherService: 'yahoo-weather'
  },

  runningInEmulator: function() {
    return Pebble.getActiveWatchInfo && /^qemu/.test(Pebble.getActiveWatchInfo().model);
  },

  loadConfig: function() {
    try {
      if (localStorage.config) {
        this.settings = JSON.parse(localStorage.config);
        weather.setWeatherService(this.settings.weatherService);
        console.log('Config loaded, using ' + weather.activeService.name + ' service');
      }
    } catch (e) { } 
  },

  saveConfig: function() {
    localStorage.config = JSON.stringify(this.settings);
  },

  openConfigPage: function() {
    // Send current config to the config page. Why do the tutorials recommend saving config on the config page? 
    // That's dumb, the app should be the source of truth for current configuration.
    var url = this.runningInEmulator() ? 'http://localhost:8080' : 'http://phdesign.com.au/pebble-phd';
    url += '?cfg=' + encodeURIComponent(JSON.stringify(this.settings));
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
    extend(this.settings, newConfig);
    this.saveConfig();

    var dictionary = {};
    if (this.settings.weatherService) {
      if (weather.setWeatherService(this.settings.weatherService))
        weather.sendWeather();
      dictionary['KEY_WEATHER_SERVICE'] = weather.activeService.name;
    }
    if (this.settings.showWeather) {
      dictionary['KEY_SHOW_WEATHER'] = !!this.settings.showWeather;
    }

    if (this.settings.showWeather || this.settings.weatherService) {
      Pebble.sendAppMessage(dictionary, function() {
        console.log('Send successful!');
      }, function() {
        console.log('Send failed!');
      });
    }
  },

};
