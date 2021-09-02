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

assert("VTerm.get_size") do
  vt = VTerm.new(30, 40)
  row, col = vt.get_size
  assert_equal(30, row)
  assert_equal(40, col)
end

assert("VTerm.set_size") do
  vt = VTerm.new(1, 2)
  vt.set_size(25, 80)
  row, col = vt.get_size
  assert_equal(25, row)
  assert_equal(80, col)
end
