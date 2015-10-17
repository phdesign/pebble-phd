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
        '!src/js/pebble-js-app.js',
        'test/js/specs/**/*.js',
        'test/js/helpers/**/*.js'
      ],
      options: {
        jshintrc: true
      }
    },

    browserify: {
      options: {
        alias: {
          'request': './src/js/simple-request.js'
        }
      },
      build: {
        src: [
          'src/js/**/*.js',
          '!src/js/pebble-js-app.js'
        ],
        dest: 'src/js/pebble-js-app.js'
      },
      test: {
        options: {
          browserifyOptions: {
            debug: true
          }
        },
        src: [
          'test/js/specs/*.spec.js',
        ],
        dest: 'test/js/specs-bundle.js'
      }
    },

    jasmine: {
      build: {
        options: {
          helpers: 'test/js/helpers/*.js',
          specs: 'test/js/specs-bundle.js',
          outfile: 'test/js/_SpecRunner.html',
          keepRunner: true
        }
      }
    }

  });

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-browserify');
  grunt.loadNpmTasks('grunt-contrib-jasmine');

  grunt.registerTask('default', ['jshint', 'browserify', 'jasmine']);

};
