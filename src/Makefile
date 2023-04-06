OCTAVE ?= octave
MKOCTFILE ?= mkoctfile
CFLAGS = -Wall -g

export OCTAVE MKOCTFILE CFLAGS

all:
	$(MAKE) -s -C util
	$(MAKE) -s -C +H5A
	$(MAKE) -s -C +H5D
	$(MAKE) -s -C +H5E
	$(MAKE) -s -C +H5F
	$(MAKE) -s -C +H5G
	$(MAKE) -s -C +H5I
	$(MAKE) -s -C +H5L
	$(MAKE) -s -C +H5LT
	$(MAKE) -s -C +H5ML
	$(MAKE) -s -C +H5O
	$(MAKE) -s -C +H5P
	$(MAKE) -s -C +H5R
	$(MAKE) -s -C +H5S
	$(MAKE) -s -C +H5T

clean:
	rm -f ../inst/+H5*/*.oct
	rm -f ./util/*.o
