all: 
	gcc -pthread -std=c99 -pedantic -Wall -Werror -o output.o mydriver.c mymalloc.c  

harness:
	
	gcc -pthread -std=c99 -D SEED=42 -D SIZE=1048576 -D OPS=200 -pedantic -Wall -Werror -o output.o harness.c mymalloc.c  

