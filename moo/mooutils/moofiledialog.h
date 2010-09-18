/*
 *   moofiledialog.h
 *
 *   Copyright (C) 2004-2008 by Yevgen Muntyan <muntyan@tamu.edu>
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

#ifndef MOO_FILE_DIALOG_H
#define MOO_FILE_DIALOG_H

#include <gtk/gtkwidget.h>

G_BEGIN_DECLS


#define MOO_TYPE_FILE_DIALOG              (moo_file_dialog_get_type ())
#define MOO_FILE_DIALOG(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_FILE_DIALOG, MooFileDialog))
#define MOO_FILE_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_FILE_DIALOG, MooFileDialogClass))
#define MOO_IS_FILE_DIALOG(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_FILE_DIALOG))
#define MOO_IS_FILE_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_FILE_DIALOG))
#define MOO_FILE_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_FILE_DIALOG, MooFileDialogClass))

typedef struct MooFileDialog        MooFileDialog;
typedef struct MooFileDialogPrivate MooFileDialogPrivate;
typedef struct MooFileDialogClass   MooFileDialogClass;

typedef enum {
    MOO_FILE_DIALOG_OPEN,
    MOO_FILE_DIALOG_OPEN_ANY,
    MOO_FILE_DIALOG_SAVE,
    MOO_FILE_DIALOG_OPEN_DIR
    /*  MOO_DIALOG_FILE_CREATE,*/
    /*  MOO_DIALOG_DIR_NEW,*/
} MooFileDialogType;

struct MooFileDialog
{
    GObject parent;
    MooFileDialogPrivate *priv;
};

struct MooFileDialogClass
{
    GObjectClass parent_class;
};

typedef char *(*MooFileDialogCheckNameFunc) (MooFileDialog *dialog,
                                             const char    *uri,
                                             gpointer       data);

GType           moo_file_dialog_get_type                (void) G_GNUC_CONST;

MooFileDialog  *moo_file_dialog_new                     (MooFileDialogType type,
                                                         GtkWidget      *parent,
                                                         gboolean        multiple,
                                                         const char     *title,
                                                         const char     *start_dir,
                                                         const char     *start_name);
void            moo_file_dialog_set_filter_mgr_id       (MooFileDialog  *dialog,
                                                         const char     *id);

gboolean        moo_file_dialog_run                     (MooFileDialog  *dialog);
char           *moo_file_dialog_get_filename            (MooFileDialog  *dialog);
char          **moo_file_dialog_get_filenames           (MooFileDialog  *dialog);
char           *moo_file_dialog_get_uri                 (MooFileDialog  *dialog);
char          **moo_file_dialog_get_uris                (MooFileDialog  *dialog);

void            moo_file_dialog_set_current_folder_uri  (MooFileDialog  *dialog,
                                                         const char     *uri);
const char     *moo_file_dialog_get_current_folder_uri  (MooFileDialog  *dialog);

void            moo_file_dialog_set_remember_size       (MooFileDialog  *dialog,
                                                         const char     *prefs_key);

void            moo_file_dialog_set_check_name_func     (MooFileDialog  *dialog,
                                                         MooFileDialogCheckNameFunc func,
                                                         gpointer        data,
                                                         GDestroyNotify  notify);
void            moo_file_dialog_set_extra_widget        (MooFileDialog  *dialog,
                                                         GtkWidget      *widget);

void            moo_file_dialog_set_encoding            (MooFileDialog  *dialog,
                                                         const char     *encoding);
const char     *moo_file_dialog_get_encoding            (MooFileDialog  *dialog);

void            moo_file_dialog_set_help_id             (MooFileDialog  *dialog,
                                                         const char     *id);

const char     *moo_file_dialog                         (GtkWidget      *parent,
                                                         MooFileDialogType type,
                                                         const char     *basename_utf8,
                                                         const char     *title,
                                                         const char     *start_dir);
const char     *moo_file_dialogp                        (GtkWidget      *parent,
                                                         MooFileDialogType type,
                                                         const char     *basename_utf8,
                                                         const char     *title,
                                                         const char     *prefs_key,
                                                         const char     *alternate_prefs_key);


G_END_DECLS

#endif /* MOO_FILE_DIALOG_H */
