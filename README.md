# mruby-libvterm   [![Build Status](https://travis-ci.org/masahino/mruby-libvterm.svg?branch=master)](https://travis-ci.org/masahino/mruby-libvterm)
VTerm class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'masahino/mruby-libvterm'
end
```
## example
```ruby
vt = VTerm.new(25, 80)
screen = vt.screen
screen.reset(true)
vt.write("Hello world")
vt.flush
```

## License
under the MIT License:
- see LICENSE file
