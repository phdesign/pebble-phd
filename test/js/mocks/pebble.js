function noop() {}

/*jshint -W079 */
var Pebble = {

    events: {},
    getActiveWatchInfo: noop,
    addEventListener: function(evt, fn) {
        this.events[evt] = fn;
    },
    sendAppMessage: noop,
    openURL: noop

};
