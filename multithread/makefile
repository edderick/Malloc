all: 
	gcc -pthread -std=c99 -pedantic -Wall -Werror -o output.o mydriver.c block.c list.c mymalloc.c  

harness:
	
	gcc -pthread -D DEBUG=0 -std=c99 -D THREADS=20 -D SEED=42 -D SIZE=8192192 -D OPS=200 -pedantic -Wall -Werror -o output.o harnessThreadSafe.c block.c list.c mymalloc.c  

