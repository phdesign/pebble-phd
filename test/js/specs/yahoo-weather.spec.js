var request = require('request');
var api = require('../../../src/js/weather-services/yahoo-weather.js');
var config = require('../../../src/js/config.js');

describe('Yahoo Weather API', function() {
  var sampleResponse;
  var googleResponse;
  var coords;

  beforeEach(function() {
    // Load sampleResponse each time so tests can alter it
    sampleResponse = require('../fixtures/yahoo-weather.json');
    googleResponse = require('../fixtures/google-geolocation.json');
    coords = { latitude: 0, longitude: 0 };

    request.and.callFake(function(options, callback) {
      if (options.url.match(/maps\.googleapis\.com/)) 
        callback(null, {}, googleResponse);
      else
        callback(null, {}, sampleResponse);
    });
  });

  it('should return a valid temperature and conditions given a valid request', function(done) {
    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(16);
      expect(values.conditions).toBe('cloudy');
      done();
    });
  });

  it('should return \'unknown\' if condition code is not recognised', function(done) {
    sampleResponse.query.results.channel.item.condition.code = -1;
    api.getCurrentConditions(coords, function(values) {
      expect(values.conditions).toBe('unknown');
      done();
    });
  });

  it('should not display conditions text longer than 9 characters', function(done) {
    var testNext = function(i) {
      sampleResponse.query.results.channel.item.condition.code = i;
      api.getCurrentConditions(coords, function(values) {
        expect(values.conditions.length).toBeLessThan(9);
        if (i < 47)
          testNext(i + 1);
        else
          done();
      });
    };
    testNext(0);
  });

  it('should return a valid temperature in fahrenheit given temperature unit is fahrenheit', function(done) {
    var origSettings = config.settings;
    config.settings = { temperatureUnit: config.FAHRENHEIT };
    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(61);
      config.settings = origSettings;
      done();
    });
  });
});
