var utils = require('./utils.js');

var stateIds = {
  tas: 'IDT60801',
  vic: 'IDV60801',
  nsw: 'IDN60801',
  qld: 'IDQ60801',
  sa: 'IDS60801',
  wa: 'IDW60801',
  nt: 'IDD60801',
  act: 'IDN60903'
};

var getSites = function() {

};

var findClosestSite = function(lat, lon) {

};

var buildRequestUrl = function(lat, lon) {
  var closestSite = findClosestSite(lat, lon);
  var stateId = stateIds[closestSite.state];
  var siteNumber = closestSite.site.siteNumber;

  return 'http://www.bom.gov.au/fwo/' + stateId + '/' + stateId + '.' + siteNumber + '.json';
};

var readWeatherValues = function(rawData) {
  var latestReading = rawData.observations.data[0];
  return {
    temp: latestReading.air_temp,
    // BOM JSON feed doesn't really give a weather summary, best bet is their cloud reading.
    conditions: latestReading.cloud === '-' ? '' : latestReading.cloud
  };
};

exports.getCurrentConditions = function(coords, done) {
  var url = buildRequestUrl(coords.latitude, coords.longitude);

  utils.ajax(url, 'GET', 
    function(responseText) {
      console.log('Received weather data from BOM');

      var json = JSON.parse(responseText);
      var values;
      try {
        values = readWeatherValues(json);
      } catch (e) {
        console.log('Error processing weather data\n', e);
      }
     
      if (!values || !values.temp || !values.conditions) {
        console.log('Unable to process weather data, aborting...');
        return;
      }
      console.log('Temperature is ' + values.temp);
      console.log('Conditions are ' + values.conditions);

      done(values);
    }      
  );
};
