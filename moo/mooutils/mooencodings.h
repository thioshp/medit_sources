/*
 *   mooencodings.h
 *
 *   Copyright (C) 2004-2006 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_ENCODINGS_H
#define MOO_ENCODINGS_H

#include <gtk/gtkcombobox.h>

G_BEGIN_DECLS


#define MOO_ENCODING_LOCALE "locale"
#define MOO_ENCODING_AUTO   "auto"
#define MOO_ENCODING_UTF8   "UTF-8"

typedef enum {
    MOO_ENCODING_COMBO_OPEN,
    MOO_ENCODING_COMBO_SAVE
} MooEncodingComboType;

void         _moo_encodings_combo_init      (GtkComboBox           *combo,
                                             MooEncodingComboType   type);
void         _moo_encodings_combo_set_enc   (GtkComboBox           *combo,
                                             const char            *enc,
                                             MooEncodingComboType   type);
const char  *_moo_encodings_combo_get_enc   (GtkComboBox           *combo,
                                             MooEncodingComboType   type);

void         _moo_encodings_attach_combo    (GtkWidget              *dialog,
                                             GtkWidget              *box,
                                             gboolean                save_mode,
                                             const char             *encoding);
const char  *_moo_encodings_combo_get       (GtkWidget              *dialog,
                                             gboolean                save_mode);

const char  *_moo_encoding_locale           (void);
gboolean     _moo_encodings_equal           (const char *enc1,
                                             const char *enc2);


G_END_DECLS

#endif /* MOO_ENCODINGS_H */
