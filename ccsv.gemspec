# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "ccsv"
  s.version = "1.1.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 1.2") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib", "ext"]
  s.authors = ["Evan Weaver, Sergey Zhumatiy"]
  s.date = "2014-11-21"
  s.description = "Ruby CSV parser gem, written in pure C."
  s.email = "serg@parallel.ru"
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["CHANGELOG", "LICENSE", "README.rdoc", "ext/ccsv.c", "ext/ccsv.h", "ext/extconf.rb", "lib"]
  s.files = ["CHANGELOG", "Gemfile", "LICENSE", "Manifest", "README.rdoc", "Rakefile", "ccsv.gemspec", "compile", "csv-benchmark/Gemfile", "csv-benchmark/README.md", "csv-benchmark/benchmark.rb", "csv-benchmark/csv/geoip.csv", "csv-benchmark/csv/presidents.csv", "csv-benchmark/csv_spec.rb", "csv-benchmark/fixtures/bad-encoding.csv", "csv-benchmark/fixtures/blank-field.csv", "csv-benchmark/fixtures/blank-rows.csv", "csv-benchmark/fixtures/col-sep-in-field.csv", "csv-benchmark/fixtures/empty-file.csv", "csv-benchmark/fixtures/escaped-quote.csv", "csv-benchmark/fixtures/long-field.csv", "csv-benchmark/fixtures/long-row.csv", "csv-benchmark/fixtures/no-col-sep.csv", "csv-benchmark/fixtures/no-row-sep.csv", "csv-benchmark/fixtures/ragged-rows.csv", "csv-benchmark/fixtures/row-sep-in-field.csv", "csv-benchmark/fixtures/trailing-col-sep.csv", "csv-benchmark/fixtures/unescaped-quote-in-quoted-field.csv", "csv-benchmark/fixtures/unescaped-quote.csv", "csv-benchmark/fixtures/unmatched-quote.csv", "csv-benchmark/fixtures/whitespace-after-quoted-field.csv", "csv-benchmark/fixtures/whitespace-around-unquoted-field.csv", "csv-benchmark/fixtures/whitespace-before-quoted-field.csv", "ext/ccsv.c", "ext/ccsv.h", "ext/extconf.rb", "lib", "spec/bench_spec.rb", "spec/ccsv_spec.rb"]
  s.homepage = "http://github.com/evan/ccsv/"
  s.licenses = ["AFL"]
  s.rdoc_options = ["--line-numbers", "--title", "Ccsv", "--main", "README.rdoc"]
  s.rubyforge_project = "evan"
  s.rubygems_version = "2.2.2"
  s.summary = "A pure-C CSV parser."
end
