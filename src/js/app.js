var app = (function(app) {

  function getLocation(fn) {
    navigator.geolocation.getCurrentPosition(
      function(pos) { 
        fn(pos.coords.latitude, pos.coords.longitude);
      },
      function(err) {
        console.log('Error requesting location!\n' + err);
      },
      {timeout: 15000, maximumAge: 60000}
    );
  }

  function sendWeather(weatherApi) {
    getLocation(function(latitude, longitude) {
      weatherApi.getCurrentConditions(latitude, longitude, function(temperature, conditions) {
          // Assemble dictionary using our keys
          var dictionary = {
            'KEY_TEMPERATURE': temperature,
            'KEY_CONDITIONS': conditions
          };

          // Send to Pebble
          Pebble.sendAppMessage(dictionary,
            function(e) {
              console.log('Weather info sent to Pebble successfully!');
            },
            function(e) {
              console.log('Error sending weather info to Pebble!\n' + JSON.stringify(e));
            }
          );
      });
    }); 
  }

  function init() {
    // Determine which weather provider to use
    var weatherApi = app.openWeatherMapApi;
    // Get the initial weather
    sendWeather(weatherApi);
  }

  // Listen for when the watchface is opened
  Pebble.addEventListener('ready', 
    function(e) {
      console.log('PebbleKit JS ready!');
      init();
    }
  );

  // Listen for when an AppMessage is received
  Pebble.addEventListener('appmessage',
    function(e) {
      console.log('AppMessage received!');
      sendWeather();
    }                     
  );

}(app || {}));
