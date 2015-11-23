var request = require('request');
var app = require('../../../src/js/app.js');
var config = require('../../../src/js/config.js');
var openWeatherMapResponse = require('../fixtures/open-weather-map.json');
var bomResponse = require('../fixtures/bom.json');

describe('App', function() {

  it('should be defined', function() {
    expect(app).toBeDefined();

    expect(Pebble.events.ready).toBeDefined();
    expect(Pebble.events.appmessage).toBeDefined();
  });

  it('should send current config to the localhost config page when running in emulator given a showConfiguration event', function() {
    var origSettings = config.settings;
    config.settings = { thingOne: true, thingTwo: 'apple' };
    
    spyOn(Pebble, 'getActiveWatchInfo').and.returnValue({
      model: 'qemu_platform_basalt'
    });
    spyOn(Pebble, 'openURL');

    Pebble.events.showConfiguration();

    expect(Pebble.openURL).toHaveBeenCalledWith('http://localhost:8080?cfg=%7B%22thingOne%22%3Atrue%2C%22thingTwo%22%3A%22apple%22%7D');

    config.settings = origSettings;
  });

  it('should send current config to the prod config page when running on phone given a showConfiguration event', function() {
    var origSettings = config.settings;
    config.settings = { thingOne: true, thingTwo: 'apple' };
    
    spyOn(Pebble, 'getActiveWatchInfo').and.returnValue({
      model: 'platform_basalt'
    });
    spyOn(Pebble, 'openURL');

    Pebble.events.showConfiguration();

    expect(Pebble.openURL).toHaveBeenCalledWith('http://phdesign.com.au/pebble-phd?cfg=%7B%22thingOne%22%3Atrue%2C%22thingTwo%22%3A%22apple%22%7D');

    config.settings = origSettings;
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
          'KEY_CONDITIONS': 'cloudy'
        }, jasmine.any(Function), jasmine.any(Function));
    });

    it('should send weather data given an appmessage event', function() {
      spyOn(Pebble, 'sendAppMessage');

      Pebble.events.appmessage();

      expect(Pebble.sendAppMessage).toHaveBeenCalledWith({
          'KEY_TEMPERATURE': 10,
          'KEY_CONDITIONS': 'cloudy'
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
