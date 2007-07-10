/*
 *   mooapp/mooappinput.h
 *
 *   Copyright (C) 2004-2007 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef __MOO_APP_INPUT__
#define __MOO_APP_INPUT__

#include <glib.h>

G_BEGIN_DECLS


typedef enum
{
    MOO_APP_CMD_ZERO = 0,
    MOO_APP_CMD_PYTHON_STRING,
    MOO_APP_CMD_PYTHON_FILE,
    MOO_APP_CMD_SCRIPT,
    MOO_APP_CMD_OPEN_FILE,
    MOO_APP_CMD_OPEN_URIS,
    MOO_APP_CMD_QUIT,
    MOO_APP_CMD_DIE,
    MOO_APP_CMD_PRESENT,
    MOO_APP_CMD_LAST
} MooAppCmdCode;


#if defined(WANT_MOO_APP_CMD_STRINGS) || defined(WANT_MOO_APP_CMD_CHARS)

/* 'g' is taken by ggap */
#define CMD_ZERO            "\0"
#define CMD_PYTHON_STRING   "p"
#define CMD_PYTHON_FILE     "P"
#define CMD_SCRIPT          "s"
#define CMD_OPEN_FILE       "f"
#define CMD_OPEN_URIS       "u"
#define CMD_QUIT            "q"
#define CMD_DIE             "d"
#define CMD_PRESENT         "r"

#endif

#ifdef WANT_MOO_APP_CMD_CHARS

static const char *moo_app_cmd_chars =
    CMD_ZERO
    CMD_PYTHON_STRING
    CMD_PYTHON_FILE
    CMD_SCRIPT
    CMD_OPEN_FILE
    CMD_OPEN_URIS
    CMD_QUIT
    CMD_DIE
    CMD_PRESENT
;

#endif /* WANT_MOO_APP_CMD_CHARS */

#define MOO_APP_INPUT_NAME_DEFAULT "main"

typedef struct _MooAppInput MooAppInput;


MooAppInput *_moo_app_input_new         (const char     *appname,
                                         const char     *name,
                                         gboolean        bind_default);
void         _moo_app_input_free        (MooAppInput    *ch);

gboolean     _moo_app_input_send_msg    (const char     *appname,
                                         const char     *name,
                                         const char     *data,
                                         gssize          len);


G_END_DECLS

#endif /* __MOO_APP_INPUT__ */
