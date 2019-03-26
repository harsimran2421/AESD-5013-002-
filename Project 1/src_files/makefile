include sources.mk


FLAGS += $(addprefix -I,$(INCLUDE_H))
OBJ = main.c logger.c light.c temperature.c i2c.c

all:
	rm -rf main cross-main harry.log
	gcc $(FLAGS) -o main $(OBJ) -lpthread -lrt -lm

cross:
	rm -rf main cross-main harry.log
	arm-linux-gcc $(FLAGS) -o cross-main $(OBJ) -lpthread -lrt -lm

clean:
	rm -rf main cross-main harry.log
