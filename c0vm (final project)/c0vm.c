/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "lib/xalloc.h"
#include "lib/stack.h"
#include "lib/contracts.h"
#include "lib/c0v_stack.h"
#include "lib/c0vm.h"
#include "lib/c0vm_c0ffi.h"
#include "lib/c0vm_abort.h"

/* call stack frames */
typedef struct frame_info frame;
struct frame_info {
  c0v_stack_t  S;   /* Operand stack of C0 values */
  ubyte       *P;   /* Function body */
  size_t       pc;  /* Program counter */
  c0_value    *V;   /* The local variables */
};

void push_int(c0v_stack_t S, int32_t i){
  c0v_push(S, int2val(i));
}

void push_ptr(c0v_stack_t S, void *a){
  c0v_push(S, ptr2val(a));
}

void push_tptr(c0v_stack_t S, void *a, uint16_t tag){
  c0v_push(S, tagged_ptr2val(a, tag));
}

int32_t pop_int(c0v_stack_t S){
  int32_t result = val2int(c0v_pop(S));
  return result;
}

void *pop_ptr(c0v_stack_t S){
  void *result = val2ptr(c0v_pop(S));
  return result;
}

c0_tagged_ptr *pop_tptr(c0v_stack_t S){
  void *result = val2tagged_ptr(c0v_pop(S));
  return result;
}

