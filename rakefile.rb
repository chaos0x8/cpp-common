#!/usr/bin/ruby

def web_require url
    system "wget #{url}" unless File.exist?(File.basename(url))
    require_relative File.basename(url)
end

web_require "https://raw.github.com/chaos0x8/ruby/master/rake-builder/RakeBuilder.rb"

INCLUDES = [ "Source" ]
FLAGS = [ "--std=c++1y", "-O3", "-DNDEBUG", "-s" ]
UT_LIBS = [ "-lgtest", "-lgmock", "-lpthread" ]

Library.new do |t|
    t.name = "lib/libcppCommon.a"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/*.cpp", "Source/Exceptions/*.cpp" ]
end

Application.new do |t|
    t.name = "bin/cppCommon-ut"
    t.includes = INCLUDES
    t.flags = FLAGS
    t.files = FileList[ "Source/TestModules/*.cpp" ]
    t.dependencies = [ "lib/libcppCommon.a" ]
    t.libs = [ "-lpthread", "-lgtest", "-lgmock", "-Llib", "-lcppCommon" ]
end

task :ut => [ "bin/cppCommon-ut" ] do
    sh "bin/cppCommon-ut"
end

task :default => [ "lib/libcppCommon.a", :ut ]
