var request = require('request');
var bomApi = require('../../../src/js/bom.js');
var bomResponse = require('../helpers/bom.json');

describe("BOM API", function() {
  var queryUrl;

  beforeEach(function() {
    request.and.callFake(function(options, callback) {
      queryUrl = options.url;
      callback(null, {}, bomResponse);
    });
  });

  it("should return a valid temperature and conditions given a valid request", function(done) {

    var coords = { latitude: -38.0829605, longitude: 145.1986868 };

    bomApi.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(24.3);
      expect(values.conditions).toBe("");
      done();
    });

  });

  it("should pass the closest weather station given a valid set of coordinates", function(done) {

    var coords = { latitude: -27.4, longitude: 153.1 };

    bomApi.getCurrentConditions(coords, function() {
      expect(queryUrl).toBe('http://www.bom.gov.au/fwo/IDQ60801/IDQ60801.94578.json');
      done();
    });

  });

});
