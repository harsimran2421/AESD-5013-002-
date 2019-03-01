share:
	gcc -o shared_memory shared_memory.c -lrt -lpthread

cross:
	arm-linux-gcc -o shared_memory_cross_compile shared_memory.c -lrt -lpthread

clean:
	rm -rf shared_memory *.txt
