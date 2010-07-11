Object.send(:remove_const, :DateTime) if defined?(::DateTime)
Object.send(:remove_const, :Date) if defined?(::Date)
require 'home_run_date'
