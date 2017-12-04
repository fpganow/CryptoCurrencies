
.PHONY: build

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
EXE=./build/binaries/mainExecutable/main
endif

ifeq ($(UNAME), Darwin)
EXE=./build/exe/main/main
endif

help:
	echo "Rules"

build:
	@#g++ main.cpp -o monero.bin
	@gradle build

run: build
	@clear && ${EXE}

pre:
	@g++ -E src/main.cpp > main.preprocessed.cpp
