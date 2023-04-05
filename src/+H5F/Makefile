OBJS = $(patsubst %.cc,../../inst/+H5F/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5F/%.oct: %.cc
	mkdir -p ../../inst/+H5F/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
