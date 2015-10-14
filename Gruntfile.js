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
      dist: {
        src: [
          'src/js/**/*.js',
          '-src/js/pebble-js-app.js'
        ],
        dest: 'src/js/pebble-js-app.js'
      }
    }

  });

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-concat');

  grunt.registerTask('default', ['jshint', 'concat']);

};
