include sources.mk

FLAGS += $(addprefix -I,$(INCLUDE_H))

OBJ = main.c logger.c uart.c decision.c user_led.c matrix.c 

all:
	rm -rf main uart-main harry.log
	gcc $(FLAGS) -o uart-main $(OBJ) -lpthread -lrt -lm

clean:
	rm -rf uart-main harry.log

