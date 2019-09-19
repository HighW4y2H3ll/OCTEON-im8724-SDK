
.PHONY: install
install:
	mkdir -p ${ROOT}/examples/testsuite
	cp -R ${SOURCE_DIR}/testsuite/* ${ROOT}/examples/testsuite/
	- find ${ROOT}/examples/testsuite/ -name .svn | xargs rm -rf
	${CC} ${CFLAGS} -W -Wall -o ${ROOT}/examples/testsuite/hugetlb-shmget ${SOURCE_DIR}/hugetlb-shmget.c
	${CC} ${CFLAGS} -W -Wall -o ${ROOT}/examples/testsuite/hugetlb-use-fs ${SOURCE_DIR}/hugetlb-use-fs.c
	${CC} ${CFLAGS} -W -Wall -o ${ROOT}/examples/testsuite/hugetlb-use-fs-1 ${SOURCE_DIR}/hugetlb-use-fs-1.c
	${CC} ${CFLAGS} -o ${ROOT}/examples/testsuite/rixi ${SOURCE_DIR}/rixi.c
	${CC} ${CFLAGS} -o ${ROOT}/examples/testsuite/i2c-multi ${SOURCE_DIR}/i2c-multi.c
