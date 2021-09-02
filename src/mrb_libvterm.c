/*
** mrb_libvterm.c - Libvterm class
**
** Copyright (c) masahino 2021
**
** See Copyright Notice in LICENSE
*/

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

static const struct mrb_data_type mrb_vterm_data_type = {
  "mrb_vterm_data", mrb_vterm_free,
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
static mrb_value mrb_vterm_get_size(mrb_state *mrb, mrb_value self)
{
  VTerm *vt = (VTerm *)DATA_PTR(self);
  int rows, cols;
  mrb_value ret_ary = mrb_ary_new(mrb);

  vterm_get_size(vt, &rows, &cols);
  mrb_ary_push(mrb, ret_ary, mrb_fixnum_value(rows));
  mrb_ary_push(mrb, ret_ary, mrb_fixnum_value(cols));
  return ret_ary;
}


void mrb_mruby_libvterm_gem_init(mrb_state *mrb)
{
  struct RClass *vterm;
  vterm = mrb_define_class(mrb, "VTerm", mrb->object_class);
  mrb_define_method(mrb, vterm, "initialize", mrb_vterm_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vterm, "set_utf8", mrb_vterm_set_utf8, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vterm, "get_utf8", mrb_vterm_get_utf8, MRB_ARGS_NONE());
  mrb_define_method(mrb, vterm, "set_size", mrb_vterm_set_size, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vterm, "get_size", mrb_vterm_get_size, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_libvterm_gem_final(mrb_state *mrb)
{
}

