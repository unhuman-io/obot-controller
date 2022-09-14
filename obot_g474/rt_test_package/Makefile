
all:
	mkdir -p build
	for j in *.h; do \
		$(MAKE) -C .. PARAM_OVERRIDE=../rt_test_package/$$j CONFIG=motor_rt_test -j GCC_PATH=; \
		cp -r ../build/motor_rt_test build/$${j%.h}; \
	done

