OBJS = $(patsubst %.cc,../../inst/+H5P/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5P/%.oct: %.cc
	mkdir -p ../../inst/+H5P/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
