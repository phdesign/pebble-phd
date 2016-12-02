var request = require('request');
var api = require('../../../src/js/weather-services/open-weather-map.js');
var config = require('../../../src/js/config.js');

describe('Open Weather Map API', function() {
  var sampleResponse;
  var coords;

  beforeEach(function() {
    // Load sampleResponse each time so tests can alter it
    sampleResponse = require('../fixtures/open-weather-map.json');
    coords = { latitude: 0, longitude: 0 };

    request.and.callFake(function(options, callback) {
      callback(null, {}, sampleResponse);
    });
  });

  it('should return a valid temperature and conditions given a valid request', function(done) {
    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(10);
      expect(values.conditions).toBe('cloudy');
      done();
    });
  });

  it('should return \'unknown\' if condition code is not recognised', function(done) {
    sampleResponse.weather[0].id = -1;
    api.getCurrentConditions(coords, function(values) {
      expect(values.conditions).toBe('unknown');
      done();
    });
  });

  it('should not display conditions text longer than 9 characters', function(done) {
    var testNext = function(i) {
      sampleResponse.weather[0].id = i;
      api.getCurrentConditions(coords, function(values) {
        expect(values.conditions.length).toBeLessThan(9);
        if (i <= 962)
          testNext(i + 1);
        else
          done();
      });
    };
    testNext(200);
  });

  it('should return a valid temperature in fahrenheit given temperature unit is fahrenheit', function(done) {
    var origSettings = config.settings;
    config.settings = { temperatureUnit: config.FAHRENHEIT };
    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(50);
      config.settings = origSettings;
      done();
    });
  });
});
