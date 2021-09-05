/*
** mrb_libvterm.c - Libvterm class
**
** Copyright (c) masahino 2021
**
** See Copyright Notice in LICENSE
*/

#include <string.h>
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mrb_libvterm.h"
#include "vterm.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static void mrb_vterm_free(mrb_state *mrb, void *ptr) {
  if (ptr != NULL) {
          vterm_free((VTerm *)ptr);
  }
}

static void mrb_vterm_screen_free(mrb_state *mrb, void *ptr) {
}

static void mrb_vterm_color_free(mrb_state *mrb, void *ptr) {
}

static const struct mrb_data_type mrb_vterm_data_type = {
  "mrb_vterm_data", mrb_vterm_free,
};

static const struct mrb_data_type mrb_vterm_screen_data_type = {
  "mrb_vterm_screen_data", mrb_vterm_screen_free,
};

static const struct mrb_data_type mrb_vterm_color_data_type = {
  "mrb_vterm_color_data", mrb_vterm_color_free,
};

static const struct mrb_data_type mrb_vterm_screen_cell_data_type = {
  "mrb_vterm_screen_cell_data", mrb_free,
};

static mrb_value mrb_vterm_init(mrb_state *mrb, mrb_value self)
{
  VTerm *vt;
  mrb_int rows, cols;

  DATA_TYPE(self) = &mrb_vterm_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "ii", &rows, &cols);
  vt = vterm_new(rows, cols);
  DATA_PTR(self) = vt;

  return self;
}

/* void vterm_set_utf8(VTerm *vt, int is_utf8); */
static mrb_value mrb_vterm_set_utf8(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  mrb_bool is_utf8;

  mrb_get_args(mrb, "b", &is_utf8);
  if (is_utf8) {
    vterm_set_utf8(vt, 1);
  } else {
    vterm_set_utf8(vt, 0);
  }
  return mrb_nil_value();
}

/* int  vterm_get_utf8(const VTerm *vt);*/
static mrb_value mrb_vterm_get_utf8(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  int is_utf8;

  is_utf8 = vterm_get_utf8(vt);
  if (is_utf8 == 1) {
    return mrb_true_value();
  } else {
    return mrb_false_value();
  }
}

/* void vterm_set_size(VTerm *vt, int rows, int cols); */
static mrb_value mrb_vterm_set_size(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  mrb_int rows, cols;

  mrb_get_args(mrb, "ii", &rows, &cols);
  vterm_set_size(vt, rows, cols);

  return mrb_nil_value();
}

/* void vterm_get_size(const VTerm *vt, int *rowsp, int *colsp); */
static mrb_value mrb_vterm_size(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  int rows, cols;
  mrb_value ret_ary = mrb_ary_new(mrb);

  vterm_get_size(vt, &rows, &cols);
  mrb_ary_push(mrb, ret_ary, mrb_fixnum_value(rows));
  mrb_ary_push(mrb, ret_ary, mrb_fixnum_value(cols));
  return ret_ary;
}

/* size_t vterm_output_read(VTerm *vt, char *buffer, size_t len); */
static mrb_value mrb_vterm_read(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  size_t buffer_size;
  mrb_int len;
  mrb_int argc;
  char *output_buffer;

  buffer_size = vterm_output_get_buffer_size(vt);
  argc = mrb_get_args(mrb, "|i", &len);
  if (argc == 1) {
    buffer_size = len;
  }
  output_buffer = (char *)mrb_malloc(mrb, sizeof(char)*(buffer_size + 1));
  vterm_output_read(vt, output_buffer, buffer_size);
  return mrb_str_new_cstr(mrb, output_buffer);
}

/* size_t vterm_input_write(VTerm *vt, const char *bytes, size_t len);*/
static mrb_value mrb_vterm_write(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  char *bytes;
  mrb_int len;
  size_t ret;
  mrb_get_args(mrb, "s", &bytes, &len);
  ret = vterm_input_write(vt, bytes, len);

  return mrb_fixnum_value(ret);
}

/* VTermScreen *vterm_obtain_screen(VTerm *vt); */
static mrb_value mrb_vterm_obtain_screen(mrb_state *mrb, mrb_value self)
{
  VTermScreen *screen;
  VTerm *vt = (VTerm *)DATA_PTR(self);
  struct RClass *vterm_screen = mrb_class_get_under(mrb, mrb_obj_class(mrb, self), "Screen");
  mrb_value screen_obj = mrb_obj_value(Data_Wrap_Struct(mrb, vterm_screen, &mrb_vterm_screen_data_type, NULL));
  screen = vterm_obtain_screen(vt);
  DATA_TYPE(screen_obj) = &mrb_vterm_screen_data_type;
  DATA_PTR(screen_obj) = screen;
  return screen_obj;
}

/* void   vterm_screen_reset(VTermScreen *screen, int hard); */
static mrb_value mrb_vterm_screen_reset(mrb_state *mrb, mrb_value self)
{
  VTermScreen *screen = (VTermScreen *)DATA_PTR(self);
  mrb_bool is_hard;
  int v = 0;
  mrb_get_args(mrb, "b", &is_hard);
  if (is_hard) {
    v = 1;
  }
  vterm_screen_reset(screen, v);
  return mrb_nil_value();
}
/* void vterm_screen_flush_damage(VTermScreen *screen); */
static mrb_value mrb_vterm_screen_flush(mrb_state *mrb, mrb_value self)
{
  VTermScreen *screen = (VTermScreen *)DATA_PTR(self);
  vterm_screen_flush_damage(screen);
  return mrb_nil_value();
}

