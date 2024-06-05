#!/usr/bin/env ruby

# frozen_string_literal: true

require 'erb'
require 'json'

require File.expand_path('error.rb', __dir__)
require File.expand_path('templates.rb', __dir__)
require File.expand_path('version.rb', __dir__)

# Extension of the ruby string class
class String
  # Source: https://github.com/rails/rails/blob/main/activesupport/lib/active_support/core_ext/string/filters.rb#L15
  # Performs a destructive squish. See String#squish.
  #   str = " foo   bar    \n   \t   boo"
  #   str.squish!                         # => "foo bar boo"
  #   str                                 # => "foo bar boo"
  def squish!
    gsub!(/[[:space:]]+/, ' ')
    strip!
    self
  end

  def camelize
    to_s.split('_').collect(&:capitalize).join
  end
end

module ErrorGen
  # Critical errors source code generator
  class Generator
    attr_reader :ret_msg, :errors, :gen_dir, :jsonfile
    attr_accessor :target_name, :prefix

    include ErrorGen::Templates

    def initialize(filename = nil)
      @ret_msg = nil
      @target_name = nil
      @prefix = nil
      @errors = []

      return unless filename

      @jsonfile = filename
      parse(filename)
    end

    # Parse the input file
    def parse(filename)
      raise ArgumentError, 'File must be in JSON format' unless File.extname(filename) == '.json'

      # Get the basename of the generated files
      @gen_dir = File.dirname(@jsonfile = filename)
      @target_name = File.basename(filename, '.json') unless @target_name && !@target_name.empty?

      # Check if prefix is valid
      valid_prefix = /^[a-zA-Z_]\w*$/
      unless @prefix.nil? || valid_prefix.match?(@prefix)
        raise ArgumentError, 'The prefix must be a valid keyword (upper or ' \
          'lower case characters, underscores and number) and must not start with a number'
      end

      # Change prefix to lowercase
      @prefix&.downcase!

      # Parse JSON file
      begin
        json = JSON.parse(File.read(filename))
        fetch_data(json)
      rescue JSON::ParserError
        @ret_msg = 'Error while parsing JSON'
        nil
      end
    end

    def fetch_data(json)
      # Save data
      json['errors'].each do |error|
        name = error['name']
        timeout = error['timeout']
        instances = error['instances']

        # Ignore errors with missing fields
        @errors.push(Error.new(name, timeout, instances)) unless name.nil? || timeout.nil? || instances.nil?
      end
    end

    def generate_h(basename = @target_name)
      filename = "#{@gen_dir}/#{basename}.h"

      File.open(filename, 'w') do |f|
        f.puts ERB.new(HEADER, trim_mode: '<>').result(binding)
        f.puts ERB.new(HH, trim_mode: '<>').result(binding)
      end
      filename
    end

    def generate_c(basename = @target_name)
      filename = "#{@gen_dir}/#{basename}.c"

      File.open(filename, 'w') do |f|
        f.puts ERB.new(HEADER, trim_mode: '<>').result(binding)
        f.puts ERB.new(CC, trim_mode: '<>').result(binding)
      end
      filename
    end
  end
end
