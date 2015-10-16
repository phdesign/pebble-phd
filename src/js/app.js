var app = (function(app) {
  var weatherApi;

  function getCoordinates(fn) {
    navigator.geolocation.getCurrentPosition(
      function(pos) { 
        fn(pos.coords);
      },
      function(err) {
        console.log('Error requesting location!\n' + err);
      },
      {timeout: 15000, maximumAge: 60000}
    );
  }

  function sendWeather() {
    if (!weatherApi)
      { console.error('Weather api provider has not been selected'); }

    getCoordinates(function(coords) {
      weatherApi.getCurrentConditions(coords, function(values) {
          // Assemble dictionary using our keys
          var dictionary = {
            'KEY_TEMPERATURE': values.temp,
            'KEY_CONDITIONS': values.conditions
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
    weatherApi = app.openWeatherMapApi;
    // Get the initial weather
    sendWeather();
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

  return app;
})(app || {});
