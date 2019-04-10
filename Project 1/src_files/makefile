include sources.mk


FLAGS += $(addprefix -I,$(INCLUDE_H))
OBJ = main.c logger.c light.c temperature.c i2c.c socket.c user_led.c intial_test.c test_light.c test_temp.c test_conversion.c 

all:
	rm -rf main cross-main harry.log
	gcc $(FLAGS) -o main $(OBJ) -lpthread -lrt -lm

cross:
	rm -rf main cross-main harry.log
	arm-linux-gcc $(FLAGS) -o cross-main $(OBJ) -lpthread -lrt -lm

debug:
	arm-linux-gcc $(FLAGS) -o debug-cross $(OBJ) -lpthread -lrt -lm -DDEBUG

client-cross:
	rm -rf client
	arm-linux-gcc $(FLAGS) -o client client.c -lrt

clean:
	rm -rf main client cross-main harry.log
