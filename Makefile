all:
	$(MAKE) -s -C src

dist: index
	$(MAKE) -s -C src dist

index: all
	$(MAKE) -s -C src index

docs: dist
	$(MAKE) -s -C src docs

check: all
	$(MAKE) -s -C src check

clean:
	$(MAKE) -s -C src clean
