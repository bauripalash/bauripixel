# Actual Binary Name
BIN=bauripixel

# Build Directory of CMake
CMAKE_BUILD_DIR=build

# CMake built binary
CMAKE_OUTPUT=$(CMAKE_BUILD_DIR)/$(BIN)

# CC
CC=cc

# TCC Path (for future use)
TCC=/usr/bin/tcc

# Performance Test Compiler
PERFCC=clang

# Release Mode Compiler
RELEASE_CC=clang



CPPCHECK_BUILD_DIR=$(CMAKE_BUILD_DIR)/cppcheck


HEADERS:= $(shell find src/ -path 'src/gen' -prune -o -path 'src/external' -prune -o -path 'src/tmpl' -prune -o -name '*.h' -print)
SOURCES:= $(shell find src/ -path 'src/gen' -prune -o -path 'src/external' -prune -o -path 'src/tmpl' -prune -o -name '*.c' -print)


all: run

.PHONY: crun
crun: build
	./$(CMAKE_OUTPUT) $(FILE)

.PHONY: build
build:
	cmake --build build --target $(BIN) --parallel

.PHONY: run
run: build
	./$(CMAKE_OUTPUT) $(FILE)

.PHONY: fmt
fmt:
	@clang-format -i -style=file --verbose $(SOURCES) $(HEADERS)

.PHONY: valgrind
valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all -s $(CMAKE_OUTPUT) $(FILE)

.PHONY: cmake_tcc
cmake_tcc:
	cmake -S . -B build -DCMAKE_C_COMPILER=$(TCC)

.PHONY: cmake_setup
cmake_setup:
	cmake -S . -B build -DCMAKE_C_COMPILER=clang

.PHONY: cmake_ninja
cmake_ninja:
	cmake -S . -B build -DCMAKE_C_COMPILER=clang -G "Ninja"

.PHONY: cmake_clang
cmake_cc:
	cmake -S . -B build -DCMAKE_C_COMPILER=clang

.PHONY: cmake_clean
cmake_clean:
	cmake --build build --target clean

.PHONY: build_dbg
build_dbg:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --target $(BIN) --parallel

.PHONY: build_rls_ninja
build_rls_ninja:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$(RELEASE_CC) -G "Ninja"
	cmake --build build --target $(BIN) --parallel

.PHONY: build_rls
build_rls:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$(RELEASE_CC)
	cmake --build build --target $(BIN) --parallel

.PHONY: build_rld
build_rld:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER=$(RELEASE_CC)
	cmake --build build --target $(BIN) --parallel

.PHONY: run_perf
run_perf: build_rld
	perf record -g --call-graph dwarf -F 999 ./$(CMAKE_OUTPUT) $(PERFFILE)
	perf script -F +pid > bauripixel.perf

.PHONY: run_callgrind
run_callgrind: build_rld
	valgrind --tool=callgrind --callgrind-out-file="$(BIN).%p.callgrind.out" --dump-instr=yes --simulate-cache=yes $(CMAKE_OUTPUT) $(PERFFILE)

.PHONY: run_clangtidy
run_clangtidy:
	clang-tidy --config-file=.clang-tidy $(SOURCES) $(HEADERS)

.PHONY: infer
infer: cmake_clean
	infer run --compilation-database build/compile_commands.json

.PHONY: run_cppcheck
run_cppcheck:
	mkdir -p $(CPPCHECK_BUILD_DIR)
	#cppcheck --project=pankti.cppcheck

.PHONY: clean
clean: cmake_clean
	rm -rf .cache

