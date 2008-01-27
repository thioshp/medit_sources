AC_DEFUN([MOO_LUA],[
  AC_REQUIRE([MOO_AC_CHECK_OS])

  MOO_LUA_CFLAGS=

  if $MOO_BUILD_LUA; then
    if test "x$MOO_OS_DARWIN" = "xyes"; then
      MOO_LUA_CFLAGS="-DLUA_USE_MACOSX"
    elif test "x$MOO_OS_MINGW" != "xyes"; then
      MOO_LUA_CFLAGS="-DLUA_USE_POSIX -DLUA_USE_DLOPEN"
    fi
  fi

  AC_SUBST([MOO_LUA_CFLAGS])
])
