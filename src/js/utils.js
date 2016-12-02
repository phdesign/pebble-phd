module.exports = {

  kelvinToCelsius: function(temp) {
    return (temp - 273.15);
  },

  kelvinToFahrenheit: function(temp) {
    return ((temp * (9 / 5)) - 459.67);
  },

  fahreheitToCelsius: function(temp) {
    return (temp - 32) * (5 / 9);
  },

  celciusToFahrenheit: function(temp) {
    return (temp * (9 / 5)) + 32;
  },

  hasOwnProperties: function(obj) {
    return obj ? Object.getOwnPropertyNames(obj).length > 0 : false;
  }

};
