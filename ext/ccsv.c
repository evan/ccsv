#include "ccsv.h"

static VALUE rb_cC;

static VALUE foreach(int argc, VALUE* argv, VALUE self) {
  char DELIM=DEF_DELIM;
  char *line = NULL;
  size_t len = 0;
  char *token,*start,*nobackslash,*t2;
  int idx,count;
  FILE *file;

  VALUE ary;


  if (argc > 2 || argc == 0) {  // there should only be 1 or 2 arguments
    rb_raise(rb_eArgError, "wrong number of arguments");
  }

  file = fopen(StringValueCStr(argv[0]), "r");
  if (file == NULL)
    rb_raise(rb_eRuntimeError, "File not found");

  if (argc == 2) {  /* 2 arguments */
    char *str=StringValueCStr(argv[1]);
    DELIM=str[0];
  }

  while (getline(&line, &len, file) != -1) {
    /* chomp! */
    if(token=index(line,EOL)){
      *token='\0';
    }
    ary = rb_ary_new();
    start=line;
    nobackslash=line;
    while(token=index(nobackslash, DELIM)){
      count=0;
      t2=token-1;
      while((t2>=line) && (*t2=='\\'))
        {++count;--t2;}
      if(count%2 ==1){ /* backslashed! skip */
        nobackslash=token;
        continue;
      }
      break;
    }
    idx = 0;

    while (token != NULL) {
      *token='\0';
      rb_ary_store(ary, idx, rb_str_new(start, token-start));
      idx ++;
      nobackslash=start=token+1;
      while(token=index(nobackslash, DELIM)){
        count=0;
        t2=token-1;
        while((t2>=line) && (*t2=='\\'))
          {++count;--t2;}
        if(count%2 ==1){ /* backslashed! skip */
          nobackslash=token;
          continue;
        }
        break;
      }
    }

    /* last item */
    rb_ary_store(ary, idx, rb_str_new(start, strlen(start)));
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

void
Init_ccsv()
{
  rb_cC = rb_define_class("Ccsv", rb_cObject);
  rb_define_singleton_method(rb_cC, "foreach", foreach, -1);
}
