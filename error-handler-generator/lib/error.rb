# frozen_string_literal: true

module ErrorGen
  # Class that represent a single critical error group
  # that will be handled by the generated source code
  class Error
    attr_reader :details
    attr_accessor :name, :timeout, :instances, :description

    # Info about a single instance of a single error
    class InstanceDetails
      attr_accessor :id, :alias

      def initialize(id, alias_name)
        @id = id
        @alias = alias_name
      end
    end

    def initialize(name, timeout, instances, description: nil, details: nil)
      @name = name
      @timeout = timeout
      @instances = instances

      # Optionals
      @description = description
      @details = []
      details&.each do |id, alias_name|
        @details.push(InstanceDetails.new(id, alias_name))
      end
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
