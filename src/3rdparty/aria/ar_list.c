/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "ar_list.h"

/*
 * Look for 'to_find' inside 'from'
 * (= lst (list 1 2 3 "hey" "hello"))
 * (print (find lst "hey" )) -> t
 *
 * @NOTE: This doesn't work on vectors!
 */
ar_Value *p_find(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *from = ar_eval(S, ar_car(args), env);
    ar_Value *to_find = ar_eval(S, ar_nth(args, 1), env);
    ar_Value *v = from;

    for (; v != NULL; v = ar_cdr(v)){
        if (is_equal(ar_car(v), to_find) != 0)
            return S->t;
    }
    return NULL;
}


ar_Value *p_reverse(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value* v = ar_eval(S, ar_car(args), env);
    return ar_reverse(v);
}


ar_Value *p_nth(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *from = ar_eval(S, ar_car(args), env);
    ar_Value *n = ar_eval(S, ar_nth(args, 1), env);
    return ar_eval(S, ar_nth(from, n->u.num.n), env);
}


ar_Value *p_nthcdr(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *from = ar_eval(S, ar_car(args), env);
    ar_Value *n = ar_eval(S, ar_nth(args, 1), env);
    int i = 0;

    while (i < n->u.num.n) {
        from = ar_cdr(from);
        i++;
    }
    return from->u.pair.cdr;
}


ar_Value *p_remove(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value* v = ar_eval(S, ar_car(args), env);
    ar_Value* what = ar_eval(S, ar_nth(args, 1), env);
    ar_Value* ret = NULL;
    int type = 0;
    int type1 = ar_type(what);

    for (; v != NULL; v = ar_cdr(v))
    {
        type = ar_type(ar_car(v));
        if ((type == AR_TSYMBOL || type == AR_TSTRING) && type == type1)
        {
            if (!(strcmp(ar_car(v)->u.str.s, what->u.str.s) == 0))
                ret = ar_new_pair(S, ar_car(v), ret);
        }
        else if (type == AR_TNUMBER && type == type1)
        {
            if (!(ar_car(v)->u.num.n == what->u.num.n) && type == type1)
                ret = ar_new_pair(S, ar_car(v), ret);
        }
        else if (type == AR_TUDATA && type == type1)
        {
            if (!(ar_car(v)->u.udata.ptr == what->u.udata.ptr))
                ret = ar_new_pair(S, ar_car(v), ret);
        }
        else
            ret = ar_new_pair(S, ar_car(v), ret);
    }
    ret = ar_reverse(ret);

    return ret;
}


ar_Value *p_remove_n(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value* v = ar_eval(S, ar_car(args), env);
    ar_Value* what = ar_eval(S, ar_nth(args, 1), env);
    int n = ar_eval_number(S, ar_nth(args, 2), env);
    int index = 1; /* how many items have been removed so far. */
    ar_Value* ret = NULL;
    int type1 = ar_type(what);


    /*
     * The logic:
     * Only add the items which are supposed to be removed
     * when we breached the limit
     */
    for (; v != NULL; v = ar_cdr(v))
    {
        int type = ar_type(ar_car(v));
        if (type == AR_TSYMBOL || type == AR_TSTRING)
        {
            if (type1 == type && strcmp(ar_car(v)->u.str.s, what->u.str.s) == 0)
            {
                if (index > n)
                    ret = ar_new_pair(S, ar_car(v), ret);
                index++;
            }
            else
                ret = ar_new_pair(S, ar_car(v), ret);
        }
        else if (type == type1 && type == AR_TNUMBER)
        {
            if (ar_car(v)->u.num.n == what->u.num.n)
            {
                if (index > n)
                    ret = ar_new_pair(S, ar_car(v), ret);
                index++;
            }
            else
                ret = ar_new_pair(S, ar_car(v), ret);
        }
        else if (type == type1 && type == AR_TUDATA)
        {
            if (ar_car(v)->u.udata.ptr != what->u.udata.ptr)
            {
                if (index > n)
                    ret = ar_new_pair(S, ar_car(v), ret);
                index++;
            }
            else
                ret = ar_new_pair(S, ar_car(v), ret);
        }
        else
            ret = ar_new_pair(S, ar_car(v), ret);
    }
    ret = ar_reverse(ret);

    return ret;
}


ar_Value *p_length(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value* v = ar_eval(S, ar_car(args), env);
    int size = 0;
    while (v != NULL)
    {
        size++;
        v = ar_cdr(v);
    }
    return ar_new_number(S, size);
}


/*
 * Look for 'to_find' inside 'from' and return a list of all found instances
 * (= lst (list 1 2 3 "hey" "hey" "hello"))
 * (print (member lst "hey")) -> ("hey" "hey")
 *
 * @NOTE: This doesn't work on vectors!
 */
ar_Value *p_member(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *from = ar_eval(S, ar_car(args), env);
    ar_Value *to_find = ar_eval(S, ar_nth(args, 1), env);
    ar_Value *v = from;
    ar_Value *ret = NULL;

    for (; v != NULL; v = ar_cdr(v)){
        if (is_equal(ar_car(v), to_find) != 0) {
            ret = ar_new_pair(S, v, ret);
        }
    }
    return ret;
}


ar_Value *p_append(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *lst = ar_eval(S, ar_car(args), env);
    ar_Value *to_append = ar_eval(S, ar_nth(args, 1), env);
    ar_Value *ret = lst;

    ret = ar_reverse(ret);
    if (ar_type(lst) == AR_TPAIR) {
        for (; to_append != NULL; to_append = ar_cdr(to_append)) {
            ret = ar_new_pair(S, ar_car(to_append), ret);
        }
    }
    else {
        ret = ar_new_pair(S, to_append, ret);
    }
    ret = ar_reverse(ret);
    return ret;
}


ar_Value *f_list(ar_State *S, ar_Value *args) {
    UNUSED(S);
    return args;
}


ar_Value *f_cons(ar_State *S, ar_Value *args) {
    return ar_new_pair(S, ar_car(args), ar_nth(args, 1));
}


ar_Value *f_car(ar_State *S, ar_Value *args) {
    ar_Value *v = ar_car(args);
    if (!v) return NULL;
    return ar_check(S, v, AR_TPAIR)->u.pair.car;
}


ar_Value *f_cdr(ar_State *S, ar_Value *args) {
    ar_Value *v = ar_car(args);
    if (!v) return NULL;
    return ar_check(S, v, AR_TPAIR)->u.pair.cdr;
}


ar_Value *f_setcar(ar_State *S, ar_Value *args) {
    return ar_check(S, ar_car(args), AR_TPAIR)->u.pair.car = ar_nth(args, 1);
}


ar_Value *f_setcdr(ar_State *S, ar_Value *args) {
    return ar_check(S, ar_car(args), AR_TPAIR)->u.pair.cdr = ar_nth(args, 1);
}


ar_Value *f_consp(ar_State *S, ar_Value *args) {
    return ar_type(ar_car(args)) == AR_TPAIR ? S->t : NULL;
}

