var $submitButton = $('#btn-submit'),
  $showWeather = $('#field-show-weather'),
  $weatherService = $('#field-weather-service');

$(function() {
  loadConfigData();
  showWeatherHandler();
  submitHandler();
});

function showWeatherHandler() {
  var toggleWeather = function(enabled) {
    $showWeather.parent('label')
      .siblings('label')
      .toggleClass('disabled', !enabled)
      .find('input, select')
      .prop('disabled', !enabled);
  }

  $showWeather.on('click', function() {
    toggleWeather(this.checked);
  });
  // Initialise the state of the page on first load
  toggleWeather($showWeather[0].checked);
}

function submitHandler() {
  $submitButton.on('click', function() {
    console.log('Submit');

    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(getConfigData()));
  });
}

function loadConfigData() {
  var config;

  try {
    config = JSON.parse(getQueryParam('cfg', '{}'));
  } catch (e) { }
  console.log('Received current config as', config);

  if (config && config.showWeather)
    $showWeather[0].checked = config.showWeather;
  if (config && config.weatherService)
    $weatherService.val(config.weatherService);
}

function getConfigData() {
  return {
    showWeather: $showWeather[0].checked,
    weatherService: $weatherService.val()
  };
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}
