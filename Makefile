

.PHONY: all clean run

all:
	cmake -B ./build -S .
	cmake --build ./build

clean:
	rm -rf ./build

run:
	./bin/TestApp

