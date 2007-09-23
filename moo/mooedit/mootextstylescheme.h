/*
 *   mootextstylescheme.h
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_TEXT_STYLE_SCHEME_H
#define MOO_TEXT_STYLE_SCHEME_H

#include <gtk/gtkwidget.h>
#include <gtk/gtktexttag.h>

G_BEGIN_DECLS


#define MOO_TYPE_TEXT_STYLE_SCHEME          (moo_text_style_scheme_get_type ())
#define MOO_TEXT_STYLE_SCHEME(object)       (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_TEXT_STYLE_SCHEME, MooTextStyleScheme))
#define MOO_IS_TEXT_STYLE_SCHEME(object)    (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_TEXT_STYLE_SCHEME))

#define MOO_TYPE_TEXT_STYLE                 (moo_text_style_get_type ())
#define MOO_TEXT_STYLE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MOO_TYPE_TEXT_STYLE, MooTextStyle))
#define MOO_IS_TEXT_STYLE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_TEXT_STYLE))

typedef struct _MooTextStyle MooTextStyle;
typedef struct _MooTextStyleScheme MooTextStyleScheme;

GType               moo_text_style_scheme_get_type      (void) G_GNUC_CONST;
GType               moo_text_style_get_type             (void) G_GNUC_CONST;

const char         *moo_text_style_scheme_get_id        (MooTextStyleScheme *scheme);
const char         *moo_text_style_scheme_get_name      (MooTextStyleScheme *scheme);

MooTextStyle       *_moo_text_style_scheme_get_bracket_match_style
                                                        (MooTextStyleScheme *scheme);
MooTextStyle       *_moo_text_style_scheme_get_bracket_mismatch_style
                                                        (MooTextStyleScheme *scheme);
void                _moo_text_style_scheme_apply        (MooTextStyleScheme *scheme,
                                                         GtkWidget          *widget);
MooTextStyle       *_moo_text_style_scheme_lookup_style (MooTextStyleScheme *scheme,
                                                         const char         *name);

void                _moo_text_style_apply_to_tag        (const MooTextStyle *style,
                                                         GtkTextTag         *tag);


G_END_DECLS

#endif /* MOO_TEXT_STYLE_SCHEME_H */
