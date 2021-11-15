CFLAGS := -Wall -std=c++17 -I../fmt/include -fPIC
OBJS := Cpu.o Rom.o Ram.o 

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

libdummy6502.a: $(OBJS)
	$(AR) rcu $@ $^

clean:
	rm libdummy6502.a *.o
