AC_DEFUN_ONCE([_MOO_AC_CONFIGARGS_H],[
moo_ac_configure_args=`echo "$ac_configure_args" | sed 's/^ //; s/\\""\`\$/\\\\&/g'`
cat >configargs.h.tmp <<EOF
static const char configure_args@<:@@:>@ = "$moo_ac_configure_args";
EOF
cmp -s configargs.h configargs.h.tmp || mv configargs.h.tmp configargs.h
AC_DEFINE(HAVE_CONFIGARGS_H, 1, [configargs.h is created])
])

AC_DEFUN_ONCE([MOO_AC_PRIV_FLAGS],[
  MOO_AC_FLAGS

  MOO_AC_FUNCS
  MOO_AC_FAM
  MOO_AC_XML
  MOO_AC_PCRE
  MOO_AC_PYTHON
  dnl must be called after MOO_AC_PYTHON
  MOO_AC_LIB

  AC_DEFINE(MOO_COMPILATION, 1, [must be 1])

  RENDER_LIBS=
  if $GDK_X11; then
    AC_CHECK_LIB(Xrender, XRenderFindFormat,[
      RENDER_LIBS="-lXrender -lXext" # XXX what the heck is this?
      AC_DEFINE(HAVE_RENDER, 1, [Define if libXrender is available.])
    ],[
      :
    ],[-lXext])

    MOO_LIBS="$MOO_LIBS $X_LIBS $X_EXTRA_LIBS -lSM -lICE"
  fi

  MOO_CFLAGS="$MOO_CFLAGS $MOO_PCRE_CFLAGS -DXDG_PREFIX=_moo_edit_xdg -DG_LOG_DOMAIN=\\\"Moo\\\""

  if test "x$GLIB_2_16" = xyes; then
    PKG_CHECK_MODULES(GIO,[gio-2.0],[:],[:])
  fi

  MOO_LIBS="$MOO_LIBS $GTK_LIBS $GTHREAD_LIBS $GIO_LIBS $MOO_PCRE_LIBS -lm"

  if test "x$build_mooedit" != "xno"; then
    MOO_CFLAGS="$MOO_CFLAGS $XML_CFLAGS"
    MOO_LIBS="$MOO_LIBS $XML_LIBS"
  fi

  MOO_CFLAGS="-I`cd "$srcdir/doc" && pwd` $MOO_CFLAGS"

  if test x$MOO_USE_FAM = xyes; then
    MOO_CFLAGS="$MOO_CFLAGS $MOO_FAM_CFLAGS"
    MOO_LIBS="$MOO_LIBS $MOO_FAM_LIBS"
  fi

  MOO_CXXFLAGS="$MOO_CFLAGS"

  if $MOO_GCC; then
    _MOO_AC_CHECK_COMPILER_OPTIONS(MOO_CFLAGS,[-fno-strict-aliasing],[verbose])
    _MOO_AC_CHECK_CPP_COMPILER_OPTIONS(MOO_CXXFLAGS,[-fno-strict-aliasing],[verbose])
#     _MOO_AC_CHECK_CPP_COMPILER_OPTIONS(MOO_CXXFLAGS,[-fno-exceptions],[verbose])
  fi

  MOO_CXXFLAGS="$MOO_CXXFLAGS $MOO_DEBUG_CXXFLAGS"
  MOO_CFLAGS="$MOO_CFLAGS $MOO_DEBUG_CFLAGS"

  AC_SUBST(MOO_LIBS)
])

##############################################################################
# MOO_AC_FLAGS(moo_top_dir)
#
AC_DEFUN_ONCE([MOO_AC_FLAGS],[
  AC_REQUIRE([MOO_AC_CHECK_OS])
  AC_REQUIRE([MOO_AC_SET_DIRS])

  MOO_PKG_CHECK_GTK_VERSIONS

  if $GDK_X11; then
    AC_PATH_XTRA
  fi

  MOO_AC_DEBUG

  if test x$MOO_OS_DARWIN = xyes; then
    _moo_ac_have_carbon=no
    AC_MSG_CHECKING([for Mac OS X Carbon support])
    AC_TRY_CPP([
      #include <Carbon/Carbon.h>
      #include <CoreServices/CoreServices.h>
    ],[
      _moo_ac_have_carbon=yes
      AC_DEFINE(HAVE_CARBON, 1, [Mac OS X Carbon])
      LDFLAGS="$LDFLAGS -framework Carbon"
    ])
    AC_MSG_RESULT([$_moo_ac_have_carbon])
  fi

  if $GDK_QUARTZ; then
    PKG_CHECK_MODULES(IGE_MAC,ige-mac-integration)
    GTK_CFLAGS="$IGE_MAC_CFLAGS"
    GTK_LIBS="$IGE_MAC_LIBS"
    LDFLAGS="$LDFLAGS -framework Cocoa"
  fi

  moo_srcdir=`cd "$srcdir/moo" && pwd`

  MOO_CFLAGS="$MOO_CFLAGS $GTK_CFLAGS -I$moo_srcdir"

  for gv in 8 12 14 16; do
    eval "var=\$GLIB_2_$gv"
    if test "x$var" != xyes; then
      MOO_CFLAGS="-I$moo_srcdir/mooutils/newgtk/glib-2.$gv $MOO_CFLAGS"
    fi
  done

  MOO_WIN32_CFLAGS=
  if test "x$MOO_OS_MINGW" = xyes; then
    MOO_CFLAGS="$MOO_CFLAGS -DWIN32_LEAN_AND_MEAN -DUNICODE"
    MOO_WIN32_CFLAGS="-I$moo_srcdir/mooutils/moowin32/mingw"
    AC_DEFINE(HAVE_MMAP, [1], [using fake mmap on windows])

    # gettimeofday is present in recent mingw
    AC_CHECK_FUNC(gettimeofday,[:],[
      MOO_WIN32_CFLAGS="$MOO_WIN32_CFLAGS -I$moo_srcdir/mooutils/moowin32/ms"
    ])
  fi

  AC_SUBST(MOO_WIN32_CFLAGS)
  AC_SUBST(MOO_CXXFLAGS)
  AC_SUBST(MOO_CFLAGS)

  _MOO_AC_CONFIGARGS_H
])
