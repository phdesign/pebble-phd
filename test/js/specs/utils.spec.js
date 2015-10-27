var utils = require('../../../src/js/utils.js');

describe('Utils', function() {

  describe('hasOwnProperties', function() {

    it('should return true given an object with properties', function() {
      var obj = { thingOne: true };
      expect(utils.hasOwnProperties(obj)).toBe(true);
    });

    it('should return true given an object with a false property', function() {
      var obj = { thingOne: false };
      expect(utils.hasOwnProperties(obj)).toBe(true);
    });

    it('should return true given an object with a null property', function() {
      var obj = { thingOne: null };
      expect(utils.hasOwnProperties(obj)).toBe(true);
    });

    it('should return false given an object with no properties', function() {
      var obj = {};
      expect(utils.hasOwnProperties(obj)).toBe(false);
    });

    it('should return false given an undefined object', function() {
      var obj;
      expect(utils.hasOwnProperties(obj)).toBe(false);
    });

    it('should return false given an null object', function() {
      var obj = null;
      expect(utils.hasOwnProperties(obj)).toBe(false);
    });

  });

});
