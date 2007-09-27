
require 'test/unit'
require 'ccsv'
require 'benchmark'

# Yeah, I know.
begin
  require 'csv'
  require 'rubygems'
  require 'fastercsv'
  require 'lightcsv'
  require 'csvscan'

  module CSVScan
    def self.foreach(file, &block)
      open(file) do |f|
        scan(f, &block)
      end
    end
  end
  
rescue LoadError
end

class TestCcsv < Test::Unit::TestCase
  
  def setup
    @dir = "#{File.dirname(__FILE__)}/../"
  end
  
  def test_should_raise
    assert_raises(RuntimeError) do
      Ccsv.foreach('fdssfd') do
      end
    end
  end
  
  def test_accuracy
    ccsv = []
    file = @dir + "data_small.csv"
    Ccsv.foreach(file) do |values|
      ccsv << values.dup
    end
    csv = []
    CSV.foreach(file) do |values|
      csv << values
    end    
    assert_equal csv, ccsv
  end
  
  def test_speed
    [Ccsv, CSVScan, LightCsv, FasterCSV, CSV].each do |klass| 
      Benchmark.bm do |x|
        x.report(klass.name) do 
          klass.foreach(@dir + "data.csv") do |values| end
        end
      end
    end
  end
  
end
