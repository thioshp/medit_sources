/*
 *   mooedit-impl.h
 *
 *   Copyright (C) 2004-2010 by Yevgen Muntyan <emuntyan@sourceforge.net>
 *
 *   This file is part of medit.  medit is free software; you can
 *   redistribute it and/or modify it under the terms of the
 *   GNU Lesser General Public License as published by the
 *   Free Software Foundation; either version 2.1 of the License,
 *   or (at your option) any later version.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with medit.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MOO_EDIT_IMPL_H
#define MOO_EDIT_IMPL_H

#include "mooedit/moolinemark.h"
#include "mooedit/mooeditor.h"
#include "mooedit/mootextview.h"
#include "mooutils/moohistorymgr.h"
#include <gio/gio.h>

G_BEGIN_DECLS

#define MOO_EDIT_IS_BUSY(edit) (_moo_edit_view_get_state (moo_edit_get_view (edit)) != MOO_EDIT_STATE_NORMAL)

extern MooEditList *_moo_edit_instances;

char       *_moo_file_get_normalized_name       (GFile          *file);
char       *_moo_edit_get_normalized_name       (MooEdit        *edit);

char       *_moo_edit_get_utf8_filename         (MooEdit        *edit);

void        _moo_edit_add_class_actions         (MooEdit        *edit);
void        _moo_edit_check_actions             (MooEdit        *edit);
void        _moo_edit_class_init_actions        (MooEditClass   *klass);

void        _moo_edit_status_changed            (MooEdit        *edit);

gboolean    _moo_edit_has_comments              (MooEdit        *edit,
                                                 gboolean       *single_line,
                                                 gboolean       *multi_line);
void        _moo_edit_comment                   (MooEdit        *edit);
void        _moo_edit_uncomment                 (MooEdit        *edit);

#define MOO_EDIT_GOTO_BOOKMARK_ACTION "GoToBookmark"
void        _moo_edit_delete_bookmarks          (MooEdit        *edit,
                                                 gboolean        in_destroy);
void        _moo_edit_line_mark_moved           (MooEdit        *edit,
                                                 MooLineMark    *mark);
void        _moo_edit_line_mark_deleted         (MooEdit        *edit,
                                                 MooLineMark    *mark);
gboolean    _moo_edit_line_mark_clicked         (MooTextView    *view,
                                                 int             line);
void        _moo_edit_update_bookmarks_style    (MooEdit        *edit);

/***********************************************************************/
/* Preferences
 */
enum {
    MOO_EDIT_SETTING_LANG,
    MOO_EDIT_SETTING_INDENT,
    MOO_EDIT_SETTING_STRIP,
    MOO_EDIT_SETTING_ADD_NEWLINE,
    MOO_EDIT_SETTING_WRAP_MODE,
    MOO_EDIT_SETTING_SHOW_LINE_NUMBERS,
    MOO_EDIT_SETTING_TAB_WIDTH,
    MOO_EDIT_SETTING_WORD_CHARS,
    MOO_EDIT_LAST_SETTING
};

extern guint *_moo_edit_settings;

void        _moo_edit_update_global_config      (void);
void        _moo_edit_init_config               (void);

void        _moo_edit_queue_recheck_config_all  (void);
void        _moo_edit_queue_recheck_config      (MooEdit        *edit);

void        _moo_edit_closed                    (MooEdit        *edit);

/***********************************************************************/
/* File operations
 */

void         _moo_edit_set_file                 (MooEdit        *edit,
                                                 GFile          *file,
                                                 const char     *encoding);
const char  *_moo_edit_get_default_encoding     (void);
void         _moo_edit_ensure_newline           (MooEdit        *edit);

void         _moo_edit_stop_file_watch          (MooEdit        *edit);

void         _moo_edit_set_status               (MooEdit        *edit,
                                                 MooEditStatus   status);

GdkPixbuf   *_moo_edit_get_icon                 (MooEdit        *edit,
                                                 GtkWidget      *widget,
                                                 GtkIconSize     size);

MooActionCollection *_moo_edit_get_actions      (MooEdit        *edit);

char *_moo_edit_normalize_filename_for_comparison (const char *filename);
char *_moo_edit_normalize_uri_for_comparison (const char *uri);

void         _moo_edit_strip_whitespace         (MooEdit        *edit);

G_END_DECLS

#endif /* MOO_EDIT_IMPL_H */
