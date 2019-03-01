all:
	gcc -o main main.c -lpthread -lrt

cross:
	arm-linux-gcc -o pthread_cross_compile main.c -lpthread -lrt

clean:
	rm -rf main harry.log
