describe("Open weather map api", function() {

  it("should return a valid temperature and conditions given a valid request", function(done) {
    spyOn(app.utils, 'ajax').and.callFake(function(url, method, fn) {
      var response = JSON.stringify({"coord":{"lon":145.2,"lat":-37.98},"weather":[{"id":801,"main":"Clouds","description":"few clouds","icon":"02n"}],"base":"stations","main":{"temp":283.02,"pressure":1025,"humidity":81,"temp_min":280.37,"temp_max":288.15},"visibility":10000,"wind":{"speed":1.5,"deg":310},"clouds":{"all":20},"dt":1444684580,"sys":{"type":1,"id":8201,"message":0.0129,"country":"AU","sunrise":1444592276,"sunset":1444638838},"id":2169455,"name":"Dandenong West","cod":200});
      fn(response);
    });

    app.openWeatherMapApi.getCurrentConditions(0, 0, function(temperature, conditions) {
      expect(temperature).toBe(10);
      expect(conditions).toBe("Clouds");
      done();
    });
  });
});
