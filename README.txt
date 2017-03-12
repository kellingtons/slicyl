Things are still pretty all over the place but try out the program for yourself:

1. On a Linux machine, cd to whereever/slicyl/src and type make

2. Type ./slicyl ../stl/liver.stl 0 0.1 100

You can replace liver.stl with other stls in the stl directory.

The first number is the smallest slicyl radius. The second is the thickness between slicyls, and the third is the maximum slicyl radius.

This program outputs a slicyl_out.marks file that is a rolled out slice by slice view of the sliced model. You have to use GIV to view it: http://giv.sourceforge.net/giv/

Thanks
kel