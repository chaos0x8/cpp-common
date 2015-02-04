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
    t.libs = [ "-lgtest", "-lgmock", "-Llib", "-lcppCommon", "-lpthread" ]
end

task :ut => [ "bin/cppCommon-ut" ] do
    sh "bin/cppCommon-ut"
end

task :default => [ "lib/libcppCommon.a", :ut ]
