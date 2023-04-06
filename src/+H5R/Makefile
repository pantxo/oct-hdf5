OBJS = $(patsubst %.cc,../../inst/+H5R/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o ../util/H5LT_c.o

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5R/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5R/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
