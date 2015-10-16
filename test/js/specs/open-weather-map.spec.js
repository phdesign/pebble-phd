var utils = require('../../../src/js/utils.js');
var openWeatherMapApi = require('../../../src/js/open-weather-map.js');

describe("Open Weather Map API", function() {
  var queryUrl;

  beforeEach(function() {
    spyOn(utils, 'ajax').and.callFake(function(url, method, fn) {
      queryUrl = url;
      fn(fixtures.openWeatherMapResponse);
    });
  });

  it("should return a valid temperature and conditions given a valid request", function(done) {

    var coords = { latitude: 0, longitude: 0 };

    openWeatherMapApi.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(10);
      expect(values.conditions).toBe("Clouds");
      done();
    });

  });

  it("should pass the closest weather station given a valid set of coordinates", function(done) {

    var coords = { latitude: 0, longitude: 0 };

    openWeatherMapApi.getCurrentConditions(coords, function() {
      expect(queryUrl).toBe('http://api.openweathermap.org/data/2.5/weather?lat=0&lon=0&APPID=15664341af304192147d1fb3f38a1f67');
      done();
    });

  });

});
