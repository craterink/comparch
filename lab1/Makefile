CC = gcc
CC_ARGS = -ansi -o
IN = in.asm
OUT = out.obj
IO_FILES = $(IN) $(OUT)
FILES = *.c *.h
OUT_EXE = lab1
DEBUG_EXE = lab1debug
TURNIN_OUTFILE = assembler.c
TURNIN_OUTDIR = turnin
REMOVE_INCLUDE_LINES = sed "/^#include/ d"

build: $(FILES)
	$(CC) $(CC_ARGS) $(OUT_EXE) $(FILES)

build_debug: $(FILES)
	$(CC) -g -o $(DEBUG_EXE) $(FILES)

clean:
	rm -f $(OUT_EXE) $(DEBUG_EXE)

go: build
	./$(OUT_EXE) $(IO_FILES)

debug: build_debug
	gdb --args ./$(DEBUG_EXE) $(IO_FILES)

turnin-clean:
	rm -rf $(TURNIN_OUTDIR)

turnin: $(FILES) turnin-clean
	mkdir $(TURNIN_OUTDIR)
	for hf in ./*.h ; do \
		cat $$hf | sed '/^#include/ d' >> $(TURNIN_OUTDIR)/$(TURNIN_OUTFILE); \
	done
	for cf in ./*.c ; do \
		cat $$cf | sed '/^#include/ d'  >> $(TURNIN_OUTDIR)/$(TURNIN_OUTFILE); \
	done


	
