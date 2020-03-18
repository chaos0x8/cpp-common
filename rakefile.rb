#!/usr/bin/ruby

# \author <https://github.com/chaos0x8>
# \copyright
# Copyright (c) 2015 - 2017, <https://github.com/chaos0x8>
#
# \copyright
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# \copyright
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

gem 'rake-builder', '~> 0.5', '>= 0.5.2'

autoload :FileUtils, 'fileutils'

require 'date'
require 'pathname'
require 'rake-builder'
require 'json'

preGenerated = []
preGenerated << GeneratedFile.new { |t|
  t.name = "Source/Generated/CacheLineSize.hpp"
  t.code = Proc.new {
    puts "Generating '#{t.name}'..."

    cacheLineSize = `getconf LEVEL1_DCACHE_LINESIZE`.chomp
    cacheLineSize = [cacheLineSize.to_i(10), 64].max

    File.open(t.name, 'w') { |f|
      f.write "#pragma once\n"
      f.write "\n"
      f.write "#define CACHE_LINE_SIZE (#{cacheLineSize}ul)\n"
      f.write "\n"
    }
  }
  t.description = "Generates '#{t.name}'"
}

['Source/Common/Exceptions'].each { |dir|
  preGenerated << Generate.includeDirectory(dir)
}

generated = []
Dir['Source/*'].select { |x| File.directory?(x) }.each { |dir|
  generated << Generate.includeDirectory(dir, requirements: preGenerated)
}

libraries = []
uts = []

config = JSON.parse(IO.read("#{File.dirname(__FILE__)}/rakefile.json"))

config['libs'].each { |lib|
  libraries << Library.new { |t|
    if lib['install']
      t.requirements << InstallPkg.new(name: "install-#{lib['name']}", pkgs: lib['install'])
    end

    t.name = lib['name']
    t.requirements << generated << preGenerated
    t.flags << (lib['flags'] || config['flags'])
    t.flags << (lib['std'] || config['std'])
    t.includes << FileList['Source', *(lib['includes'] || [])]
    t.sources << FileList[*lib['sources']]
    t.description = "Builds library #{t.name}"
    t.pkgs << (lib['pkgs'] || [])
  } unless lib['wip']
}

config['uts'].each { |ut|
  uts << Executable.new { |t|
    if ut['install']
      t.requirements << InstallPkg.new(name: "install-#{ut['name']}", pkgs: ut['install'])
    end

    t.name = ut['name']
    t.requirements << generated << preGenerated
    t.flags << (ut['flags'] || config['flags'])
    t.flags << (ut['std'] || config['std'])
    t.includes << FileList['Source', 'Test', *(ut['includes'] || [])]
    t.sources << FileList[*ut['sources']]
    t.description = "Builds library #{t.name}"
    t.libs << ['-pthread', '-lgtest', '-lgmock']
    t.libs << (ut['libs'] || [])
    t.pkgs << (ut['pkgs'] || [])
  } unless ut['wip']
}

#FLAGS = [ '-Wall', '-Werror', '-g', '-Wno-deprecated' ]
FLAGS = [ '-Wall', '-Werror', '-O3', '-s', '-DNDEBUG', '-Wno-deprecated' ]

PKG_MAP = {
  'Gtkmm' => ['gtkmm-3.0'],
  'EmbededRuby' => ['ruby'],
  'SqLite' => ['sqlite3'],
  'GL' => ['glew'],
  'CurlWrapper' => ['libcurl'],
  'CryptoWrapper' => ['libcrypto++']
}

FLAGS_MAP = {
  'Gtkmm' => ['--std=c++14'],
  'EmbededRuby' => ['--std=c++14'],
  :default => ['--std=c++17']
}

desc 'Builds all generated files'
multitask(:generated => Names[generated, preGenerated])

desc 'Removes generated files'
task(:clean_generated) {
  Names[generated, preGenerated].each { |x|
    FileUtils.rm(x, verbose: true) if File.exists?(x)
  }
}

desc 'Builds all libraries'
multitask(:libraries => Names[libraries])

desc 'Builds everything & runs uts'
multitask(:default => Names[libraries, uts]) {
  uts.each { |x|
    sh x.name
  }
}

desc 'Cleans build'
task(:clean) {
  ['.obj', 'bin', 'lib'].each { |fn|
    if File.directory? fn
      FileUtils.rm_rf fn, verbose: true
    elsif File.exist? fn
      FileUtils.rm fn, verbose: true
    end
  }
}
