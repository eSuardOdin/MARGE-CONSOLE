SRCS := $(wildcard src/*.c)
BUILD := build/oupinaiz

.PHONY: all clean doc

all: $(SRCS)
	@echo "Compiling..."
	gcc $(SRCS) -o $(BUILD) $$(sdl2-config --cflags --libs) -lm -I./include
	@echo "Done !"
	cd games && make
clean:
	@echo "Cleaning $(BUILD)"
	rm -f $(BUILD)
	@echo "Done !"
	cd games && make clean
doc:
	doxygen MargeDoxyfile