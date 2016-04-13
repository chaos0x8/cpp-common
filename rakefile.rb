#!/usr/bin/ruby

# \author <https://github.com/chaos0x8>
# \copyright
# Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
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

def web_require url
    system "wget #{url}" unless File.exist?(File.basename(url))
    require_relative File.basename(url)
end

web_require "https://raw.github.com/chaos0x8/rake-builder/master/RakeBuilder.rb"

if ARGV.index("nodebug").nil?
    FLAGS = [ "--std=c++14", "-g" ]
else
    FLAGS = [ "--std=c++14", "-O3", "-s", "-DNDEBUG" ]

    task :nodebug
end

INCLUDES = [ "Source" ]
generatedFiles = [ "Source/Generated/CacheLineSize.hpp" ]

def authorNotice
    f = File.open "LICENSE", "r"
    data = f.read
    f.close

    text = "/*!\n"
    data.split("\n").each do |line|
        if line.strip == ""
            text += "\n"
        else
            text += "    #{line}\n"
        end
    end

    "#{text}*/\n\n"
end

def generateDirectoryInclude *directories
    existing = Dir.glob("Source/*").select { |d| File.directory?(d) }
    existing = existing.collect { |d| File.basename(d) }

    (directories + (existing - directories)).each do |directory|
        yield "Source/#{directory}.hpp"

        file "Source/#{directory}.hpp" => "rakefile.rb" do |fileName|
            raise RuntimeError.new("Directory doesn't exist: 'Source/#{directory}'") unless File.directory?("Source/#{directory}")
            puts "Generating '#{fileName}'..."

            files = FileList["Source/#{directory}/*.hpp"]

            f = File.open(fileName.to_s, "w")
            f.write authorNotice
            f.write "#pragma once\n"
            f.write "\n"
            files.each { |x| f.write "#include <#{directory}/#{File.basename(x)}>\n" }
            f.write "\n"
            f.close
        end
    end
end

directory "Source/Generated"
file "Source/Generated/CacheLineSize.hpp" => [ "Source/Generated", "rakefile.rb" ] do |fileName|
    CACHE_LINE_SIZE = `getconf LEVEL1_DCACHE_LINESIZE`.chomp

    puts "Generating '#{fileName}'..."

    f = File.open(fileName.to_s, "w")
    f.write authorNotice
    f.write "#pragma once\n"
    f.write "\n"
    f.write "#define CACHE_LINE_SIZE (#{CACHE_LINE_SIZE}ul)\n"
    f.write "\n"
    f.close
end

generateDirectoryInclude 'Generated', 'Common/Exceptions' do |fileName|
    generatedFiles.push fileName
end

Library.new do |t|
    t.name = "lib/libcommon.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Common/*.cpp", "Source/Common/Exceptions/*.cpp" ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = "lib/libcommonSqLite.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/SqLite/*.cpp" ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = "lib/libcommonGtkmm.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Gtkmm/*.cpp" ]
    t.libs = [ Pkg.new("gtkmm-3.0") ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = "lib/libcommonParallel.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Parallel/*.cpp" ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = "lib/libcommonNetwork.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Network/*.cpp", "Source/Network/Detail/*.cpp" ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = "lib/libcommonGL.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/GL/*.cpp", "Source/GL/Detail/*.cpp" ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = "lib/libcommonSfml.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Sfml/*.cpp", "Source/Sfml/Detail/*.cpp" ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = 'lib/libcommonNCurses.a'
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ 'Source/NCurses/*.cpp' ]
    t.libs = [ Pkg.new('ncurses') ]
    t.dependencies = generatedFiles
end

Library.new do |t|
    t.name = 'lib/libcommonEmbededRuby.a'
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ 'Source/EmbededRuby/*.cpp', 'Source/EmbededRuby/Detail/*.cpp' ]
    t.libs = [ Pkg.new('ruby-1.9') ]
    t.dependencies = generatedFiles
end

Application.new do |t|
    t.name = "bin/common-ut"
    t.includes = INCLUDES
    t.flags = FLAGS + [ '-DUT_MODE=1' ]
    t.files = FileList[ "Source/Common/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommon.a" ] + generatedFiles
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommon", "-lpthread", "-lrt" ]
end

Application.new do |t|
    t.name = "bin/commonTraits-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Traits/TestModules/*.cpp" ]
    t.libs = [ "-lgtest", "-lgmock", "-lpthread" ]
end

Application.new do |t|
    t.name = "bin/commonParallel-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Parallel/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommonParallel.a" ] + generatedFiles
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommonParallel", "-lpthread" ]
end

Application.new do |t|
    t.name = "bin/commonSqLite-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/SqLite/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommonSqLite.a" ] + generatedFiles
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommonSqLite", "-lpthread", Pkg.new('sqlite3') ]
end

Application.new do |t|
    t.name = "bin/commonNetwork-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Network/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommon.a", "lib/libcommonNetwork.a" ] + generatedFiles
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommonNetwork", "-lcommon", "-lpthread" ]
end

Application.new do |t|
    t.name = "bin/commonGL-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/GL/**TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommon.a", "lib/libcommonGL.a" ] + generatedFiles
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommonGL", "-lcommon", "-lpthread", Pkg.new('glew') ]
end

Application.new do |t|
    t.name = 'bin/commonEmbededRuby-ut'
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ 'Source/EmbededRuby/**TestModules/*.cpp' ]
    t.dependencies = [ 'lib/libcommon.a', 'lib/libcommonEmbededRuby.a' ] + generatedFiles
    t.libs = [ '-lgtest', '-lgmock', '-Llib', '-lcommonEmbededRuby', '-lcommon', '-lpthread', Pkg.new('ruby-1.9') ]
end

utList = { :commonUT => 'bin/common-ut', :commonTraitsUT => 'bin/commonTraits-ut', :commonSqLiteUT => 'bin/commonSqLite-ut', :commonParallelUT => 'bin/commonParallel-ut',
           :commonNetworkUT => 'bin/commonNetwork-ut', :commonGlUT => 'bin/commonGL-ut', :commonEmbededRubyUT => 'bin/commonEmbededRuby-ut' }
utList.each do |tar, res|
    desc "Compiles and runs ut fule #{res}"
    multitask tar => res do
        sh res
    end
end

subProjects = Dir['*/**/rakefile.rb']

desc 'Compile and run all uts'
task :ut => utList.keys

desc 'Compile all libs'
multitask :libs => Target.tasks(/\.a$/)

desc 'Compile all libs and run all uts'
task :default => [ :libs, :ut ]
