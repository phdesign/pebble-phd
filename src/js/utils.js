var app = (function(app) {
  console.log('utils loaded');

  app.utils = {
    
    ajax: function (url, type, callback) {
      var req = new XMLHttpRequest();
      req.onload = function () {
        callback(this.responseText);
      };
      req.open(type, url);
      req.send();
    }

  };

  return app;

})(app || {});
