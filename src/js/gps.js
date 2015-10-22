module.exports = {

    getCoordinates: function(callback) {
      //callback({ latitude: -37.123, longitude: 144.123 });
      navigator.geolocation.getCurrentPosition(
        function(pos) { 
          console.log('Found you. Coordinates ' + pos.coords.latitude + ', ' + pos.coords.longitude);
          callback(pos.coords);
        },
        function(err) {
          console.log('Error requesting location!\n' + err);
        },
        {timeout: 15000, maximumAge: 60000}
      );
    }

};
