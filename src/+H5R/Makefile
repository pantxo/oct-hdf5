OBJS = $(patsubst %.cc,../../inst/+H5R/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5R/%.oct: %.cc
	mkdir -p ../../inst/+H5R/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
