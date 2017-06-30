.PHONY: all
all:
	@echo "\n\033[1;36m[Building C and C++ sources]\033[0m"
	@cd kernelsrc && make --no-print-directory

.PHONY: clean
clean:
	@cd kernelsrc && make clean --no-print-directory
