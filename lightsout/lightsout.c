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
#include "lib/queue.h"
#include "lib/boardutil.h"
#include <limits.h>
#include <math.h>

// void print_checkboard(bitvector B){
// //   printf("--printcheck--\n");
//   for (int i = 0; i < 4; i ++){
//     printf("i=%d: ",i);
//     printf("%d",bitvector_get(B,i));
//     printf(", ");
//   }
//   printf("\n");
// }

// bool bitvector_equal_lightsout(bitvector B1, bitvector B2, 
//                                uint8_t width, uint8_t height)
// {
//   REQUIRES(is_valid_boardsize(width, height));
//   for (uint8_t row = 0; row < height; row++) {
//     for (uint8_t col = 0; col < width; col++) {
//       int i = row * width + col;
//       uint8_t b1 = bitvector_get(B1, i);
//       uint8_t b2 = bitvector_get(B2, i);
//       if (b1 != b2) return false;
//     }
//   }
//   return true;
// }

bitvector press_button(bitvector B, uint8_t row, uint8_t col, 
                       uint8_t width, uint8_t height)
{
  //   printf("--pressbuttton--\n");
  uint8_t i = get_index(row, col, width, height);
  //   print_checkboard(B);
  bitvector result = bitvector_flip(B, i);

  if (col > 0){ //the min value a col can be is 0
    // printf("--pressbuttton-- c > 0,r=%d c=%d\n",row,col-1);
    uint8_t l = get_index(row, col - 1, width, height);
    result = bitvector_flip(result, l);
  }
  if (col < width - 1){ //the max value a col can be is width-1
    // printf("--pressbuttton-- c < w-1,r=%d c=%d\n",row,col+1);
    uint8_t r = get_index(row, col + 1, width, height);
    result = bitvector_flip(result, r);
  }
  if (row > 0){
    // printf("--pressbuttton-- r > 0,r=%d c=%d\n",row-1,col);
    uint8_t u = get_index(row - 1, col, width, height);
    result = bitvector_flip(result, u);
  }
  if (row < height - 1){ 
    // printf("--pressbuttton-- r < w+1,r=%d c=%d\n",row+1,col);
    uint8_t d = get_index(row + 1, col, width, height);
    result = bitvector_flip(result, d);
  }
  return result;
}

void print_success(struct board_data *DAT, hdict_t H,
                   uint8_t width, uint8_t height){
  // printf("r=%d h=%d c=%d w=%d\n",
  //        DAT->flipped_row,height,DAT->flipped_col,width);
  if (DAT->flipped_row >= height || DAT->flipped_col >= width) return;
  bitvector newboard = press_button(DAT->board, DAT->flipped_row, 
                                    DAT->flipped_col, width, height);
  struct board_data *DAT_new = ht_lookup(H, newboard);
  printf("%d:%d\n",DAT->flipped_row,DAT->flipped_col);
  print_success(DAT_new, H, width, height);
//   print_board(DAT->board, width, height);
//   printf("\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: lightsout <board name>\n");
    return EXIT_FAILURE; // see below for what this means
}
  char *board_filename = argv[1];

  uint8_t *width = xcalloc(sizeof(uint8_t), 1);
  uint8_t *height = xcalloc(sizeof(uint8_t), 1);
  bitvector *vec = xcalloc(sizeof(bitvector), 1);

  file_read(board_filename, vec, width, height);
  queue_t Q = queue_new();
  struct board_data *first_DAT = xcalloc(sizeof(struct board_data), 1);
  first_DAT->board = *vec;
  // first_DAT->prev_board = NULL;
  first_DAT->flipped_row = -1;
  first_DAT->flipped_col = -1;
  enq(Q, (void*)first_DAT);

  hdict_t H = ht_new(10000);
  ht_insert(H, first_DAT);

  // I'm shocked... this direspects interface and causes all my errors
  bitvector compare = bitvector_new(); //!!!!! 

//   printf("\ncompare= \n");
  // print_checkboard(compare);
//   print_board(compare, *width, *height);
//   printf("\n");

  while (!queue_empty(Q)) {
    // printf("\n--main WHILE NEW ITERATION-- new board is dequeued\n");
    // Find a board that we haven’t looked at yet from the queue
    struct board_data *DAT = (struct board_data *)deq(Q);

    // Consider all the moves
    for (uint8_t row = 0; row < *height; row++) {
      // printf("----main ROW NEW ITERATION-- row>%d\n",row);
      for (uint8_t col = 0; col < *width; col++) {
        // printf("------main COL NEW ITERATION-- col>%d\n",col);
        bitvector newboard = press_button(DAT->board, 
                                          row, col, *width, *height);
        if (bitvector_equal(newboard, compare)) { //??
                
          print_success(DAT, H, *width, *height);
          printf("%d:%d\n",row,col);
          //   print_board(newboard, *width, *height);
          //   printf("\n");

          queue_free(Q, NULL);
          free(width);
          free(height);
          free(vec);
          hdict_free(H);
          // printf("You got all the lights out!\n");
          return EXIT_SUCCESS;
        }
        
        // if hash table H doesn’t contain newboard
        // printf("--main-- row>%d col>%d newboard>%d goalboard>%d \n",
        //        row,col,newboard,compare);
        // print_checkboard(newboard);
        // print_board(newboard, *width, *height);
        if (ht_lookup(H, newboard) == NULL) {
        //   printf("--main-- H doesn't contain this newboard\n");
          // Allocate memory to hold hashtable element N
          struct board_data *N = xcalloc(sizeof(struct board_data), 1);
        
          // Set the field N->board to newboard, set other fields
          N->board = newboard;
          // N->prev_board = DAT;
          N->flipped_row = row;
          N->flipped_col = col;

          // Insert N into the hashtable H
          ht_insert(H, N);

          // Enqueue N into the queue Q
          enq(Q, (void*)N);
        }
        // printf("--main-- H contains this newboard\n");
      }   
    }
  }
  queue_free(Q, NULL);
  free(width);
  free(height);
  free(vec);
  hdict_free(H);
    // printf("This board is not solvable!\n");
  return EXIT_FAILURE;
}
