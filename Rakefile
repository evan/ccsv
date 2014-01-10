require 'bundler'
Bundler.require(:default, :development)

require 'echoe'

Echoe.new("ccsv") do |p|
  p.author = ["Evan Weaver","Sergey Zhumatiy"]
  p.project = "evan"
  p.email = "serg@parallel.ru"
  p.summary = "A pure-C CSV parser."
  p.description = "Ruby CSV parser gem, written in pure C."
  p.url = "http://github.com/evan/ccsv/"
  p.docs_host = "evan.github.com/fauna/"
end

require 'rake/testtask'
Rake::TestTask.new do |t|
	  t.pattern = "spec/*_spec.rb"
end
