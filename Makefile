SRCS := $(wildcard *.c)
BUILD := build/cpu.x

all: $(SRCS)
	@echo "Compiling..."
	gcc $(SRCS) -o $(BUILD) $$(sdl2-config --cflags --libs)
	@echo "Done !"
	cd scripts && make
clean:
	@echo "Cleaning $(BUILD)"
	rm -f $(BUILD)
	@echo "Done !"