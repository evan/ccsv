# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name        = "ccsv"
  s.version     = 1.1.0
  s.authors     = ["Evan Weaver"]
  s.email       = ["evan@fauna.org"]
  s.homepage    = "https://github.com/evan/ccsv"
  s.summary     = %q{A pure-C CSV parser for Ruby}
  s.license     = 'AFL'

  s.files         = `git ls-files`.split("\n")
  s.test_files    = s.files.grep(/^spec/)
  s.require_paths = ["lib"]
end