CC = gcc
IO_FILES = in.txt out.txt
FILES = *.c *.h
OUT_EXE = lab1
DEBUG_EXE = lab1debug

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)

build_debug: $(FILES)
	$(CC) -g -o $(DEBUG_EXE) $(FILES)

clean:
	rm -f $(OUT_EXE) $(DEBUG_EXE)

go: build
	./$(OUT_EXE) $(IO_FILES)

debug: build_debug
	gdb --args ./$(DEBUG_EXE) $(IO_FILES)

