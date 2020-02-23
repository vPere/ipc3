# ipclab3
The objective of this lab is to work with semaphores

On the first section you have to program two codes step1.c and step2.c. the first one to be executed must be step1.



Each process/program must ask for a number and a word, and copy the word (as many times as the number) on a file named log.txt, till the user enter a number equal to 0. The writting on file must be: words from step1, then words from step2, then words from step1....



If step1 execution is:

1 hello

2 bye

3 common

0

and step2 execution is

2 hola

3 adios

1 como

2 no

0

the output file must be:

1: hello

2: hola

2: hola

1: bye

1: bye

2: adios

2: adios

2: adios

1: common

1: common

1: common

2: como

2: no

2: no

Then you have to write a code step3.c that has to create two processes, that will act the same way writing on a file name log2.txt

