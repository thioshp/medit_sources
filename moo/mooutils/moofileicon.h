/*
 *   moofileicon.h
 *
 *   Copyright (C) 2004-2008 by Yevgen Muntyan <muntyan@tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_FILE_ICON_H
#define MOO_FILE_ICON_H

#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

typedef enum {
    MOO_ICON_EMBLEM_LINK = 1 << 0
} MooIconEmblem;

typedef enum {
    MOO_ICON_MIME = 0,
    MOO_ICON_HOME,
    MOO_ICON_DESKTOP,
    MOO_ICON_TRASH,
    MOO_ICON_DIRECTORY,
    MOO_ICON_BROKEN_LINK,
    MOO_ICON_NONEXISTENT,
    MOO_ICON_BLOCK_DEVICE,
    MOO_ICON_CHARACTER_DEVICE,
    MOO_ICON_FIFO,
    MOO_ICON_SOCKET,
    MOO_ICON_FILE,
    MOO_ICON_BLANK,
    MOO_ICON_INVALID
} MooIconType;

typedef struct {
    const char *mime_type; /* interned inside mime code */
    MooIconType type;
    MooIconEmblem emblem;
} MooFileIcon;

/* thread-safe */
void         moo_file_icon_for_file     (MooFileIcon    *icon,
                                         const char     *path);
/* needs gtk lock */
GdkPixbuf   *moo_file_icon_get_pixbuf   (MooFileIcon    *icon,
                                         GtkWidget      *widget,
                                         GtkIconSize     size);
/* needs gtk lock */
GdkPixbuf   *moo_get_icon_for_file      (const char     *path,
                                         GtkWidget      *widget,
                                         GtkIconSize     size);

MooFileIcon *moo_file_icon_new          (void);
MooFileIcon *moo_file_icon_copy         (MooFileIcon    *icon);
void         moo_file_icon_free         (MooFileIcon    *icon);


G_END_DECLS

#endif /* MOO_FILE_ICON_H */
