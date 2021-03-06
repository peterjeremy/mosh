#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([mosh], [1.1.94d], [mosh-devel@mit.edu])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([src/frontend/mosh-client.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PATH_PROG([PROTOC], [protoc], [])
AS_IF([test x"$PROTOC" = x],
  [AC_MSG_ERROR([cannot find protoc, the Protocol Buffers compiler])])

WARNING_CXXFLAGS=""
PICKY_CXXFLAGS=""
AC_ARG_ENABLE([compile-warnings],
  [AS_HELP_STRING([--enable-compile-warnings@<:@=no/yes/maximum/error@:>@],
     [Turn on compiler warnings])],
  [case "$enableval" in
     no)
       ;;
     '' | yes)
       WARNING_CXXFLAGS="-Wall"
       ;;
     maximum)
       WARNING_CXXFLAGS="-Wall"
       PICKY_CXXFLAGS="-Wextra -pedantic -Wno-long-long -Weffc++"
       ;;
     error)
       WARNING_CXXFLAGS="-Wall -Werror"
       PICKY_CXXFLAGS="-Wextra -pedantic -Wno-long-long -Weffc++"
       ;;
     *)
       AC_MSG_ERROR([Unknown argument '$enableval' to --enable-compile-warnings])
       ;;
   esac],
  [WARNING_CXXFLAGS="-Wall"])
AC_SUBST([WARNING_CXXFLAGS])
AC_SUBST([PICKY_CXXFLAGS])

# We want to check for compiler flag support, but there is no way to make
# clang's "argument unused" warning fatal.  So we invoke the compiler through a
# wrapper script that greps for this message.

saved_CC="$CC"
saved_CXX="$CXX"
saved_LD="$LD"
flag_wrap="$srcdir/scripts/wrap-compiler-for-flag-check"
CC="$flag_wrap $CC"
CXX="$flag_wrap $CXX"
LD="$flag_wrap $LD"

# We use the same hardening flags for C and C++.  We must check that each flag
# is supported by both compilers.
AC_DEFUN([check_cc_cxx_flag],
 [AC_LANG_PUSH(C)
  AX_CHECK_COMPILE_FLAG([$1],
   [AC_LANG_PUSH(C++)
    AX_CHECK_COMPILE_FLAG([$1], [$2], [$3], [-Werror $4])
    AC_LANG_POP(C++)],
   [$3], [-Werror $4])
  AC_LANG_POP(C)])
AC_DEFUN([check_link_flag],
 [AX_CHECK_LINK_FLAG([$1], [$2], [$3], [-Werror $4])])

AC_ARG_ENABLE([hardening],
  [AS_HELP_STRING([--enable-hardening],
    [Enable compiler and linker options to frustrate memory corruption exploits @<:@yes@:>@])],
  [hardening="$enableval"],
  [hardening="yes"])

HARDEN_CFLAGS=""
HARDEN_LDFLAGS=""
AS_IF([test x"$hardening" != x"no"], [
  check_cc_cxx_flag([-fno-strict-overflow], [HARDEN_CFLAGS="$HARDEN_CFLAGS -fno-strict-overflow"])

  # This one will likely succeed, even on platforms where it does nothing.
  check_cc_cxx_flag([-D_FORTIFY_SOURCE=2], [HARDEN_CFLAGS="$HARDEN_CFLAGS -D_FORTIFY_SOURCE=2"])

  check_cc_cxx_flag([-fstack-protector-all],
   [HARDEN_CFLAGS="$HARDEN_CFLAGS -fstack-protector-all"
    check_cc_cxx_flag([-Wstack-protector], [HARDEN_CFLAGS="$HARDEN_CFLAGS -Wstack-protector"],
      [], [-fstack-protector-all])
    check_cc_cxx_flag([--param ssp-buffer-size=1], [HARDEN_CFLAGS="$HARDEN_CFLAGS --param ssp-buffer-size=1"],
      [], [-fstack-protector-all])])

  check_cc_cxx_flag([-fPIE],
   [check_link_flag([-fPIE -pie],
     [HARDEN_CFLAGS="$HARDEN_CFLAGS -fPIE"
      HARDEN_LDFLAGS="$HARDEN_LDFLAGS -pie"],
     [check_link_flag([-fPIE -Wl,-pie],
       [HARDEN_CFLAGS="$HARDEN_CFLAGS -fPIE"
        HARDEN_LDFLAGS="$HARDEN_LDFLAGS -Wl,-pie"])])])

  check_link_flag([-Wl,-z,relro],
   [HARDEN_LDFLAGS="$HARDEN_LDFLAGS -Wl,-z,relro"
    check_link_flag([-Wl,-z,now], [HARDEN_LDFLAGS="$HARDEN_LDFLAGS -Wl,-z,now"])])])
