#
# @(#)Makefile
#


PLUGIN_NAME := clip

ifeq ("${FORHOST}","true")
    # TODO: For i386
    EXTRA_LINKDIR := src/TurboActivate/amd64
    LDFLAGS := -Wl,-R,'$$ORIGIN'
else
    PALMPDK := /opt/PalmPDK
    
    # compiler commands
    PATH    := ${PATH}:${PALMPDK}/bin:${PALMPDK}/arm-gcc/bin
    CC      := arm-none-linux-gnueabi-gcc
    
    # compiler flags
    INCDIR  := ${PALMPDK}/include
    LIBDIR  := ${PALMPDK}/device/lib
    SYSROOT := ${PALMPDK}/arm-gcc/sysroot
    CCFLAGS := -mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp \
              -O2 -Wall -Wno-multichar -Wno-strict-aliasing $(FLAGS) \
              -I${INCDIR} -I${INCDIR}/SDL --sysroot=${SYSROOT}
    LDFLAGS := -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp \
              -L${LIBDIR} -Wl,--allow-shlib-undefined -lSDL -lpdl \
              -Wl,-R,'$$ORIGIN'
    EXTRA_LINKDIR := src/TurboActivate/arm
    
    package: com.phxdevices.clip_0.0.1_all.ipk binaries
    PKG_DIR := com.phxdevices.clip
endif

binaries: ${PKG_DIR}/${PLUGIN_NAME} ${PKG_DIR}/libTurboActivate.so

${PKG_DIR}/libTurboActivate.so: ${EXTRA_LINKDIR}/libTurboActivate.so
	cp ${EXTRA_LINKDIR}/libTurboActivate.so ${PKG_DIR}/libTurboActivate.so

acl_licenase/TurboActivate.dat: src/TurboActivate.dat
	cp src/TurboActivate.dat ${PKG_DIR}

${PKG_DIR}/${PLUGIN_NAME}: src/main.c src/${PLUGIN_NAME}.c src/TurboActivate.h acl_licenase/TurboActivate.dat
	${CC} ${CCFLAGS} ${LDFLAGS} src/main.c src/${PLUGIN_NAME}.c -o ${PKG_DIR}/${PLUGIN_NAME} -L${EXTRA_LINKDIR} -lTurboActivate

com.phxdevices.clip_0.0.1_all.ipk: ${PKG_DIR}/appinfo.json binaries
	palm-package ${PKG_DIR}

.PHONY : clean
clean:
	${RM} ${PKG_DIR}/${PLUGIN_NAME} ${PKG_DIR}/libTurboActivate.so *.ipk

clean_license:
	ls -lh /var/lib/.local/.*81152356 2>/dev/null || /bin/true
	ls -lh /var/lib/.config/.*81152356 2>/dev/null || /bin/true
	ls -lh /var/lib/.prof/.*81152356 2>/dev/null || /bin/true
	ls -lh ${HOME}/.local/.*81152356 2>/dev/null || /bin/true
	ls -lh ${HOME}/.config/.*81152356 2>/dev/null || /bin/true
	ls -lh ${HOME}/.prof/.*81152356 2>/dev/null || /bin/true
	${RM} /var/lib/.local/.*81152356
	${RM} /var/lib/.config/.*81152356
	${RM} /var/lib/.prof/.*81152356
	${RM} ${HOME}/.local/.*81152356
	${RM} ${HOME}/.config/.*81152356
	${RM} ${HOME}/.prof/.*81152356

# vim: set noexpandtab
