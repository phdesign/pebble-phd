/*jshint -W079 */
var Pebble = {
    events: {}
};
Pebble.addEventListener = function(evt, fn) {
    Pebble.events[evt] = fn;
};
Pebble.sendAppMessage = function() {};
