tests:
	set -e +f; \
	for i in test-*/Makefile; do \
		echo '==== Making:' $$(dirname $$i); \
		cp *.h *.cpp $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i); \
	done

runtests:
	set -e +f; \
	for i in test-*/Makefile; do \
		echo '==== Running:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i) run; \
	done

clean:
	set -e +f; \
	for i in test-*/Makefile; do \
		echo '==== Cleaning:' $$(dirname $$i); \
		rm -rf $$(dirname $$i)/*.h $$(dirname $$i)/*.cpp &> /dev/null || true; \
		$(MAKE) -C $$(dirname $$i) clean; \
	done
