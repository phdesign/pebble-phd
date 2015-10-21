module.exports = function(grunt) {

  var appConfig = {
    info: grunt.file.readJSON('appinfo.json'),
    keys: grunt.file.readJSON('keys.json')
  };

  grunt.initConfig({

    config: appConfig,

    copy: {
      main: {
        options: {
          process: function (content, path) {
            return grunt.template.process(content);
          }
        },
        files: {
          'src/appinfo.h': ['src/appinfo.tmpl.h']
        }
      },
    },

    jshint: {
      files: [
        'Gruntfile.js', 
        'src/js/**/*.js',
        '!src/js/pebble-js-app.js',
        '!src/js/*.tmpl.js',
        'test/js/specs/**/*.js',
        'test/js/helpers/**/*.js'
      ],
      options: {
        jshintrc: true
      }
    },

    browserify: {
      build: {
        options: {
          alias: {
            'request': './src/js/request.js',
            'request-callback-wrapper': './src/js/request-callback-wrapper.js'
          }
        },
        src: [
          'src/js/**/*.js',
          '!src/js/pebble-js-app.js',
          '!src/js/*.tmpl.js'
        ],
        dest: 'src/js/pebble-js-app.js'
      },
      test: {
        options: {
          browserifyOptions: {
            debug: true
          },
          alias: {
            'request': './test/js/helpers/mock-request.js',
            'request-callback-wrapper': './src/js/request-callback-wrapper.js'
          }
        },
        src: [
          'test/js/specs/*.spec.js',
          'test/js/specs/*.js'
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
    },
    
    'gh-pages': {
      options: {
        base: 'config'
      },
      src: ['**/*']
    }

  });

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-browserify');
  grunt.loadNpmTasks('grunt-contrib-jasmine');
  grunt.loadNpmTasks('grunt-contrib-copy');
  grunt.loadNpmTasks('grunt-gh-pages');

  grunt.registerTask('default', ['copy', 'jshint', 'browserify', 'jasmine']);
  grunt.registerTask('deploy', ['gh-pages']);

};
