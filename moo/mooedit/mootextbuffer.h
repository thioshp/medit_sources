/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4; coding: utf-8 -*-
 *
 *   mootextbuffer.h
 *
 *   Copyright (C) 2004-2005 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef __MOO_TEXT_BUFFER_H__
#define __MOO_TEXT_BUFFER_H__

#include <gtk/gtktextbuffer.h>
#include <mooedit/moolang.h>

G_BEGIN_DECLS


#define MOO_TYPE_TEXT_BUFFER              (moo_text_buffer_get_type ())
#define MOO_TEXT_BUFFER(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_TEXT_BUFFER, MooTextBuffer))
#define MOO_TEXT_BUFFER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_TEXT_BUFFER, MooTextBufferClass))
#define MOO_IS_TEXT_BUFFER(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_TEXT_BUFFER))
#define MOO_IS_TEXT_BUFFER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_TEXT_BUFFER))
#define MOO_TEXT_BUFFER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_TEXT_BUFFER, MooTextBufferClass))


typedef struct _MooTextBuffer         MooTextBuffer;
typedef struct _MooTextBufferPrivate  MooTextBufferPrivate;
typedef struct _MooTextBufferClass    MooTextBufferClass;

struct _MooTextBuffer
{
    GtkTextBuffer  parent;

    MooTextBufferPrivate *priv;
};

struct _MooTextBufferClass
{
    GtkTextBufferClass parent_class;

    void (*cursor_moved) (MooTextBuffer      *buffer,
                          const GtkTextIter  *iter);
};


GType       moo_text_buffer_get_type                    (void) G_GNUC_CONST;

GtkTextBuffer *moo_text_buffer_new                      (GtkTextTagTable    *table);

void        moo_text_buffer_set_lang                    (MooTextBuffer      *buffer,
                                                         MooLang            *lang);
MooLang    *moo_text_buffer_get_lang                    (MooTextBuffer      *buffer);

void        moo_text_buffer_set_highlight               (MooTextBuffer      *buffer,
                                                         gboolean            highlight);
gboolean    moo_text_buffer_get_highlight               (MooTextBuffer      *buffer);

void        moo_text_buffer_set_bracket_match_style     (MooTextBuffer      *buffer,
                                                         const MooTextStyle *style);
void        moo_text_buffer_set_bracket_mismatch_style  (MooTextBuffer      *buffer,
                                                         const MooTextStyle *style);
void        moo_text_buffer_set_brackets                (MooTextBuffer      *buffer,
                                                         const char         *brackets);
void        moo_text_buffer_set_check_brackets          (MooTextBuffer      *buffer,
                                                         gboolean            check);

gboolean    moo_text_buffer_has_text                    (MooTextBuffer      *buffer);
gboolean    moo_text_buffer_has_selection               (MooTextBuffer      *buffer);

void        moo_text_buffer_apply_scheme                (MooTextBuffer      *buffer,
                                                         MooTextStyleScheme *scheme);

void        _moo_text_buffer_ensure_highlight           (MooTextBuffer      *buffer,
                                                         int                 first_line,
                                                         int                 last_line);
void        _moo_text_buffer_apply_syntax_tag           (MooTextBuffer      *buffer,
                                                         GtkTextTag         *tag,
                                                         const GtkTextIter  *start,
                                                         const GtkTextIter  *end);


G_END_DECLS

#endif /* __MOO_TEXT_BUFFER_H__ */
