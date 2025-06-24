BIN:=bauripixel
CMAKE_OUTPUT:=build/$(BIN)/$(BIN)
ZIG_OUTPUT:=zig-out/bin/$(BIN)
HEADERS:= $(shell find src/include -path 'src/external' -prune -o -path 'src/include/exported' -prune -o -name '*.h' -print)
SOURCES:= $(shell find src/ -path 'src/external' -prune -o -name '*.c' -print)

all: run

.PHONY: build
build:
	cmake --build build

.PHONY: run
run: build
	./$(CMAKE_OUTPUT)

.PHONY: cmake_setup
cmake_setup:
	cmake -S . -B build -G Ninja


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
