BIN:=bauripixel
CMAKE_OUTPUT:=build/$(BIN)/$(BIN)
ZIG_OUTPUT:=zig-out/bin/$(BIN)
HEADERS:= $(shell find src/include -path 'src/external' -prune -o -path 'src/include/exported' -prune -o -name '*.h' -print)
SOURCES:= $(shell find src/ -path 'src/external' -prune -o -name '*.c' -print)
PACKER:=./packer.py

all: run

.PHONY: build
build:
	cmake --build build

.PHONY: run
run: build
	./$(CMAKE_OUTPUT)

.PHONY:zvalgrind
zvalgrind: zbuild
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(ZIG_OUTPUT) > valgrind.log 2>&1

.PHONY:valgrind
valgrind: build
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(CMAKE_OUTPUT) > valgrind.log 2>&1

.PHONY: cmake_setup
cmake_setup:
	cmake -S . -B build -G Ninja

.PHONY: cmake_setup_clang
cmake_setup_clang:
	cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang


.PHONY: zbuild
zbuild:
	zig build

.PHONY: zrun
zrun:
	zig build run


.PHONY: fmt
fmt:
	@clang-format -i -style=file --verbose $(SOURCES) $(HEADERS)


.PHONY: infer
infer: cmake_clean
	infer run --compilation-database build/compile_commands.json

.PHONY: cmake_clean
cmake_clean:
	cd build && ninja clean

.PHONY: clean
clean: cmake_clean
	rm -rf .zig-cache
	rm -rf .cache
	rm -rf zig-out

# Resource Packing

.PHONE: rdtools
rdtools:
	$(PACKER) assets/media/drawtools/drawtools_icon_minimal.png -o src/include/exported/drawtoolicons.h
