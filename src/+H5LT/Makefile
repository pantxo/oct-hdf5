OBJS = $(patsubst %.cc,../../inst/+H5LT/%.oct,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.oct

../../inst/+H5LT/%.oct: %.cc
	mkdir -p ../../inst/+H5LT/
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -lhdf5_serial_hl -o $@ ${LIBS} $<
