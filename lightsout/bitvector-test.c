/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"
#include <limits.h>
#include "lib/xalloc.h"

// You are welcome to define helper functions for your tests
// bool bitvector_get_32(bitvector B, uint8_t i)
//   /*@requires 0 <= i < BITVECTOR_LIMIT; @*/
// {
//   REQUIRES(i < BITVECTOR_LIMIT);
//   bitvector mask = (bitvector)1;
//   mask = mask<<i; //sth like 00010000 the rightmost is the 0th bit.
//   bitvector cleared = (B & mask);
//   // printf("--bitget-- B>%d i>%d \n",B,i);
//   return cleared == mask;
// }

void print_checkboard_32(bitvector B){
//   printf("--printcheck--\n");
  for (int i = 0; i < BITVECTOR_LIMIT; i ++){
    printf("i=%d: ",i);
    // printf("%d",bitvector_get_32(B,i));
    printf("%d",bitvector_get(B,i));
    printf(", ");
  }
  printf("\n");
}

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

// void print_checkboard_util(bitvector B){//debug
//   // printf("--printcheck--\n");
//   for (int i = 0; i < 4; i ++){
//     printf("i=%d: %d, ",i,bitvector_get(B,i));
//   }
//   printf("\n");
// }

bool check_array_with_num(bitvector B, uint64_t num){
//   printf("--printcheck--\n");
  for (int i = 0; i < BITVECTOR_LIMIT; i ++){
    if (bitvector_get(B, i) != num64_get(num, i)) return false;
  }
  return true;
}


// int main(){
//   unsigned char x = 0xFF;//255;
//   printf("uchar x=%d\n",x);
//   x = x>>1;
//   printf("uchar x>>1 =%d \n",x);
//   printf("%d\n",(x==0xFF));
//   signed char x2 = (signed char)x;
//   printf("char x2=%d\n",x2);
//   int y = (int)x2;
//   printf("int y=%d\n",y);

//   return 0;
// }

// int main(){
//   // int x = 1073741823;
//   printf("%d\n",INT_MAX + 1/2 * INT_MAX);
//   printf("%d\n",INT_MAX * 1/2);
// }

int main() {
  // Create a few bitvectors

  // Using them, test the functions you wrote in file bitvector.c
  bitvector b1 = bitvector_new();
  assert(check_array_with_num(b1, 0));
  // assert((int)b1 == 0);
  b1 = bitvector_flip(b1, 0);
  assert(check_array_with_num(b1, 1));
  // assert((int)b1 == 0x00000001);
  b1 = bitvector_flip(b1, 2);
  assert(check_array_with_num(b1, 0x00000005));
  b1 = bitvector_flip(b1, 0);
  b1 = bitvector_flip(b1, 1);
  // assert((int)b1 == 0x00000006);
  assert(check_array_with_num(b1, 0x00000006));

  bitvector b2 = bitvector_new();
  // assert((int)b2 == 0);
  // assert(b2 == (bitvector)0);//for num
  assert(check_array_with_num(b2, 0));//for arr
  b2 = bitvector_flip(b2, 2);
  // assert((int)b2 == 0x00000004);
  assert(check_array_with_num(b2, 0x00000004));
  b2 = bitvector_flip(b2, 1);
  // assert((int)b2 == 0x00000006);
  assert(check_array_with_num(b2, 0x00000006));
  assert(bitvector_equal(b1, b2));
  // printf("1\n");
  b2 = bitvector_flip(b2, 1);
  // assert((int)b2 == 0x00000004);
  assert(check_array_with_num(b2, 0x00000004));
  // printf("2\n");s
  b2 = bitvector_flip(b2, 1);
  // assert((int)b2 == 0x00000006);
  assert(check_array_with_num(b2, 0x00000006));

  bitvector b3 = bitvector_new();
  for (int i = 0; i < BITVECTOR_LIMIT; i ++){
    b3 = bitvector_flip(b3, i);
  }
  // assert(bitvector_equal(b3, (bitvector)0xFFFFFFFFFFFFFFFF));//fo num
  assert(check_array_with_num(b3, 0xFFFFFFFFFFFFFFFF));//for arr
  // print_checkboard_32(b3);
  // print_checkboard_32((bitvector)0x01FFFFFF);//garbage
  // assert(bitvector_equal(b3, (bitvector)0x01FFFFFF));//garbage


  for (int i = BITVECTOR_LIMIT - 1; i >= 0; i --){
    b3 = bitvector_flip(b3, i);
  }
  // assert((int)b3 == 0);//for num
  // assert(b3 == (bitvector)0);//for num
  assert(check_array_with_num(b3, 0));//for arr
  
  bitvector b4 = bitvector_new();
  assert(bitvector_equal(b3, b4));


  printf("All tests passed!\n");
  return 0;
}
