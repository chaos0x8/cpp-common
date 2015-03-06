#!/usr/bin/ruby

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
GENERATED_FILES = [ "Source/Parallel.hpp", "Source/Exceptions.hpp", "Source/Generated.hpp", "Source/Gtkmm.hpp", "Source/SqLite.hpp", "Source/Generated/CacheLineSize.hpp" ]

def generateDirectoryInclude *directories
    directories.each do |directory|
        file "Source/#{directory}.hpp" => "rakefile.rb" do |fileName|
            puts "Generating '#{fileName}'..."

            files = FileList["Source/#{directory}/*.hpp"]

            f = File.open(fileName.to_s, "w")
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
    f.write "#pragma once\n"
    f.write "\n"
    f.write "#define CACHE_LINE_SIZE (#{CACHE_LINE_SIZE}ul)\n"
    f.write "\n"
    f.close
end

generateDirectoryInclude "Parallel", "Exceptions", "Generated", "Gtkmm", "SqLite"

Library.new do |t|
    t.name = "lib/libcommon.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/*.cpp", "Source/Exceptions/*.cpp" ]
    t.dependencies = GENERATED_FILES
end

Library.new do |t|
    t.name = "lib/libcommonSqLite.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/SqLite/*.cpp" ]
    t.dependencies = GENERATED_FILES
end

Library.new do |t|
    t.name = "lib/libcommonGtkmm.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Gtkmm/*.cpp" ]
    t.libs = [ Pkg.new("gtkmm-3.0") ]
    t.dependencies = GENERATED_FILES
end

Application.new do |t|
    t.name = "bin/common-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommon.a" ] + GENERATED_FILES
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommon", "-lpthread" ]
end

Application.new do |t|
    t.name = "bin/commonParallel-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/Parallel/TestModules/*.cpp" ]
    t.dependencies = GENERATED_FILES
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommon", "-lpthread" ]
end

Application.new do |t|
    t.name = "bin/commonSqLite-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/SqLite/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcommonSqLite.a" ] + GENERATED_FILES
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcommonSqLite", "-lpthread" ]
end

task :ut => [ "bin/common-ut", "bin/commonSqLite-ut", "bin/commonParallel-ut" ] do
    sh "bin/common-ut"
    sh "bin/commonSqLite-ut"
    sh "bin/commonParallel-ut"
end

task :default => [ "lib/libcommon.a", "lib/libcommonSqLite.a", "lib/libcommonGtkmm.a", :ut ]
task :generated => GENERATED_FILES
