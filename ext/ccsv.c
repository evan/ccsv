#include "ruby.h"
#include "ccsv.h"

static VALUE rb_cC;
/* static VALUE foreach(VALUE self, VALUE filename) { */
static VALUE foreach(int argc, VALUE* argv, VALUE self){
    VALUE filename;
    VALUE delimiter;
  rb_scan_args(argc, argv, "11", &filename, &delimiter);
  if(NIL_P(delimiter)){
      delimiter=rb_str_new(",",1);
  }
 const char *strDelimiter = StringValueCStr(delimiter);
	strncat(strDelimiter,"\n",1);
  FILE *file = fopen(StringValueCStr(filename), "r");
  if (file == NULL)
    rb_raise(rb_eRuntimeError, "File not found");
  char *line = NULL;
  size_t len = 0;
  char *token;
  int idx;

  VALUE ary;

  while (getline(&line, &len, file) != -1) {
    ary = rb_ary_new();
    token = strtok(line, strDelimiter);
    idx = 0;

    while (token != NULL) {
      rb_ary_store(ary, idx, rb_str_new(token, strlen(token)));
      idx ++;
      token = strtok(NULL, strDelimiter);
    }

    /* OBJ_FREEZE(ary); */
    rb_yield(ary);
    /* FL_UNSET((ary), FL_FREEZE); */

    /* for(idx = 0; idx < RARRAY_LEN(ary); idx ++) {
      rb_ary_store(ary, idx, Qnil);
    } */

  }

  fclose(file);

  return Qnil;
}

void Init_ccsv()
{
  rb_cC = rb_define_class("Ccsv", rb_cObject);
  
/*  rb_define_singleton_method(rb_cC, "foreach", foreach, 1);*/
  rb_define_singleton_method(rb_cC, "foreach", foreach, -1);
}
