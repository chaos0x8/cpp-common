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

require 'date'
require 'pathname'

gem 'rake-builder', '~> 0.3.0'

require 'RakeBuilder'

#FLAGS = [ '--std=c++14', '-g' ]
FLAGS = [ '--std=c++14', '-O3', '-s', '-DNDEBUG' ]

PKG_MAP = {
  'Gtkmm' => ['gtkmm-3.0'],
  'EmbededRuby' => ['ruby'],
  'SqLite' => ['sqlite3'],
  'GL' => ['glew'],
  'CurlWrapper' => ['libcurl']
}

license = File.open('LICENSE', 'r') { |f|
    result = Array.new

    result << "/*!\n"
    f.each_line { |line|
        line = line.gsub('#{year}', DateTime::now.year.to_s)

        result << " *  #{line}".rstrip + "\n"
    }
    result << " */\n\n"

    result.join('')
}

generated = Array.new

generated << GeneratedFile.new { |t|
    t.name = "Source/Generated/CacheLineSize.hpp"
    t.code = Proc.new {
        puts "Generating '#{t.name}'..."

        cacheLineSize = `getconf LEVEL1_DCACHE_LINESIZE`.chomp

        File.open(t.name, 'w') { |f|
            f.write license
            f.write "#pragma once\n"
            f.write "\n"
            f.write "#define CACHE_LINE_SIZE (#{cacheLineSize}ul)\n"
            f.write "\n"
        }
    }
    t.description = "Generates '#{t.name}'"
}

tmp = [ 'Source/Generated', 'Source/Common/Exceptions' ] + Dir['Source/*'].select{ |x| File.directory?(x) }
tmp.uniq.each { |dir|
    generated << GeneratedFile.new { |t|
        t.name = "#{dir}.hpp"
        t.code = Proc.new {
            puts "Generating '#{t.name}'..."

            headers = Dir["#{dir}/*.hpp"].collect { |h|
                Pathname.new(h).relative_path_from(Pathname.new('Source'))
            }

            File.open(t.name, 'w') { |f|
                f.write license
                f.write "#pragma once\n"
                f.write "\n"
                f.write headers.collect{ |h| "#include <#{h}>\n" }.join('')
                f.write "\n"
            }
        }
        t.description = "Generates '#{t.name}'"
    }
}

libCommon = Library.new { |t|
    t.name = 'lib/libcommon.a'
    t.includes << [ 'Source' ]
    t.flags << [ FLAGS ]
    t.requirements << generated
    t.sources << Dir['Source/Common/**/*.cpp'] - Dir['Source/Common/TestModules/*.cpp']
    t.description = "Builds library '#{t.name}'"
}

libraries = Array.new
uts = Array.new

uts << Executable.new { |t|
    t.name = "bin/Common-ut"
    t.includes << [ 'Source' ]
    t.flags << [ FLAGS, '-pthread' ]
    t.requirements << generated
    t.libs << [ '-lgtest', '-lgmock' ]
    t.libs << [ libCommon ]
    t.sources << Dir['Source/Common/TestModules/*.cpp']
    t.description = "Builds ut target '#{t.name}'"
}

tmp = Dir['Source/*'].select{ |x| File.directory?(x) } - [ 'Source/Common' ]
tmp.each { |dir|
    pkgs = PKG_MAP[File.basename(dir)] || []

    sources = Dir["#{dir}/**/*.cpp"] - Dir["#{dir}/**/TestModules/*.cpp"]

    libraries << Library.new { |t|
        t.name = "lib/libcommon#{File.basename(dir)}.a"
        t.includes << [ 'Source' ]
        t.flags << [ FLAGS ]
        t.requirements << generated
        t.libs << libCommon
        t.pkgs << pkgs
        t.sources << sources
        t.description = "Builds library '#{t.name}'"
    } if sources.size > 0

    sources = Dir["#{dir}/**/TestModules/*.cpp"]

    uts << Executable.new { |t|
        t.name = "bin/#{File.basename(dir)}-ut"
        t.description = "Builds ut target '#{t.name}'"
        t.includes << [ 'Source' ]
        t.flags << [ FLAGS, '-pthread' ]
        t.requirements << generated
        t.libs << [ '-lgtest', '-lgmock' ]
        t.libs << libraries.select { |l| l.name == "lib/libcommon#{File.basename(dir)}.a" }
        t.libs << libCommon
        t.pkgs << pkgs
        t.sources << sources
    } if sources.size > 0
}

desc 'Builds all generated files'
multitask(:generated => RakeBuilder::Names[generated])

desc 'Builds all libraries'
multitask(:libraries => RakeBuilder::Names[libraries])

desc 'Builds everything & runs uts'
multitask(:default => RakeBuilder::Names[libraries, uts]) {
    uts.each { |x|
        sh x.name
    }
}

