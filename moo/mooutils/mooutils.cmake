SET(moo_utils_enum_headers
	mooutils/moodialogs.h
	mooutils/moofiledialog.h
	mooutils/moouixml.h
	mooutils/moowindow.h
)

SET(mooutils_sources
	mooutils/mooutils.cmake
	${moo_utils_enum_headers}
	mooutils/mooarray.h
	mooutils/mooutils-thread.c
	mooutils/mooutils-thread.h
	mooutils/moohistorymgr.c
	mooutils/moohistorymgr.h
	mooutils/moo-environ.h
	mooutils/mooaccel.c
	mooutils/mooaccel.h
	mooutils/mooaccelbutton.c
	mooutils/mooaccelbutton.h
	mooutils/mooaccelprefs.c
	mooutils/mooaccelprefs.h
	mooutils/mooaction-private.h
	mooutils/mooaction.c
	mooutils/mooaction.h
	mooutils/mooactionbase-private.h
	mooutils/mooactionbase.c
	mooutils/mooactionbase.h
	mooutils/mooactioncollection.c
	mooutils/mooactioncollection.h
	mooutils/mooactionfactory.c
	mooutils/mooactionfactory.h
	mooutils/mooactiongroup.c
	mooutils/mooactiongroup.h
	mooutils/mooapp-ipc.c
	mooutils/mooapp-ipc.h
	mooutils/mooappinput.c
	mooutils/mooappinput.h
	mooutils/mooatom.h
	mooutils/moobigpaned.c
	mooutils/moobigpaned.h
	mooutils/mooclosure.c
	mooutils/mooclosure.h
	mooutils/moocombo.c
	mooutils/moocombo.h
	mooutils/moocompat.h
	mooutils/moodialogs.c
	mooutils/mooeditops.c
	mooutils/mooeditops.h
	mooutils/mooencodings-data.h
	mooutils/mooencodings.c
	mooutils/mooencodings.h
	mooutils/mooentry.c
	mooutils/mooentry.h
	mooutils/moofiledialog.c
	mooutils/moofileicon.c
	mooutils/moofileicon.h
	mooutils/moofilewatch.c
	mooutils/moofilewatch.h
	mooutils/moofilewriter.c
	mooutils/moofilewriter.h
	mooutils/moofilewriter-private.h
	mooutils/moofiltermgr.c
	mooutils/moofiltermgr.h
	mooutils/moofontsel.c
	mooutils/moofontsel.h
	mooutils/mooglade.c
	mooutils/mooglade.h
	mooutils/moohelp.c
	mooutils/moohelp.h
	mooutils/moohistorycombo.c
	mooutils/moohistorycombo.h
	mooutils/moohistorylist.c
	mooutils/moohistorylist.h
	mooutils/mooi18n.c
	mooutils/mooi18n.h
	mooutils/moolist.h
	mooutils/moomarkup.c
	mooutils/moomarkup.h
	mooutils/moomenu.c
	mooutils/moomenu.h
	mooutils/moomenuaction.c
	mooutils/moomenuaction.h
	mooutils/moomenumgr.c
	mooutils/moomenumgr.h
	mooutils/moomenutoolbutton.c
	mooutils/moomenutoolbutton.h
	mooutils/moo-mime.c
	mooutils/moo-mime.h
	mooutils/moonotebook.c
	mooutils/moonotebook.h
	mooutils/mooonce.h
	mooutils/moopane.c
	mooutils/moopane.h
	mooutils/moopaned.c
	mooutils/moopaned.h
	mooutils/mooprefs.c
	mooutils/mooprefs.h
	mooutils/mooprefsdialog.c
	mooutils/mooprefsdialog.h
	mooutils/mooprefspage.c
	mooutils/mooprefspage.h
	mooutils/moospawn.c
	mooutils/moospawn.h
	mooutils/moostock.c
	mooutils/moostock.h
	mooutils/mootype-macros.h
	mooutils/moouixml.c
	mooutils/mooundo.c
	mooutils/mooundo.h
	mooutils/mooutils.h
	mooutils/mooutils-debug.h
	mooutils/mooutils-enums.c
	mooutils/mooutils-enums.h
	mooutils/mooutils-file.c
	mooutils/mooutils-file.h
	mooutils/mooutils-fs.c
	mooutils/mooutils-fs.h
	mooutils/mooutils-gobject-private.h
	mooutils/mooutils-gobject.c
	mooutils/mooutils-gobject.h
	mooutils/mooutils-macros.h
	mooutils/mooutils-mem.h
	mooutils/mooutils-messages.h
	mooutils/mooutils-misc.c
	mooutils/mooutils-misc.h
	mooutils/mooutils-script.c
	mooutils/mooutils-script.h
	mooutils/mooutils-tests.h
	mooutils/mooutils-treeview.c
	mooutils/mooutils-treeview.h
	mooutils/moowindow.c
	mooutils/stock-file-24.h
	mooutils/stock-file-selector-24.h
	mooutils/stock-terminal-24.h
)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/genmarshals_c.cmake
"FILE(WRITE marshals.c.tmp \"#include \\\"marshals.h\\\"\")
EXECUTE_PROCESS(COMMAND ${GLIB_GENMARSHAL} --prefix=_moo_marshal --body ${CMAKE_CURRENT_SOURCE_DIR}/marshals.list
    OUTPUT_VARIABLE _marshals_body)
