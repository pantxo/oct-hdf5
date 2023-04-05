OBJS = $(patsubst %.cc,../../inst/+H5O/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5O/%.oct: %.cc
	mkdir -p ../../inst/+H5O/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
