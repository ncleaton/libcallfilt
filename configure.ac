AC_INIT([libcallfilt], [1.0])
AC_CONFIG_SRCDIR([libcallfilt.c])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

dnl Check for execveat
AC_MSG_CHECKING(for execveat)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <unistd.h>]],
 [[ execveat(0, NULL, NULL, NULL, 0) ]])],
 [ AC_MSG_RESULT(yes); AC_DEFINE(HAVE_EXECVEAT, 1, [Define this symbol if you have execveat]) ],
 [ AC_MSG_RESULT(no)]
)

dnl Check for posix_spawn
AC_MSG_CHECKING(for posix_spawn)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <spawn.h>]],
 [[ posix_spawn(NULL, NULL, NULL, NULL, NULL, NULL) ]])],
 [ AC_MSG_RESULT(yes); AC_DEFINE(HAVE_POSIX_SPAWN, 1, [Define this symbol if you have posix_spawn]) ],
 [ AC_MSG_RESULT(no)]
)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
