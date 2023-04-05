OBJS = $(patsubst %.cc,../../inst/+H5L/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5L/%.oct: %.cc
	mkdir -p ../../inst/+H5L/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