int execute(struct bc0_file *bc0) {
  REQUIRES(bc0 != NULL);

  // char w = 20;
  // short x = 20;
  // int y = 20;
  // long z = 0xFF;
  // short q = z<<64;
  // (void )w;
  // (void )x;
  // (void )y;
  // (void )z;
  // (void )q;

  // printf("q=%d\n",(int)(y<<20));
  
  /* Variables */
  c0v_stack_t S = c0v_stack_new(); /* Operand stack of C0 values */
  ubyte *P = bc0->function_pool->code; /* Array of bytes that make up the current function */
  size_t pc = 0;     /* Current location within the current byte array P */
  c0_value *V = xcalloc(bc0->function_pool->num_vars, sizeof(c0_value));   /* Local variables (you won't need this till Task 2) */
  // (void) V;      // silences compilation errors about V being currently unused

  /* The call stack, a generic stack that should contain pointers to frames */
  /* You won't need this until you implement functions. */
  gstack_t callStack = stack_new();

  while (true) {

    #ifdef DEBUG
        /* You can add extra debugging information here */
        fprintf(stderr, "Opcode %x -- Stack size: %zu -- PC: %zu\n",
                P[pc], c0v_stack_size(S), pc);
    #endif

    switch (P[pc]) {

    /* Additional stack operation: */

    case POP: {
      pc++;
      c0v_pop(S);
      break;
    }

    case DUP: {
      pc++;
      c0_value v = c0v_pop(S);
      c0v_push(S,v);
      c0v_push(S,v);
      break;
    }

    case SWAP:{
      pc ++;
      c0_value v1 = c0v_pop(S);
      c0_value v2 = c0v_pop(S);
      c0v_push(S,v1);
      c0v_push(S,v2);
      break;
    }

    /* Returning from a function.
     * This currently has a memory leak! You will need to make a slight
     * change for the initial tasks to avoid leaking memory.  You will
     * need to revise it further when you write INVOKESTATIC. */

    case RETURN: {
      c0_value retval = c0v_pop(S);
      // if (!c0v_stack_empty(S)) c0v_stack_print(S);
      // Another way to print only in DEBUG mode
      ASSERT(c0v_stack_empty(S));

      c0v_stack_free(S);
      free(V);

      if (!stack_empty(callStack)){
        frame *f = (frame *)pop(callStack);
        S = f->S;
        P = f->P;
        pc = f->pc;
        V = f->V;
        c0v_push(S, retval);
        free(f);
        break;
      }
      else {
        // Free everything before returning from the execute function!
        stack_free(callStack, NULL);
        int result = val2int(retval);
        IF_DEBUG(fprintf(stderr, "Returning %d from execute()\n", result));
        return result;
      }
    }


    /* Arithmetic and Logical operations */

    case IADD:{
      // c0v_stack_print(S);
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result;
      // result = x + y;
      if (y > INT_MAX - x){
        result = (int32_t)((uint32_t)x + (uint32_t)y);
      }
      else result = x + y;
      push_int(S, result);
      // IF_DEBUG(fprintf(stderr,"--iadd-- x=%d,y=%d,res=%d\n",x,y,result));
      // c0v_stack_print(S);
      pc ++;
      break;
    }

    case ISUB:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result;
      // printf("--sub-- x=%d y=%d\n",x,y);
      // result = x - y;
      if (x < y + INT_MIN){
        // printf("--sub-- 1");
        result = (int32_t)((uint32_t)x - (uint32_t)y); //??
      }
      else {
        // printf("--sub-- 2");
        result = x - y;}
      push_int(S, result);
      break;
    }

    case IMUL:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result; 
      if (x == 0 || y == 0) result = 0;
      else if (x > INT_MAX / y){
        result = (int32_t)((uint32_t)x * (uint32_t)y); //??
      }
      else result = x * y;
      push_int(S, result);
      // IF_DEBUG(fprintf(stderr,"--imul-- x=%d,y=%d,res=%d\n",x,y,result));
      break;
    }

    case IDIV:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result; 
      if (y == 0) c0_arith_error("Invalid divisor");
      if (x == INT_MIN && y == -1) c0_arith_error("division overflows");
      result = x / y;
      push_int(S, result);
      break;
    }

    case IREM:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result; 
      if (y == 0) c0_arith_error("Invalid divisor");
      if (x == INT_MIN && y == -1) c0_arith_error("division overflows");
      result = x % y;
      push_int(S, result);
      break;
    }

    case IAND:{
      pc ++;
      int32_t x = pop_int(S);
      int32_t y = pop_int(S);
      int32_t result;
      result = x & y;
      push_int(S, result);
      break;
    }

    case IOR:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result;
      result = x | y;
      push_int(S, result);
      break;
    }

    case IXOR:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result;
      result = x ^ y;
      push_int(S, result);
      break;
    }

    case ISHR:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result;
      char *error_text = "Invalid shift operand";
      if (y < 0 || y >= 32) c0_arith_error(error_text);
      result = x >> y;
      // printf("--ishr-- x=%d, y=%d, result=%d\n",x,y,result);
      push_int(S, result);
      break;
    }

    case ISHL:{
      pc ++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      int32_t result;
      char *error_text = "Invalid shift operand";
      if (y < 0 || y >= 32) c0_arith_error(error_text);
      result = x << y;
      // printf("--ishl-- x=%d, y=%d, result=%d\n",x,y,result);
      push_int(S, result);
      break;
    }


    /* Pushing constants */

    case BIPUSH:{
      // c0v_stack_print(S);
      char v = (char)(P[pc + 1]);
      // IF_DEBUG(fprintf(stderr, "--bipush-- pushed %d\n",P[pc + 1]));
      push_int(S, (int)v);
      // c0v_stack_print(S);
      pc += 2;
      break;
    }

    case ILDC:{
      ubyte c1 = (P[pc + 1]); 
      ubyte c2 = (P[pc + 2]);
      int x = (int)(bc0->int_pool[((c1<<8)|c2)]);
      // IF_DEBUG(fprintf(stderr, "--ildc-- c1=%d, c2=%d, index=%d, x=%d\n",c1,c2,(c1<<8)|c2,x));
      push_int(S, x);
      pc += 3;
      break;
    }

    case ALDC:{
      ubyte c1 = (P[pc + 1]);
      ubyte c2 = (P[pc + 2]);
      // char *a = &((bc0->string_pool)[(unsigned int)((c1<<8)|c2)]);
      char *a = &((bc0->string_pool)[(c1<<8)|c2]);//should i keep the unsigned int cast//??
      push_ptr(S, (void *)a);
      pc += 3;
      break;
    }

    case ACONST_NULL:{
      pc ++;
      push_ptr(S, NULL);
      break;
    }


    /* Operations on local variables */

    case VLOAD:{
      ubyte i = (P[pc + 1]);
      c0_value v = V[i];
      c0v_push(S, v);
      // IF_DEBUG(c0v_stack_print(S));
      pc += 2;
      break;
    }

    case VSTORE:{
      ubyte i = (P[pc + 1]);
      c0_value v = c0v_pop(S);
      V[i] = v;
      pc += 2;
      break;
    }


    /* Assertions and errors */

    case ATHROW:{
      char *a = (char *)pop_ptr(S);
      c0_user_error(a);
      pc ++;
      break;
    }

    case ASSERT:{
      // IF_DEBUG(fprintf(stderr,"--assert-- called\n"));
      char *a = (char *)pop_ptr(S);
      int32_t x = pop_int(S);
      if (x == 0) c0_assertion_failure(a);
      pc ++;
      break;
    }


    /* Control flow operations */

    case NOP:{
      pc ++;
      break;
    }

    case IF_CMPEQ:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if (val_equal(v1, v2)) pc += (size_t)((int16_t)((o1<<8)|o2));
      else pc += 3;
      break;
    }

    case IF_CMPNE:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if (!val_equal(v1, v2)) pc += (size_t)((int16_t)((o1<<8)|o2));
      else pc += 3;
      break;
    }

    case IF_ICMPLT:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x < y) pc += (size_t)((int16_t)((o1<<8)|o2));
      else pc += 3;
      break;
    }

    case IF_ICMPGE:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x >= y) pc += (size_t)((int16_t)((o1<<8)|o2));
      else pc += 3;
      break;
    }

    case IF_ICMPGT:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x > y) pc += (size_t)((int16_t)((o1<<8)|o2));
      else pc += 3;
      break;
    }

    case IF_ICMPLE:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      IF_DEBUG(fprintf(stderr, "--if_icmple-- x=%d,y=%d\n",x,y));
      if (x <= y) pc += (size_t)((int16_t)((o1<<8)|o2));
      else pc += 3;
      break;
    }

    case GOTO:{
      ubyte o1 = P[pc + 1]; 
      ubyte o2 = P[pc + 2];
      pc += (size_t)((int16_t)((o1<<8)|o2));
      IF_DEBUG(fprintf(stderr, "--goto-- pc2=%d, (o1<<8)|o2=%d\n",
                       (int)pc,((o1<<8)|o2)));
      break;
    }

    /* Function call operations: */

    case INVOKESTATIC:{    
      ubyte c1 = P[pc + 1]; 
      ubyte c2 = P[pc + 2];
      pc += 3;

      frame *f = xcalloc(sizeof(frame), 1); //is this part right//?
      f->V = V;
 
      uint8_t num_args = (&(bc0->function_pool[((c1<<8)|c2)]))->num_args;
      uint8_t num_vars = (&(bc0->function_pool[((c1<<8)|c2)]))->num_vars;
      // V = xcalloc(bc0->function_pool->num_vars, sizeof(c0_value)); 
      V = xcalloc(num_vars, sizeof(c0_value)); 
      for (int i = num_args - 1; i >= 0; i --){
        V[i] = c0v_pop(S);
      }

      f->S = S;
      f->P = P;
      f->pc = pc;
      push(callStack, (void *)f); //or &f//??

      S = c0v_stack_new(); 
      P = (&(bc0->function_pool[((c1<<8)|c2)]))->code;
      pc = 0;   
      break;
    }

    case INVOKENATIVE:{    
      ubyte c1 = P[pc + 1]; 
      ubyte c2 = P[pc + 2];
      pc += 3;

      uint8_t num_args = (&(bc0->native_pool[((c1<<8)|c2)]))->num_args;
      uint16_t idx = (&(bc0->native_pool[((c1<<8)|c2)]))->function_table_index;

      c0_value *A = xcalloc(sizeof(c0_value), num_args);
      for (int i = num_args - 1; i >= 0; i --){
        A[i] = c0v_pop(S);
      }

      // native_fn *g = *(native_function_table + idx); //?? why both this and no derefence can work?
      c0_value retval = (*native_function_table[idx])(A);
      c0v_push(S, retval);
      free(A);
      break;
    }

    /* Memory allocation and access operations: */

    case NEW:{    
      ubyte s = P[pc + 1]; 
      pc += 2;
      push_ptr(S, xcalloc((size_t)s, 1));
      break;
    }

    case IMLOAD:{
      // c0v_stack_print(S);
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      push_int(S, *(int32_t *)a);
      // c0v_stack_print(S);
      pc ++; 
      break;
    }

    case IMSTORE:{
      // c0v_stack_print(S);
      int32_t x = pop_int(S);
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      *(int32_t *)a = x;
      pc ++;
      break;
    }

    case AMLOAD:{
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      push_ptr(S, *(int32_t **)a);
      pc ++;
      break;
    }

    case AMSTORE:{
      void *b = (void *)pop_ptr(S);
      void **a = pop_ptr(S);//1* or 2*//?? //!!
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      *a = b;
      pc ++;
      break;
    }

    case CMLOAD:{
      // c0v_stack_print(S);
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");

      //!! a is on stack, but the thing a points to is not on stack, 
      // therefore not neccesarily a int32_t. in this case, a is pointer to char
      push_int(S, (*(char *)a));
      pc ++; 
      break;
    }

    case CMSTORE:{
      // c0v_stack_print(S);
      int32_t x = pop_int(S);
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      *(char *)a = x & 0x7F;
      pc ++;
      break;
    }

    case AADDF:{
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      ubyte f = P[pc + 1]; 
      push_ptr(S, (void *)(((char *)a) + f));//!!
      pc += 2;
      break;
    }


    /* Array operations: */

    case NEWARRAY:{
      int32_t n= pop_int(S);
      if (n < 0) c0_memory_error("Invalid length\n");
      ubyte s = P[pc + 1]; 
      c0_array *a = xcalloc(sizeof(c0_array), 1);
      a->count = n;
      a->elt_size = s;
      // IF_DEBUG(fprintf(stderr, "--newarray-- n=%d, s=%d, sizeof(int)=%d, sizeof(s)=%d\n",n,s,(int)sizeof(int),(int)sizeof(s)));
      a->elems = (void *)(xcalloc(s, n));
      push_ptr(S, a);
      pc += 2;
      // c0v_stack_print(S);
      break;
    }

    case ARRAYLENGTH:{
      void *a = pop_ptr(S);
      if (a == NULL) {
        // printf("--arraylen-- this array is NULL\n");
        push_int(S, 0);
        pc ++;
        break;
      }
      // printf("--arraylen-- this array is NOT NULL\n");
      uint32_t count = ((c0_array *)a)->count;
      push_int(S, count);
      pc ++;
      break;
    }

    case AADDS:{
      int32_t i= pop_int(S);
      void *a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Attempt to dereference NULL pointers\n");
      if (i < 0 || (uint32_t)i >= ((c0_array *)a)->count) c0_memory_error("Invalid array offset\n");
      c0_array *A = (c0_array *)a;
      uint32_t s = A->elt_size;
      // IF_DEBUG(fprintf(stderr, "--aadds-- s=%d, i=%d\n",s,i));
      push_ptr(S, (char *)(A->elems) + s * i); //!! should cast to char, same reason as before
      pc ++;
      break;
    }

    /* BONUS -- C1 operations */

    case CHECKTAG:{
      ubyte c1 = P[pc + 1]; 
      ubyte c2 = P[pc + 2];
      pc += 3;

      c0_tagged_ptr *a = pop_tptr(S);
      if (a != NULL && a->tag != ((c1<<8)|c2)) c0_memory_error("Invalid generic pointer tag");
      if (a == NULL) push_ptr(S, NULL);
      else push_ptr(S, a->p); //??
      break;
    }

    case HASTAG:{
      ubyte c1 = P[pc + 1]; 
      ubyte c2 = P[pc + 2];
      pc += 3;
      int32_t x;

      c0_tagged_ptr *a = pop_tptr(S);
      if (a != NULL && a->tag != ((c1<<8)|c2)) x = 0;
      else x = 1;
      push_int(S, x);
      break;
    }

    case ADDTAG:{
      ubyte c1 = P[pc + 1]; 
      ubyte c2 = P[pc + 2];
      pc += 3;

      void *a = pop_ptr(S);
      push_tptr(S, a, (uint16_t)((c1<<8)|c2));
      break;
    }

    case ADDROF_STATIC:{
      // c0v_stack_print(S);
      ubyte c1 = (P[pc + 1]); 
      ubyte c2 = (P[pc + 2]);
      uint16_t index = (uint16_t)((c1<<8)|c2);
      void *a = create_funptr(false, index);
      // IF_DEBUG(fprintf(stderr, "--ildc-- c1=%d, c2=%d, index=%d, x=%d\n",c1,c2,(c1<<8)|c2,x));
      push_ptr(S, a);
      pc += 3;
      // c0v_stack_print(S);
      break;
    }

    case ADDROF_NATIVE:{
      ubyte c1 = (P[pc + 1]); 
      ubyte c2 = (P[pc + 2]);
      uint16_t index = (uint16_t)((c1<<8)|c2);
      void *a = create_funptr(true, index);
      // IF_DEBUG(fprintf(stderr, "--ildc-- c1=%d, c2=%d, index=%d, x=%d\n",c1,c2,(c1<<8)|c2,x));
      push_ptr(S, a);
      pc += 3;
      break;
    }

    case INVOKEDYNAMIC:{    
      // c0v_stack_print(S);
      void *a = pop_ptr(S);
      uint16_t pool_index = funptr2index(a);
      if (is_native_funptr(a)){
        uint16_t num_args = (&(bc0->native_pool[pool_index]))->num_args;
        uint16_t table_index = (&(bc0->native_pool[pool_index]))->function_table_index;
        c0_value *A = xcalloc(sizeof(c0_value), num_args);
        for (int i = num_args - 1; i >= 0; i --){
          A[i] = c0v_pop(S);
        }
        c0_value retval = (*native_function_table[table_index])(A);
        c0v_push(S, retval);
        free(A);
        pc ++;
        break;
      }
      else{
        pc ++;
        frame *f = xcalloc(sizeof(frame), 1); //is this part right//?
        f->V = V;
  
        uint8_t num_args = (&(bc0->function_pool[pool_index]))->num_args;
        uint8_t num_vars = (&(bc0->function_pool[pool_index]))->num_vars;
        V = xcalloc(num_vars, sizeof(c0_value)); 
        for (int i = num_args - 1; i >= 0; i --){
          V[i] = c0v_pop(S);
        }

        f->S = S;
        f->P = P;
        f->pc = pc;
        push(callStack, (void *)f);
  
        S = c0v_stack_new(); 
        P = (&(bc0->function_pool[pool_index]))->code;
        pc = 0;   
        break;
      }
      break;
    }

    default:
      fprintf(stderr, "invalid opcode: 0x%02x\n", P[pc]);
      abort();
    }
  }

  /* cannot get here from infinite loop */
  assert(false);
}
