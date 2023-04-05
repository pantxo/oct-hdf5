OBJS = $(patsubst %.cc,../../inst/+H5ML/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5ML/%.oct: %.cc
	mkdir -p ../../inst/+H5ML/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
