ACLOCAL_AMFLAGS = '-I m4'

libcallfilt.1: README.pod
	pod2man --name libcallfilt --center ' ' --release ' ' --official $< $@.tmp && mv -f $@.tmp $@

man_MANS = libcallfilt.1
bin_PROGRAMS = libcallfilt

libcallfilt_SOURCES = libcallfilt.c

pkglib_LTLIBRARIES = libdenyexec.la
libdenyexec_la_SOURCES = lib-denyexec.c
libdenyexec_la_LDFLAGS = -version-info 0:0:0

AM_CPPFLAGS = -DINST_PKGLIBDIR=\"$(pkglibdir)\"
