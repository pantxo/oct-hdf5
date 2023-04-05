OBJS = $(patsubst %.cc,../../inst/+H5G/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5G/%.oct: %.cc
	mkdir -p ../../inst/+H5G/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
