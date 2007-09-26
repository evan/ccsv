
require 'test/unit'
require 'ccsv'

class TestCcsv < Test::Unit::TestCase
  
  def setup
    @data = "cat,dog,fish,14\n57,-5,fish,14\ndog,cat,fish,14"
    @filename = '/tmp/ccsv.test.csv'
    File.open(@filename, 'w') do |f|
      f.puts @data
    end
  end
  
  def test_ccsv
    a = []
    Ccsv.foreach(@filename) do |*values|
      a << values
    end
    assert_equal @data, a.map{|s| s.join(',') }.join("\n")
  end
  
end
