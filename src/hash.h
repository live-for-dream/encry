#ifndef _HASH_INCLUDED
#define _HASH_INCLUDED

#include <types.h>

int calc_hash(string_t *key, hash_t *htable);
void set_index(uint size);
uint get_prime();


#endif
