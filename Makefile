.PHONY: dummy6502 dummy6502_tests dummy6502_ui

all: dummy6502 dummy6502_tests dummy6502_ui
	./dummy6502_tests/dummy6502_tests

dummy6502:
	$(MAKE) -C $@

dummy6502_tests:
	$(MAKE) -C $@

dummy6502_ui:
	$(MAKE) -C $@

clean:
	cd dummy6502 && $(MAKE) clean
	cd dummy6502_tests && $(MAKE) clean
	cd dummy6502_ui && $(MAKE) clean

