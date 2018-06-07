/**
 * Copyright (c) 2016 rxi
 * Copyright (c) 2017-2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef ARIA_H
#define ARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


#define AR_VERSION "0.2.0"

#define UNUSED(x) ((void) x)

#ifdef __APPLE__
	#define ARIA_APPLE 0
#elif _WIN32  /* both 32 and 64 bits are covered here */
	#define ARIA_WINDOWS 1
#elif __linux__
	#define ARIA_LINUX 2
#endif
#define ARIA_UNIX \
    if (ARIA_APPLE || ARIA_LINUX) \
        #define ARIA_UNIX 3 \

#ifdef ARIA_UNIX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#include "utils/vector.h"

typedef struct ar_Value ar_Value;
typedef struct ar_State ar_State;
typedef struct ar_Chunk ar_Chunk;
typedef struct ar_Frame ar_Frame;


typedef void *(*ar_Alloc)(void *udata, void *ptr, size_t size);
typedef ar_Value* (*ar_CFunc)(ar_State *S, ar_Value* args);
typedef ar_Value* (*ar_Prim)(ar_State *S, ar_Value* args, ar_Value *env);

struct ar_Value {
  unsigned char type, mark;
  union {
	struct { ar_Value *r  ;                       } ret;
    struct { ar_Value *name; int line;            } dbg;
    struct { ar_Value *pair, *left, *right;       } map;
    struct { ar_Value *car, *cdr, *dbg;           } pair;
    struct {
	#ifdef AR_FLOAT
		float n;
	#else
		double n;
	#endif
	} num;
    struct { ar_Value *params, *body, *env;       } func;
    struct { void *ptr; ar_CFunc gc, mark;        } udata;
    struct { ar_Value *parent, *map;              } env;
    struct { ar_CFunc fn;                         } cfunc;
    struct { ar_Prim fn;                          } prim;
    struct { char *s; size_t len; uint64_t hash;  } str;
  } u;
};


struct ar_Frame {
  struct ar_Frame *parent;  /* Parent stack frame */
  ar_Value *caller;         /* Calling function pair */
  jmp_buf *err_env;         /* Jumped to on error, if it exists */
  int stack_idx;            /* Index on stack where frame's values start */
};


struct ar_State {
  ar_Alloc alloc;           /* Allocator function */
  void *udata;              /* Pointer passed as allocator's udata */
  ar_Value *global;         /* Global environment */
  ar_Frame base_frame;      /* Base stack frame */
  ar_Frame *frame;          /* Current stack frame */
  int frame_idx;            /* Current stack frame index */
  ar_Value *t;              /* Symbol `t` */
  ar_Value *nil;            /* Symbol `nil` */
  ar_CFunc panic;           /* Called if an unprotected error occurs */
  ar_Value *err_args;       /* Error args passed to error handler */
  ar_Value *oom_error;      /* Value thrown on an out of memory error */
  ar_Value *oom_args;       /* Args passed to err handler on out of mem */
  ar_Value *parse_name;     /* Parser's current chunk name */
  int parse_line;           /* Parser's current line */
  ar_Value **gc_stack;      /* Stack of values (protected from GC) */
  int gc_stack_idx;         /* Current index for the top of the gc_stack */
  int gc_stack_cap;         /* Max capacity of protected values stack */
  ar_Chunk *gc_chunks;      /* List of all chunks */
  ar_Value *gc_pool;        /* Dead (usable) Values */
  int gc_count;             /* Counts down number of new values until GC */
};


enum {
  AR_TNIL = 0,
  AR_TDBGINFO = 1,
  AR_TMAPNODE = 2,
  AR_TPAIR = 3,
  AR_TVECTOR = 4,
  AR_TNUMBER = 5,
  AR_TSTRING = 6,
  AR_TSYMBOL = 7,
  AR_TFUNC = 8,
  AR_TMACRO = 9,
  AR_TPRIM = 10,
  AR_TCFUNC = 11,
  AR_TENV = 12,
  AR_TUDATA = 13,/* AR_TUDATA is not managed by Aria's GC, you must free the data yourself*/
  AR_TBREAK = 14
};

#define ar_get_global(S,x)    ar_eval(S, ar_new_symbol(S, x), (S)->global)
#define ar_bind_global(S,x,v) ar_bind(S, ar_new_symbol(S, x), v, (S)->global)
#define ar_call_global(S,f,a) ar_call(S, ar_get_global(S, f), a)

#define ar_check_string(S,v)  ar_to_string(S, ar_check(S, v, AR_TSTRING))
#define ar_check_udata(S,v)   ar_to_udata(S, ar_check(S, v, AR_TUDATA))
#define ar_check_number(S,v)  ar_to_number(S, ar_check(S, v, AR_TNUMBER))

