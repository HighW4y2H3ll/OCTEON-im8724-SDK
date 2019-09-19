
include ../.config

.PHONY: install
install:
ifdef CONFIG_native-toolchain
	- mv ${ROOT}/usr/lib/* ${ROOT}/${LIBDIR}/ > /dev/null 2>&1
	# Move back the GCC directory.
	- mv ${ROOT}/${LIBDIR}/gcc ${ROOT}/usr/lib > /dev/null 2>&1
	# Remove locales since they not needed for now
	- rm ${ROOT}/usr/share/locale > /dev/null 2>&1
else
ifdef CONFIG_build-test
	- cp ${ROOT}/usr/lib/* ${ROOT}/${LIBDIR}/ > /dev/null 2>&1
else
	- mv ${ROOT}/usr/lib/* ${ROOT}/${LIBDIR}/ > /dev/null 2>&1
	rm -rf ${ROOT}/usr/include
endif
endif
	find ${ROOT} -name gconv | xargs rm -rf
	- find ${ROOT} -name .svn | xargs rm -rf
	- find ${ROOT} -name doc | xargs rm -fr
	- find ${ROOT} -name man | xargs rm -fr
	- find ${ROOT} -name info | xargs rm -fr
ifndef CONFIG_native-toolchain
	- find ${ROOT} -name '*.a' | xargs rm -fr
endif
ifdef CFG_STRIP_BINARIES
	for f in `find ${ROOT}/bin ${ROOT}/sbin ${ROOT}/usr/bin ${ROOT}/usr/sbin ${ROOT}/examples ${ROOT}/usr/libexec* -not -type l -and -not -type d`; \
	do if sh -c "file $$f | grep -q ELF"; then ${STRIP} $$f; fi; done
endif
ifdef CFG_STRIP_LIBRARIES
	find ${ROOT} \( -name '*.so.*' -or -name '*.so' \) -and -not -type d -and -size +10 | xargs -r chmod +w
	find ${ROOT} \( -name '*.so.*' -or -name '*.so' \) -and -not -type d -and -size +10 | xargs -r ${STRIP} --strip-debug
	find ${ROOT} \( -name '*.so.*' -or -name '*.so' \) -and -not -type d -and -size +10 | xargs -r chmod -w,g-w,o-w

	find ${ROOT} \( -name '*.a' \) -and -not -type d -and -size +10 | xargs -r chmod +w
	find ${ROOT} \( -name '*.a' \) -and -not -type d -and -size +10 | xargs -r ${STRIP} --strip-debug
	find ${ROOT} \( -name '*.a' \) -and -not -type d -and -size +10 | xargs -r chmod -w,g-w,o-w
endif
ifdef CFG_STRIP_KERNEL_MODULES
	find ${ROOT}/lib/modules -name '*.ko' | xargs -r -n 1 ${STRIP} --strip-debug
endif
ifneq (${CFG_EXTRA_FILES_DIR},"")
	cp -R ${CFG_EXTRA_FILES_DIR}/. ${ROOT}
else
	if [ -d ${SOURCE_DIR}/../user-include ]; then cp -R ${SOURCE_DIR}/../user-include/. ${ROOT}; fi
endif
	sudo chown -R -h root ${ROOT}
	sudo chgrp -R -h root ${ROOT}

