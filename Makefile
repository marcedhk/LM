#
# @(#)Makefile
#

ifeq ("${FORHOST}","true")
    # TODO: For i386
    EXTRA_LINKDIR = src/TurboActivate/amd64
    LDFLAGS = -Wl,-R,'$$ORIGIN'
else
    PALMPDK = /opt/PalmPDK
    
    # compiler commands
    PATH    := ${PATH}:${PALMPDK}/bin:${PALMPDK}/arm-gcc/bin
    CC      = arm-none-linux-gnueabi-gcc
    
    # compiler flags
    INCDIR  = ${PALMPDK}/include
    LIBDIR  = ${PALMPDK}/device/lib
    SYSROOT = ${PALMPDK}/arm-gcc/sysroot
    CCFLAGS = -mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp \
              -O2 -Wall -Wno-multichar -Wno-strict-aliasing $(FLAGS) \
              -I${INCDIR} -I${INCDIR}/SDL --sysroot=${SYSROOT}
    LDFLAGS = -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp \
              -L${LIBDIR} -Wl,--allow-shlib-undefined -lSDL -lpdl \
              -Wl,-R,'$$ORIGIN'
    EXTRA_LINKDIR = src/TurboActivate/arm
    
    package: com.phxdevices.acl-license_0.0.1_all.ipk binaries
endif

binaries: acl_license/acl_license acl_license/libTurboActivate.so

acl_license/libTurboActivate.so: ${EXTRA_LINKDIR}/libTurboActivate.so
	cp ${EXTRA_LINKDIR}/libTurboActivate.so acl_license/libTurboActivate.so

acl_licenase/TurboActivate.dat: src/TurboActivate.dat
	cp src/TurboActivate.dat acl_license

acl_license/acl_license: src/main.c src/acl_license.c src/TurboActivate.h acl_licenase/TurboActivate.dat
	mkdir -p acl_license
	${CC} ${CCFLAGS} ${LDFLAGS} src/main.c src/acl_license.c -o acl_license/acl_license -L${EXTRA_LINKDIR} -lTurboActivate

com.phxdevices.acl-license_0.0.1_all.ipk: acl_license/appinfo.json binaries
	palm-package acl_license

.PHONY : clean
clean:
	${RM} acl_license/acl_license acl_license/libTurboActivate.so *.ipk

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
