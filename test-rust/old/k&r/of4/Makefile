CC ?= cc



SRC = *.c 


all: main

main: $(SRC)
	$(CC) $(SRC)

clean:
	rm -f *.ihx *.hex *.bin a.out

fmt:
	clang-format --style=GNU --verbose -i *.c *.h 


