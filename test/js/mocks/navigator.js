// node runner has no geolocation, browser does.
/*jshint -W079 */
var navigator = navigator || {};
navigator.geolocation = navigator.geolocation || {};
navigator.geolocation.getCurrentPosition = navigator.geolocation.getCurrentPosition || function() {};
