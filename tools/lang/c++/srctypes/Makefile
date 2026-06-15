THIS_DIR := $(patsubst %/,%,$(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

.PHONY: default check clean check-src check-tests

default: check

check: check-src check-tests

check-src:
	$(MAKE) -C $(THIS_DIR)/src check

check-tests:
	$(MAKE) -C $(THIS_DIR)/tests/basic check

clean:
	$(MAKE) -C $(THIS_DIR)/tests/basic clean
	$(MAKE) -C $(THIS_DIR)/src clean
