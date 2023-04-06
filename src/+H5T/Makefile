OBJS = $(patsubst %.cc,../../inst/+H5T/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o ../util/H5LT_c.o

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5T/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5T/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
