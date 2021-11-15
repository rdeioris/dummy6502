CFLAGS := -Wall -std=c++17 -I../fmt/include -fPIC
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:cpp=o)

%.o: %.cpp TestMachine.h catch.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

dummy6502_tests: $(OBJS)
	$(CC) $(CFLAGS) -o $@ *.o ../dummy6502/libdummy6502.a ../fmt/libfmt.a -lstdc++

clean:
	rm dummy6502_tests *.o