/* int vterm_screen_get_cell(const VTermScreen *screen, VTermPos pos, VTermScreenCell *cell); */
static mrb_value mrb_vterm_screen_cell_at(mrb_state *mrb, mrb_value self)
{
  VTermScreen *screen = (VTermScreen *)DATA_PTR(self);
  mrb_int row, col;
  VTermPos pos;
  VTermScreenCell *cell = (VTermScreenCell *)mrb_malloc(mrb, sizeof(VTermScreenCell));
  int ret;
  struct RClass *vterm_screen_cell = mrb_class_get_under(mrb, mrb_obj_class(mrb, self), "Cell");
  mrb_value cell_obj = mrb_obj_value(Data_Wrap_Struct(mrb, vterm_screen_cell,
      &mrb_vterm_screen_cell_data_type, NULL));

  mrb_get_args(mrb, "ii", &row, &col);
  pos.row = row;
  pos.col = col;
  ret = vterm_screen_get_cell(screen, pos, cell);
  DATA_TYPE(cell_obj) = &mrb_vterm_screen_cell_data_type;
  DATA_PTR(cell_obj) = cell;
  return cell_obj;
}

/*
typedef struct {
#define VTERM_MAX_CHARS_PER_CELL 6
  uint32_t chars[VTERM_MAX_CHARS_PER_CELL];
  char     width;
  VTermScreenCellAttrs attrs;
  VTermColor fg, bg;
} VTermScreenCell;
*/
static mrb_value mrb_vterm_screen_cell_chars(mrb_state *mrb, mrb_value self)
{
  VTermScreenCell *cell = (VTermScreenCell *)DATA_PTR(self);
  char *chars = mrb_malloc(mrb, (cell->width + 1)*sizeof(const char));
  int i;
  for (i = 0; i < cell->width; i++) {
    chars[i] = cell->chars[i];
  }
  return mrb_str_new_cstr(mrb, chars);
}

static mrb_value mrb_vterm_screen_cell_fg(mrb_state *mrb, mrb_value self)
{
  VTermScreenCell *cell = (VTermScreenCell *)DATA_PTR(self);
  struct RClass *vterm_color = mrb_class_get_under(mrb, mrb_class_get(mrb, "VTerm"), "Color");
  mrb_value color_obj = mrb_obj_value(Data_Wrap_Struct(mrb, vterm_color,
      &mrb_vterm_color_data_type, NULL));

  DATA_TYPE(color_obj) = &mrb_vterm_color_data_type;
  DATA_PTR(color_obj) = &(cell->fg);
  return color_obj;
}

static mrb_value mrb_vterm_screen_cell_bg(mrb_state *mrb, mrb_value self)
{
  VTermScreenCell *cell = (VTermScreenCell *)DATA_PTR(self);
  struct RClass *vterm_color = mrb_class_get_under(mrb, mrb_class_get(mrb, "VTerm"), "Color");
  mrb_value color_obj = mrb_obj_value(Data_Wrap_Struct(mrb, vterm_color,
      &mrb_vterm_color_data_type, NULL));

  DATA_TYPE(color_obj) = &mrb_vterm_color_data_type;
  DATA_PTR(color_obj) = &(cell->bg);
  return color_obj;
}

void mrb_mruby_libvterm_gem_init(mrb_state *mrb)
{
  struct RClass *vterm, *screen, *color, *cell;
  vterm = mrb_define_class(mrb, "VTerm", mrb->object_class);
  screen = mrb_define_class_under(mrb, vterm, "Screen", mrb->object_class);
  color = mrb_define_class_under(mrb, vterm, "Color", mrb->object_class);
  cell = mrb_define_class_under(mrb, screen, "Cell", mrb->object_class);

  mrb_define_method(mrb, vterm, "initialize", mrb_vterm_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vterm, "set_utf8", mrb_vterm_set_utf8, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vterm, "get_utf8", mrb_vterm_get_utf8, MRB_ARGS_NONE());
  mrb_define_method(mrb, vterm, "set_size", mrb_vterm_set_size, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vterm, "size", mrb_vterm_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, vterm, "write", mrb_vterm_write, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vterm, "read", mrb_vterm_read, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, vterm, "screen", mrb_vterm_obtain_screen, MRB_ARGS_NONE());

  /* VTerm::Screen */
  mrb_define_method(mrb, screen, "reset", mrb_vterm_screen_reset, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, screen, "flush", mrb_vterm_screen_flush, MRB_ARGS_NONE());
  mrb_define_method(mrb, screen, "cell_at", mrb_vterm_screen_cell_at, MRB_ARGS_REQ(2));

  /* VTerm::Screen::Cell */
  mrb_define_method(mrb, cell, "chars", mrb_vterm_screen_cell_chars, MRB_ARGS_NONE());
  mrb_define_method(mrb, cell, "fg", mrb_vterm_screen_cell_fg, MRB_ARGS_NONE());
  mrb_define_method(mrb, cell, "bg", mrb_vterm_screen_cell_bg, MRB_ARGS_NONE());

  DONE;
}

void mrb_mruby_libvterm_gem_final(mrb_state *mrb)
{
}

