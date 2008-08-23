# -%- lang: makefile; indent-width: 8; use-tabs: true -%-
# UGLY_MK_TRIGGER = UGLY_LIBS

noinst_LTLIBRARIES =
noinst_LIBRARIES =

if MOO_INSTALL_LIB
noinst_LTLIBRARIES += @MODULE@.la
else
noinst_LIBRARIES += @MODULE@.a
endif

@MODULE@_la_SOURCES = $(@MODULE@_SOURCES_)
@MODULE@_a_SOURCES = $(@MODULE@_SOURCES_)

nodist_@MODULE@_la_SOURCES = $(nodist_@MODULE@_SOURCES_)
nodist_@MODULE@_a_SOURCES = $(nodist_@MODULE@_SOURCES_)

@MODULE@_la_LIBADD = $(nodist_@MODULE@_LIBADD_)
@MODULE@_a_LIBADD = $(nodist_@MODULE@_LIBADD_)

@MODULE@_la_LDFLAGS = $(nodist_@MODULE@_LDFLAGS_)

AM_CFLAGS = $(AM_CFLAGS_)
@MODULE@_a_CFLAGS = $(AM_CFLAGS_)

AM_CXXFLAGS = $(AM_CXXFLAGS_)
@MODULE@_a_CXXFLAGS = $(AM_CXXFLAGS_)
