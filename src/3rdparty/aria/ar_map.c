/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "ar_map.h"
#include "utils/hash.h"


ar_Value *p_map(ar_State *S, ar_Value *args, ar_Value *env) {
	size_t size = HASH_DEFAULT_SIZE;
	/*
	 * Check for optional size of map argument.
	 */
	if (ar_type(ar_car(args)) == AR_TNUMBER) {
		size = ar_eval_number(S, ar_car(args), env);
	}
	hash_t *hash = hash_init(size);
	while (ar_cdr(args)) {
		const char *key = ar_eval_string(S, ar_car(args), env);
		args = ar_cdr(args);
		ar_Value *value = ar_eval(S, ar_car(args), env);
		args = ar_cdr(args);

		hash_add(hash, key, (ar_Value*)value);
	}
	return ar_new_map(S, hash);
}


ar_Value *p_map_add(ar_State *S, ar_Value *args, ar_Value *env) {
	hash_t *map = ar_eval_map(S, ar_car(args), env);
	const char* key = ar_eval_string(S, ar_nth(args, 1), env);
	ar_Value *value = ar_eval(S, ar_nth(args, 2), env);
	hash_add(map, key, value);
	return value;
}


ar_Value *p_map_remove(ar_State *S, ar_Value *args, ar_Value *env) {
	hash_t *map = ar_eval_map(S, ar_car(args), env);
	const char* key = ar_eval_string(S, ar_nth(args, 1), env);
	hash_remove(map, key);
	return NULL;
}


ar_Value *p_map_get(ar_State *S, ar_Value *args, ar_Value *env) {
	hash_t *map = ar_eval_map(S, ar_car(args), env);
	const char* key = ar_eval_string(S, ar_nth(args, 1), env);
	ar_Value *value = (ar_Value*)hash_get(map, key);
	if (value) {
		return value;
	}
	else {
		ar_error_str(S, "Couldn't find key: %s %s\n", key, "in map!");
	}
	return NULL;
}

