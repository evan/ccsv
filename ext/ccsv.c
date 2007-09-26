
#include "ccsv.h"

static VALUE rb_cC;

static VALUE foreach(VALUE self, VALUE filename) {

  FILE *file = fopen(StringValueCStr(filename), "r");
  if (file == NULL)
    rb_raise(rb_eRuntimeError, "File not found");
  
  char line[MAX_LENGTH];
  char *token;
  int idx;

  VALUE ary = rb_ary_new();
  
  while (fgets(line, sizeof(line), file) != NULL) {
    token = strtok(line, DELIMITERS);
    idx = 0;
    
    while (token != NULL) {
      rb_ary_store(ary, idx, rb_str_new(token, strlen(token)));
      idx ++;
      token = strtok(NULL, DELIMITERS);
    }
    
    OBJ_FREEZE(ary);
    rb_yield(ary);
    FL_UNSET((ary), FL_FREEZE);
    
  }

  return Qnil;
}

void
Init_ccsv()
{
  rb_cC = rb_define_class("Ccsv", rb_cObject);
  rb_define_singleton_method(rb_cC, "foreach", foreach, 1);
}
