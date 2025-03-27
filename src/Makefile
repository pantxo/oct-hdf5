OCTAVE ?= octave
MKOCTFILE ?= mkoctfile
CFLAGS = -Wall -g
INCLUDES = $(shell ${OCTAVE} -f --eval "printf ('%s', __octave_config_info__().build_environment.HDF5_CPPFLAGS)")
LIBS = $(shell ${OCTAVE} -f --eval "printf ('%s %s', __octave_config_info__().build_environment.HDF5_LDFLAGS, __octave_config_info__().build_environment.HDF5_LIBS)")
PWD =  $(shell pwd)
SOURCES = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.oct,$(SOURCES))
UTIL_OBJS = ./util/h5_oct_util.o ./util/h5_data_util.o ./util/H5LT_c.o
PKG_ADD     = $(shell grep -Pho '(?<=// PKG_ADD: ).*' $(SOURCES))

export OCTAVE MKOCTFILE CFLAGS LIBS INCLUDES

all: $(OBJS)

./util/%.o:
	$(MAKE) -s -C util

__H5A__.oct: __H5A__.cc ${UTIL_OBJS} ./util/H5LT_c.o ./util/h5_data_util.o
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} $<.o

__H5D__.oct: __H5D__.cc ${UTIL_OBJS} ./util/H5LT_c.o ./util/h5_data_util.o
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} $<.o

__H5LT__.oct: __H5LT__.cc ${UTIL_OBJS}
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} -lhdf5_hl ${UTIL_OBJS} $<.o


%.oct: %.cc ${UTIL_OBJS}
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} $<.o

index: all
	$(OCTAVE) -f --eval "cd private; make_index ('$(PWD)')"

dist: index
	$(OCTAVE) -f --eval "cd private; make_dist ('$(PWD)', '$(PWD)/..')"

docs: dist
	$(OCTAVE) -f --eval "cd private; make_docs ('$(PWD)/..')"

check: all
	@$(OCTAVE) -f --silent --path $(PWD)/../test --path $(PWD)/../test/data --path $(PWD)/../test/matio_test_datasets --path $(PWD)/../test/hdf5_matlab_examples --path $(PWD)/../inst --path $(PWD)/../src --eval 'warning ("off");' --eval '${PKG_ADD}' --eval "__oct_hdf5_tests__ ('$(PWD)')"

clean:
	rm -f ./*.oct
	rm -f ./*.o
	rm -rf ../inst/testdir
	rm -f ./util/*.o
	rm -f ../INDEX
