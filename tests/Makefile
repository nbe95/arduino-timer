tests:
	set -e; \
	for i in test-*/Makefile; do \
		name=$$(dirname $$i); \
		echo -e '\n===== Making:' $$name '====='; \
		$(MAKE) -C ./$$name -j; \
	done

runtests:
	set -e; \
	for i in test-*/Makefile; do \
		name=$$(dirname $$i); \
		echo -e '\n===== Running:' $$name '====='; \
		./$$name/$$name.out; \
	done

clean:
	set -e; \
	for i in test-*/Makefile; do \
		name=$$(dirname $$i); \
		echo -e '\n===== Cleaning:' $$name '====='; \
		$(MAKE) -C ./$$name clean; \
	done;
