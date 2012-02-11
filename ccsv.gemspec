# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "ccsv"
  s.version = "0.1.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 1.2") if s.respond_to? :required_rubygems_version=
  s.authors = ["Evan Weaver"]
  s.date = "2012-02-11"
  s.description = "A pure-C CSV parser."
  s.email = ""
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["CHANGELOG", "LICENSE", "README.rdoc", "ext/ccsv.c", "ext/ccsv.h", "ext/extconf.rb"]
  s.files = ["CHANGELOG", "LICENSE", "Manifest", "README.rdoc", "Rakefile", "ext/ccsv.c", "ext/ccsv.h", "ext/extconf.rb", "test/data.csv", "test/data_small.csv", "test/unit/test_ccsv.rb", "ccsv.gemspec"]
  s.homepage = "http://github.com/evan/ccsv/"
  s.rdoc_options = ["--line-numbers", "--inline-source", "--title", "Ccsv", "--main", "README.rdoc"]
  s.require_paths = ["lib", "ext"]
  s.rubyforge_project = "fauna"
  s.rubygems_version = "1.8.15"
  s.summary = "A pure-C CSV parser."
  s.test_files = ["test/unit/test_ccsv.rb"]

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
