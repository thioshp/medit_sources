/*
 *   moopython.h
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

#ifndef MOO_PYTHON_H
#define MOO_PYTHON_H

#include <glib.h>

G_BEGIN_DECLS

gboolean    moo_python_init             (void);
void        moo_python_deinit           (void);
gboolean    moo_python_ref              (void);
void        moo_python_unref            (void);

gboolean    moo_python_run_string       (const char *string);
gboolean    moo_python_run_file         (const char *filename);
gboolean    moo_python_run_string_full  (const char *prefix,
                                         const char *string);

G_END_DECLS

#endif /* MOO_PYTHON_H */