require 'bundler'
Bundler.require(:default, :test)

require 'minitest/autorun'
require 'minitest/benchmark'
require 'ccsv'
require 'csv'

__END__
TEST_CSV_FILES={
	100=>"/tmp/test1.csv",
	10000=>"/tmp/test2.csv",
	100000=>"/tmp/test3.csv"
}

def create_csvf(name,limit)
	open(name,"w") do |f|
        	1.upto(limit) do |n|
			f.puts [n,2*n,3+n].join(',')
		end
	end
end

describe "Ccsv Benchmark" do
  bench_range { [100,10_000,100_000] } 
  before do
	  [100,10_000,100_000].each do |n|
		  create_csvf(TEST_CSV_FILES[n],n)
	  end
  end

  after do
	  [100,10_000,100_000].each do |n|
		  FileUtils.rm TEST_CSV_FILES[n]
	  end
  end

  bench_performance_linear "Ccsv", 0.8 do |n|
	Ccsv.foreach(TEST_CSV_FILES[n]) do |x| end
  end
  bench_performance_linear "CSV", 0.8 do |n|
	CSV.foreach(TEST_CSV_FILES[n]) do |x| end
  end
end
