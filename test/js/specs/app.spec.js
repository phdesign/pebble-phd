var request = require('request');
var app = require('../../../src/js/app.js');
var config = require('../../../src/js/config.js');
var openWeatherMapResponse = require('../fixtures/open-weather-map.json');
var bomResponse = require('../fixtures/bom.json');

describe('app', function() {

  it('should be defined', function() {
    expect(app).toBeDefined();

    expect(Pebble.events.ready).toBeDefined();
    expect(Pebble.events.appmessage).toBeDefined();
  });

  describe('open-weather-map', function() {

    beforeEach(function() {
      config.settings.weatherService = 'open-weather-map';

      request.and.callFake(function(options, callback) {
        callback(null, {}, openWeatherMapResponse);
      });

      spyOn(navigator.geolocation, 'getCurrentPosition').and.callFake(function(success, error, options) {
        success({ coords: { latitude: 0, longitude: 0 } });
      });
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
      config.settings.weatherService = 'bom';

      request.and.callFake(function(options, callback) {
        callback(null, {}, bomResponse);
      });

      spyOn(navigator.geolocation, 'getCurrentPosition').and.callFake(function(success, error, options) {
        success({ coords: { latitude: 0, longitude: 0 } });
      });
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
