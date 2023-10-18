CFLAGS := -Wall -std=c++17 -I../fmt/include -I../SDL/include -I../imgui -fPIC
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:cpp=o)

all: dummy6502_ui

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

imgui.o: ../imgui/imgui.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

imgui_tables.o: ../imgui/imgui_tables.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

imgui_widgets.o: ../imgui/imgui_widgets.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

imgui_draw.o: ../imgui/imgui_draw.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

imgui_impl_sdl2.o: ../imgui/backends/imgui_impl_sdl2.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

imgui_impl_sdlrenderer2.o: ../imgui/backends/imgui_impl_sdlrenderer2.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

dummy6502_ui: $(OBJS) imgui.o imgui_tables.o imgui_widgets.o imgui_draw.o imgui_impl_sdlrenderer2.o imgui_impl_sdl2.o
	$(CC) $(CFLAGS) -o $@ *.o ../dummy6502/libdummy6502.a ../fmt/libfmt.a -lstdc++ -L../SDL/build/.libs -lSDL2-2.0.0
	#install_name_tool -change @rpath/libSDL2-2.0.dylib @executable_path/libSDL2-2.0.dylib dummy6502_ui

clean:
	rm dummy6502_ui *.o
