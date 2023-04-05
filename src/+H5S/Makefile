OBJS = $(patsubst %.cc,../../inst/+H5S/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5S/%.oct: %.cc
	mkdir -p ../../inst/+H5S/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
