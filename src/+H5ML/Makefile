OBJS = $(patsubst %.cc,../../inst/+H5ML/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5ML/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5ML/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