FILE(APPEND marshals.c.tmp \"\${_marshals_body}\")
file(RENAME marshals.c.tmp marshals.c)
")
add_custom_command(OUTPUT marshals.c
    COMMAND ${CMAKE_COMMAND} -P genmarshals_c.cmake
    MAIN_DEPENDENCY marshals.list)
LIST(APPEND built_mooutils_sources marshals.c)


# if MOO_DEV_MODE
# built_moo_sources += mooutils/mooutils-enums.h.stamp mooutils/mooutils-enums.c.stamp
# mooutils/mooutils-enums.h.stamp: $(moo_utils_enum_headers) Makefile mooutils/mooutils-enums.tmpl.h
# 	$(AM_V_at)$(MKDIR_P) mooutils
# 	$(AM_V_GEN)( cd $(srcdir) && \
# 		$(GLIB_MKENUMS) --template mooutils/mooutils-enums.tmpl.h $(moo_utils_enum_headers) ) > mooutils/mooutils-enums.h.tmp
# 	$(AM_V_at)cmp -s mooutils/mooutils-enums.h.tmp $(srcdir)/mooutils/mooutils-enums.h || \
# 		mv mooutils/mooutils-enums.h.tmp $(srcdir)/mooutils/mooutils-enums.h
# 	$(AM_V_at)rm -f mooutils/mooutils-enums.h.tmp
# 	$(AM_V_at)echo stamp > mooutils/mooutils-enums.h.stamp
# mooutils/mooutils-enums.c.stamp: $(moo_utils_enum_headers) Makefile mooutils/mooutils-enums.tmpl.c
# 	$(AM_V_at)$(MKDIR_P) mooutils
# 	$(AM_V_GEN)( cd $(srcdir) && \
# 		$(GLIB_MKENUMS) --template mooutils/mooutils-enums.tmpl.c $(moo_utils_enum_headers) ) > mooutils/mooutils-enums.c.tmp
# 	$(AM_V_at)cmp -s mooutils/mooutils-enums.c.tmp $(srcdir)/mooutils/mooutils-enums.c || \
# 		mv mooutils/mooutils-enums.c.tmp $(srcdir)/mooutils/mooutils-enums.c
# 	$(AM_V_at)rm -f mooutils/mooutils-enums.c.tmp
# 	$(AM_V_at)echo stamp > mooutils/mooutils-enums.c.stamp
# endif

SET(mooutils_win32_sources
    mooutils/mooutils-win32.c
)
SET(mooutils_unittest_sources
    mooutils/moo-test-utils.c
    mooutils/moo-test-utils.h
    mooutils/moo-test-macros.h
)

LIST(APPEND mooutils_extra_dist ${mooutils_win32_sources} ${mooutils_unittest_sources})

if(WIN32)
    LIST(APPEND mooutils_sources ${mooutils_win32_sources})
endif()

LIST(APPEND mooutils_sources ${mooutils_unittest_sources})

foreach(input_file
    mooutils/glade/mooaccelprefs.glade
    mooutils/glade/mooaccelbutton.glade
    mooutils/glade/moologwindow.glade
)
    ADD_GXML(${input_file})
endforeach(input_file)

SET(PIXMAPS
    mooutils/pixmaps/hide.png
    mooutils/pixmaps/close.png
    mooutils/pixmaps/sticky.png
    mooutils/pixmaps/detach.png
    mooutils/pixmaps/attach.png
    mooutils/pixmaps/keepontop.png
    mooutils/pixmaps/medit.png
)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/genpixbufs.cmake
"EXECUTE_PROCESS(COMMAND ${GDK_PIXBUF_CSOURCE} --static --build-list 
    MOO_HIDE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/hide.png
    MOO_CLOSE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/close.png
    MOO_STICKY_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/sticky.png
    MOO_DETACH_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/detach.png
    MOO_ATTACH_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/attach.png
    MOO_KEEP_ON_TOP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/keepontop.png
    MEDIT_ICON ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/medit.png
    SYMLINK_ARROW ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/symlink.png
    SYMLINK_ARROW_SMALL ${CMAKE_CURRENT_SOURCE_DIR}/mooutils/pixmaps/symlink-small.png
    OUTPUT_FILE moo-pixbufs.h.tmp)
file(RENAME moo-pixbufs.h.tmp moo-pixbufs.h)
")
add_custom_command(OUTPUT moo-pixbufs.h
    COMMAND ${CMAKE_COMMAND} -P genpixbufs.cmake
    DEPENDS )
LIST(APPEND built_mooutils_sources moo-pixbufs.h)

# built_moo_sources += stock-moo.h moofileicon-symlink.h
# EXTRA_DIST += $(PIXMAPS)
# stock-moo.h: $(PIXMAPS)
# 	$(AM_V_GEN)$(GDK_PIXBUF_CSOURCE) --static --build-list \
# 		MOO_HIDE_ICON $(moo_srcdir)/mooutils/pixmaps/hide.png \
# 		MOO_CLOSE_ICON $(moo_srcdir)/mooutils/pixmaps/close.png \
# 		MOO_STICKY_ICON $(moo_srcdir)/mooutils/pixmaps/sticky.png \
# 		MOO_DETACH_ICON $(moo_srcdir)/mooutils/pixmaps/detach.png \
# 		MOO_ATTACH_ICON $(moo_srcdir)/mooutils/pixmaps/attach.png \
# 		MOO_KEEP_ON_TOP_ICON $(moo_srcdir)/mooutils/pixmaps/keepontop.png \
# 		MEDIT_ICON $(moo_srcdir)/mooutils/pixmaps/medit.png \
# 			> $@.tmp && mv $@.tmp $@
#
# EXTRA_DIST += mooutils/pixmaps/symlink.png mooutils/pixmaps/symlink-small.png
# moofileicon-symlink.h: mooutils/pixmaps/symlink.png mooutils/pixmaps/symlink-small.png
# 	$(AM_V_GEN)$(GDK_PIXBUF_CSOURCE) --static --build-list \
# 			> $@.tmp && mv $@.tmp $@
