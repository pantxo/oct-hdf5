OBJS = $(patsubst %.cc,../../inst/+H5E/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o ../util/H5LT_c.o ../util/h5_data_util.o

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5E/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5E/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} $<.o