AC_SUBST([HARDEN_CFLAGS])
AC_SUBST([HARDEN_LDFLAGS])

# Also check for a few non-hardening-related flags.
MISC_CXXFLAGS=""
AC_LANG_PUSH(C++)
AX_CHECK_COMPILE_FLAG([-fno-default-inline],
  [MISC_CXXFLAGS="$MISC_CXXFLAGS -fno-default-inline"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([-pipe],
  [MISC_CXXFLAGS="$MISC_CXXFLAGS -pipe"], [], [-Werror])
AC_LANG_POP(C++)
AC_SUBST([MISC_CXXFLAGS])

# End of flag tests.
CC="$saved_CC"
CXX="$saved_CXX"
LD="$saved_LD"

AC_ARG_ENABLE([client],
  [AS_HELP_STRING([--enable-client], [Build the mosh-client program @<:@yes@:>@])],
  [build_client="$enableval"],
  [build_client="yes"])
AM_CONDITIONAL([BUILD_CLIENT], [test x"$build_client" != xno])

AC_ARG_ENABLE([server],
  [AS_HELP_STRING([--enable-server], [Build the mosh-server program @<:@yes@:>@])],
  [build_server="$enableval"],
  [build_server="yes"])
AM_CONDITIONAL([BUILD_SERVER], [test x"$build_server" != xno])

AC_ARG_ENABLE([tests],
  [AS_HELP_STRING([--enable-tests], [Build tests])],
  [build_tests="$enableval"],
  [build_tests="no"])
AM_CONDITIONAL([BUILD_TESTS], [test x"$build_tests" != xno])

AC_ARG_ENABLE([examples],
  [AS_HELP_STRING([--enable-examples], [Build the miscellaneous programs in src/examples @<:@no@:>@])],
  [build_examples="$enableval"],
  [build_examples="no"])
AM_CONDITIONAL([BUILD_EXAMPLES], [test x"$build_examples" != xno])

# Checks for libraries.
AC_ARG_WITH([utempter],
  [AS_HELP_STRING([--with-utempter], [write utmp entries using libutempter @<:@check@:>@])],
  [with_utempter="$withval"],
  [with_utempter="check"])
AS_IF([test x"$with_utempter" != xno],
  [AC_SEARCH_LIBS([utempter_remove_record], [utempter],
    [AC_DEFINE([HAVE_UTEMPTER], [1], [Define if libutempter is available.])],
    [AS_IF([test x"$with_utempter" = xcheck],
      [AC_MSG_WARN([Unable to find libutempter; utmp entries will not be made.])],
      [AC_MSG_ERROR([--with-utempter was given but libutempter was not found.])])])])

AC_SEARCH_LIBS([compress], [z], , [AC_MSG_ERROR([Unable to find zlib.])])

AC_ARG_WITH([skalibs],
  [AS_HELP_STRING([--with-skalibs[=DIR]],
     [root directory of skalibs installation])],
  [with_skalibs=$withval
   AS_IF([test x"$withval" != xyes],
     [SKALIBS_CPPFLAGS="-I$withval/include"
      SKALIBS_LDFLAGS="-L$withval/lib"])],
  [with_skalibs=no])
AC_ARG_WITH([skalibs-include],
  [AS_HELP_STRING([--with-skalibs-include=DIR],
     [specify exact include dir for skalibs headers])],
  [SKALIBS_CPPFLAGS="-I$withval"])
AC_ARG_WITH([skalibs-libdir],
  [AS_HELP_STRING([--with-skalibs-libdir=DIR],
     [specify exact library dir for skalibs libraries])],
  [SKALIBS_LDFLAGS="-L$withval"])

STDDJB_CPPFLAGS=""
STDDJB_LDFLAGS=""
AS_IF([test x"$with_skalibs" != xno],
  [AX_CHECK_LIBRARY([SKALIBS], [selfpipe.h], [stddjb], [],
     [AC_MSG_ERROR([Unable to find skalibs.])])
   AC_SUBST([STDDJB_CPPFLAGS], ["$SKALIBS_CPPFLAGS"])
   AC_SUBST([STDDJB_LDFLAGS], ["$SKALIBS_LDFLAGS -lstddjb"])])

have_signalfd="no"
AC_CHECK_DECL([signalfd],
  [have_signalfd="yes"
   AC_DEFINE([HAVE_SIGNALFD], [1],
     [Define if signalfd is available.])],
  , [[#include <sys/signalfd.h>]])

AS_IF([test x"$have_signalfd" = xno],
  [AC_DEFINE([USE_LIBSTDDJB], [1],
    [Define if we should call functions from libstddjb (part of skalibs)])])
AM_CONDITIONAL([USE_LIBSTDDJB],
  [test x"$have_signalfd" = xno])

# Build the bundled libstddjb only if we'll use it and we don't have a
# path for skalibs.
AM_CONDITIONAL([COND_THIRD_LIBSTDDJB],
  [test x"$have_signalfd" = xno && test x"$with_skalibs" = xno])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h langinfo.h limits.h locale.h netinet/in.h stddef.h stdint.h inttypes.h stdlib.h string.h sys/ioctl.h sys/resource.h sys/socket.h sys/stat.h sys/time.h termios.h unistd.h wchar.h wctype.h], [], [AC_MSG_ERROR([Missing required header file.])])

AC_CHECK_HEADERS([pty.h util.h libutil.h paths.h])
AC_CHECK_HEADERS([endian.h sys/endian.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINTPTR_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MBRTOWC
AC_CHECK_FUNCS([gettimeofday setrlimit inet_ntoa iswprint memchr memset nl_langinfo posix_memalign setenv setlocale sigaction socket strchr strdup strerror strtol wcwidth])

AC_SEARCH_LIBS([clock_gettime], [rt], [AC_DEFINE([HAVE_CLOCK_GETTIME], [1], [Define if clock_gettime is available.])])

AS_IF([test x"$__CYGWIN__" != x && test x"$TINFO_CPPFLAGS" = x],
  [TINFO_CPPFLAGS="-I/usr/include/ncurses"])

PKG_CHECK_MODULES([TINFO], [tinfo], ,
  [PKG_CHECK_MODULES([TINFO], [ncurses], ,
     [AX_CHECK_LIBRARY([TINFO], [curses.h], [tinfo],
        [AC_SUBST([TINFO_CFLAGS], ["$TINFO_CPPFLAGS"])
         AC_SUBST([TINFO_LIBS], ["$TINFO_LDFLAGS -ltinfo"])],
        [AX_CHECK_LIBRARY([NCURSES], [curses.h], [ncurses],
           [AC_SUBST([TINFO_CFLAGS], ["$TINFO_CPPFLAGS"])
            AC_SUBST([TINFO_LIBS], ["$TINFO_LDFLAGS -lncurses"])],
           [AC_MSG_ERROR([Unable to find libtinfo or libncurses])])])])])

AC_CHECK_DECL([forkpty],
  [AC_DEFINE([FORKPTY_IN_LIBUTIL], [1],
     [Define if libutil.h necessary for forkpty().])],
  , [[#include <libutil.h>]])

AC_ARG_VAR([poll_CFLAGS], [C compiler flags for poll])
AC_ARG_VAR([poll_LIBS], [linker flags for poll])
AS_IF([test -z "$poll_LIBS"], [
  AC_CHECK_LIB([poll], [poll], [poll_LIBS="-lpoll"])
])

# Check to make sure poll() can handle stdin and ptys
AC_CACHE_CHECK([whether poll can handle ptys],
               [ac_cv_poll_pty],
[
  save_CFLAGS="$CFLAGS"
  save_LIBS="$LIBS"

  AS_IF([test "x$poll_CFLAGS" != "x"],
    [CFLAGS="$CFLAGS $poll_CFLAGS"])

  AS_IF([test "x$poll_LIBS" != "x"],
    [LIBS="$LIBS $poll_LIBS"])

  LIBS="$LIBS -lutil"

  AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#if HAVE_PTY_H
#include <pty.h>
#elif HAVE_UTIL_H
#include <util.h>
#endif
#if FORKPTY_IN_LIBUTIL
#include <libutil.h>
#endif
#include <sys/poll.h>

int master, slave;
struct pollfd pollfds[ 1 ];
]], [[
if ( openpty( &master, &slave, NULL, NULL, NULL ) < 0 ) {
   perror( "openpty" );
   exit( 1 );
}
pollfds[ 0 ].fd = master;
pollfds[ 0 ].events = POLLIN;
int active_fds = poll( pollfds, 1, 100 );
if ( active_fds < 0 ) {
   perror( "poll" );
   exit( 1 );
}
if ( pollfds[ 0 ].revents & (POLLERR | POLLHUP | POLLNVAL) ) {
   exit( 2 );
}
]])],
	[ac_cv_poll_pty=yes],
	[ac_cv_poll_pty=no])
  CFLAGS="$save_CFLAGS"
  LIBS="$save_LIBS"
])
AM_CONDITIONAL([COND_THIRD_POLL], [test "x$ac_cv_poll_pty" = "xno"])
AM_COND_IF([COND_THIRD_POLL],
  [
    poll_CFLAGS="-I\$(top_srcdir)/third/poll"
    poll_LIBS="\$(top_builddir)/third/poll/libpoll.a"
  ])

AC_MSG_CHECKING([whether pipe2(..., O_CLOEXEC) is supported])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
int pipefd[2];
]], [[(void) pipe2(pipefd, O_CLOEXEC);]])],
  [AC_DEFINE([HAVE_PIPE2_CLOEXEC], [1],
     [Define if pipe2(..., O_CLOEXEC) is available.])
   AC_MSG_RESULT([yes])],
  [AC_MSG_RESULT([no])])

