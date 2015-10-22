/*jshint -W079 */
var Pebble = {

    events: {},
    getActiveWatchInfo: function() {},
    addEventListener: function(evt, fn) {
        this.events[evt] = fn;
    },
    sendAppMessage: function() {}

};
