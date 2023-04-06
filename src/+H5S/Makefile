OBJS = $(patsubst %.cc,../../inst/+H5S/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5S/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5S/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
