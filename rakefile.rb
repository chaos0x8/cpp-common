#!/usr/bin/ruby

# \author <https://github.com/chaos0x8>
# \copyright
# Copyright (c) 2015, <https://github.com/chaos0x8>
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

web_require "https://raw.github.com/chaos0x8/ruby/master/rake-builder/RakeBuilder.rb"

if ARGV.index("nodebug").nil?
    FLAGS = [ "--std=c++1y", "-g" ]
else
    FLAGS = [ "--std=c++1y", "-O3", "-s", "-DNDEBUG" ]

    task :nodebug
end

INCLUDES = [ "Source" ]
UT_LIBS = [ "-lgtest", "-lgmock", "-lpthread" ]
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
    directories.each do |directory|
        yield "Source/#{directory}.hpp"

        file "Source/#{directory}.hpp" => "rakefile.rb" do |fileName|
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

generateDirectoryInclude "Traits", "Parallel", "Generated", "Gtkmm", "SqLite", "Common", "Network", "Sfml", "GL" do |fileName|
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

Application.new do |t|
    t.name = "bin/common-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Common/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommon.a" ] + generatedFiles
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommon", "-lpthread", "-lrt" ]
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
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommonSqLite", "-lpthread" ]
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

task :commonUT => "bin/common-ut" do
    sh "bin/common-ut"
end
task :commonSqLiteUT => "bin/commonSqLite-ut" do
    sh "bin/commonSqLite-ut"
end
task :commonParallelUT => "bin/commonParallel-ut" do
    sh "bin/commonParallel-ut"
end
task :commonNetworkUT => "bin/commonNetwork-ut" do
    sh "bin/commonNetwork-ut"
end
task :commonGlUT => "bin/commonGL-ut" do
    sh "bin/commonGL-ut"
end

task :ut => [ :commonUT, :commonSqLiteUT, :commonParallelUT, :commonNetworkUT, :commonGlUT ]

task :default => [ "lib/libcommonGtkmm.a", "lib/libcommonSfml.a", :ut ]
task :generated => generatedFiles
