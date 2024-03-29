#Normal Compilation is set to Linux
all: demu bin

demu: main.cpp arch/D16i/core/core.cpp terminal.cpp terminal.h arch/D16i/core/core.h
	g++ main.cpp arch/D16i/core/core.cpp terminal.cpp -l pthread -o demu

windows: mingw32_comp wbin

mingw32_comp: main.cpp arch/D16i/core/core.cpp terminal.cpp terminal.h
	x86_64-w64-mingw32-g++ -o demu.exe main.cpp arch/D16i/core/core.cpp terminal.cpp -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic


bin:
	cp demu bin/

wbin: demu.exe
	cp demu.exe bin/windows/demu.exe
