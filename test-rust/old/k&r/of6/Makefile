CC ?= cc



SRC = *.c 


all: main

main: $(SRC)
	$(CC) $(SRC) -o md5_file

clean:
	rm -f *.ihx *.hex *.bin a.out

fmt:
	clang-format --style=GNU --verbose -i *.c *.h 


