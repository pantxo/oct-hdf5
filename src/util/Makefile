OBJS = $(patsubst %.cc,%.o,$(wildcard *.cc))

all: $(OBJS)

clean:
	rm -f *.o

%.o: %.cc
	$(MKOCTFILE) ${CFLAGS} ${INCLUDES} -o $@ ${LIBS} -c $<
