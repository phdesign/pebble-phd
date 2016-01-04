module.exports = function(grunt) {

  var appConfig = {
    info: grunt.file.readJSON('appinfo.json'),
    keys: grunt.file.readJSON('keys.json'),
    enableLogs: false,
    paths: {
      bundle: 'src/js/pebble-js-app.js',
      testBundle: 'test/js/specs-bundle.js'
    }
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
          'src/appinfo.h': ['src/appinfo.tpl.h']
        }
      },
    },

    jshint: {
      files: [
        'Gruntfile.js', 
        'src/js/**/*.js',
        '!<%= config.paths.bundle %>',
        '!src/js/*.tpl.js',
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
          '!<%= config.paths.bundle %>',
          '!src/js/*.tpl.js'
        ],
        dest: '<%= config.paths.bundle %>'
      },
      test: {
        options: {
          browserifyOptions: {
            debug: true
          },
          alias: {
            'request': './test/js/mocks/request.js',
            'request-callback-wrapper': './src/js/request-callback-wrapper.js'
          }
        },
        src: [
          'test/js/specs/*.spec.js'
        ],
        dest: '<%= config.paths.testBundle %>'
      }
    },

    jasmine: {
      build: {
        options: {
          helpers: [
            'test/js/mocks/pebble.js',
            'test/js/mocks/navigator.js'
          ],
          specs: '<%= config.paths.testBundle %>',
          outfile: 'test/js/_SpecRunner.html',
          keepRunner: true
        }
      }
    },

    removelogging: {
      build: {
        src: '<%= config.paths.bundle %>'
      },
      test: {
        src: '<%= config.paths.testBundle %>'
      }
    },

    run: {
      options: {
        cwd: 'test/c/',
        failOnError: true
      },
      make: {
        cmd: 'make'
      },
      test: {
        cmd: './test'
      }
    },
    
    'gh-pages': {
      options: {
        base: 'config'
      },
      src: ['**/*']
    }

  });

  grunt.loadNpmTasks('grunt-browserify');
  grunt.loadNpmTasks('grunt-contrib-copy');
  grunt.loadNpmTasks('grunt-contrib-jasmine');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-gh-pages');
  grunt.loadNpmTasks('grunt-remove-logging');
  grunt.loadNpmTasks('grunt-run');

  grunt.registerTask('build', 'Build and test the system', function(target) {
    if (target == 'nologs') 
      appConfig.enableLogs = false;

    var tasks = ['copy', 'jshint', 'browserify', 'removelogging:test', 'jasmine', 'run:make', 'run:test'];
    if (!appConfig.enableLogs)
      tasks.push('removelogging:build');
    grunt.task.run(tasks);
  });

  grunt.registerTask('default', ['build']);
  grunt.registerTask('deploy', ['gh-pages']);

};
