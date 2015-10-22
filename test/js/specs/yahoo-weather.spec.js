var request = require('request');
var api = require('../../../src/js/yahoo-weather.js');
var sampleResponse = require('../fixtures/yahoo-weather.json');

describe('Yahoo Weather API', function() {

  beforeEach(function() {
    request.and.callFake(function(options, callback) {
      callback(null, {}, sampleResponse);
    });
  });

  it('should return a valid temperature and conditions given a valid request', function(done) {

    var coords = { latitude: 0, longitude: 0 };

    api.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(16);
      expect(values.conditions).toBe('Mostly Cloudy');
      done();
    });

  });

});
