/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ar_string.h"

ar_Value *p_readchar(ar_State *S, ar_Value *args, ar_Value *env) {
    const char *string = ar_eval_string(S, ar_car(args), env);
    size_t index = ar_eval_number(S, ar_nth(args, 1), env);

    return ar_new_number(S, string[index]);
}


ar_Value *p_strcmp(ar_State *S, ar_Value *args, ar_Value *env) {
    const char *str1 = ar_eval_string(S, ar_car(args), env);
    const char *str2 = ar_eval_string(S, ar_nth(args, 1), env);
    return ar_new_number(S, strcmp(str1, str2));
}

ar_Value *p_substr(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *str = ar_eval(S, ar_car(args), env);
    if (ar_type(str) != AR_TSTRING) ar_error_str(S, "Expected string or symbol");
    int slen = str->u.str.len;
    int start = ar_opt_number(S, ar_nth(args, 1), env, 0);
    int len = ar_opt_number(S, ar_nth(args, 2), env, slen);
    if (start < 0) start = slen + start;
    if (start < 0) len += start, start = 0;
    if (start + len > slen) len = slen - start;
    if (len < 0) len = 0;
    return ar_new_stringl(S, &str->u.str.s[start], len);
}


ar_Value *p_strpos(ar_State *S, ar_Value *args, ar_Value *env) {
    const char *haystack = ar_eval_string(S, ar_car(args), env);
    const char *needle = ar_eval_string(S, ar_nth(args, 1), env);
    unsigned offset = ar_opt_number(S, ar_nth(args, 2), env, 0);
    const char *p;
    if (offset >= strlen(haystack) ) return NULL;
    p = strstr(haystack + offset, needle);
    return p ? ar_new_number(S, p - haystack) : NULL;
}


ar_Value *f_string(ar_State *S, ar_Value *args) {
    ar_Value *res = NULL, **last = &res;
    ar_Value *v = args;
    while (v) {
        last = ar_append_tail(S, last, ar_to_string_value(S, ar_car(v), 0));
        v = ar_cdr(v);
    }
    return join_list_of_strings(S, res);
}


ar_Value *f_strlen(ar_State *S, ar_Value *args) {
    return ar_new_number(S, ar_check(S, ar_car(args), AR_TSTRING)->u.str.len);
}


ar_Value *f_chr(ar_State *S, ar_Value *args) {
    char c = ar_check_number(S, ar_car(args));
    return ar_new_stringl(S, &c, 1);
}


ar_Value *f_ord(ar_State *S, ar_Value *args) {
    return ar_new_number(S, *ar_check_string(S, ar_car(args)));
}

