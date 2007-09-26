
#include "ccsv.h"

static VALUE rb_cC;

static VALUE foreach(VALUE self, VALUE filename) {

  FILE *file = fopen(StringValueCStr(filename), "r");
  if (file == NULL)
    rb_raise(rb_eRuntimeError, "File not found");
  
  char line[MAX_LENGTH];
  char *token;
  VALUE ary = rb_ary_new();
  int offset;
  
  while (fgets(line, sizeof(line), file) != NULL) {
    token = strtok(line, DELIMITERS);
    offset = 0;
    
    while (token != NULL) {
      rb_ary_store(ary, offset, rb_str_new(token, strlen(token)));
      offset ++;
      token = strtok(NULL, DELIMITERS);
    }
    rb_yield(ary);
  }

  return Qnil;
}

void
Init_ccsv()
{
  rb_cC = rb_define_class("Ccsv", rb_cObject);
  rb_define_singleton_method(rb_cC, "foreach", foreach, 1);
}
