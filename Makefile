.ONESHELL:
all: parse_sdf

parse_sdf:
	cd build
	cmake ..
	$(MAKE)
