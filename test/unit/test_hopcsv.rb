
require 'test/unit'
require 'hopcsv'
require 'benchmark'

# Yeah, I know.
begin
  require 'csv'
  require 'rubygems'
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
      Hopcsv.foreach('fdssfd') do
      end
    end
  end

  def test_accuracy
    ccsv = []
    file = @dir + "data_small.csv"
    Hopcsv.foreach(file) do |values|
      ccsv << values.dup
    end
    csv = []
    CSV.foreach(file) do |values|
      csv << values
    end
    assert_equal csv, ccsv
  end

  def test_speed
    Benchmark.bm(5) do |x|
      [Hopcsv, CSV].each do |klass| # CSVScan, LightCsv,
        x.report(klass.name) do
          klass.foreach(@dir + "data.csv") do |values| end
        end
      end
    end
  end

end
