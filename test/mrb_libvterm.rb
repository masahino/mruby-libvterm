##
## Libvterm Test
##

assert("VTerm.new") do
  vt = VTerm.new(25, 80)
  assert_kind_of(VTerm, vt)
end

assert("VTerm.set_utf8") do
  vt = VTerm.new(25, 80)
  assert_equal(false, vt.get_utf8)
  vt.set_utf8(true)
  assert_equal(true, vt.get_utf8)
end

assert("VTerm.size") do
  vt = VTerm.new(30, 40)
  assert_equal([30, 40], vt.size)
end

assert("VTerm.set_size") do
  vt = VTerm.new(1, 2)
  vt.set_size(25, 80)
  assert_equal([25, 80], vt.size)
end

assert("VTerm.write") do
  vt = VTerm.new(25, 80)
  ret = vt.write("Hello World!")
  assert_equal(12, ret)
end

assert("VTerm.read") do
  vt = VTerm.new(1, 20)
  screen = vt.screen
  screen.reset(true)
  vt.write("\e[6n")
  assert_equal("\e[1;1R", vt.read())
end

assert("VTerm::Screen") do
  vt = VTerm.new(10, 10)
  screen = vt.screen
  assert_kind_of(VTerm::Screen, screen)
end

assert("VTerm::Screen.reset") do
  vt = VTerm.new(25, 80)
  screen = vt.screen
  screen.reset(true)

end

assert("VTerm::Screen.flush") do
  vt = VTerm.new(25, 80)
  screen = vt.screen
  screen.reset(true)
  vt.write("hoge")
  screen.flush
end

assert("VTerm::Screen.cell_at") do
  vt = VTerm.new(25, 80)
  screen = vt.screen
  screen.reset(true)
  vt.write("hoge")
  cell = screen.cell_at(0, 0)
  assert_kind_of(VTerm::Screen::Cell, cell)
end

assert("VTerm::Screen::Cell.chars") do
  vt = VTerm.new(25, 80)
  screen = vt.screen
  screen.reset(true)
  cell = screen.cell_at(0, 0)
  assert_equal("", cell.chars)
  vt.write("abc")
  cell = screen.cell_at(0, 0)
  assert_equal("a", cell.chars)
end

assert("VTerm::Screen::Cell.fg") do
  vt = VTerm.new(25, 80)
  screen = vt.screen
  screen.reset(true)
  vt.write("\033[31mHello\033[0m")
  cell = screen.cell_at(0, 0)
  assert_kind_of(VTerm::Color, cell.fg)
  assert_kind_of(VTerm::Color, cell.bg)
end