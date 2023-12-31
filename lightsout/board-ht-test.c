/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdlib.h> // Standard C library: free(), NULL...
#include <stdbool.h> // Standard true, false, and bool type
#include <string.h> // Standard C version of lib/cstring.o0
//citation: the code above and comments are copied from strbuf-writup
#include <stdio.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "lib/bitvector.h"
#include "board-ht.h"
#include <limits.h>

// You are welcome to define helper functions for your tests
bool num64_get(uint64_t B, uint8_t i)
  /*@requires 0 <= i < BITVECTOR_LIMIT; @*/
{
  REQUIRES(i < BITVECTOR_LIMIT);
  uint64_t mask = 1;
  mask = mask << i; //sth like 00010000 the rightmost is the 0th bit.
  uint64_t cleared = (B & mask);
  // printf("--bitget-- B>%d i>%d \n",B,i);
  return cleared == mask;
}

bool check_array_with_num(bitvector B, uint64_t num){
//   printf("--printcheck--\n");
  for (int i = 0; i < BITVECTOR_LIMIT; i ++){
    if (bitvector_get(B, i) != num64_get(num, i)) return false;
  }
  return true;
}

int main() {
  // Create a few struct board_data

  // Using them, test the functions you wrote in file board-ht.c
  hdict_t H = ht_new(10);
  struct board_data *DAT = xcalloc(sizeof(struct board_data),1);
  // DAT->board = 0x00000000;
  DAT->board = bitvector_new();
  // assert(ht_lookup(H, 0) == NULL);//for num
  assert(ht_lookup(H, DAT->board) == NULL);
  // assert(ht_lookup(H, (bitvector)INT_MAX) == NULL);
  // assert(ht_lookup(H, (bitvector)INT_MIN) == NULL);
  ht_insert(H, DAT);
  assert(ht_lookup(H, DAT->board) == DAT);
  // assert(ht_lookup(H, 0) == DAT);
  // assert(ht_lookup(H, (bitvector)INT_MAX) == NULL);

  // assert(ht_lookup(H, INT_MIN) == NULL);
  // assert(entry_key(DAT) == &(DAT->board));

  struct board_data *DAT2 = xcalloc(sizeof(struct board_data),1);
  // DAT2->board = 0x00000001;
  DAT2->board = bitvector_new();
  DAT2->board = bitvector_flip(DAT2->board, 0);
  // assert(entry_key(DAT2) == &(DAT2->board));
  ht_insert(H, DAT2);
  assert(ht_lookup(H, DAT2->board) == DAT2);
  // assert(ht_lookup(H, 1) == DAT2);//doesn't respect interface, only for num

  struct board_data *DAT3 = xcalloc(sizeof(struct board_data),1);
  // DAT3->board = (bitvector)0x39D3F83D;
  DAT3->board = bitvector_new();
  for (uint8_t i = 0; i < BITVECTOR_LIMIT; i ++){
    if (i % 4 == 0){
      DAT3->board = bitvector_flip(DAT3->board, i);
    }
  }
  ht_insert(H, DAT3);
  assert(check_array_with_num(DAT3->board, 0x1111111111111111));
  assert(ht_lookup(H, DAT3->board) == DAT3);
  // assert(ht_lookup(H, (bitvector)0x39D3F83D) == DAT3);
  // assert(ht_lookup(H, (bitvector)0x39D3F83E) == NULL);


  printf("All tests passed!\n");
  return 0;
}
