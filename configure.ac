dnl Process this file with autoconf to produce a configure script.

AC_INIT
AC_CONFIG_SRCDIR([ray_trace.c])
AM_INIT_AUTOMAKE(ray_trace, 1.0)
AM_CONFIG_HEADER(config.h)

smr_WITH_BUILD_PATH

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_PROG_CC
dnl AC_PROG_CXX

AC_PROG_LIBTOOL

dnl Find all the X stuff - needed for bicgl
AC_PATH_XTRA

mni_REQUIRE_BICPL
dnl check to make sure that this version of bicpl contains the additions 
dnl from what used to be libdavid
AC_CHECK_HEADER(bicpl/deform.h,,AC_MSG_ERROR([cannot find deform.h - make sure you have bicpl 1.4.0 or later]))

# time to hunt for OpenGL
AX_CHECK_GL
AX_CHECK_GLU
AX_CHECK_GLUT

AC_SUBST(GL_LIBS)
AC_SUBST(GL_CFLAGS)
AC_SUBST(GLU_LIBS)
AC_SUBST(GLU_CFLAGS)
AC_SUBST(GLUT_LIBS)
AC_SUBST(GLUT_CFLAGS)


AC_CONFIG_FILES([Makefile
Graphics/Makefile
Graphics/GLUT_windows/Makefile
Graphics/G_graphics/Makefile
Graphics/OpenGL_graphics/Makefile])
AC_OUTPUT

