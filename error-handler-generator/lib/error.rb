# frozen_string_literal: true

module ErrorGen
  # Class that represent a single critical error instance
  # that will be handled by the generated source code
  class Error
    attr_accessor :name, :timeout, :instances

    def initialize(name, timeout, instances)
      @name = name
      @timeout = timeout
      @instances = instances
    end

    def to_s
      <<~STRING
        #{@name}: {
          timeout: #{@timeout},
          instances: #{@instances}
        }
      STRING
    end
  end
end
