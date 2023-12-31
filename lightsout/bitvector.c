#include <stdlib.h> // Standard C library: free(), NULL...
#include <stdbool.h> // Standard true, false, and bool type
#include <string.h> // Standard C version of lib/cstring.o0
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h" // Our allocation library
//citation: the code above and comments are copied from strbuf-writup
#include <stdint.h>  // int
#include "lib/bitvector.h"

// integer implementation
/* Get a new bitvector with everything set to 'false'. */
bitvector bitvector_new(){
  return (bitvector)0;
}

/* Get the ith bit of the bitvector n. */
bool bitvector_get(bitvector B, uint8_t i)
  /*@requires 0 <= i < BITVECTOR_LIMIT; @*/
{
  REQUIRES(i < BITVECTOR_LIMIT);
  bitvector mask = (bitvector)1;
  mask = mask << i; //sth like 00010000 the rightmost is the 0th bit.
  bitvector cleared = (B & mask);
  // printf("--bitget-- B>%d i>%d \n",B,i);
  return cleared == mask;
}

// void print_checkboard_util(bitvector B){//debug
//   // printf("--printcheck--\n");
//   for (int i = 0; i < 4; i ++){
//     printf("i=%d: %d, ",i,bitvector_get(B,i));
//   }
//   printf("\n");
// }


/* Toggle the ith bit of the bitvector n, returning a new bitvector. */
/* The old bitvector remains unchanged. */
bitvector bitvector_flip(bitvector B, uint8_t i)
  /*@requires 0 <= i < BITVECTOR_LIMIT; @*/
{
  // printf("--bitflip--\n");
  // print_checkboard_util(B);
  REQUIRES(i < BITVECTOR_LIMIT);
  bool bit_is_1 = bitvector_get(B, i);
  if (bit_is_1){
    bitvector mask = (bitvector)1;
    mask = ~(mask << i); // making sth like 00010000, then 11101111
    bitvector result = B & mask;
    // printf("--bitflip-- flip %d to %d, B before=%d i=%d mask>%d res>%d\n",
    //        bit_is_1, bitvector_get(result, i), B,i,mask,result);
    // print_checkboard_util(result);
    return result;  
  }
  else{ //that bit is 0
    bitvector mask = (bitvector)1;
    mask = mask << i; //sth like 00010000
    bitvector result = B | mask;
    // printf("--bitflip-- flip %d to %d, B before=%d i=%d mask>%d res>%d\n",
    //        bit_is_1,bitvector_get(result, i),B,i,mask,result);
    // print_checkboard_util(result);
    return result;
  }
}

/* Compare two bitvectors for equality. */

bool bitvector_equal(bitvector B1, bitvector B2)
{
  bitvector mask = (bitvector)1;
  mask = ~mask;//making sth like 11111110
  mask = (mask << (BITVECTOR_LIMIT - 1));//making sth like 10000000
  mask = ~mask;//sth like 01111111
  return ((B1 & mask) == (B2 & mask)); //clear the bits after BITVECTOR_LIMIT

  // for (int i = 0; i < BITVECTOR_LIMIT; i++){
  //   uint8_t b1 = bitvector_get(B1, i);
  //   uint8_t b2 = bitvector_get(B2, i);
  //   if (b1 != b2) return false;
  // }
  // return true;
}







// // array implementation
// /* Get a new bitvector with everything set to 'false'. */
// bitvector bitvector_new(){
//   bool *B = xcalloc(BITVECTOR_LIMIT, sizeof(bool));
//   return B;
// }

// /* Get the ith bit of the bitvector n. */
// bool bitvector_get(bitvector B, uint8_t i)
// {
//   REQUIRES(i < BITVECTOR_LIMIT);
//   return B[i];
// }

// // void print_checkboard_util(bitvector B){//debug
// //   // printf("--printcheck--\n");
// //   for (int i = 0; i < 4; i ++){
// //     printf("i=%d: %d, ",i,bitvector_get(B,i));
// //   }
// //   printf("\n");
// // }


// bitvector bitvector_copy_arr(bitvector B){
//   bitvector B_result = bitvector_new();
//   for (uint8_t i = 0; i < BITVECTOR_LIMIT; i ++){
//     B_result[i] = B[i];
//   }
//   return B_result;
// }

// /* Toggle the ith bit of the bitvector n, returning a new bitvector. */
// /* The old bitvector remains unchanged. */
// bitvector bitvector_flip(bitvector B, uint8_t i)
//   /*@requires 0 <= i < BITVECTOR_LIMIT; @*/
// {
//   // printf("--bitflip--\n");
//   // print_checkboard_util(B);
//   REQUIRES(i < BITVECTOR_LIMIT);
//   bitvector B_result = bitvector_copy_arr(B);
//   bool bit_is_1 = bitvector_get(B_result, i);
//   if (bit_is_1){
//     B_result[i] = false;
//     // printf("--bitflip-- flip %d to %d, B before=%d i=%d mask>%d res>%d\n",
//     //        bit_is_1, bitvector_get(result, i), B,i,mask,result);
//     // print_checkboard_util(result);
//     assert(!bitvector_equal(B, B_result));
//     return B_result;  
//   }
//   else{ //that bit is 0
//     B_result[i] = true;
//     // printf("--bitflip-- flip %d to %d, B before=%d i=%d mask>%d res>%d\n",
//     //        bit_is_1,bitvector_get(result, i),B,i,mask,result);
//     // print_checkboard_util(result);
//     assert(!bitvector_equal(B, B_result));
//     return B_result;
//   }
// }

// /* Compare two bitvectors for equality. */

// bool bitvector_equal(bitvector B1, bitvector B2)
// {
//   for (uint8_t i = 0; i < BITVECTOR_LIMIT; i ++){
//     if (bitvector_get(B1, i) != bitvector_get(B2, i)) return false;
//   }
//   return true;
// }

