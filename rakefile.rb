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

file "Source/CacheLineSize.hpp" do |fileName|
    CACHE_LINE_SIZE = `getconf LEVEL1_DCACHE_LINESIZE`.chomp

    puts "Generating '#{fileName}'..."

    f = File.open(fileName.to_s, "w")
    f.write "#pragma once\n"
    f.write "\n"
    f.write "#define CACHE_LINE_SIZE (#{CACHE_LINE_SIZE}ul)\n"
    f.write "\n"
    f.close
end

Library.new do |t|
    t.name = "lib/libcppCommon.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/*.cpp", "Source/Exceptions/*.cpp" ]
    t.dependencies = [ "Source/CacheLineSize.hpp" ]
end

Application.new do |t|
    t.name = "bin/cppCommon-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcppCommon.a" ]
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcppCommon", "-lpthread" ]
end

task :ut => [ "bin/cppCommon-ut" ] do
    sh "bin/cppCommon-ut"
end

task :default => [ "lib/libcppCommon.a", :ut ]
