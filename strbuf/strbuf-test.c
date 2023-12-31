/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdlib.h> // Standard C library: free(), NULL...
#include <stdbool.h> // Standard true, false, and bool type
#include <string.h> // Standard C version of lib/cstring.o0
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h" // Our allocation library
#include "strbuf.h" // The string buffer interface
#include <assert.h>

// You are welcome to define helper functions for your tests

bool my_is_strbuf(strbuf *sb){
  if (sb == NULL || sb->buf == NULL
      || (int)sb->len < 0 
      || (int)sb->len >= (int)sb->limit || !is_strbuf(sb)) return false;
  for (int i = 0; i < (int)(sb->len); i++){
    if (sb->buf[i] == '\0') return false;
  }
  return sb->buf[(int)sb->len] == '\0'; 
}

void test_B_and_B2(strbuf *B)
{
  assert(B != NULL && is_strbuf(B) && my_is_strbuf(B));
  assert(B->buf[0] == 'a');
  assert(B->buf[1] == 'a');
  assert(B->buf[2] == 'b');
  assert(B->buf[3] == 'c');
  assert(B->buf[4] == 'b');
  assert(B->buf[5] == 'c');
  assert(B->buf[6] == ' ');
  assert(B->buf[7] == 'e');
  assert(B->buf[8] == 'f');
  assert(B->buf[9] == ' ');
  assert(B->buf[10] == 'e');
  assert(B->buf[11] == 'f');
  assert(B->buf[12] == 'g');
  assert(B->buf[13] == 'g');
  assert(B->buf[14] == '\0');
}

bool check_copy_str(char *check, strbuf *sb)
{
  assert(sb != NULL && is_strbuf(sb) && my_is_strbuf(sb));
  assert(check != NULL);
  assert(check[(int)(sb->len)] == '\0');
  assert(strlen(check) == sb->len);
  for (int i = 0; i < (int)(sb->len) + 1; i++){
    if (check[i] != sb->buf[i]) return false;
  }
  // ENSURES(check[(int)strlen(check)] == '\0');
  free(check);
  return true;
}

bool check_add(strbuf *sb)
{
  return is_strbuf(sb) && my_is_strbuf(sb);// && sb->len == strlen(sb->buf);
}

bool check_new(strbuf *sb, int init_limit){
  if (sb == NULL || sb->buf == NULL || (int)sb->limit != init_limit 
      || (int)sb->len != 0 
      || (int)sb->len >= (int)sb->limit || !is_strbuf(sb)) return false;
  for (int i = 0; i < (int)(sb->len); i++){
    if (sb->buf[i] == '\0') return false;
  }
  return sb->buf[(int)sb->len] == '\0';
}


