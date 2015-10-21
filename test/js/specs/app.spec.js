var request = require('request');
var app = require('../../../src/js/app.js');
var openWeatherMapResponse = require('../helpers/open-weather-map.json');
var bomResponse = require('../helpers/bom.json');

describe('app', function() {

  describe('open-weather-map', function() {

    beforeEach(function() {
      app.setWeatherService('open-weather-map');

      request.and.callFake(function(options, callback) {
        callback(null, {}, openWeatherMapResponse);
      });

      spyOn(navigator.geolocation, 'getCurrentPosition').and.callFake(function(success, error, options) {
        success({ coords: { latitude: 0, longitude: 0 } });
      });
    });

    it('should be defined', function() {
      expect(app).toBeDefined();

      expect(Pebble.events.ready).toBeDefined();
      expect(Pebble.events.appmessage).toBeDefined();
    });

    it('should send weather data given a ready event', function() {
      spyOn(Pebble, 'sendAppMessage');

      Pebble.events.ready();

      expect(Pebble.sendAppMessage).toHaveBeenCalledWith({
          'KEY_TEMPERATURE': 10,
          'KEY_CONDITIONS': 'Clouds'
        }, jasmine.any(Function), jasmine.any(Function));
    });

    it('should send weather data given an appmessage event', function() {
      spyOn(Pebble, 'sendAppMessage');

      Pebble.events.appmessage();

      expect(Pebble.sendAppMessage).toHaveBeenCalledWith({
          'KEY_TEMPERATURE': 10,
          'KEY_CONDITIONS': 'Clouds'
        }, jasmine.any(Function), jasmine.any(Function));
    });

  });

  describe('bom', function() {

    beforeEach(function() {
      app.setWeatherService('bom');

      request.and.callFake(function(options, callback) {
        callback(null, {}, bomResponse);
      });

      spyOn(navigator.geolocation, 'getCurrentPosition').and.callFake(function(success, error, options) {
        success({ coords: { latitude: 0, longitude: 0 } });
      });
    });

    it('should be defined', function() {
      expect(app).toBeDefined();

      expect(Pebble.events.ready).toBeDefined();
      expect(Pebble.events.appmessage).toBeDefined();
    });

    it('should send weather data given a ready event', function() {
      spyOn(Pebble, 'sendAppMessage');

      Pebble.events.ready();

      expect(Pebble.sendAppMessage).toHaveBeenCalledWith({
          'KEY_TEMPERATURE': 24.3,
          'KEY_CONDITIONS': ''
        }, jasmine.any(Function), jasmine.any(Function));
    });

    it('should send weather data given an appmessage event', function() {
      spyOn(Pebble, 'sendAppMessage');

      Pebble.events.appmessage();

      expect(Pebble.sendAppMessage).toHaveBeenCalledWith({
          'KEY_TEMPERATURE': 24.3,
          'KEY_CONDITIONS': ''
        }, jasmine.any(Function), jasmine.any(Function));
    });

  });
});
