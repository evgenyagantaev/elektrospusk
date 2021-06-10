TARGET=elspusk_kamera
LIBS=-lbcm2835 -lncurses -lpthread -lm
OBJECTS=main.o init.o udp_pipe_thread.o step_driver.o command_thread.o mma845_driver.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(OBJECTS) -o $(TARGET) $(LIBS)

main.o: main.c main.h
	gcc -c -g main.c -o main.o

init.o: init.c init.h main.h step_driver.h mma845_driver.h
	gcc -c -g init.c -o init.o

udp_pipe_thread.o: udp_pipe_thread.c main.h
	gcc -c -g udp_pipe_thread.c -o udp_pipe_thread.o

step_driver.o: step_driver.c step_driver.h
	gcc -c -g step_driver.c -o step_driver.o

command_thread.o: command_thread.c main.h
	gcc -c -g command_thread.c -o command_thread.o

mma845_driver.o: mma845_driver.c mma845_driver.h main.h
	gcc -c -g mma845_driver.c -o mma845_driver.o	

clean:
	rm -rf *.o $(TARGET)
