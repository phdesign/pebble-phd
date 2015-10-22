var gps = require('./gps.js');
var openWeatherMapApi = require('./open-weather-map.js');
var bomApi = require('./bom.js');
var yahooApi = require('./yahoo-weather.js');

module.exports = {

  activeService: yahooApi,

  sendWeather: function() {
    var self = this;
    
    if (!this.activeService)
      { console.error('Weather api provider has not been selected'); }

    gps.getCoordinates(function(coords) {
      self.activeService.getCurrentConditions(coords, function(values) {
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
  },

  // Return true if the weather service was changed, false if it's the same value
  setWeatherService: function(serviceKey) {
    if (serviceKey === this.activeService.key)
      return false;

    switch (serviceKey) {
      case 'open-weather-map': 
        this.activeService = openWeatherMapApi;
        break;
      case 'bom':
        this.activeService = bomApi;
        break;
      default:
        this.activeService = yahooApi;
    }

    return true;
  }

};
