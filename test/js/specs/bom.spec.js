var request = require('request');
var api = require('../../../src/js/weather-services/bom.js');
var sampleResponse = require('../fixtures/bom.json');
var config = require('../../../src/js/config.js');

describe('BOM API', function() {
  var queryUrl;

  beforeEach(function() {
    request.and.callFake(function(options, callback) {
      queryUrl = options.url;
      callback(null, {}, sampleResponse);
    });
  });

  it('should return a valid temperature and conditions given a valid request', function(done) {
    var coords = { latitude: -38.0829605, longitude: 145.1986868 };
    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(24);
      expect(values.conditions).toBe('');
      done();
    });
  });

  it('should pass the closest weather station given a valid set of coordinates', function(done) {
    var coords = { latitude: -27.4, longitude: 153.1 };
    api.getCurrentConditions(coords, function() {
      expect(queryUrl).toBe('http://www.bom.gov.au/fwo/IDQ60801/IDQ60801.94578.json');
      done();
    });
  });

  it('should pass the closest weather station given a valid set of coordinates - melbourne', function(done) {
    var coords = { latitude: -37.817177071239264, longitude: 144.95854035597037 };
    // Melbourne Latrobe St should be replaced with Melbourne (Olympic Park)
    api.getCurrentConditions(coords, function() {
      expect(queryUrl).toBe('http://www.bom.gov.au/fwo/IDV60801/IDV60801.95936.json');
      done();
    });
  });

  it('should return a valid temperature in fahrenheit given temperature unit is fahrenheit', function(done) {
    var origSettings = config.settings;
    config.settings = { temperatureUnit: config.FAHRENHEIT };
    var coords = { latitude: -38.0829605, longitude: 145.1986868 };
    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(76);
      config.settings = origSettings;
      done();
    });
  });
});
