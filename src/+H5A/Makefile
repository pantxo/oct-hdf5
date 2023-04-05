OBJS = $(patsubst %.cc,../../inst/+H5A/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5A/%.oct: %.cc
	mkdir -p ../../inst/+H5A/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
