OBJS = $(patsubst %.cc,../../inst/+H5D/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5D/%.oct: %.cc
	mkdir -p ../../inst/+H5D/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