int main() {
  printf("--main start testing");

  int limit = 2;
  char *C = xcalloc(limit, sizeof(char));
  C[0] = 'a';
  C[1] = '\0';
//   char *C = string_to_chararray("a");

  int limit2 = 3;
  char *C2 = xcalloc(limit2, sizeof(char));
  C2[0] = 'b';
  C2[1] = 'c';
  C2[2] = '\0';
//   char *C2 = string_to_chararray("bc");

  printf("--main testing B4\n"); // test isbuf
  strbuf *B4 = NULL;
  assert(!is_strbuf(B4)); //sb=NULL
  free(B4);
  B4 = strbuf_new(1);
  assert(check_new(B4, 1));
  free(B4->buf);
  B4->buf = NULL;
  assert(!is_strbuf(B4)); //sb->buf=NULL
  free(B4);
  B4 = strbuf_new(1);
  B4->limit = (size_t)0;
  assert(!is_strbuf(B4)); //sb->limit<1
  B4->limit = (size_t)1;
  B4->len = (size_t)-1;
  assert(!is_strbuf(B4)); //len<0
  B4->len = (size_t)1;
  assert(!is_strbuf(B4)); // len=limit
  B4->len = (size_t)3;
  assert(!is_strbuf(B4)); // len>limit
  B4->len = (size_t)0;
  assert(is_strbuf(B4)); //correct
  free(strbuf_free(B4));
  B4 = strbuf_new(2); //limit=2
  assert(check_new(B4, 2));
  B4->len = (size_t)1;
  assert(!is_strbuf(B4)); //len(=1)!=strlen(=0)
  B4->buf[0] = '1';
  B4->buf[1] = '\0';
  assert(is_strbuf(B4));

  strbuf_add(B4, C, 1);
  strbuf_addstr(B4, C);
  assert(check_add(B4));
  assert(B4->buf[0] == '1');
  assert(B4->buf[1] == 'a');
  assert(B4->buf[2] == 'a');
  B4->buf[1] = '2';
  assert(B4->buf[0] == '1');
  assert(B4->buf[1] == '2');
  assert(B4->buf[2] == 'a');
  strbuf_add(B4, C2, limit2 - 1);
  strbuf_addstr(B4, C2);
  assert(check_add(B4));
  assert(B4->buf[0] == '1');
  assert(B4->buf[1] == '2');
  assert(B4->buf[2] == 'a');
  assert(B4->buf[3] == 'b');
  assert(B4->buf[4] == 'c');
  assert(B4->buf[5] == 'b');
  assert(B4->buf[6] == 'c');

  B4->buf[0] = '\0';
  assert(!is_strbuf(B4)); //len!=strlen
  B4->buf[0] = '1';
  B4->buf[3] = '\0';
  assert(!is_strbuf(B4)); //len!=strlen
  B4->buf[3] = '2';
  B4->buf[6] = '\0';
  assert(!is_strbuf(B4)); //len!=strlen
  B4->buf[6] = 'c';

  strbuf *B = strbuf_new(1);
  assert(check_new(B, 1));
  printf("--main testing B");
  assert(check_copy_str(strbuf_str(B), B));
  strbuf_add(B, C, (size_t)(limit - 1));
  strbuf_addstr(B, C);
  assert(check_add(B));
  assert(B->buf[0] == 'a');
  assert(B->buf[1] == 'a');
  assert(B->buf[2] == '\0');
  assert(check_copy_str(strbuf_str(B), B));

  strbuf_add(B, C2, (size_t)(limit2 - 1));
  strbuf_addstr(B, C2);
  assert(check_add(B));
  assert(B->buf[2] == 'b');
  assert(B->buf[3] == 'c');
  assert(B->buf[4] == 'b');
  assert(B->buf[5] == 'c');
  assert(B->buf[6] == '\0');

  int limit3 = 1;
  char *C3 = xcalloc(limit3, sizeof(char));
  C3[0] = '\0';
  strbuf_add(B, C3, (size_t)(limit3 - 1));
  strbuf_addstr(B, C3);
  assert(check_add(B));
  assert(B->buf[6] == '\0');
  assert(check_copy_str(strbuf_str(B), B));


  int limit4 = 6;
  char *C4 = xcalloc(limit4, sizeof(char));
  C4[0] = ' ';
  C4[1] = 'e';
  C4[2] = 'f';
  C4[3] = '\0';
//   char *C4 = string_to_chararray(" ef");
  strbuf_add(B, C4, (size_t)3);
  strbuf_addstr(B, C4);
  assert(check_add(B));
  assert(B->buf[6] == ' ');
  assert(B->buf[7] == 'e');
  assert(B->buf[8] == 'f');
  assert(B->buf[9] == ' ');
  assert(B->buf[10] == 'e');
  assert(B->buf[11] == 'f');
  assert(B->buf[12] == '\0');
  
  int limit5 = 6;
  char *C5 = xcalloc(limit5, sizeof(char));
  C5[0] = 'g';
  C5[1] = '\0';
  // C5[2] = 'h';
  // C5[3] = '\0';
  strbuf_add(B, C5, (size_t)1);
  strbuf_addstr(B, C5);
  assert(check_add(B));
  test_B_and_B2(B);
  assert(check_copy_str(strbuf_str(B), B));
  
  strbuf *B2 = strbuf_new((size_t)3);
  assert(check_new(B2, 3));
  printf("--main testing B2\n");
  assert(check_copy_str(strbuf_str(B2), B2));
  int B2_limit_before = B2->limit;
  int B2_len_before = B2->len;
  strbuf_add(B2, C, (size_t)(limit - 1));
  assert((int)B2->limit == (int)B2_limit_before 
         || (int)B2_limit_before - (int)B2_len_before <= limit - 1);
  strbuf_addstr(B2, C);
  assert(check_add(B2));
  assert(B2->buf[0] == 'a');
  assert(B2->buf[1] == 'a');
  assert(B2->buf[2] == '\0');
  strbuf_add(B2, C2, (size_t)(limit2 - 1));
  B2_limit_before = B2->limit;
  B2_len_before = B2->len;
  strbuf_addstr(B2, C2);
  assert((int)B2->limit == (int)B2_limit_before 
         || (int)B2_limit_before - (int)B2_len_before <= limit2 - 1);
  assert(B2->buf[2] == 'b');
  assert(B2->buf[3] == 'c');
  assert(B2->buf[4] == 'b');
  assert(B2->buf[5] == 'c');
  assert(B2->buf[6] == '\0');
  B2_limit_before = B2->limit;
  B2_len_before = B2->len;
  strbuf_add(B2, C3, (size_t)(limit3 - 1));
  assert((int)B2->limit == (int)B2_limit_before 
         || (int)B2_limit_before - (int)B2_len_before <= limit3 - 1);
  strbuf_addstr(B2, C3);
  assert(check_add(B2));
  assert(B2->buf[6] == '\0');
  strbuf_add(B2, C4, (size_t)3);
  assert(check_add(B2));
  B2_limit_before = B2->limit;
  B2_len_before = B2->len;
  strbuf_addstr(B2, C4);
  assert(check_add(B2));
  assert((int)B2->limit == (int)B2_limit_before 
         || (int)B2_limit_before - (int)B2_len_before <= 3);
  assert(B2->buf[6] == ' ');
  assert(B2->buf[7] == 'e');
  assert(B2->buf[8] == 'f');
  assert(B2->buf[9] == ' ');
  assert(B2->buf[10] == 'e');
  assert(B2->buf[11] == 'f');
  assert(B2->buf[12] == '\0');
  B2_limit_before = B2->limit;
  B2_len_before = B2->len;
  strbuf_add(B2, C5, (size_t)1);
  assert((int)B2->limit == (int)B2_limit_before 
         || (int)B2_limit_before - (int)B2_len_before <= 1);
  strbuf_addstr(B2, C5);
  assert(check_add(B2));
  test_B_and_B2(B2);
  ASSERT(check_copy_str(strbuf_str(B2), B2));


  int B3_limit_original = 3;
  strbuf *B3 = strbuf_new((size_t)B3_limit_original);
  assert(check_new(B3, 3));
  printf("--main testing B3\n");
  int B3_limit_before = B3->limit;
  int B3_len_before = (int)B3->len;
  for (int i = 0; i < 500; i ++){
    B3_limit_before = B3->limit;
    B3_len_before = (int)B3->len;
    strbuf_add(B3, C2, (size_t)(limit2 - 1));
    assert((int)B3->limit == (int)B3_limit_before 
           || (int)B3_limit_before - (int)B3_len_before <= (limit2 - 1));
  }
  printf("--main B3 pt 1 done\n");
  for (int i = 0; i < 100; i ++){
    strbuf_addstr(B3, C2);
  }
  assert(check_add(B3));
  printf("--main B3 pt 2 done\n");
  // print_strbuf(B3);
  for (int i = 0; i < 20; i ++){ 
    // only check the first part and last part to save time
    if (i % 2 == 0) assert(B3->buf[i] == 'b');
    else assert(B3->buf[i] == 'c');
  }
  printf("--main B3 pt 3 done\n");
  for (int i = 980; i < 1000; i ++){
    if (i % 2 == 0) assert(B3->buf[i] == 'b');
    else assert(B3->buf[i] == 'c');
  }
  assert(check_copy_str(strbuf_str(B3), B3));
  printf("--main B3 pt 4 done\n");

  char *C6 = xcalloc(1001, sizeof(char));
  for (int i = 0; i < 1000; i ++){
    int num = i % 94 + 33;
    C6[i] = (char)num;
  }
  C6[1000] = '\0';
  // for (int i = 0; i < 1000; i ++){
  //   printf("C6[%d] = %c\n",i,C6[i]);
  // }
  // printf("strlen>%d\n",strlen(C6, 0));
  strbuf_add(B3, C6, (size_t)1000);
  strbuf_addstr(B3, C6);
  assert(check_add(B3));
  assert(check_copy_str(strbuf_str(B3), B3));
  printf("--main B3 pt 5 done\n");

  printf("--main testing B5\n"); // test strbuf_str
  strbuf *B5 = strbuf_new(((size_t)1));
  assert(check_new(B5, 1));
  strbuf_add(B5, C3, (size_t)0);
  strbuf_addstr(B5, C3);
  assert(check_add(B5));
  assert(check_copy_str(strbuf_str(B5), B5));
  strbuf_add(B5, C4, (size_t)3);
  strbuf_addstr(B5, C4); //defdef0
  assert(check_add(B5));
  B5->buf[5] = '\0';
  B5->len --;
  assert(check_copy_str(strbuf_str(B5), B5));

  printf("--main testing freeing\n");
  char *free_check = B->buf;
  char *B_buf = strbuf_free(B);
  assert(free_check == B_buf); //aliasing!
  free(free_check);
  // free(B_buf);
  
  printf("--main testing freeing C\n");
  free(C);
  free(C2);
  free(C3);
  free(C4);
  free(C5);
  free(C6);

  printf("--main testing freeing B2\n");
  char *free_check2 = B2->buf;
  char *B2_buf = strbuf_free(B2);
  assert(free_check2 == B2_buf);
  free(free_check2);
  // free(B2_buf);

  printf("--main testing freeing B3\n");
  char *free_check3 = B3->buf;
  char *B3_buf = strbuf_free(B3);
  assert(free_check3 == B3_buf);
  free(free_check3);
  // free(B3_buf);

  char *free_check4 = B4->buf;
  char *B4_buf = strbuf_free(B4);
  assert(free_check4 == B4_buf);
  free(free_check4);
  // free(B4_buf);

  char *free_check5 = B5->buf;
  char *B5_buf = strbuf_free(B5);
  assert(free_check5 == B5_buf);
  free(free_check5);
  // free(B5_buf);

  printf("All tests passed!\n");
  return 0;
}
