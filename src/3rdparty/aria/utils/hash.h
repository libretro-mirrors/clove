/*
Copyright (c) 2017-2018, Muresan Vlad Mihail
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
*/
#ifndef _HASH_MAP_H
#define _HASH_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/*
 * The maximum values of buckets this hash-map can have
 * Low value - more chances of collision
 * High value - less chances of collision but more ram usage.
 */
#define HASH_DEFAULT_SIZE 34866
/*
 * Maximum size a key can have in bytes
 */
#define HASH_DEFAULT_KEY_LIMIT 64

typedef struct hash_table_t
{
	char key[HASH_DEFAULT_KEY_LIMIT];
	void* value;

	struct hash_table_t* next;
} hash_table_t;

/* Main data type */
typedef struct hash_t
{
	/* Size of this hashmap */
	size_t size;
	struct hash_table_t** table;
} hash_t;

/*
 * Allocates memory and inits everything
 */
hash_t* hash_init(size_t size);
/*
 * Private function
 */
size_t hash_make(hash_t* h, const char* key);
/*
 * Adds new value in hash
 */
void hash_add(hash_t* h, const char* key, void* value);
/*
 * Gets a specific key from hash if it exists, return NULL otherwise.
 */
void* hash_get(hash_t* hash, const char* key);
/*
 * We look at that index and we count the number of elements in that location
 */
size_t hash_number_of_items_in_index(hash_t* h, size_t index);
/*
 * Useful for debug
 */
void hash_print(hash_t* h);
void hash_print_in_index(hash_t* h, size_t index);
/*
 * Removes a specific key if it exists
 */
void hash_remove(hash_t* h, const char* key);
/*
 * Disposes all allocated memory
 */
void hash_destroy(hash_t* hash);
#endif

