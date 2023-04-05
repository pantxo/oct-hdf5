OBJS = $(patsubst %.cc,../../inst/+H5T/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5T/%.oct: %.cc
	mkdir -p ../../inst/+H5T/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
