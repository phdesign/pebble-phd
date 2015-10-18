var extend = require('extend');

function ajax(params) {
  console.log('Sending ' + params.method + ' request to ' + params.uri);
  var req = new XMLHttpRequest();
  req.onload = function () {
    this.body = this.responseText;
    console.log(this.responseText.substr(0, 50));

    if (params.json) {
      try { this.body = JSON.parse(this.responseText); }
      catch (e) { return params.callback(e, this); }
    }

    params.callback(null, this, this.body);
  };
  req.open(params.method, params.uri);
  req.send();
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

  if (!params.uri && params.url) {
    params.uri = params.url;
    delete params.url;
  }

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
