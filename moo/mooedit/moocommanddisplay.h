/*
 *   moocommanddisplay.h
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_COMMAND_DISPLAY_H
#define MOO_COMMAND_DISPLAY_H

#include <mooutils/mooutils-treeview.h>
#include <mooedit/moocommand.h>
#include <gtk/gtkcombobox.h>
#include <gtk/gtknotebook.h>

G_BEGIN_DECLS


#define MOO_TYPE_COMMAND_DISPLAY            (_moo_command_display_get_type ())
#define MOO_COMMAND_DISPLAY(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_COMMAND_DISPLAY, MooCommandDisplay))
#define MOO_COMMAND_DISPLAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COMMAND_DISPLAY, MooCommandDisplayClass))
#define MOO_IS_COMMAND_DISPLAY(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_COMMAND_DISPLAY))
#define MOO_IS_COMMAND_DISPLAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COMMAND_DISPLAY))
#define MOO_COMMAND_DISPLAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COMMAND_DISPLAY, MooCommandDisplayClass))

typedef struct _MooCommandDisplay MooCommandDisplay;
typedef struct _MooCommandDisplayClass MooCommandDisplayClass;

GType               _moo_command_display_get_type   (void);
MooCommandDisplay  *_moo_command_display_new        (GtkComboBox        *type_combo,
                                                     GtkNotebook        *notebook,
                                                     GtkWidget          *treeview,
                                                     GtkWidget          *new_btn,
                                                     GtkWidget          *delete_btn,
                                                     GtkWidget          *up_btn,
                                                     GtkWidget          *down_btn);

void                _moo_command_display_set        (MooCommandDisplay  *display,
                                                     MooCommandFactory  *factory,
                                                     MooCommandData     *data);
gboolean            _moo_command_display_get        (MooCommandDisplay  *display,
                                                     MooCommandFactory **factory,
                                                     MooCommandData    **data);


G_END_DECLS

#endif /* MOO_COMMAND_DISPLAY_H */
