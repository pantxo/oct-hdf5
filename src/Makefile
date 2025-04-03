OCTAVE       ?= octave
MKOCTFILE    ?= mkoctfile
CFLAGS        = -Wall -g
INCLUDES     := $(shell ${OCTAVE} -f --eval "printf ('%s', __octave_config_info__().build_environment.HDF5_CPPFLAGS)")

LIBS         := $(shell ${OCTAVE} -f --eval "printf ('%s %s', __octave_config_info__().build_environment.HDF5_LDFLAGS, __octave_config_info__().build_environment.HDF5_LIBS)")
PWD          :=  $(shell pwd)
SOURCES      := $(wildcard *.cc)
OBJS         := $(patsubst %.cc,%.oct,$(SOURCES))
SOURCES_UTIL := $(wildcard *.cc)
OBJS_UTIL    := $(patsubst %.cc,%.o,$(SOURCES_UTIL))
UNAME        := $(shell uname -o)
ifeq ($(UNAME),Darwin)
 GREP         = grep
else
 GREP         = ggrep
endif
PKG_ADD      := $(shell $(GREP) -Pho '(?<=// PKG_ADD: ).*' $(SOURCES))

## Test Octave version
OCT_MAJ_VER  := $(shell ${OCTAVE} -f --silent --eval "printf ('%s', strsplit (version, \".\"){1})")
oct_ge = $(shell \
echo test | awk '{if($(OCT_MAJ_VER) < $(1)) {print 0} else {print 1}}')

## The octave::execution_exception::message method only exist for Octave >= 6 
ifeq ($(call oct_ge,6),1)
CFLAGS+= -DHAVE_EXCEPTION_MSG
endif

all: $(OBJS)

%.o: %.cc
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $@ $<

__H5A__.oct: __H5A__.o ./util/h5_oct_util.o ./util/H5LT_c.o ./util/h5_data_util.o
	$(MKOCTFILE) -o $@ ${LIBS} $< ./util/h5_oct_util.o ./util/H5LT_c.o ./util/h5_data_util.o

__H5D__.oct: __H5D__.o ./util/h5_oct_util.o ./util/H5LT_c.o ./util/h5_data_util.o
	$(MKOCTFILE) -o $@ ${LIBS} $< ./util/h5_oct_util.o ./util/H5LT_c.o ./util/h5_data_util.o

__H5LT__.oct: __H5LT__.o ./util/h5_oct_util.o ./util/H5LT_c.o
	$(MKOCTFILE) -o $@ ${LIBS} -lhdf5_hl $< ./util/h5_oct_util.o ./util/H5LT_c.o

%.oct: %.o  ./util/h5_oct_util.o ./util/H5LT_c.o
	$(MKOCTFILE) -o $@ ${LIBS} $< ./util/h5_oct_util.o ./util/H5LT_c.o

index: all
	$(OCTAVE) -f --eval "cd private; make_index ('$(PWD)')"

dist: index
	$(OCTAVE) -f --eval "cd private; make_dist ('$(PWD)', '$(PWD)/..')"

docs: dist
	$(OCTAVE) -f --eval "cd private; make_docs ('$(PWD)/..')"

run:
	@$(OCTAVE) -f --persist --path $(PWD)/../test --path $(PWD)/../test/data --path $(PWD)/../test/matio_test_datasets --path $(PWD)/../test/hdf5_matlab_examples --path $(PWD)/../inst --path $(PWD)/../src --eval 'warning ("off", "Octave:autoload-relative-file-name");' --eval '${PKG_ADD}'

check: all
	@$(OCTAVE) -f --silent --path $(PWD)/../test --path $(PWD)/../test/data --path $(PWD)/../test/matio_test_datasets --path $(PWD)/../test/hdf5_matlab_examples --path $(PWD)/../inst --path $(PWD)/../src --eval 'warning ("off", "Octave:autoload-relative-file-name");' --eval '${PKG_ADD}' --eval "__oct_hdf5_tests__ ('$(PWD)')"

clean:
	@rm -f ./*.oct
	@rm -f ./*.o
	@rm -rf ../inst/testdir
	@rm -f ./util/*.o
	@rm -f ../INDEX