AC_CHECK_DECLS([__builtin_bswap64, __builtin_ctz])

AC_CHECK_DECL([mach_absolute_time],
  [AC_DEFINE([HAVE_MACH_ABSOLUTE_TIME], [1],
     [Define if mach_absolute_time is available.])],
  , [[#include <mach/mach_time.h>]])

AC_CHECK_DECL([htobe64],
  [AC_DEFINE([HAVE_HTOBE64], [1],
  [Define if htobe64 and friends exist.])],
  [AC_CHECK_DECL([OSSwapHostToBigInt64],
     [AC_DEFINE([HAVE_OSX_SWAP], [1],
        [Define if OSSwapHostToBigInt64 and friends exist.])],
     [AC_MSG_WARN([Unable to find byte swapping functions; using built-in routines.])],
     [[#include <libkern/OSByteOrder.h>]])],
  [[#if defined(HAVE_ENDIAN_H)
#include <endian.h>
#elif defined(HAVE_SYS_ENDIAN_H)
#include <sys/endian.h>
#endif]])

AC_CHECK_DECL([IP_MTU_DISCOVER],
  [AC_DEFINE([HAVE_IP_MTU_DISCOVER], [1],
     [Define if IP_MTU_DISCOVER is a valid sockopt.])],
  , [[#include <netinet/ip.h>]])

AC_CHECK_DECL([__STDC_ISO_10646__],
  [],
  [AC_MSG_WARN([C library doesn't advertise wchar_t is Unicode (OS X works anyway with workaround).])],
  [[#include <wchar.h>]])

AC_CHECK_DECL([IUTF8],
  [AC_DEFINE([HAVE_IUTF8], [1],
     [Define if IUTF8 is a defined termios mode.])],
  [AC_MSG_WARN([No IUTF8 termios mode; character-erase of multibyte character sequence probably does not work properly in canonical mode on this platform.])],
  [[#include <termios.h>]])

# Checks for protobuf
PKG_CHECK_MODULES([protobuf], [protobuf])

AC_CONFIG_FILES([
  Makefile
  third/Makefile
  third/libstddjb/Makefile
  third/poll/Makefile
  src/Makefile
  src/crypto/Makefile
  src/frontend/Makefile
  src/network/Makefile
  src/protobufs/Makefile
  src/statesync/Makefile
  src/terminal/Makefile
  src/util/Makefile
  scripts/Makefile
  src/examples/Makefile
  src/tests/Makefile
  man/Makefile
])
AC_OUTPUT
