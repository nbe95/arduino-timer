tests:
	set -e; \
	for i in test-*/Makefile; do \
		name=$$(dirname $$i); \
		echo -e '\n===== Making:' $$name '====='; \
		../tools/ci/copy-test-files.sh "$$name" ../src/; \
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
		rm -rf ./$$name/src/ &>/dev/null || true; \
		$(MAKE) -C ./$$name clean; \
	done;
