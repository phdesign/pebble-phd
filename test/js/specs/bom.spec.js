var utils = require('../../../src/js/utils.js');
var bomApi = require('../../../src/js/bom.js');

describe("BOM API", function() {
  var queryUrl;

  beforeEach(function() {
    spyOn(utils, 'ajax').and.callFake(function(url, method, fn) {
      queryUrl = url;
      fn(fixtures.bomResponse);
    });
  });

  it("should return a valid temperature and conditions given a valid request", function(done) {

    var coords = { latitude: 0, longitude: 0 };

    bomApi.getCurrentConditions(coords, function(values) {
      expect(values.temp).toBe(24.3);
      expect(values.conditions).toBe("Mostly clear");
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
