OCTAVE ?= octave
MKOCTFILE ?= mkoctfile
CFLAGS = -Wall -g
INCLUDES = $(shell ${OCTAVE} -f --eval "printf ('%s', __octave_config_info__().build_environment.HDF5_CPPFLAGS)")
LIBS = $(shell ${OCTAVE} -f --eval "printf ('%s %s', __octave_config_info__().build_environment.HDF5_LDFLAGS, __octave_config_info__().build_environment.HDF5_LIBS)")
PWD =  $(shell pwd)

export OCTAVE MKOCTFILE CFLAGS LIBS INCLUDES

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

index: all
	$(OCTAVE) -f --eval "cd private; make_index ('$(PWD)')"

dist: index
	$(OCTAVE) -f --eval "cd private; make_dist ('$(PWD)', '$(PWD)/..')"

docs: dist
	$(OCTAVE) -f --eval "cd private; make_docs ('$(PWD)/..')"

check: all
	$(OCTAVE) -f --path $(PWD)/../test --path $(PWD)/../test/data --path $(PWD)/../test/matio_test_datasets --path $(PWD)/../test/hdf5_matlab_examples --path $(PWD)/../inst --path $(PWD)/../src --eval "__oct_hdf5_tests__ ('$(PWD)')"

clean:
	rm -f ../inst/+H5*/*.oct
	rm -f ./util/*.o
	rm -f ./+H5*/*.o
	rm -f ../INDEX
