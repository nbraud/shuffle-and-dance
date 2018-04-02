LDLIBS+= -lsodium

.PHONY: default clean
default: main

clean:
	git clean -dfX

main: rand.o shuffle.o main.o
