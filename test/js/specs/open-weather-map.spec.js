var request = require('request');
var api = require('../../../src/js/weather-services/open-weather-map.js');
var sampleResponse = require('../fixtures/open-weather-map.json');

describe('Open Weather Map API', function() {

  beforeEach(function() {
    request.and.callFake(function(options, callback) {
      callback(null, {}, sampleResponse);
    });
  });

  it('should return a valid temperature and conditions given a valid request', function(done) {

    var coords = { latitude: 0, longitude: 0 };

    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(10);
      expect(values.conditions).toBe('Clouds');
      done();
    });

  });

});
