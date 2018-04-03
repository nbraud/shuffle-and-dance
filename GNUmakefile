LDLIBS+= -lsodium

.PHONY: default clean run
default: main

clean:
	git clean -dfX

main: rand.o shuffle.o main.o
	$(CXX) $^ $(LDARGS) $(LDLIBS) -o $@

run: main
	taskset -c 1 ./main
