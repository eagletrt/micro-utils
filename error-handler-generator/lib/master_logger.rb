# frozen_string_literal: true

require 'logger'

module ErrorGen
  # Static class used to log info, warnings and errors to the terminal
  class MasterLogger
    @log = Logger.new($stderr)

    def self.setup(log_level = Logger::INFO)
      @log.level = log_level
      @log.formatter = proc do |severity, _datetime, _progname, msg|
        case severity
        when 'ANY'
          "\033[1;35m[UNKNOWN]\033[0m: #{msg}\n"
        when 'FATAL'
          "\033[1;31m[FATAL]\033[0m: #{msg}\n"
        when 'ERROR'
          "\033[1;31m[ERROR]\033[0m: #{msg}\n"
        when 'WARN'
          "\033[1;33m[WARNING]\033[0m: #{msg}\n"
        when 'INFO'
          "\033[1;34m[INFO]\033[0m: #{msg}\n"
        when 'DEBUG'
          "\033[1;32m[DEBUG]\033[0m: #{msg}\n"
        end
      end
    end

    def self.unknown(msg)
      @log.unknown(msg)
    end

    def self.fatal(msg)
      @log.fatal(msg)
    end

    def self.error(msg)
      @log.error(msg)
    end

    def self.warn(msg)
      @log.warn(msg)
    end

    def self.info(msg)
      @log.info(msg)
    end

    def self.debug(msg)
      @log.debug(msg)
    end
  end
end