#define ar_try(S, err_val, blk, err_blk)                  \
  do {                                                    \
    jmp_buf err_env__, *old_env__ = (S)->frame->err_env;  \
    S->frame->err_env = &err_env__;                       \
    if (setjmp(err_env__)) {                              \
      ar_Value *err_val = (S)->err_args;                  \
      (S)->frame->err_env = old_env__;                    \
      err_blk;                                            \
    } else {                                              \
      blk;                                                \
      (S)->frame->err_env = old_env__;                    \
    }                                                     \
  } while (0)

ar_State *ar_new_state(ar_Alloc alloc, void *udata);
void ar_close_state(ar_State *S);
ar_CFunc ar_at_panic(ar_State *S, ar_CFunc fn);
void ar_error(ar_State *S, ar_Value *err);
void ar_error_str(ar_State *S, const char *fmt, ...);

int is_equal(ar_Value *v1, ar_Value *v2);

ar_Value *ar_new_env(ar_State *S, ar_Value *parent);
ar_Value *ar_new_pair(ar_State *S, ar_Value *car, ar_Value *cdr);
ar_Value *ar_new_list(ar_State *S, size_t n, ...);
#ifdef AR_FLOAT
ar_Value *ar_new_number(ar_State *S, float n);
#else
ar_Value *ar_new_number(ar_State *S, double n);
#endif
ar_Value *ar_new_udata(ar_State *S, void *ptr);
ar_Value *ar_new_stringl(ar_State *S, const char *str, size_t len);
ar_Value *ar_new_string(ar_State *S, const char *str);
ar_Value *ar_new_symbol(ar_State *S, const char *name);
ar_Value *ar_new_cfunc(ar_State *S, ar_CFunc fn);
ar_Value *ar_new_prim(ar_State *S, ar_Prim fn);
ar_Value *ar_new_vector(ar_State *S, vec_t *vector);

ar_Value *ar_reverse(ar_Value *p);
int ar_type(ar_Value *v);
size_t ar_length(ar_Value *v);
const char *ar_type_str(int type);
ar_Value *ar_check(ar_State *S, ar_Value *v, int type);
ar_Value *ar_car(ar_Value *v);
ar_Value *ar_cdr(ar_Value *v);
ar_Value *ar_nth(ar_Value *v, int idx);
ar_Value **ar_append_tail(ar_State *S, ar_Value **last, ar_Value *v);
ar_Value *ar_to_string_value(ar_State *S, ar_Value *v, int quotestr);

const char *ar_to_stringl(ar_State *S, ar_Value *v, size_t *len);
const char *ar_to_string(ar_State *S, ar_Value *v);
vec_t *ar_to_vector(ar_State *S, ar_Value *v);
void *ar_to_udata(ar_State *S, ar_Value *v);

#ifdef AR_FLOAT
float ar_to_number(ar_State *S, ar_Value *v);
#else
double ar_to_number(ar_State *S, ar_Value *v);
#endif

ar_Value *ar_bind(ar_State *S, ar_Value *sym, ar_Value *v, ar_Value *env);
ar_Value *ar_set(ar_State *S, ar_Value *sym, ar_Value *v, ar_Value *env);

void ar_mark(ar_State *S, ar_Value *v);
void ar_gc(ar_State *S);

ar_Value *ar_parse(ar_State *S, const char *str, const char *name);
ar_Value *ar_eval(ar_State *S, ar_Value *v, ar_Value *env);
ar_Value *ar_call(ar_State *S, ar_Value *fn, ar_Value *args);
ar_Value *ar_do_list(ar_State *S, ar_Value *body, ar_Value *env);
ar_Value *ar_do_string(ar_State *S, const char *str);
ar_Value *ar_do_file(ar_State *S, const char *filename);
ar_Value *join_list_of_strings(ar_State *S, ar_Value *list);

const char *ar_opt_string(ar_State *S, ar_Value *v, ar_Value *env,
const char *def);
void *ar_opt_udata(ar_State *S, ar_Value *v, ar_Value *env, void *def);


#ifdef AR_FLOAT
float ar_opt_number(ar_State *S, ar_Value *v, ar_Value *env, float def);
#else
double ar_opt_number(ar_State *S, ar_Value *v, ar_Value *env, double def);
#endif

#ifdef AR_FLOAT
float ar_eval_number(ar_State *S, ar_Value *v, ar_Value *env);
#else
double ar_eval_number(ar_State *S, ar_Value *v, ar_Value *env);
#endif
bool ar_eval_boolean(ar_State *S, ar_Value *v, ar_Value *env);
const char *ar_eval_string(ar_State *S, ar_Value *v, ar_Value *env);
vec_t* ar_eval_vector(ar_State *S, ar_Value *v, ar_Value *env);
void *ar_eval_udata(ar_State *S, ar_Value *v, ar_Value *env);

#include "ar_io.h"
#include "ar_list.h"
#include "ar_math.h"
#include "ar_string.h"
#include "ar_vector.h"


#ifdef __cplusplus
}
#endif

#endif

