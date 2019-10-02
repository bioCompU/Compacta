#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Compacta], [1.0], [fgrazom@gmail.com])
AC_CONFIG_SRCDIR([src/Compacta.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
CXXFLAGS="-DVERSION=1.0 -O3 -ffast-math -I./ $CXXFLAGS $CPPFLAGS"

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)
AC_OPENMP
AC_SUBST(AM_CXXFLAGS,"$OPENMP_CXXFLAGS")

AC_ARG_WITH(sam-tools, [ --with-sam_tools
	 Specify the main directory of the samtools 1.0])
AC_ARG_WITH(z-lib, [ --with_zlib
	In Case zlib is not automatically detected, specify the directory where libz.a is located])	 
AC_ARG_WITH(pthread-lib, [ --with-pthread_lib
	In case pthread lib is not automatically detected, specify the directory where libpthread.so is located])

if test "$with_sam_tools" != ""
then
	HTSLIB=`echo $with_sam_tools/htslib-*.*`
        CXXFLAGS="-I$with_sam_tools -I$HTSLIB $CXXFLAGS"
	LIBS="$LIBS $HTSLIB/libhts.a"
	LDFLAGS="-L$with_sam_tools $LDFLAGS"
	LD_RUN_PATH="$LD_RUN_PATH:$with_sam_tools"
fi

if test "$with_zlib" != ""
then
	LIBS="$LIBS $with_zlib/libz.a"
fi

if test "$with_pthread_lib" != ""
then
	PTHREAD_LIB=`echo $with_pthread_lib/libpthread*.so`
	LIBS="$LIBS $PTHREAD_LIB"
fi

# Checks for libraries.
AC_CHECK_HEADERS([bam.h sam.h], [], [
 	echo  "Error, could not find samtools headers, please specify a valid samtools 1.0 location with: " &&
        echo  "./configure --with-sam_tools=<directory of sam tools containing sam.h and bam.h>"
	#exit -1
])
AC_CHECK_LIB([bam], [main],[],[
	echo "Error, could not find samtools bam lib, please specify a valid samtools 1.0 location with:" &&
	echo "./configure --with-sam_tools=<directory of sam tools containing bam lib>" 
#	exit -1
])
AC_CHECK_LIB([pthread], [main],[],[
	echo "Error, could not find pthread library, please install pthread library" &&
	echo "if library is installed but not detected, please specify libpthread.so location with: " &&
	echo "./configure --with-pthread_lib=<Directory of pthread where libpthread.so is located>"
#	exit -1
])

AC_CHECK_LIB([z], [main], [],[
	echo "Error, Could not fin zlib library, please install zlib or " &&
	echo "if zlib is installed but not detected, please specify libz.a location with: " &&
	echo "./configure --with-zlib=<Directory where libz.a file is located>"
#	exit -1 
])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
tempflags="$CXXFLAGS"
CXXFLAGS=-std=c++0x
AC_MSG_CHECKING([ if compiler supports -std=c++0x])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],[tempflags="$tempflags -std=c++0x -DUNORDEREDMAP"])
CXXFLAGS="$tempflags"

AC_SUBST(LD_RUN_PATH,$LD_RUN_PATH)
AC_OUTPUT
