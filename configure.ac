#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.




AC_PREREQ([2.64])
AC_INIT([avm], [0.1-alpha], [antonio.natale97@hotmail.com])
AC_CONFIG_SRCDIR([src/main.c])


AU_ALIAS([AC_PROG_JAVAC], [AX_PROG_JAVAC])
AC_DEFUN([AX_PROG_JAVAC],[
m4_define([m4_ax_prog_javac_list],["gcj -C" guavac jikes javac])
AS_IF([test "x$JAVAPREFIX" = x],
      [test "x$JAVAC" = x && AC_CHECK_PROGS([JAVAC], [m4_ax_prog_javac_list])],
      [test "x$JAVAC" = x && AC_CHECK_PROGS([JAVAC], [m4_ax_prog_javac_list], [], [$JAVAPREFIX/bin])])
m4_undefine([m4_ax_prog_javac_list])
test "x$JAVAC" = x && AC_MSG_ERROR([no acceptable Java compiler found in \$PATH])
AC_PROVIDE([$0])
])








# Checks for programs.
AC_PROG_CC
AC_PROG_JAVAC




AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [Compile in debug mode]), [
	case $enableval in
		no) debug=false ;;
		yes) debug=true ;;
	esac
], debug=false)


AC_ARG_ENABLE(jni, AS_HELP_STRING([--disable-jni], [Disable Java Native Interface (Require libffi)]), [
	case $enableval in
		no) jni=false ;;
		yes) jni=true ;;
	esac
], jni=true)

AC_ARG_ENABLE(jar, AS_HELP_STRING([--disable-jar], [Disable Java Archive Interface (Require libzip, zlib)]), [
	case $enableval in
		no) jar=false ;;
		yes) jar=true ;;
	esac
], jar=true)

AC_ARG_ENABLE(gc, AS_HELP_STRING([--disable-gc], [Disable Garbage Collection (Require TinyGC or Boehm-GC)]), [
	case $enableval in
		no) gc=false ;;
		yes) gc=true ;;
	esac
], gc=true)

AC_ARG_WITH(sysroot, AS_HELP_STRING([--with-sysroot[=PATH]], [Search for dependent libraries within DIR (or the compiler's sysroot if not specified).]), [
	sysroot=$withval
], sysroot="")


AC_DEFINE_UNQUOTED([CONFIG_SYSROOT], ["$sysroot"])
AC_DEFINE_UNQUOTED([APP_CC], ["$CC"])

AC_DEFINE([APP_NAME], ["avm"])
AC_DEFINE([APP_VERSION], ["0.1-alpha"])
AC_DEFINE([APP_COPY], ["2015 Antonio Natale"])

AC_SUBST([CONFIG_SYSROOT], $sysroot)
AC_SUBST([CONFIG_PACKAGE], "avm")
AC_SUBST([CONFIG_VERSION], "0.1-alpha")
AC_SUBST([CONFIG_COPY], "2015 Antonio Natale")



if test $jni == true
then
	AC_CHECK_LIB([ffi], [ffi_call])
	AC_CHECK_HEADERS([ffi.h])
fi

if test $jar == true	
then
	AC_CHECK_LIB([z], [inflate])
	AC_CHECK_LIB([zip], [zip_open])
	AC_CHECK_HEADERS([zip.h])
fi

if test $gc == true
then
	AC_CHECK_LIB([gc], [GC_malloc])
	AC_CHECK_HEADERS([gc.h])
fi

if test $debug == true
then
	AC_DEFINE([DEBUG], [1])
fi


AC_SUBST([CONFIG_LIBS], $LIBS)


# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdarg.h stdint.h sys/types.h stdarg.h stdlib.h stdio.h string.h math.h fcntl.h unistd.h errno.h sched.h])



AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT
