.PHONY: dummy6502 dummy6502_tests

all: dummy6502 dummy6502_tests
	./dummy6502_tests/dummy6502_tests

dummy6502:
	$(MAKE) -C $@

dummy6502_tests:
	$(MAKE) -C $@

clean:
	cd dummy6502 && $(MAKE) clean
	cd dummy6502_tests && $(MAKE) clean

