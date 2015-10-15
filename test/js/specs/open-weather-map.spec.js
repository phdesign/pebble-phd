describe("Open weather map api", function() {

  it("should return a valid temperature and conditions given a valid request", function(done) {
    spyOn(app.utils, 'ajax').and.callFake(function(url, method, fn) {
      fn(fixtures.openWeatherMapResponse);
    });

    app.openWeatherMapApi.getCurrentConditions(0, 0, function(temperature, conditions) {
      expect(temperature).toBe(10);
      expect(conditions).toBe("Clouds");
      done();
    });
  });

});
