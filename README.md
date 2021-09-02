# mruby-libvterm   [![Build Status](https://travis-ci.org/masahino/mruby-libvterm.svg?branch=master)](https://travis-ci.org/masahino/mruby-libvterm)
Libvterm class
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
p Libvterm.hi
#=> "hi!!"
t = Libvterm.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
