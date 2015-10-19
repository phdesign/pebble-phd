var extend = require('extend');

function ajax(options) {
  console.log('Sending ' + options.method + ' request to ' + options.uri);
  var req = new XMLHttpRequest();
  req.onload = function () {
    this.body = this.responseText;
    console.log(this.responseText.substr(0, 50));

    if (options.json) {
      try { this.body = JSON.parse(this.responseText); }
      catch (e) { return options.callback(e, this); }
    }

    options.callback(null, this, this.body);
  };
  req.open(options.method, options.uri);
  req.send();
}

function appendQueryString(options) {
  var serialize = function(obj) {
    var str = [];
    for(var p in obj)
      if (obj.hasOwnProperty(p)) {
        str.push(encodeURIComponent(p) + '=' + encodeURIComponent(obj[p]));
      }
    return str.join('&');
  };
  
  if(options.qs){
    var qs = (typeof options.qs == 'string')? options.qs : serialize(options.qs);
    if(options.uri.indexOf('?') !== -1){ //no get params
        options.uri = options.uri+'&'+qs;
    }else{ //existing get params
        options.uri = options.uri+'?'+qs;
    }
  }
}

function initParams(uri, options, callback) {
  if (typeof options === 'function') {
    callback = options;
  }

  var params = {};
  if (typeof options === 'object') {
    extend(params, options, {uri: uri});
  } else if (typeof uri === 'string') {
    extend(params, {uri: uri});
  } else {
    extend(params, uri);
  }

  // Swap url -> uri if used
  if (!params.uri && params.url) {
    params.uri = params.url;
    delete params.url;
  }

  appendQueryString(params);

  params.callback = callback;
  return params;
}

function request(uri, options, callback) {
  if (typeof uri === 'undefined') {
    throw new Error('undefined is not a valid uri or options object.');
  }

  var params = initParams(uri, options, callback);

  params.method = params.method || 'GET';
  params.headers = params.headers || {};
  params.body = params.body || null;

  if (params.json) {
    params.headers.accept = params.headers.accept || 'application/json';
    if (params.method !== 'GET') {
      params.headers['content-type'] = 'application/json';
    }

    if (typeof params.json !== 'boolean') {
      params.body = JSON.stringify(params.json);
    } else if (typeof params.body !== 'string') {
      params.body = JSON.stringify(params.body);
    }
  }

  ajax(params);
}
module.exports = request;
