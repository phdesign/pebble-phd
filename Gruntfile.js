module.exports = function(grunt) {

  var appConfig = {
    info: grunt.file.readJSON("appinfo.json"),
    keys: grunt.file.readJSON("keys.json")
  };

  grunt.initConfig({

    config: appConfig,

    jshint: {
      files: [
        'Gruntfile.js', 
        'src/js/**/*.js',
        '-src/js/pebble-js-app.js'
      ],
      options: {
        jshintrc: true
      }
    },

    concat: {
      build: {
        src: [
          'src/js/**/*.js',
          '-src/js/pebble-js-app.js'
        ],
        dest: 'src/js/pebble-js-app.js'
      }
    },

    jasmine: {
      build: {
        src: 'src/js/pebble-js-app.js',
        options: {
          helpers: [
            'test/js/mocks.js',
            'test/js/fixtures.js'
          ],
          specs: 'test/js/*.spec.js'
        }
      }
    },

    requirejs: {
      build: {
        options: {
          baseUrl: "src/js",
          mainConfigFile: "src/js/app.js",
          //name: "path/to/almoud", // assumes a production build using almond
          out: "src/js/pebble-js-app.js"
        }
      }
    }

  });

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-jasmine');
  grunt.loadNpmTasks('grunt-contrib-requirejs');

  grunt.registerTask('default', ['jshint', 'concat', 'jasmine']);

};
