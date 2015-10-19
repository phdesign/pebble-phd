module.exports = {

  kelvinToCelsius: function(temp) {
    return Math.round(temp - 273.15);
  },

  fahreheitToCelsius: function(temp) {
    return (temp - 32) * (5 / 9);
  }

};
