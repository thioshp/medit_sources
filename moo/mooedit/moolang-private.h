/*
 *   moolang-private.h
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

#ifndef MOOEDIT_COMPILATION
#error "This file may not be used"
#endif

#ifndef MOO_LANG_PRIVATE_H
#define MOO_LANG_PRIVATE_H

#include "mooedit/gtksourceview/gtksourceview-api.h"
#include "mooedit/moolang.h"
#include "mooedit/moolangmgr.h"
#include "mooedit/mooeditconfig.h"

G_BEGIN_DECLS

#define MOO_LANG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_LANG, MooLangClass))
#define MOO_IS_LANG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_LANG))
#define MOO_LANG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_LANG, MooLangClass))

typedef struct _MooLangPrivate MooLangPrivate;
typedef struct _MooLangClass   MooLangClass;

struct _MooLang {
    GtkSourceLanguage base;
    MooLangPrivate *priv;
};

struct _MooLangClass {
    GtkSourceLanguageClass base_class;
};


GtkSourceEngine *_moo_lang_get_engine   (MooLang    *lang);


G_END_DECLS

#endif /* MOO_LANG_H */
