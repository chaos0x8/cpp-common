gem 'rake-builder', '~> 0.6', '>= 0.6.1'

require 'rake-builder'
require 'open3'
require 'json'

config = JSON.parse(IO.read('rakefile.json'))

files = []

config['std::string'].each { |name|
  files << GeneratedFile.new { |t|
    t.desc = 'Generated file'
    t.name = "#{name}.hpp"
    t.code = proc {
      d = []
      d << "#pragma once"
      d << ""
      d << "#include <stdexcept>"
      d << "#include <string>"
      d << ""
      d << "namespace Common::Exceptions {"
      d << "  using namespace std::string_literals;"
      d << ""
      d << "  class #{name} : public std::runtime_error {"
      d << "  public:"
      d << "    explicit #{name}(const std::string& msg)"
      d << "      : std::runtime_error(\"#{name} '\"s + msg + \"'\"s) {}"
      d << "  };"
      d << "}"

      out, st = Open3.capture2e('clang-format', '-assume-filename', t.name, '-style=file', stdin_data: d.join("\n"))
      if st.exitstatus == 0
        IO.write(t.name, out)
      else
        IO.write(t.name, d.join("\n"))
      end
    }
  }
}

config['void'].each { |name|
  files << GeneratedFile.new { |t|
    t.desc = 'Generated file'
    t.name = "#{name}.hpp"
    t.code = proc {
      d = []
      d << "#pragma once"
      d << ""
      d << "#include <stdexcept>"
      d << ""
      d << "namespace Common::Exceptions {"
      d << "  class #{name} : public std::runtime_error {"
      d << "  public:"
      d << "    explicit #{name}()"
      d << "      : std::runtime_error(\"#{name}\") {}"
      d << "  };"
      d << "}"

      out, st = Open3.capture2e('clang-format', '-assume-filename', t.name, '-style=file', stdin_data: d.join("\n"))
      if st.exitstatus == 0
        IO.write(t.name, out)
      else
        IO.write(t.name, d.join("\n"))
      end
    }
  }
}

multitask(:default => files.collect(&:name))
