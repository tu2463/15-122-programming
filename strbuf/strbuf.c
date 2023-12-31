/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
/*
 * String Buffer Library
 *
 * 15-122 Principles of Imperative Computation
 * This version exposes the externals, requires
 * discipline from client! */

#include <stdlib.h> // Standard C library: free(), NULL...
#include <stdbool.h> // Standard true, false, and bool type
#include <string.h> // Standard C version of lib/cstring.o0
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h" // Our allocation library
#include "strbuf.h" // The string buffer interface

/*** Implementation ***/
// bool c_is_cstring(char *str, int off, int limit){
//   REQUIRES(limit <= (int)(strlen(str)));
//   for (int i = off; i < limit; i++){
//     if (str[i] == '\0') return true;
//   }
//   return false;
// }

void print_strbuf(strbuf* sb){
  for (int i = 0; i < (int)(sb->len); i++){
    printf("%s%d%s%c\n","i>",i," char>",sb->buf[i]);
  }
}

void print_buf(char *buf){
  for (int i = 0; i < (int)(strlen(buf)); i++){
    printf("%s%d%s%c\n","i>",i," char>",buf[i]);
  }
}

bool is_strbuf(strbuf *sb)
{
  if (sb == NULL || sb->buf == NULL || (int)sb->limit < 1 || (int)sb->len < 0 
      || sb->len >= sb->limit) return false;
  for (int i = 0; i < (int)(sb->len); i++){
    if (sb->buf[i] == '\0') return false;
  }
  return sb->buf[sb->len] == '\0';
}

strbuf *strbuf_new(size_t init_limit)
{
  REQUIRES((int)init_limit > 0);
  strbuf *new = xmalloc(sizeof(strbuf));
  new->limit = (int)(init_limit);
  new->buf = (xcalloc(new->limit, sizeof(char)));
  new->len = 0;
  new->buf[new->len] = '\0';
  ENSURES(is_strbuf(new)); 
  return new;
}


char *strbuf_str(strbuf *sb)
{
  REQUIRES(is_strbuf(sb));
  char *result = xcalloc(sb->len + 1, sizeof(char));
  ASSERT(result != NULL);
  // result = strncpy(sb->buf, result, sb->len);
  for (int i = 0; i < (int)(sb->len + 1); i++){
    // printf("--c str i>%d sb->len>%d sb->limit>%d \n", 
    //        i,(int)sb->len,(int)sb->limit);
    result[i] = sb->buf[i];
  }
  // print_buf(result);
  ENSURES((int)(strlen(result)) == (int)(sb->len));
  ENSURES(result[(int)(strlen(result))] == '\0');
  // ENSURES((int)(strlen(result)) == (int)(strlen(result)));
  ENSURES(result != NULL);
  //ensures is_cstring //??
  return result;
}

void strbuf_add_resize(strbuf *sb, int len)
{
  REQUIRES(is_strbuf(sb));
  // sb->limit = (sb->limit + len) * 2;
  while ((sb->len) + len >= sb->limit){
    sb->limit = sb->limit * 2;
  }
  char *new_buf = xcalloc(sb->limit, sizeof(char));
  ASSERT(new_buf != NULL);
  for (int i = 0; i <= (int)(sb->len); i++){
    ASSERT(sb->buf[i] != '\0' || i == (int)(sb->len));
    new_buf[i] = sb->buf[i];
  }
  free(sb->buf);
  sb->buf = new_buf;
  ENSURES(is_strbuf(sb));
}

void strbuf_add(strbuf *sb, char *str, size_t len)
{
  REQUIRES(is_strbuf(sb));
  REQUIRES(str != NULL);
  REQUIRES(len == (strlen(str)));
  if (sb->len + len >= sb->limit){
    // printf("--add resize\n");
    strbuf_add_resize(sb, (int)len);
  }
  ASSERT(sb->len + len < sb->limit);
//   printf("%s%d%s%d\n","--add now sb->len>",(int)(sb->len),
//          " sb->limit>",(int)(sb->limit));
  // sb->buf[(sb->len) + len] = sb->buf[sb->len];
  sb->buf[(int)(sb->len) + len] = '\0';
  for (int i = 0; i < (int)len; i ++){
    // printf("%s%c\n","--add str[i]>",str[i]);
    ASSERT(str[i] != '\0' || i == (int)len);
    ASSERT(sb->len <= i + sb->len && i + sb->len < sb->limit);
    sb->buf[i + (int)sb->len] = str[i];
  }
  sb->len = (int)sb->len + len;
  ASSERT(sb->len == strlen(sb->buf));
//   print_strbuf(sb);
  ENSURES(is_strbuf(sb));
}
void strbuf_addstr(strbuf *sb, char *str){
  REQUIRES(is_strbuf(sb));
  REQUIRES(str != NULL);
  strbuf_add(sb, str, strlen(str));
  ENSURES(is_strbuf(sb));
}

char *strbuf_free(strbuf *sb){
  REQUIRES(is_strbuf(sb));
  char *freed = sb->buf;
  free(sb);
  return freed;
}