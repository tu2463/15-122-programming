15-122 Principles of Imperative Computation
Lights Out

==========================================================

Files you won't modify:
   lib/contracts.h     - Contracts for C
   lib/xalloc.h        - NULL-checking allocation
   lib/boardutil.{c,h} - Useful Lights-Out relevant functions
   lib/hdict.h         - Dictionaries, implemented as hash tables
   lib/queue.h         - Queues
   lib/bitvector.h     - Interface to bit vectors
   lib/*.o             - machine-specific object files
   Makefile            - Utility for building this project

Files you may extend:
   board-ht.h          - Interface to hashtables keyed by boards
   bitvector-test.c    - Your unit tests for bitvector.c (Optional)
   board-ht-test.c     - Your unit tests for board-ht.c (Optional)

Files that don't exist yet:
   bitvector.c         - Bit vector implementation
   board-ht.c          - Bit vector hashtable wrapper implementation
   lightsout.c         - Lights out implementation (needs a main() function)

==========================================================

Compiling your bitvector unit tests //I added sth for testing for 2015 spring final practice exam questions on 2023/4/29. They've been commented out.
   % make bitvector-test
   % ./bitvector-test

Compiling your board-ht unit tests
   % make board-ht-test
   % ./board-ht-test

Compiling and running your lights out solver (with -DDEBUG)
   % make
   % ./lightsout-d boards/2x2-0.txt

./lightsout-d boards/2x2-0.txt | loplayer boards/2x2-0.txt
./lightsout-d boards/2x2-
./lightsout-d boards/3x2-00.txt | loplayer boards/3x2-00.txt
./lightsout-d boards/3x2-02.txt | loplayer boards/3x2-02.txt
./lightsout-d boards/3x2-1F.txt | loplayer boards/3x2-1F.txt
./lightsout-d boards/3x2-0F.txt | loplayer boards/3x2-0F.txt
./lightsout-d boards/3x2-22.txt | loplayer boards/3x2-22.txt
./lightsout-d boards/3x2-30.txt | loplayer boards/3x2-30.txt
./lightsout-d boards/3x2-31.txt | loplayer boards/3x2-31.txt
./lightsout-d boards/3x2-34.txt | loplayer boards/3x2-34.txt

./lightsout boards/4x4-no-solution.txt | loplayer boards/4x4-no-solution.txt
./lightsout boards/4x4-other-7.txt | loplayer boards/4x4-other-7.txt
./lightsout boards/4x4-other-no.txt | loplayer boards/4x4-other-no.txt
./lightsout boards/4x4-solvable-in-3.txt | loplayer boards/4x4-solvable-in-3.txt
./lightsout boards/4x4-solvable-in-7.txt | loplayer boards/4x4-solvable-in-7.txt

./lightsout boards/5x6-0.txt | loplayer boards/5x6-0.txt
./lightsout boards/5x6-1.txt | loplayer boards/5x6-1.txt
./lightsout boards/5x6-2.txt | loplayer boards/5x6-2.txt //a bit slow
./lightsout boards/5x6-3.txt | loplayer boards/5x6-3.txt
./lightsout boards/5x6-4.txt | loplayer boards/5x6-4.txt //6step a bit slow
./lightsout boards/5x5-0.txt | loplayer boards/5x5-0.txt //4step
./lightsout boards/5x5-1.txt | loplayer boards/5x5-1.txt //5step
./lightsout boards/5x5-2.txt | loplayer boards/5x5-2.txt //6step

O##O
#OO#
O#OO
O###

O#OO
###O r=1,c=2
O##O
O###

OOOO
OOOO r=1,c=1
OO#O
O###
then r=3,c=2->solved

5x5
OO#OO
O###O
OO#OO
OOOOO
OOOOO


Compiling and running your lights out solver (without -DDEBUG)
   % make
   % ./lightsout boards/2x2-0.txt

==========================================================

Submitting from the command line on andrew:
   % autolab122 handin lightsout lightsout.c board-ht.h board-ht.c bitvector.c
then display autolab's feedback by running:
   % autolab122 feedback

Creating a tarball to submit with autolab.andrew.cmu.edu web interface:
   % tar -czvf handin.tgz lightsout.c board-ht.h board-ht.c bitvector.c
