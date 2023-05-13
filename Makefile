all:
	$(MAKE) -s -C src

dist:
	$(MAKE) -s -C src dist

index:
	$(MAKE) -s -C src index

check:
	$(MAKE) -s -C src check

clean:
	$(MAKE) -s -C src clean
