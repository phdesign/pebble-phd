var gps = require('./gps.js');
var config = require('./config.js');
var services = [
  require('./weather-services/open-weather-map.js'),
  require('./weather-services/bom.js'),
  require('./weather-services/yahoo-weather.js')
];

module.exports = {

  activeService: null,

  sendWeather: function() {
    var self = this;
    
    if (!this.activeService)
      { console.error('Weather api provider has not been selected'); }

    gps.getCoordinates(function(coords) {
      self.activeService.getCurrentConditions(coords, function(values) {
          // Assemble dictionary using our keys
          var dictionary = {
            'KEY_TEMPERATURE': values.temp,
            'KEY_CONDITIONS': values.conditions,
            'KEY_TEMPERATURE_UNIT': config.settings.temperatureUnit
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
  },

  // Return true if the weather service was changed, false if it's the same value
  setWeatherService: function(serviceKey) {
    if (this.activeService && serviceKey === this.activeService.key)
      return false;

    for (var i = 0; i < services.length; i++) {
      if (services[i].key === serviceKey) {
        this.activeService = services[i];
        break;
      }
    }
    if (i == services.length) {
        console.warn('Unrecognised weather service key \'' + serviceKey + '\'');
        return false;
    }

    return true;
  }

};
