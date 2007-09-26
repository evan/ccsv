
require 'test/unit'
require 'ccsv'

# Yeah, I know.
require 'csv'
require 'rubygems'
require 'fastercsv'
require '/Users/eweaver/tmp/lightcsv-0.2/lib/lightcsv'
require 'benchmark'

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
    [Ccsv, LightCsv, FasterCSV, CSV].each do |klass|
      Benchmark.bm(1) do |x|
        x.report(klass.name) do 
          klass.foreach(@dir + "data.csv") do |values| end
        end
      end
    end
  end
  
end
