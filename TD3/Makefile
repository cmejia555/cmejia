# Makefile usage:
# 	make: compiles project using nasm compiler
# 	make clean: removes created files
#
# Author: Matias Silveiro

# Project name
PROJECT = video_21-03

# Assembler source files
FILES = $(PROJECT).asm
LISTING = $(PROJECT).lst
OUTPUT = mi_rom.bin
OUTPUT_DIR = debug/
FORMAT = bin

($PROJECT):
	@echo Compiling project $(PROJECT)
	nasm -f $(FORMAT) $(FILES) -l $(addprefix $(OUTPUT_DIR), $(LISTING)) -o $(addprefix $(OUTPUT_DIR), $(OUTPUT))



.PHONY : clean
clean:
	@echo	''
	@echo	'---> Limpieza de archivos'
	rm -f -r $(OUTPUT_DIR)*
