15-122 Principles of Imperative Computation
The C0VM

==========================================================

Useful information:
   c0vm-ref.txt           - Bytecode implementation reference

Files you won't modify:
   lib/contracts.h     - Contracts for C
   lib/xalloc.h        - NULL-checking allocation
   lib/stack.h         - Stacks (with O(1) stack_size!)
   lib/c0v_stack.h     - C0 Value Stacks (with O(1) stack_size!)
   lib/c0vm_abort.h    - Functions for your C0VM to report runtime errors
   lib/c0vm_c0ffi.h    - Interface to the library functions
   lib/c0vm.h          - Header file, contains C0VM types and functions
   lib/read_program.h  - Reading in C0 bytecode from a file
   lib/*.o             - machine-specific object files
   c0vm_main.c         - Main function - loads bytecode, handles return

Files you will modify:
   c0vm.c              - Executes C0 bytecode

==========================================================
valgrind --track-origins=yes

Compiling a C0 program to create a .bc0 file (produces tests/iadd.bc0):
   % cc0 -b tests/iadd.c0

cc0 -b tests/arrays.c0
cc0 -x tests/arrays.c0
./c0vmd tests/arrays.bc0
./c0vm tests/arrays.bc0

cc0 -b tests/chararrays.c0
cc0 -x tests/chararrays.c0
./c0vmd tests/chararrays.bc0
./c0vm tests/chararrays.bc0

cc0 -x tests/iadd.c0
./c0vmd tests/iadd.bc0
./c0vm tests/iadd.bc0

cc0 -b tests/ishr.c0
cc0 -x tests/ishr.c0
./c0vmd tests/ishr.bc0

cc0 -b tests/dsquared.c0
cc0 -x tests/dsquared.c0
./c0vmd tests/dsquared.bc0
./c0vm tests/dsquared.bc0

cc0 -b tests/hellosir.c0
cc0 -x tests/hellosir.c0
./c0vmd tests/hellosir.bc0
./c0vm tests/hellosir.bc0

cc0 -b tests/isqrt.c0
cc0 -x tests/isqrt.c0
./c0vmd tests/isqrt.bc0

cc0 -b tests/mid.c0
cc0 -x tests/mid.c0
./c0vmd tests/mid.bc0

cc0 -b tests/test_arith_and_logic.c0
cc0 -x tests/test_arith_and_logic.c0
./c0vmd tests/test_arith_and_logic.bc0

cc0 -b tests/test_t2.c0
cc0 -x tests/test_t2.c0
./c0vmd tests/test_t2.bc0
./c0vm tests/test_t2.bc0

cc0 -b tests/test_t3.c0
cc0 -x tests/test_t3.c0
./c0vmd tests/test_t3.bc0

cc0 -b tests/test_t4.c0
cc0 -x tests/test_t4.c0
./c0vmd tests/test_t4.bc0
./c0vm tests/test_t4.bc0

cc0 -b -d tests/test_t5.c0
cc0 -x -d tests/test_t5.c0
./c0vmd tests/test_t5.bc0
./c0vm tests/test_t5.bc0

cc0 -b -d tests/test_t5_2.c0
cc0 -x -d tests/test_t5_2.c0
./c0vmd tests/test_t5_2.bc0
./c0vm tests/test_t5_2.bc0

cc0 -b -d tests/test_t6.c1
cc0 -x -d tests/test_t6.c1
./c0vmd tests/test_t6.bc1
./c0vm tests/test_t6.bc1

cd ~/private/15122/pg/c0vm
~/private/15122/pg/c0vm/c0vm
~/private/15122/pg/c0vm/c0vmd
################################### lec code ###################################
### 12-hashing ###
cd ~/private/15122/"lecture code"/12-hashing
cc0 -b -dx stack.c1 stack-test.c1
~/private/15122/pg/c0vm/c0vm stack-test.bc1


### 14-generic ###
cd ~/private/15122/"lecture code"/14-generic/voidstar
cc0 -b -dx hdict.c1 produce.c1 produce-test.c1
~/private/15122/pg/c0vm/c0vm produce-test.bc1

cc0 -b -dx lib/*.c0 hdict.c1 words.c1 words-test.c1
~/private/15122/pg/c0vm/c0vm words-test.bc1

cd ~/private/15122/"lecture code"/14-generic/generic
cc0 -b -dx lib/*.c0 hdict.c1 produce.c1 words.c1 combined-test.c1
~/private/15122/pg/c0vm/c0vm combined-test.bc1

cd ~/private/15122/"lecture code"/14-generic/generic-with-print
cc0 -b -dx lib/*.c0 hdict.c1 produce.c1 words.c1 combined-test.c1
~/private/15122/pg/c0vm/c0vm combined-test.bc1


################################### pg ###################################
### bloom ###
cd ~/private/15122/pg/bloom
cc0 -b -d -x bloom-worst.c0 bloom-test.c0
~/private/15122/pg/c0vm/c0vm bloom-test.bc0

cc0 -b -d -W -w -x bloom-expensive.c0 bloom-test.c0

cc0 -b -d -W -w -x bloom2.c0 test-pack.c0
~/private/15122/pg/c0vm/c0vm test-pack.bc0

cc0 -b -d -W -w -x bloom2.c0 pwd.c0 test-pwd.c0
~/private/15122/pg/c0vm/c0vm test-pwd.bc0

### tbuf ###
cd ~/private/15122/pg/tbuf
cc0 -b -w tbuf.c0 tbuf-test.c0 test-main.c0
~/private/15122/pg/c0vm/c0vm test-main.bc0

cc0 -b -W -w tbuf.c0 editor.c0 editor-test.c0 test-main.c0
~/private/15122/pg/c0vm/c0vm test-main.bc0

cc0 -b -W -w tbuf.c0 editor.c0 lovas-E0.c0
~/private/15122/pg/c0vm/c0vm lovas-E0.bc0

### clac exp ###
~/private/15122/pg/exp
// need clac input
cc0 -b -d -W -w lib/*.o0 clac.c0 clac-main.c0
~/private/15122/pg/c0vm/c0vm clac-main.bc0

cc0 -b -d -W -w lib/*.o0 clac.c0 clac-test.c0
~/private/15122/pg/c0vm/c0vm clac-test.bc0

cc0 -b -d -W -w lib/*.o0 dict.c0 dict-test.c0
~/private/15122/pg/c0vm/c0vm dict-test.bc0

cc0 -b -d -W -w lib/*.o0 dict.c0 parse.c0 is_precstack-test.c0
~/private/15122/pg/c0vm/c0vm is_precstack-test.bc0

cc0 -b -d -W -w lib/*.o0 clac.c0 dict.c0 parse.c0 exp-test.c0
~/private/15122/pg/c0vm/c0vm exp-test.bc0

### doslingos ###
cd ~/private/15122/doslingos
cc0 -b -d -W -w lib/*.o0 doslingos.c0 doslingos-test.c0
~/private/15122/pg/c0vm/c0vm doslingos-test.bc0

// should have assertion failure
cc0 -b -d -w lib/*.o0 doslingos-awful.c0 doslingos-test.c0
~/private/15122/pg/c0vm/c0vm doslingos-test.bc0

cc0 -b -W -w lib/*.o0 doslingos.c0 analysis.c0
~/private/15122/pg/c0vm/c0vm analysis.bc0
texts/small_vocab.txt texts/scott_tweet.txt
texts/news_vocab.txt texts/shakespeare.txt
texts/small_vocab.txt texts/text.txt

### images ###
~/private/15122/pg/images_final
cc0 -b -d -W -w pixel.o0 imageutil.c0 rotate.c0 rotate-main.c0
~/private/15122/pg/c0vm/c0vm rotate-main.bc0 -i img/carnegie.png -o img/carnegie-my-output.png
imagediff -i img/rotate-carnegie.png -j img/carnegie-my-output.png -o diff.png

cc0 -b -W -w pixel.o0 imageutil.c0 mask.c0 maskblur-main.c0
~/private/15122/pg/c0vm/c0vm maskblur-main.bc0 -i img/cmu.png -o img/cmu-my-maskblur.png -m masks/mask-blur.txt
~/private/15122/pg/c0vm/c0vmd maskblur-main.bc0 -i img/cmu.png -o img/cmu-my-maskblur.png -m masks/mask-blur.txt
imagediff -i img/mask-gaussian-cmu.png -j img/cmu-my-maskblur.png -o diff.png

cc0 -b -W -w pixel.o0 imageutil.c0 mask.c0 maskedge-main.c0
~/private/15122/pg/c0vm/c0vm maskedge-main.bc0 -i img/cmu.png -o img/cmu-my-maskedge.png
imagediff -i img/mask-edge-cmu.png -j img/cmu-my-maskedge.png -o diff.png

### pixel ###
~/private/15122/pg/pixels_final
cc0 -b -d -W pixel.o0 quantize.c0 quantize-test.c0
~/private/15122/pg/c0vm/c0vm quantize-test.bc0

cc0 -b -d -W pixel-int.c0 pixel-test.c0
~/private/15122/pg/c0vm/c0vm pixel-test.bc0

cc0 -b -d -W pixel.o0 multireturn.c0 multireturn-test.c0
~/private/15122/pg/c0vm/c0vm multireturn-test.bc0

Compiling and running your C0VM implementation (with -DDEBUG)
(Must compile iadd.c0 to iadd.bc0 first)
   % make
   % ./c0vmd tests/iadd.bc0

Compiling and running your C0VM implementation (without -DDEBUG)
(Must compile iadd.c0 to iadd.bc0 first)
   % make
   % ./c0vm tests/iadd.bc0

==========================================================

Submitting from the command line on andrew (CHECKPOINT):
   % autolab122 handin c0vmcheck c0vm.c
then display autolab's feedback by running:
   % autolab122 feedback c0vmcheck

Submitting from the command line on andrew (FINAL):
   % autolab122 handin c0vm c0vm.c
then display autolab's feedback by running:
   % autolab122 feedback

Creating a tarball to submit with autolab.autolab.cmu.edu web interface:
   % tar -czvf handin.tgz c0vm.c
