var bom = require('bom-api');

var readWeatherValues = function(rawData) {
  var latestReading = rawData[0];
  return {
    temp: latestReading.air_temp,
    // BOM JSON feed doesn't really give a weather summary, best bet is their cloud reading.
    conditions: '' //latestReading.cloud === '-' ? '' : latestReading.cloud
  };
};

exports.getCurrentConditions = function(coords, done) {
  
  bom.getBomDataByLatitudeLongitude(coords.latitude, coords.longitude, function(data){
    console.log('Received weather data from BOM');

    var values;
    try {
      values = readWeatherValues(data);
    } catch (e) {
      console.log('Error processing weather data\n', e);
    }
    /*jshint -W041 */
    if (!values || values.temp == null || values.conditions == null) {
      console.log('Unable to process weather data, aborting...');
      return;
    }
    console.log('Temperature is ' + values.temp);
    console.log('Conditions are ' + values.conditions);

    done(values);
  });

};
