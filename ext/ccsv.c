#include <limits.h>
#include "ccsv.h"

static VALUE rb_cC;

/* Ccsv.foreach(filename,delimiter,[index],[range,...]) do |line| ... */

struct pair_st {
  long int low,high;
};

#define MAX_INTERVALS 1024

static VALUE foreach(int argc, VALUE* argv, VALUE self) {
  char DELIM=DEF_DELIM;
  char *line = NULL;
  size_t len = 0;
  char *token,*start,*nobackslash,*t2, *str;
  int idx,count,pairs_count,searchfield,flag,i,array_length,range_i,len2;
  long check;
  FILE *file;
  ID min_method, max_method;
  VALUE min_val, max_val;
  VALUE tmp_value, rest_args, filename;
  ID    array_length_method; /*----------------------------------------*/
  struct pair_st pairs[MAX_INTERVALS];

  VALUE ary;

  rb_scan_args(argc,argv,"1*", &filename, &rest_args);

/*  if (argc == 0) {  // there should only be 1 or 2 arguments
    rb_raise(rb_eArgError, "wrong number of arguments");
  }

  file = fopen(StringValueCStr(argv[0]), "r");
  if (file == NULL)
    rb_raise(rb_eRuntimeError, "File not found");
*/
  file = fopen(StringValueCStr(filename), "r");
  if(file==NULL){
    rb_raise(rb_eRuntimeError, "File not found");
  }

  if (argc >1 ) { /* delimiter */
    tmp_value=rb_ary_entry(rest_args,0);
    str=StringValueCStr(tmp_value);
    DELIM=str[0];
  }

  if (argc >2 ) { /* search index */
    tmp_value=rb_ary_entry(rest_args,1);
    searchfield=NUM2INT(tmp_value);
  }
  else{
    searchfield=-1;
  }

  min_val=rb_funcall(rest_args,rb_intern("length"), 0);
  array_length=NUM2INT(min_val);
  /*rb_warn("Length=%d",array_length);*/

  min_method = rb_intern("min");
  max_method = rb_intern("max");
  /*------------test_id    = rb_intern("class");*/
  range_i=0;
  for(idx=2;idx<array_length;++idx){
    min_val=rb_funcall(rb_ary_entry(rest_args,idx),rb_intern("length"), 0);
    len2=NUM2INT(min_val);
    for(i=0;i<len2;++i){
      VALUE e=rb_ary_entry(rb_ary_entry(rest_args,idx),i);
      if(range_i>MAX_INTERVALS)
        rb_raise(rb_eRuntimeError, "Too much ranges passed");
      if(TYPE(e) == T_NIL){
        pairs[range_i].low=LONG_MIN;
        pairs[range_i].high=LONG_MAX;

        continue; /* just skip nil */
      }
      if (! (rb_respond_to(e, min_method) & rb_respond_to(e, max_method)))
        rb_raise(rb_eRuntimeError, "Not range passed to Ccsv.foreach");

      min_val=rb_funcall(e, min_method, 0);
      max_val=rb_funcall(e, max_method, 0);
  /*    rb_warn("!\n");*/
      pairs[range_i].low=NUM2LONG(min_val);
      /*rb_warn("2\n");*/
      pairs[range_i].high=NUM2LONG(max_val);
      /*rb_warn("RANGE: %ld .. %ld (%d)\n",(long)pairs[range_i].low,(long)pairs[range_i].high,(int)(range_i));*/
      range_i++;
    }
  }
  pairs_count=range_i;

  while (getline(&line, &len, file) != -1) {
    /* chomp! */
    if(token=index(line,EOL)){
      *token='\0';
    }
    /*rb_warning("4\n");*/
    ary = rb_ary_new();
    start=line;
    nobackslash=line;
    while(token=index(nobackslash, DELIM)){
/*      rb_warning("5\n");*/
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
    flag=1;

    while (token != NULL) {
      *token='\0';
      if(searchfield==idx){
        flag=0;
        /* do check! */
        sscanf(start,"%ld",&check);
        for(i=0;i<pairs_count;++i){
          /*rb_warn("check %ld: [%ld .. %ld]",check,pairs[i].low,pairs[i].high);*/
          if(pairs[i].low<check && pairs[i].high>check){
            /*rb_warn("check passed");*/
            flag=1; /* yahooo! */
            break;
          }
        }
      }

      /* not in ranges! */
      if(flag==0)
        break;

      rb_ary_store(ary, idx, rb_str_new(start, token-start));
      idx++;
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
    if(flag==0)
      continue;

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
  rb_define_const(rb_cC, "MAX", LONG2NUM(LONG_MAX));
  rb_define_const(rb_cC, "MIN", LONG2NUM(LONG_MIN));
}
