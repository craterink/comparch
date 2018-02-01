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
	# 0) add all nonlocal #includes from all .c and .h files
	for hf in ./*.[ch] ; do \
		cat $$hf | sed '/^\(#include\)/!d' \
		         | sed '/^\(#include [^<]\)/ d' \
				>> $(TURNIN_OUTDIR)/$(TURNIN_OUTFILE); \
	done
	# 1) add all #defines from all .h files, except the _HEADERFILE_H_ ones
	for hf in ./*.h ; do \
		cat $$hf | sed '/^\(#define\)/!d' \
		         | sed '/^\(#define _[A-Z]\{1,\}_H_\)/ d' \
				>> $(TURNIN_OUTDIR)/$(TURNIN_OUTFILE); \
	done
	# 2) add all lines except #defines, #includes, 
	#       #ifndefs, "extern ..." and #endifs from all .h files
	for hf in ./*.h ; do \
		cat $$hf | sed \
		'/\(^#include\)\|\(^#define\)\|\(^#ifndef\)\|\(^#endif\)\|\(^extern \)/ d' \
		>> $(TURNIN_OUTDIR)/$(TURNIN_OUTFILE); \
	done
	# 3) add all lines except #includes from .c files
	for cf in ./*.c ; do \
		cat $$cf | sed '/^#include/ d' >> $(TURNIN_OUTDIR)/$(TURNIN_OUTFILE); \
	done


	
