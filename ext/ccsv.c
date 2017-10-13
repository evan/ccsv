#include <limits.h>
#include "ccsv.h"

static VALUE rb_cC;

/* Ccsv.foreach(filename,delimiter,[index],[range,...]) do |line| ... */

struct pair_st {
  long int low,high;
};

#define MAX_INTERVALS 1024

static VALUE foreach(int argc, VALUE* argv, VALUE self) {
  char *DELIM=DEF_DELIM;
  char *line = NULL;
  char *line2 =NULL, *new_line=NULL;
  size_t len = 0, len2=0, i, pairs_count;
  char *token,*start,*nobackslash,*t2;
  size_t idx,count,searchfield,flag,array_length,range_i;
  long check;
  int  was_read, backslashed;
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
    DELIM=StringValueCStr(tmp_value);;
  }

  if (argc >2 ) { /* search index */
    tmp_value=rb_ary_entry(rest_args,1);
    searchfield=NUM2INT(tmp_value);
  }
  else{
    searchfield=-1;
  }

  array_length_method=rb_funcall(rest_args,rb_intern("length"), 0);
  array_length=NUM2INT(array_length_method);

  min_method = rb_intern("first");
  max_method = rb_intern("last");
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
      pairs[range_i].low=NUM2LONG(min_val);
      pairs[range_i].high=NUM2LONG(max_val);
      range_i++;
    }
  }
  pairs_count=range_i;

  /* main loop lines reading */

  while ((was_read=getline(&line, &len, file)) != -1) {

    if(was_read<1)
      continue;

    backslashed = 0;
    len=was_read-1;
    /* try to join escaped lines */
    for(;;) {
      /* check for backslashed newline */
      if(line[len]!=EOL)
        break;
      if(line[len]==CR){
        len-=1;
      }
      //rb_warn("NONLAST (%c,%c)",line[len],line[len]);
      count=0;
      t2=line+len-1;
      while((t2>=line) && (*t2=='\\'))
        {++count;--t2;}
      if(count%2 ==1){ /* backslashed! skip */
        /* get another line... */
        line2=NULL;
        len2=0;
        if((was_read=getline(&line2, &len2, file)) != -1) {
          len2=was_read;
          if(new_line)
            free(new_line); //!!!!
          new_line=malloc(len+len2+1);
          strcpy(new_line,line);
          new_line[len-1]='\n';
          strcpy(new_line+len,line2);
          line=new_line;
          //nobackslash=len+1;
          start=new_line;
          len+=len2-1;
          nobackslash=start;
          backslashed = 1;
          free(line);
          free(line2);
          continue;
        }
      }
      break;
    }
    /* chomp! */
    if(line[len]==EOL){
      if(line[len-1]==CR)
        len-=1;
      line[len]='\0';
    }
    /* skip empty line */
    if(len<2)
      continue;

    ary = rb_ary_new();
    start=line;
    nobackslash=line;

    idx = 0;
    flag=1;

    while (nobackslash != NULL) {

      /* get full field */
      while(token=strstr(nobackslash, DELIM)){
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

      if(token)
        *token='\0';
      else
        token=start+strlen(start);

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
      nobackslash=start=token+strlen(DELIM);
      while(token=strstr(nobackslash, DELIM)){
        count=0;
        t2=token-1;
        while((t2>=line) && (*t2=='\\'))
          {++count;--t2;}
        if(count%2 ==1){ /* backslashed! skip */
          nobackslash=token+strlen(DELIM);
          continue;
        }
        break;
      }
      nobackslash=token;
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
  if(!backslashed)
    free(line);

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
