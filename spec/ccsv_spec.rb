require 'bundler'
Bundler.require(:default, :test)

require 'minitest/spec'
require 'minitest/autorun'
require 'ccsv'

TEST_CSV="/tmp/test.csv"
DEF_SIZE=20000

def create_csv(delimiter=',',limit=DEF_SIZE)
	open(TEST_CSV,"w") do |f|
        	1.upto(limit) do |n|
			f.puts [n,2*n,3+n].join(delimiter)
		end
	end
end


describe Ccsv do
	before do
		@csv=[]
	end

	after do
		begin; FileUtils.rm TEST_CSV; rescue; end
	end

	it 'reads csv with default delimiter' do
		create_csv
		Ccsv.foreach(TEST_CSV) do |v|
			@csv << v
		end
		@csv[15000].must_equal(['15001','30002','15004'])
		@csv.size.must_equal(DEF_SIZE)
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

	it 'reads csv with a multi-character delimiter' do
		create_csv('|^|')
		Ccsv.foreach(TEST_CSV,'|^|') do |v|
			@csv << v
		end
		@csv[15000].must_equal(['15001','30002','15004'])
	end

	it 'raises error' do
		proc {
			Ccsv.foreach('/non-existent-file') do |x| end
		}.must_raise(RuntimeError)
	end

	it 'filters csv' do
		create_csv
		Ccsv.foreach(TEST_CSV,',',0,[3000..4000]) do |v|
			@csv << v
		end
		@csv[0].must_equal(['3001','6002','3004'])
		@csv.size.must_equal(999)
	end

  it 'reads a line with an escaped delimiter' do
    open(TEST_CSV, 'w') do |file|
      file.puts "field1,field2,field3,field\\,with comma,field5"
    end
    Ccsv.foreach(TEST_CSV, ',') do |v|
      v[0].must_equal('field1')
      v[1].must_equal('field2')
      v[2].must_equal('field3')
      v[3].must_equal('field\\,with comma')
      v[4].must_equal('field5')
    end
  end
end
