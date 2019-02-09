ACLOCAL_AMFLAGS = '-Im4'

libcallfilt.1: README.pod
	pod2man --name libcallfilt --center ' ' --release ' ' --official $< $@.tmp && mv -f $@.tmp $@

libdenyexec.so: lib-denyexec.c
	$(COMPILE) -shared -fPIC -o $@ $<

overloaddir = $(pkglibdir)
overload_DATA = libdenyexec.so

man_MANS = libcallfilt.1
bin_PROGRAMS = libcallfilt

libcallfilt_SOURCES = libcallfilt.c

AM_CPPFLAGS = -DINST_PKGLIBDIR=\"$(pkglibdir)\"

check_PROGRAMS = t_exec_harness
t_exec_harness_SOURCES = t_exec_harness.c
TESTS = run_tests
