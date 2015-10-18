var request = require('request');
var openWeatherMapApi = require('../../../src/js/open-weather-map.js');
var openWeatherMapResponse = require('../helpers/open-weather-map.json');

describe('Open Weather Map API', function() {
  var queryUrl;

  beforeEach(function() {
    request.and.callFake(function(options, callback) {
      queryUrl = options.url;
      callback(null, {}, openWeatherMapResponse);
    });
  });

  it('should return a valid temperature and conditions given a valid request', function(done) {

    var coords = { latitude: 0, longitude: 0 };

    openWeatherMapApi.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(10);
      expect(values.conditions).toBe('Clouds');
      done();
    });

  });

  it('should pass the closest weather station given a valid set of coordinates', function(done) {

    var coords = { latitude: 0, longitude: 0 };

    openWeatherMapApi.getCurrentConditions(coords, function() {
      expect(queryUrl).toBe('http://api.openweathermap.org/data/2.5/weather?lat=0&lon=0&APPID=15664341af304192147d1fb3f38a1f67');
      done();
    });

  });

});
