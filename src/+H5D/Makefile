OBJS = $(patsubst %.cc,../../inst/+H5D/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o ../util/h5_data_util.o

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5D/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5D/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
