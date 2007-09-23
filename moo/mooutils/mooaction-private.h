/*
 *   mooaction-private.h
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_ACTION_PRIVATE_H
#define MOO_ACTION_PRIVATE_H

#include <mooutils/mooaction.h>
#include <mooutils/mooclosure.h>
#include <gtk/gtkactiongroup.h>

G_BEGIN_DECLS


void        _moo_sync_toggle_action         (GtkAction          *action,
                                             gpointer            master,
                                             const char         *prop,
                                             gboolean            invert);

gpointer    _moo_action_get_window          (gpointer            action);

/* defined in mooactionbase.c */
gboolean    _moo_action_get_dead            (gpointer            action);
gboolean    _moo_action_get_has_submenu     (gpointer            action);

const char *_moo_action_get_display_name    (gpointer            action);
GtkActionGroup *_moo_action_get_group       (gpointer            action);

void        _moo_action_set_no_accel        (gpointer            action,
                                             gboolean            no_accel);
gboolean    _moo_action_get_no_accel        (gpointer            action);
gboolean    _moo_action_get_connect_accel   (gpointer            action);
gboolean    _moo_action_get_accel_editable  (gpointer            action);
char       *_moo_action_make_accel_path     (gpointer            action);
void        _moo_action_set_accel_path      (gpointer            action,
                                             const char         *accel_path);
const char *_moo_action_get_accel_path      (gpointer            action);
const char *_moo_action_get_default_accel   (gpointer            action);

void        _moo_action_ring_the_bells_it_has_tooltip   (GtkAction  *action);


G_END_DECLS

#endif /* MOO_ACTION_PRIVATE_H */
