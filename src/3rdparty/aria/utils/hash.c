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
#include "hash.h"

hash_t* hash_init(size_t size)
{
  hash_t* hash = malloc(sizeof(hash_t));

  if (size != HASH_DEFAULT_SIZE)
    hash->size = size;
  else
    hash->size = HASH_DEFAULT_SIZE;

  hash->table = malloc(sizeof(hash_table_t) * hash->size);

  size_t i = 0;
  for (; i < hash->size; i++)
    {
      hash->table[i] = malloc(sizeof(hash_table_t));
      memset(hash->table[i]->key, '\0', sizeof(char)*HASH_DEFAULT_KEY_LIMIT);
      hash->table[i]->value = 0;
      hash->table[i]->next = NULL;
    }

  return hash;
}

size_t hash_make(hash_t* h, const char* key)
{
  size_t hash = 0;
  size_t key_length = strlen(key);
  size_t index = key_length;

  size_t i = 0;
  for (; i < key_length; i++)
    {
      hash += key[i];
      hash *= 31;
    }

  index = hash % h->size;

  return index;

}

void hash_remove(hash_t* h, const char* key)
{
  size_t index = hash_make(h, key);

  hash_table_t* d;
  hash_table_t* o1;
  hash_table_t* o2;

  /* 1) index is empty */
  if (strcmp(h->table[index]->key, "") == 0 && h->table[index]->value == 0)
    return;

  /* 2) only 1 item contained in bucket and has matching key */
  if (strcmp(h->table[index]->key, key) == 0
      && h->table[index]->next == NULL )
    {
      strcpy(h->table[index]->key, "");
      h->table[index]->value = 0;

      return;
    }

  /* 3) match is located in the first item in the index but
         * there are more items in the bucket. */
  if (h->table[index] != NULL && strcmp(h->table[index]->key, key) == 0)
    {

      d = h->table[index];
      if (h->table[index]->next != NULL)
        {
          h->table[index] = h->table[index]->next;
        }

      if (d)
        free(d);
    }
  /* 4) the index contains items but the first is not a match*/
  o1 = h->table[index]->next;
  o2 = h->table[index];

  while (o1 != NULL && strcmp(o1->key, key) != 0)
    {
      o2 = o1;
      o1 = o1->next;
    }

  /*4.1 no match*/
  if (o1 == NULL)
    {
      /*4.2 match*/
      return;
    }
  else
    {
      d = o1;
      if (o1->next != NULL)
        {
          o1 = o1->next;
          o2->next = o1;
        }

      if (d)
        free(d);
    }
}

void* hash_get(hash_t* hash, const char* key)
{
  size_t index = hash_make(hash, key);

  void* value = NULL;

  hash_table_t* d = hash->table[index];

  while (d != NULL)
    {
      if (strcmp(d->key, key) == 0) {
          value = d->value;
          return value;
        }
      d = d->next;
    }

  return value;
}

void hash_add(hash_t* h, const char* key, void* value)
{
  size_t index = hash_make(h, key);
  size_t len = strlen(key);

  if (strcmp(h->table[index]->key, "") == 0)
    {
      /*default, we can add*/
      strcpy(h->table[index]->key, key);
      h->table[index]->key[len] = '\0';
      h->table[index]->value = value;
    }
  else
    {
      /* we have a value, we must construct a linked list to avoid collision*/
      hash_table_t* d = h->table[index]; /*data*/
      /*
         * Push the new value to end of the linked list.
         * We also check if the pushed key has been stored
         * previously and if it has then we replace its value
         * with the new one
         */
	  if (d->next != NULL) {
		  while (d->next != NULL) {
			  if (strcmp(d->key, key) == 0) {
				  d->value = value;
				  return;
			  }
			  d = d->next;
		  }
	  }
	  else {
		  if (strcmp(d->key, key) == 0) {
			  d->value = value;
			  return;
		  }
	  }

      hash_table_t* n = malloc(sizeof(hash_table_t)); /*next*/

      strcpy(n->key, key);
      n->key[len] = '\0';

      n->value = value;
      n->next = NULL;

      d->next = n;
    }
}

size_t hash_number_of_items_in_index(hash_t* h, size_t index)
{
  size_t count = 0;
  if (strcmp(h->table[index]->key, "") == 0)
    /* Empty*/
    return count;
  else
    {
      /* We have some values in this bucket/index.*/

      /*
                 * We increment here the variable because this
                 * might be the only '->next' this index has.
                 */
      count++;
      hash_table_t* d = h->table[index];
      while (d->next != NULL)
        {
          count++;
          d = d->next;
        }
      return count;
    }
  return 0;
}

void hash_print_in_index(hash_t* h, size_t index)
{

  hash_table_t* d = h->table[index];

  if (d != NULL && strcmp(d->key, "") == 0)
    {
      printf("Index: %lu is empty.\n", index);
      return;
    }

  while (d != NULL)
    {
      printf("%s %p \n", d->key, d->value);
      d = d->next;
    }
}

void hash_print(hash_t* h)
{
  size_t num;
  size_t i = 0;
  for (; i < h->size; i++)
    {
      num = hash_number_of_items_in_index(h, i);
      if (strcmp(h->table[i]->key, "") != 0)
        printf("index: %lu key: %s list of items: %lu \n",
               i, h->table[i]->key, num);
    }
}

void hash_destroy(hash_t* hash)
{
  size_t i = 0;
  for (; i < hash->size; i++)
    hash_remove(hash, hash->table[i]->key);
  free(hash->table);
  free(hash);
}
