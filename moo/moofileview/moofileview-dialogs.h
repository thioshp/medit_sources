/*
 *   moofileview-dialogs.h
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_FILE_VIEW_COMPILATION
#error "This file may not be included"
#endif

#ifndef MOO_FILE_VIEW_DIALOGS_H
#define MOO_FILE_VIEW_DIALOGS_H

#include "moofileview/moofile.h"
#include <gtk/gtkdialog.h>

G_BEGIN_DECLS


#define MOO_TYPE_FILE_PROPS_DIALOG              (_moo_file_props_dialog_get_type ())
#define MOO_FILE_PROPS_DIALOG(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_FILE_PROPS_DIALOG, MooFilePropsDialog))
#define MOO_FILE_PROPS_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_FILE_PROPS_DIALOG, MooFilePropsDialogClass))
#define MOO_IS_FILE_PROPS_DIALOG(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_FILE_PROPS_DIALOG))
#define MOO_IS_FILE_PROPS_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_FILE_PROPS_DIALOG))
#define MOO_FILE_PROPS_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_FILE_PROPS_DIALOG, MooFilePropsDialogClass))


typedef struct _MooFilePropsDialog        MooFilePropsDialog;
typedef struct _MooFilePropsDialogClass   MooFilePropsDialogClass;

struct _MooFilePropsDialog
{
    GtkDialog base;
    struct MooFilePropsXml *xml;
    GtkWidget *notebook;
    GtkWidget *icon;
    GtkWidget *entry;
    GtkWidget *table;
    MooFile *file;
    MooFolder *folder;
};

struct _MooFilePropsDialogClass
{
    GtkDialogClass base_class;
};


GType       _moo_file_props_dialog_get_type     (void) G_GNUC_CONST;

GtkWidget  *_moo_file_props_dialog_new          (GtkWidget          *parent);
void        _moo_file_props_dialog_set_file     (MooFilePropsDialog *dialog,
                                                 MooFile            *file,
                                                 MooFolder          *folder);

char       *_moo_file_view_create_folder_dialog (GtkWidget          *parent,
                                                 MooFolder          *folder);
char       *_moo_file_view_save_drop_dialog     (GtkWidget          *parent,
                                                 const char         *dirname);


G_END_DECLS

#endif /* MOO_FILE_VIEW_DIALOGS_H */
