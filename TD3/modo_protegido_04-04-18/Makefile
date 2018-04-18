# Makefile usage:
# 	make: compiles project using nasm compiler
# 	make clean: removes created files
#
# Author: Matias Silveiro

# Project name
PROJECT = letras

# Assembler source files
FILES = $(PROJECT).asm
LISTING = $(PROJECT).lst
OUTPUT = $(PROJECT).bin

FORMAT = bin

($PROJECT):
	@echo Compiling project $(PROJECT)
	nasm -f $(FORMAT) $(FILES) -l $(LISTING) -o $(OUTPUT)
	
clean:
	@echo Cleaning build directory
	rm $(LISTING) $(OUTPUT)
