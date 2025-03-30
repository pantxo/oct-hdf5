all:
	$(MAKE) -s -C src

dist:
	$(MAKE) -s -C src dist

index:
	$(MAKE) -s -C src index

docs:
	$(MAKE) -s -C src docs

run:
	$(MAKE) -s -C src run

check:
	$(MAKE) -s -C src check

clean:
	$(MAKE) -s -C src clean
