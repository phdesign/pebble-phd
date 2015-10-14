var app = (function(app) {

  app.openWeatherMapApi = {

    getCurrentConditions: function(latitude, longitude, fn) {
      // Construct URL
      var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
          latitude + '&lon=' + longitude + '&APPID=15664341af304192147d1fb3f38a1f67';

      // Send request to OpenWeatherMap
      app.utils.ajax(url, 'GET', 
        function(responseText) {
          // responseText contains a JSON object with weather info
          //console.log(responseText);
          var json = JSON.parse(responseText);

          // Temperature in Kelvin requires adjustment
          var temperature = Math.round(json.main.temp - 273.15);
          console.log('Temperature is ' + temperature);

          // Conditions
          var conditions = json.weather[0].main;      
          console.log('Conditions are ' + conditions);

          fn(temperature, conditions);
        }      
      );
    }

  };

  return app;

}(app || {}));
