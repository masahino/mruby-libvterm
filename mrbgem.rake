MRuby::Gem::Specification.new('mruby-libvterm') do |spec|
  
  spec.license = 'MIT'
  spec.authors = 'masahino'

  def self.run_command(env, command)
    fail "#{command} failed" unless system(env, command)
  end

  def spec.download_libvterm
    require 'open-uri'
    libvterm_url = "http://www.leonerd.org.uk/code/libvterm/libvterm-0.1.4.tar.gz"
    libvterm_build_root = "#{build_dir}/libvterm/"
    libvterm_dir = "#{libvterm_build_root}/libvterm-0.1.4"
    libvterm_a = "#{libvterm_dir}/libvterm.a"

    unless File.exists?(libvterm_a)
      unless File.exists?(libvterm_dir)
        URI.open(libvterm_url, "r") do |http|
          libvterm_tar = http.read
          FileUtils.mkdir_p libvterm_build_root
          IO.popen("tar xfz - -C #{filename libvterm_build_root}", "wb") do |f|
            f.write libvterm_tar
          end
        end
      end
      Dir.chdir(libvterm_dir) do |t|
        e = {
          'CC'  => "#{build.cc.command} #{build.cc.flags.join(' ')}",
          'CXX' => "#{build.cxx.command} #{build.cxx.flags.join(' ')}",
          'LD'  => "#{build.linker.command} #{build.linker.flags.join(' ')}",
          'AR'  => build.archiver.command
        }
        run_command e, 'make libvterm.la'
        sh %Q{cp .libs/libvterm.a ./libvterm.a}
      end
    end

    self.linker.flags_before_libraries << libvterm_a
    [self.cc, self.cxx, self.objc, self.mruby.cc, self.mruby.cxx, self.mruby.objc].each do |cc|
      cc.include_paths << libvterm_dir + "/include"
    end
  end
end
