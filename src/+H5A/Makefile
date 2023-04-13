OBJS = $(patsubst %.cc,../../inst/+H5A/%.oct,$(wildcard *.cc))
UTIL_OBJS = ../util/h5_oct_util.o

all: $(OBJS)

clean:
	rm -f *.o

../../inst/+H5A/%.oct: %.cc ${UTIL_OBJS}
	mkdir -p ../../inst/+H5A/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} $<.o

../../inst/+H5A/read.oct: read.cc ${UTIL_OBJS} ../util/H5LT_c.o ../util/h5_data_util.o
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} ../util/H5LT_c.o ../util/h5_data_util.o $<.o

../../inst/+H5A/write.oct: write.cc ${UTIL_OBJS} ../util/H5LT_c.o ../util/h5_data_util.o
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -c -o $<.o $<
	$(MKOCTFILE) ${CFLAGS} -o $@ ${LIBS} ${UTIL_OBJS} ../util/H5LT_c.o ../util/h5_data_util.o $<.o
