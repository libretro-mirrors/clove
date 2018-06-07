/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <math.h>
#include "ar_math.h"

ar_Value *p_max(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a, b;
	#else
	double a, b;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    b = ar_eval_number(S, ar_nth(args, 1), env);
    return a > b ? ar_new_number(S, a) : ar_new_number(S, b);
}


ar_Value *p_min(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a, b;
	#else
	double a, b;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    b = ar_eval_number(S, ar_nth(args, 1), env);
    return a < b ? ar_new_number(S, a) : ar_new_number(S, b);
}


ar_Value *p_floor(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a;
	#else
	double a;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    return ar_new_number(S, floor(a));
}


ar_Value *p_ceil(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a;
	#else
	double a;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    return ar_new_number(S, ceil(a));
}


ar_Value *p_abs(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a;
	#else
	double a;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    return ar_new_number(S, a < 0 ? -a : a);
}


ar_Value *p_cos(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a;
	float res;
	#else
	double a;
	double res;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
	#ifdef FLOAT
	res = cosf(a);
	#else
	res = cos(a);
	#endif

    return ar_new_number(S, res);
}


ar_Value *p_sin(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a;
	float res;
	#else
	double a;
	double res;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
	#ifdef FLOAT
	res = sinf(a);
	#else
	res = sin(a);
	#endif

    return ar_new_number(S, res);
}


ar_Value *p_pi(ar_State *S, ar_Value *args, ar_Value *env)  {
	UNUSED(args);
	UNUSED(env);
	#ifdef AR_FLOAT
	return ar_new_number(S, 3.141592);
	#else
	return ar_new_number(S, 3.14159265358);
	#endif
}


ar_Value *p_sqrt(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a;
	#else
	double a;
	#endif

	a = ar_eval_number(S, ar_car(args), env);
    return ar_new_number(S, sqrt(a));
}


ar_Value *p_pow(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a, pow_val = 2;
	#else
	double a, pow_val = 2;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    ar_Value *b = ar_eval(S, ar_nth(args, 1), env);

    if (ar_type(b) != AR_TNIL)
        pow_val = b->u.num.n;

    return ar_new_number(S, pow(a, pow_val));
}


ar_Value *p_random_seed(ar_State *S, ar_Value *args, ar_Value *env) {
    int seed = 0;

    if (ar_type(ar_car(args)) != AR_TNIL)
        seed = ar_eval(S, ar_car(args), env)->u.num.n;
    else
        seed = time(NULL);

    srand(seed);

    return NULL;
}

ar_Value *p_random(ar_State *S, ar_Value *args, ar_Value *env) {
    int min, max;

    int length = ar_length(args);

    switch (length) {
    case 0:
        return ar_new_number(S,  rand());
    case 1:
        return ar_new_number(S, rand()%(int)(ar_eval_number(S, ar_car(args), env)));
    case 2:
        min = ar_eval_number(S, ar_car(args), env);
        max = ar_eval_number(S, ar_nth(args, 1), env);
        return ar_new_number(S,
                             rand()%(int)(((max+1)-min))+min);
    }
    return NULL;
}


ar_Value *p_mod(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float a, b;
	#else
	double a,b;
	#endif

    a = ar_eval_number(S, ar_car(args), env);
    b = ar_eval_number(S, ar_nth(args, 1), env);
    if (b == 0.) ar_error_str(S, "expected a non-zero divisor");
    return ar_new_number(S, a - b * (long) (a / b));
}


ar_Value *p_add(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float sum;
	#else
	double sum;
	#endif

    sum = ar_eval_number(S, ar_car(args), env);
	do {
		args = ar_cdr(args);
		sum += ar_eval_number(S, ar_car(args), env);
	} while (ar_cdr(args));
    return ar_new_number(S, sum);
}


ar_Value *p_sub(ar_State *S, ar_Value *args, ar_Value *env) {
    if (ar_type(ar_nth(args, 1)) == AR_TNIL) {
        return ar_new_number(S, -ar_eval_number(S, ar_car(args), env));
    }
	#ifdef FLOAT
	float sub;
	#else
	double sub;
	#endif

    sub = ar_eval_number(S, ar_car(args), env);
	do {
		args = ar_cdr(args);
		sub -= ar_eval_number(S, ar_car(args), env);
	} while (ar_cdr(args));
    return ar_new_number(S, sub);
}

ar_Value *p_mul(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float mul;
	#else
	double mul;
	#endif

 	mul = ar_eval_number(S, ar_car(args), env);
	do {
		args = ar_cdr(args);
		mul *= ar_eval_number(S, ar_car(args), env);
	} while (ar_cdr(args));
    return ar_new_number(S, mul);
}


ar_Value *p_div(ar_State *S, ar_Value *args, ar_Value *env) {
	#ifdef FLOAT
	float div;
	#else
	double div;
	#endif

	div = ar_eval_number(S, ar_car(args), env);
	do {
		args = ar_cdr(args);
		div /= ar_eval_number(S, ar_car(args), env);
	} while (ar_cdr(args));
	return ar_new_number(S, div);
}

