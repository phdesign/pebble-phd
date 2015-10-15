describe("app", function() {

  it("should be defined", function() {
    expect(app).toBeDefined();
  });

  it("should send weather data on ready event", function() {

    spyOn(app.utils, 'ajax').and.callFake(function(url, method, fn) {
      fn(fixtures.openWeatherMapResponse);
    });

  });

});
