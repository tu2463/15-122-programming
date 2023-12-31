15-122 Principles of Imperative Computation
Images

==========================================================

Files you will modify:
   imageutil.c0          - Skeleton image helper functions
   images-test.c0        - Test your manipulations (optional)

Files you won't modify:
   pixel.o0              - Pixel library
   remove-green.c0       - You can modify this to respect the pixel/imageutil
                           interfaces if you'd like
   remove-green-main.c0  - Runs remove_green()
   remove-green-test.c0  - Some unit tests for remove_green()
   rotate-main.c0        - Runs rotate()
   maskblur-main.c0      - Runs apply_mask() to do blurring
   maskedge-main.c0      - Runs apply_mask() to do edge detection

Files that don't exist yet:
   rotate.c0             - Task 3
   mask.c0               - Task 4

Data:
   masks/mask-blur-slightly.txt   - 3x3 blur mask
   masks/mask-blur.txt            - 5x5 blur mask
   masks/mask-blur-more.txt       - 5x5 blur mask
   masks/mask-sobelX.txt          - 3x3 Sobel filter
   masks/mask-sobelY.txt          - 3x3 Sobel filter
   img/g5.png
   img/carnegie.png
   img/scs.png
   img/cmu.png
   img/tinytestpattern.png        - 3x2 image, may be good for testing
   img/g5-remove-green.png        - result of remove_green()
   img/g5-remove-green-bug.png    - result of a buggy remove_green()
   img/rotate-carnegie.png        - result of rotate()
   img/mask-gaussian-cmu.png      - result of maskblur with mask-blur.txt
   img/mask-edge-cmu.png          - result of maskedge

==========================================================

Using the imagediff utility: you have been given the result of running
remove_green on both a correct and a buggy implementation of
g5.png. Running the following command shows that the two images differ
by 600 pixels.

   % imagediff -i img/g5-remove-green.png -j img/g5-remove-green-bug.png
   Loaded image img/g5-remove-green.png. Dimensions are 800 by 600.
   Loaded image img/g5-remove-green-bug.png. Dimensions are 800 by 600.
   Number of pixels with different colors: 600 out of 480000.
   0

If you want to visually see where these 600 different pixels actually
are, you can get imagediff to print out a new image that highlights
the places where differences occured:

   % imagediff -i img/g5-remove-green.png -j img/g5-remove-green-bug.png -o diff.png
   % display diff.png &

In addition to writing test cases, this can be a useful way of
debugging your code.

==========================================================

Displaying the pixel library interface
   % cc0 -i pixel.o0

Compiling remove-green and test cases:
   % cc0 -d -W -w -o remove-green-test pixel.o0 imageutil.c0 remove-green.c0 remove-green-test.c0
   % ./remove-green-test

   % cc0 -d -W -w -o remove-green pixel.o0 imageutil.c0 remove-green.c0 remove-green-main.c0
   % ./remove-green -i img/g5.png -o img/g5-my-output.png
       (This creates the output file img/g5-my-output.png. If you
        left off the "-o img/g5-my-output.png" part, the output
        file would be called img/g5_remove-green.png.)

Compiling your unit tests. You can omit rotate.c0 or mask.c0 if you haven't written test cases for rotate() or mask(), respectively:
   % cc0 -d -W -w -o images-test pixel.o0 imageutil.c0 rotate.c0 mask.c0 images-test.c0
   % ./images-test

Compiling rotate:
   % cc0 -d -W -w -o rotate pixel.o0 imageutil.c0 rotate.c0 rotate-main.c0
   % ./rotate -i img/carnegie.png -o img/carnegie-my-output.png
       (This creates the output file img/carnegie-my-output.png. If
        you left off the "-o img/carnegie-my-output.png" part, the
        output file would be called img/carnegie_rotate.png.)
   % imagediff -i img/rotate-carnegie.png -j img/carnegie-my-output.png -o diff.png

Compiling mask:
   % cc0 -d -W -w -o maskblur pixel.o0 imageutil.c0 mask.c0 maskblur-main.c0
   % ./maskblur -i img/cmu.png -o img/cmu-my-maskblur.png -m masks/mask-blur.txt
       (This creates the output file img/cmu-my-maskblur.png. If you
        left off the "-o img/cmu-my-maskblur.png" part, the output
        file would be called img/cmu_maskblur.png.)
   % imagediff -i img/mask-gaussian-cmu.png -j img/cmu-my-maskblur.png -o diff.png

   % cc0 -d -W -w -o maskedge pixel.o0 imageutil.c0 mask.c0 maskedge-main.c0
   % ./maskedge -i img/cmu.png -o img/cmu-my-maskedge.png
       (This creates the output file img/cmu-my-maskedge.png. If you
        left off the "-o img/cmu-my-maskedge.png" part, the output
        file would be called img/cmu_maskedge.png.)
   % imagediff -i img/mask-edge-cmu.png -j img/cmu-my-maskedge.png -o diff.png

Compiling manipulate:
   % cc0 -d -W -w -o manipulate pixel.o0 imageutil.c0 rotate.c0 mask.c0 manipulate.c0 manipulate-main.c0
   % ./manipulate -i img/g5.png -o img/g5-my-manip.png

==========================================================

Submitting from the command line on andrew:
   % autolab122 handin images imageutil.c0 rotate.c0 mask.c0 images-test.c0
or
   % autolab122 handin images imageutil.c0 rotate.c0 mask.c0 images-test.c0 manipulate.c0
then display autolab's feedback by running:
   % autolab122 feedback

Creating a tarball to submit with autolab.cs.cmu.edu web interface:
   % tar -czvf handin.tgz imageutil.c0 rotate.c0 mask.c0 images-test.c0
or
   % tar -czvf handin.tgz imageutil.c0 rotate.c0 mask.c0 images-test.c0 manipulate.c0
