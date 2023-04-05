OBJS = $(patsubst %.cc,../../inst/+H5I/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5I/%.oct: %.cc
	mkdir -p ../../inst/+H5I/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
