include $(top_srcdir)/automake/multilib.am
include $(top_srcdir)/automake/compile.am

include_rtemsdir = $(includedir)/rtems

include_rtems_HEADERS = include/confdefs.h
include_rtems_HEADERS += include/rtems/bsd.h
include_rtems_HEADERS += include/rtems/chain.h
include_rtems_HEADERS += include/rtems/config.h
include_rtems_HEADERS += include/rtems/counter.h
include_rtems_HEADERS += include/rtems/extension.h
include_rtems_HEADERS += include/rtems/extensionimpl.h
include_rtems_HEADERS += include/rtems/fatal.h
include_rtems_HEADERS += include/rtems/init.h
include_rtems_HEADERS += include/rtems/io.h
include_rtems_HEADERS += include/rtems/ioimpl.h
include_rtems_HEADERS += include/rtems/mptables.h
include_rtems_HEADERS += include/rtems/cbs.h
include_rtems_HEADERS += include/rtems/profiling.h
include_rtems_HEADERS += include/rtems/rbheap.h
include_rtems_HEADERS += include/rtems/rbtree.h
include_rtems_HEADERS += include/rtems/scheduler.h
include_rtems_HEADERS += include/rtems/timecounter.h
include_rtems_HEADERS += include/rtems/timespec.h
include_rtems_HEADERS += include/rtems/version.h

EXTRA_DIST = include/rtems/README

noinst_LIBRARIES = libsapi.a
libsapi_a_SOURCES = src/extension.c src/extensioncreate.c \
    src/extensiondelete.c src/extensionident.c src/fatal.c src/exinit.c \
    src/exshutdown.c src/io.c src/ioclose.c src/iocontrol.c \
    src/ioinitialize.c src/ioopen.c src/ioread.c src/ioregisterdriver.c \
    src/iounregisterdriver.c src/iowrite.c src/posixapi.c  \
    src/getversionstring.c \
    src/chainappendnotify.c src/chaingetnotify.c src/chaingetwait.c \
    src/chainprependnotify.c src/rbheap.c src/interrtext.c \
    src/fatalsrctext.c src/version.c
libsapi_a_SOURCES += src/chainprotected.c
libsapi_a_SOURCES += src/cpucounterconverter.c
libsapi_a_SOURCES += src/delayticks.c
libsapi_a_SOURCES += src/delaynano.c
libsapi_a_SOURCES += src/rbtree.c
libsapi_a_SOURCES += src/rbtreefind.c
libsapi_a_SOURCES += src/rbtreeinsert.c
libsapi_a_SOURCES += src/profilingiterate.c
libsapi_a_SOURCES += src/profilingreportxml.c
libsapi_a_SOURCES += src/tcsimpleinstall.c
libsapi_a_CPPFLAGS = $(AM_CPPFLAGS)

#
# Create a new Version VC Key header if the VC state has changed.
#
vc_key_stamp = $(am__leading_dot)vc-key-stamp

libsapi_a_CPPFLAGS += -I.

BUILT_SOURCES = version-vc-key.h

.PHONY: $(vc_key_stamp)

$(vc_key_stamp):

version-vc-key.h: $(vc_key_stamp)
	@+current_vc_key=""; \
	if test -f $(vc_key_stamp); then \
	 current_vc_key=`cat $(vc_key_stamp)`; \
	fi; \
	vc_key=`$(top_srcdir)/sapi/vc-key.sh $(top_srcdir) $$current_vc_key`; \
	if test "$$vc_key" != "matches"; then \
	 echo "Generating version-vc-key.h"; \
	 if test "$$vc_key" == "release"; then \
	  vc_header_key="\/\* No version control key found; release\? \*\/"; \
	 else \
	  vc_header_key="#define RTEMS_VERSION_VC_KEY \"$$vc_key\""; \
	 fi; \
	 cat $(top_srcdir)/sapi/version-vc-key.h.in | \
	  sed -e "s/@VERSION_VC_KEY@/$$vc_header_key/g" > version-vc-key.h; \
	 echo "$$vc_key" > $(vc_key_stamp); \
	fi

version.$(OBJEXT):$ version-vc-key.h

all-local: version-vc-key.h

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
