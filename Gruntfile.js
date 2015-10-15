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
        'test/js/**/*.js'
      ],
      options: {
        jshintrc: true
      }
    },

    concat: {
      build: {
        src: [
          'src/js/**/*.js',
          '!src/js/pebble-js-app.js'
        ],
        dest: 'src/js/pebble-js-app.js'
      }
    },

    jasmine: {
      build: {
        src: 'src/js/pebble-js-app.js',
        options: {
          helpers: 'test/js/helpers/*.js',
          specs: 'test/js/specs/*.spec.js',
          outfile: 'test/js/_SpecRunner.html',
          keepRunner: true
        }
      }
    },

  });

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-jasmine');

  grunt.registerTask('default', ['jshint', 'concat', 'jasmine']);

};