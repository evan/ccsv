require 'bundler'
Bundler.require(:default, :test)

gem 'minitest'
#require 'minitest/benchmark'
require 'minitest/autorun'
#require 'minitest/spec'
require 'ccsv'
#require 'csv'

TEST_CSV="/tmp/test.csv"

module CSVScan
  def self.foreach(file, &block)
    open(file) do |f|
      scan(f, &block)
    end
  end
end

def create_csv(delimiter=',')
	open(TEST_CSV,"w") do |f|
        	1.upto(100000) do |n|
			f.puts [n,2*n,3+n].join(delimiter)
		end
	end
end


describe Ccsv do
	before do
		@csv=[]
	end

	it 'reads csv with default delimiter' do
		create_csv
		Ccsv.foreach(TEST_CSV) do |v|
			@csv << v
		end
		@csv[15000].must_equal(['15001','30002','15004'])
		@csv.size.must_equal(100000)
	end

	it 'reads csv with tab delimiter' do
		create_csv("\t")
		Ccsv.foreach(TEST_CSV,"\t") do |v|
			@csv << v
		end
		@csv[15000].must_equal(['15001','30002','15004'])
	end

	it 'reads csv with comma delimiter' do
		create_csv(',')
		Ccsv.foreach(TEST_CSV,',') do |v|
			@csv << v
		end
		@csv[15000].must_equal(['15001','30002','15004'])
	end

	it 'raises error' do
		proc {
			Ccsv.foreach('/non-existent-file') do |x| end
		}.must_raise(RuntimeError)
	end

#	bench_performance_linear 'just read', 0.9 do |n|
#		create_csv(',',n)
#		Ccsv.foreach(TEST_CSV)
#	end
end

#describe 'my benchmarks' do
#end

__END__
require 'test/unit'
require 'ccsv'
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
    Benchmark.bm(5) do |x|
      [Ccsv, CSV].each do |klass| # CSVScan, LightCsv,
        x.report(klass.name) do
          klass.foreach(@dir + "data.csv") do |values| end
        end
      end
    end
  end

end
