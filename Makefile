# for testing

.PHONY: all test build clean cleansweep

all: test

test: clean build
	env/bin/python tests/test_me

build:
	env/bin/python ./setup.py install

clean:
	$(RM) sidewinder/_sidewinder.c
	$(RM) sidewinder/*.pyc

cleansweep: clean
	$(RM) -r build env

develop: cleansweep
	virtualenv env
	$(MAKE) build
