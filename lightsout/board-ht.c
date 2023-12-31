#include <stdlib.h> // Standard C library: free(), NULL...
#include <stdbool.h> // Standard true, false, and bool type
#include <string.h> // Standard C version of lib/cstring.o0
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h" // Our allocation library
//citation: the code above and comments are copied from strbuf-writup
#include "lib/bitvector.h"
#include "lib/hdict.h"
#include "board-ht.h"

/* IMPORTANT: You must respect the hash set interface in hdict.h when
 * writing your implementation of these functions! */

/* Initializes a new hash table with the given capacity */


hdict_key entry_key(hdict_entry B){
  REQUIRES(B != NULL);
  bitvector *keyboard = &((struct board_data *)B)->board;
  return (void *)(keyboard);
  // return (void *)(&(((struct board_data *)B)->board));
  // return (void *)(&((struct board_data *)B)->board);
}

bool key_equal(hdict_key k1,  hdict_key k2){
  REQUIRES(k1 != NULL && k2 != NULL);
  bitvector b1 = *(bitvector *)k1;
  bitvector b2 = *(bitvector *)k2;
  return bitvector_equal(b1, b2);
}

void entry_free(hdict_entry e){
  free(e);
}

// size_t key_hash(hdict_key key)
// {
//   REQUIRES(key != NULL);
//   bitvector B = *(bitvector *)key;
//   size_t result = 0;
//   // result += B;
//   int len = BITVECTOR_LIMIT;
//   uint8_t is_bit_1 = 0;
//   for (int i = 0; i < len; i++){
//     if (bitvector_get(B, i)) is_bit_1 = 1;
//     else is_bit_1 = 0;
//     result = result*31 + is_bit_1;
//   }
//   // return (size_t)B;
//   return result;
// }

size_t key_hash(hdict_key key)
{
  REQUIRES(key != NULL);
  bitvector B = *(bitvector *)key;
  size_t result = 0;
  int len = BITVECTOR_LIMIT;
  size_t is_bit_1 = 0;
  for (size_t i = 0; i < (size_t)len; i++){
    if (bitvector_get(B, i)) is_bit_1 = 1;
    else is_bit_1 = 0;
    result = result + (is_bit_1<<i);
  }
  return result;
}

hdict_t ht_new(size_t capacity)
  /*@requires capacity > 0; @*/
  /*@ensures \result != NULL; @*/
{
  // printf("--htnew--\n");
  REQUIRES(0 < capacity);
  hdict_t result =  hdict_new(capacity, &entry_key, 
                              &key_equal, &key_hash, &entry_free);
  ENSURES(result != NULL);
  return result;
}

/* ht_lookup(H,B) returns
 * NULL if no struct containing the board B exists in H
 * A struct containing the board B if one exists in H.
 */

 // hdict_entry result = hdict_lookup(H, (void *)&bitvector);
struct board_data *ht_lookup(hdict_t H, bitvector B)
  /*@requires H != NULL; @*/
{
  // printf("--htlookup-- bitvec>%d \n",B);
  REQUIRES(H != NULL);
  bitvector *key = xcalloc(sizeof(bitvector),1);
  *key = B;
  // printf("1\n");
  hdict_entry result = hdict_lookup(H, (void *)key);
  // hdict_entry result = hdict_lookup(H, &B);
  // printf("2\n");
  ENSURES(result ==  NULL || hdict_lookup(H, entry_key(result)) == result);
  free(key);
  return (struct board_data *)result;
}

/* ht_insert(H,e) has no return value, because it should have as
 * a precondition that no struct currently in the hashtable contains
 * the same board as DAT->board.
 */
void ht_insert(hdict_t H, struct board_data *DAT)
  /*@requires H != NULL; @*/
{
  // printf("--htinsert-- DAT->board>%d \n",DAT->board);
  REQUIRES(H != NULL && DAT != NULL);
  REQUIRES(hdict_lookup(H, entry_key((void*)DAT)) == NULL);
  hdict_insert(H, (void*)DAT);
  ENSURES(hdict_lookup(H, entry_key((void*)DAT)) == (void*)DAT); 
}