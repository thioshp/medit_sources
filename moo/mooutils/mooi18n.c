/*
 *   mooi18n.c
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

#include "mooutils/mooi18n.h"
#include <glib.h>


const char *
moo_gettext (const char *string)
{
#ifdef ENABLE_NLS
    static gboolean been_here = FALSE;

    g_return_val_if_fail (string != NULL, NULL);

    if (!been_here)
    {
        been_here = TRUE;
        bindtextdomain (GETTEXT_PACKAGE, MOO_LOCALE_DIR);
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    }

    return dgettext (GETTEXT_PACKAGE, string);
#else /* !ENABLE_NLS */
    return string;
#endif /* !ENABLE_NLS */
}
