
.PHONY: build

help:
	echo "Rules"

build:
	@#g++ main.cpp -o monero.bin
	@gradle build

run: build
	@clear && ./build/binaries/mainExecutable/main
	@#./build/binaries/mainExecutable/main

pre:
	@g++ -E src/main.cpp > main.preprocessed.cpp
