OBJS = $(patsubst %.cc,../../inst/+H5LT/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o ../util/H5LT_c.o
LIBS += -lhdf5_hl

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5LT/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5LT/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
