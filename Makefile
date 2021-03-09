.ONESHELL:
all: cmake

cmake:
	cd build
	cmake ..
	$(MAKE)
