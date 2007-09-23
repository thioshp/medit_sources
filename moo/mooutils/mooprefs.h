/*
 *   mooprefs.h
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_PREFS_H
#define MOO_PREFS_H

#include <glib-object.h>
#include <gdk/gdkcolor.h>
#include <mooutils/moomarkup.h>

#ifndef G_GNUC_NULL_TERMINATED
#if __GNUC__ >= 4
#define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#else
#define G_GNUC_NULL_TERMINATED
#endif
#endif

G_BEGIN_DECLS


#define MOO_TYPE_PREFS_TYPE         (moo_prefs_type_get_type ())
#define MOO_TYPE_PREFS_MATCH_TYPE   (moo_prefs_match_type_get_type ())


typedef enum {
    MOO_PREFS_RC,
    MOO_PREFS_STATE
} MooPrefsType;

typedef enum {
    MOO_PREFS_MATCH_KEY       = 1 << 0,
    MOO_PREFS_MATCH_PREFIX    = 1 << 1,
    MOO_PREFS_MATCH_REGEX     = 1 << 2
} MooPrefsMatchType;


GType           moo_prefs_type_get_type (void) G_GNUC_CONST;
GType           moo_prefs_match_type_get_type (void) G_GNUC_CONST;

gboolean        moo_prefs_load          (const char     *file_rc,
                                         const char     *file_state,
                                         GError        **error);
gboolean        moo_prefs_save          (const char     *file_rc,
                                         const char     *file_state,
                                         GError        **error);

MooMarkupDoc   *moo_prefs_get_markup    (MooPrefsType    prefs_type);

void            moo_prefs_new_key       (const char     *key,
                                         GType           value_type,
                                         const GValue   *default_value,
                                         MooPrefsType    prefs_type);
void            moo_prefs_delete_key    (const char     *key);

GType           moo_prefs_get_key_type  (const char     *key);
gboolean        moo_prefs_key_registered(const char     *key);

GSList         *moo_prefs_list_keys     (MooPrefsType    prefs_type);

const GValue   *moo_prefs_get           (const char     *key);
const GValue   *moo_prefs_get_default   (const char     *key);
void            moo_prefs_set           (const char     *key,
                                         const GValue   *value);
void            moo_prefs_set_default   (const char     *key,
                                         const GValue   *value);

void            moo_prefs_new_key_bool  (const char     *key,
                                         gboolean        default_val);
void            moo_prefs_new_key_int   (const char     *key,
                                         int             default_val);
void            moo_prefs_new_key_string(const char     *key,
                                         const char     *default_val);
void            moo_prefs_new_key_color (const char     *key,
                                         const GdkColor *default_val);
void            moo_prefs_new_key_enum  (const char     *key,
                                         GType           enum_type,
                                         int             default_val);
void            moo_prefs_new_key_flags (const char     *key,
                                         GType           flags_type,
                                         int             default_val);

void            moo_prefs_create_key    (const char     *key,
                                         MooPrefsType    prefs_type,
                                         GType           value_type,
                                         ...);

char           *moo_prefs_make_key      (const char     *first_comp,
                                         ...) G_GNUC_NULL_TERMINATED;
char           *moo_prefs_make_keyv     (const char     *first_comp,
                                         va_list         var_args);

const char     *moo_prefs_get_string    (const char     *key);
const char     *moo_prefs_get_filename  (const char     *key);
gboolean        moo_prefs_get_bool      (const char     *key);
gdouble         moo_prefs_get_number    (const char     *key);
const GdkColor *moo_prefs_get_color     (const char     *key);
int             moo_prefs_get_int       (const char     *key);
int             moo_prefs_get_enum      (const char     *key);
int             moo_prefs_get_flags     (const char     *key);

void            moo_prefs_set_string    (const char     *key,
                                         const char     *val);
void            moo_prefs_set_filename  (const char     *key,
                                         const char     *val);
void            moo_prefs_set_number    (const char     *key,
                                         double          val);
void            moo_prefs_set_int       (const char     *key,
                                         int             val);
void            moo_prefs_set_bool      (const char     *key,
                                         gboolean        val);
void            moo_prefs_set_color     (const char     *key,
                                         const GdkColor *val);
void            moo_prefs_set_enum      (const char     *key,
                                         int             val);
void            moo_prefs_set_flags     (const char     *key,
                                         int             val);

typedef void  (*MooPrefsNotify)         (const char     *key,
                                         const GValue   *newval,
                                         gpointer        data);

guint           moo_prefs_notify_connect    (const char     *pattern,
                                             MooPrefsMatchType match_type,
                                             MooPrefsNotify  callback,
                                             gpointer        data,
                                             GDestroyNotify  notify);
gboolean        moo_prefs_notify_block      (guint           id);
gboolean        moo_prefs_notify_unblock    (guint           id);
gboolean        moo_prefs_notify_disconnect (guint           id);


G_END_DECLS

#endif /* MOO_PREFS_H */
