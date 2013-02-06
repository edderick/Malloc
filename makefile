all:
	gcc -pthread -std=c99 -D SEED=99991 -D SIZE=65536 -D OPS=200 -pedantic -Wall -Werror -o output.o harness.c block.c list.c mymalloc.c  

debug: 
	gcc -pthread -std=c99 -pedantic -Wall -Werror -o output.o mydriver.c block.c list.c mymalloc.c  


