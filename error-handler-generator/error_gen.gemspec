# frozen_string_literal: true

require File.expand_path('lib/version.rb', __dir__)

# Get current date
date = Time.now.strftime('%Y-%m-%m')

Gem::Specification.new do |spec|
  spec.name = 'error-gen'
  spec.version = ErrorGen::VERSION
  spec.date = date
  spec.summary = 'Critical error handler source generator for embedded systems'
  spec.description = 'A C/C++ code generator that creates source files to manage
  critical errors in a embedded system environment'
  spec.authors = ['Antonio Gelain']
  spec.email = 'antonio.gelain@studenti.unitn.it'
  spec.files = Dir['bin/error_gen', 'lib/**/*.rb', 'error_gen.gemspec']
  spec.executables << 'error_gen'
  spec.homepage = 'https://github.com/eagletrt/micro-utils/tree/master/error-handler-generator'
  spec.license = 'AGPL-3.0-only'
  spec.platform = Gem::Platform::RUBY
  spec.required_ruby_version = '>= 2.7.0'
  spec.metadata = {
    'source_code_uri' => 'https://github.com/eagletrt/micro-utils/tree/master/error-handler-generator'
  }
end
