OBJS = $(patsubst %.cc,../../inst/+H5E/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5E/%.oct: %.cc
	mkdir -p ../../inst/+H5E/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} $<
