# AAKASH JANA AAJ284 11297343
# NANDISH JHA NAJ474 11282001

UNAME_S := $(shell uname -s 2>nul || echo Windows_NT)

# Detect OS
ifeq ($(UNAME_S), Windows_NT)
    detected_OS := Windows
    CLEAR_CMD = cls
    DEL_CMD = del /Q
    EXE_EXT = .exe
    LIBS = -luser32 -lkernel32
else ifneq (,$(findstring MINGW64,$(UNAME_S)))
    detected_OS := MINGW64
    CLEAR_CMD = clear
    DEL_CMD = rm -f
    EXE_EXT = .exe
    LIBS = -luser32 -lkernel32
else
    detected_OS := Linux
    CLEAR_CMD = clear
    DEL_CMD = rm -f
    EXE_EXT =
    LIBS = -lpthread
    PTHREADS = /student/cmpt332/pthreads/
    PTHREADS_LIB = /student/cmpt332/pthreads/lib/
endif

# Compiler Flags
CC = gcc
CFLAGS = -c -g -std=gnu90 -Wall -pedantic 	# C compiler flags
CPPFLAGS = -I. 								# C pre-processor flags
LDFLAGS = $(LIBS)							# Linker flags

.PHONY: all clean help partA1 partA2 partA3 partA4 partC

# Default build target depending on what OS it is
all:
ifeq ($(detected_OS),Windows)
	@echo "Detected Windows — building only PartA1"
	make partA1
else ifeq ($(detected_OS),MINGW64)
	@echo "Detected MinGW64 — building only PartA1"
	make partA1
else
	@echo "Detected Linux — building PartA2, PartA3, PartA4, PartC"
	make partA2
	make partA3
	make partA4
	make partC
endif

# Help target
help:
	@echo "=== Available Make Targets ==="
	@echo "  make all     - Build everything valid for your platform"
	@echo "  make partA1  - Windows threading"
	@echo "  make partA2  - UBC pthreads"
	@echo "  make partA3  - POSIX pthreads"
	@echo "  make partA4  - UNIX processes"
	@echo "  make partC   - List library"
	@echo "  make clean   - Remove build files"
	@echo ""

# =======================
# Part A1 - Windows with CreateThread
# =======================
partA1: partA1.o square1.o
	@echo Detected OS: $(detected_OS)
	@echo building partA1 with Windows threads...
	$(CC) partA1.o square1.o -o partA1$(EXE_EXT) $(LIBS)

partA1.o: partA1.c partA.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA1 partA1.c -o partA1.o

square1.o: square.c square.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA1 square.c -o square1.o

# =======================
# Part A2 - Linux with UBC pthreads  
# =======================
partA2: partA2.o square2.o
	@echo Detected OS: $(detected_OS)
	@echo building partA2 with UBC pthreads...
	$(CC) partA2.o square2.o -o partA2$(EXE_EXT) -L$(PTHREADS_LIB)Linuxx86_64 $(LIBS)

partA2.o: partA2.c partA.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA2 -I$(PTHREADS) -I. partA2.c -o partA2.o

square2.o: square.c square.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA2 -I$(PTHREADS) -I. square.c -o square2.o

# =======================
# Part A3 - Linux with POSIX pthreads
# =======================
partA3: partA3.o square3.o
	@echo Detected OS: $(detected_OS)
	@echo building partA3 with POSIX pthreads...
	$(CC) partA3.o square3.o -o partA3$(EXE_EXT) $(LIBS)

partA3.o: partA3.c partA.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA3 partA3.c -o partA3.o

square3.o: square.c square.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA3 square.c -o square3.o

# =======================
# Part A4 - Linux with UNIX Processes
# =======================
partA4: partA4.o square4.o
	@echo Detected OS: $(detected_OS)
	@echo building partA4 with UNIX Processes...
	$(CC) partA4.o square4.o -o partA4$(EXE_EXT) $(LIBS)

partA4.o: partA4.c partA.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA4 partA4.c -o partA4.o

square4.o: square.c square.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -DBUILD_PARTA4 square.c -o square4.o

# =======================
# Part C - Linux with List Library
# =======================
partC: mytestlist.o list_movers.o list_adders.o list_removers.o
	@echo Detected OS: $(detected_OS)
	@echo building partC with list library...
	$(CC) mytestlist.o list_movers.o list_adders.o list_removers.o -o partC$(EXE_EXT)

mytestlist.o: mytestlist.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) mytestlist.c -o mytestlist.o

list_movers.o: list_movers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) list_movers.c -o list_movers.o

list_adders.o: list_adders.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) list_adders.c -o list_adders.o

list_removers.o: list_removers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) list_removers.c -o list_removers.o

# Target to clean up the directory
clean:
	$(CLEAR_CMD)
	@echo Cleaning up $(detected_OS)...
	$(DEL_CMD) *.o partA1$(EXE_EXT) partA2 partA3 partA4 partC