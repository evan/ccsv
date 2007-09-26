
require 'test/unit'
require 'ccsv'
require 'csv'

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
    start = Time.now
    Ccsv.foreach(@dir + "data.csv") do |values|     
    end
    dur = Time.now - start
    puts "\n#{dur} seconds\n"
  end
  
end
