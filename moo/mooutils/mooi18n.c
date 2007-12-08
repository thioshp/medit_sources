/*
 *   mooi18n.c
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#include "mooutils/mooi18n.h"
#include "mooutils/mooutils-misc.h"
#include <glib.h>


#ifdef ENABLE_NLS
static void
init_gettext (void)
{
    static gboolean been_here = FALSE;

    if (!been_here)
    {
        been_here = TRUE;
        bindtextdomain (GETTEXT_PACKAGE, moo_get_locale_dir ());
        bindtextdomain (GETTEXT_PACKAGE "-gsv", moo_get_locale_dir ());
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
        bind_textdomain_codeset (GETTEXT_PACKAGE "-gsv", "UTF-8");
#endif
    }
}
#endif /* ENABLE_NLS */

const char *
moo_gettext (const char *string)
{
#ifdef ENABLE_NLS
    g_return_val_if_fail (string != NULL, NULL);
    init_gettext ();
    return dgettext (GETTEXT_PACKAGE, string);
#else /* !ENABLE_NLS */
    return string;
#endif /* !ENABLE_NLS */
}

const char *
_moo_gsv_gettext (const char *string)
{
#ifdef ENABLE_NLS
    g_return_val_if_fail (string != NULL, NULL);
    init_gettext ();
    return dgettext (GETTEXT_PACKAGE "-gsv", string);
#else /* !ENABLE_NLS */
    return string;
#endif /* !ENABLE_NLS */
}

char *
_moo_gsv_dgettext (const char *domain, const char *string)
{
#ifdef ENABLE_NLS
    gchar *tmp;
    const gchar *translated;

    g_return_val_if_fail (string != NULL, NULL);

    init_gettext ();

    if (domain == NULL)
        return g_strdup (_moo_gsv_gettext (string));

    translated = dgettext (domain, string);

    if (strcmp (translated, string) == 0)
        return g_strdup (_moo_gsv_gettext (string));

    if (g_utf8_validate (translated, -1, NULL))
        return g_strdup (translated);

    tmp = g_locale_to_utf8 (translated, -1, NULL, NULL, NULL);

    if (tmp == NULL)
        return g_strdup (string);
    else
        return tmp;
#else
    return g_strdup (string);
#endif /* !ENABLE_NLS */
}
