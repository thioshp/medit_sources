/*
 *   moocommand.h
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

#ifndef MOO_COMMAND_H
#define MOO_COMMAND_H

#include <gtk/gtkwidget.h>
#include <mooedit/moooutputfilter.h>

G_BEGIN_DECLS


#define MOO_TYPE_COMMAND                    (moo_command_get_type ())
#define MOO_COMMAND(object)                 (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_COMMAND, MooCommand))
#define MOO_COMMAND_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COMMAND, MooCommandClass))
#define MOO_IS_COMMAND(object)              (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_COMMAND))
#define MOO_IS_COMMAND_CLASS(klass)         (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COMMAND))
#define MOO_COMMAND_GET_CLASS(obj)          (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COMMAND, MooCommandClass))

#define MOO_TYPE_COMMAND_CONTEXT            (moo_command_context_get_type ())
#define MOO_COMMAND_CONTEXT(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_COMMAND_CONTEXT, MooCommandContext))
#define MOO_COMMAND_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COMMAND_CONTEXT, MooCommandContextClass))
#define MOO_IS_COMMAND_CONTEXT(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_COMMAND_CONTEXT))
#define MOO_IS_COMMAND_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COMMAND_CONTEXT))
#define MOO_COMMAND_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COMMAND_CONTEXT, MooCommandContextClass))

#define MOO_TYPE_COMMAND_FACTORY            (moo_command_factory_get_type ())
#define MOO_COMMAND_FACTORY(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_COMMAND_FACTORY, MooCommandFactory))
#define MOO_COMMAND_FACTORY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COMMAND_FACTORY, MooCommandFactoryClass))
#define MOO_IS_COMMAND_FACTORY(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_COMMAND_FACTORY))
#define MOO_IS_COMMAND_FACTORY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COMMAND_FACTORY))
#define MOO_COMMAND_FACTORY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COMMAND_FACTORY, MooCommandFactoryClass))

#define MOO_TYPE_COMMAND_DATA               (moo_command_data_get_type ())

typedef struct _MooCommand                  MooCommand;
typedef struct _MooCommandClass             MooCommandClass;
typedef struct _MooCommandContext           MooCommandContext;
typedef struct _MooCommandContextPrivate    MooCommandContextPrivate;
typedef struct _MooCommandContextClass      MooCommandContextClass;
typedef struct _MooCommandData              MooCommandData;
typedef struct _MooCommandFactory           MooCommandFactory;
typedef struct _MooCommandFactoryClass      MooCommandFactoryClass;

typedef enum /*< flags >*/
{
    MOO_COMMAND_NEED_DOC        = 1 << 0,
    MOO_COMMAND_NEED_FILE       = 1 << 1,
    MOO_COMMAND_NEED_SAVE       = 1 << 2,
    MOO_COMMAND_NEED_SAVE_ALL   = 1 << 3,
    MOO_COMMAND_NEED_WINDOW     = 1 << 4
} MooCommandOptions;

struct _MooCommandContext {
    GObject base;
    MooCommandContextPrivate *priv;
};

struct _MooCommandContextClass {
    GObjectClass base_class;
};

struct _MooCommand {
    GObject base;
    /* read-only */
    MooCommandOptions options;
};

struct _MooCommandClass {
    GObjectClass base_class;

    gboolean    (*check_sensitive)  (MooCommand         *cmd,
                                     gpointer            doc,
                                     gpointer            window);
    void        (*run)              (MooCommand         *cmd,
                                     MooCommandContext  *ctx);
};

struct _MooCommandFactory {
    GObject base;
    char *name;
    char *display_name;
    char **keys;
    guint n_keys;
};

struct _MooCommandFactoryClass {
    GObjectClass base_class;

    MooCommand *(*create_command) (MooCommandFactory *factory,
                                   MooCommandData    *data,
                                   const char        *options);
    GtkWidget  *(*create_widget)  (MooCommandFactory *factory);
    void        (*load_data)      (MooCommandFactory *factory,
                                   GtkWidget         *widget,
                                   MooCommandData    *data);
    gboolean    (*save_data)      (MooCommandFactory *factory,
                                   GtkWidget         *widget,
                                   MooCommandData    *data);
    gboolean    (*data_equal)     (MooCommandFactory *factory,
                                   MooCommandData    *data1,
                                   MooCommandData    *data2);
};


GType               moo_command_get_type            (void) G_GNUC_CONST;
GType               moo_command_context_get_type    (void) G_GNUC_CONST;
GType               moo_command_data_get_type       (void) G_GNUC_CONST;
GType               moo_command_factory_get_type    (void) G_GNUC_CONST;

MooCommand         *moo_command_create              (const char         *name,
                                                     const char         *options,
                                                     MooCommandData     *data);

void                moo_command_run                 (MooCommand         *cmd,
                                                     MooCommandContext  *ctx);
gboolean            moo_command_check_context       (MooCommand         *cmd,
                                                     MooCommandContext  *ctx);
gboolean            moo_command_check_sensitive     (MooCommand         *cmd,
                                                     gpointer            doc,
                                                     gpointer            window);

void                moo_command_set_options         (MooCommand         *cmd,
                                                     MooCommandOptions   options);
MooCommandOptions   moo_command_get_options         (MooCommand         *cmd);

MooCommandOptions   moo_command_options_parse       (const char         *string);

void                moo_command_factory_register    (const char         *name,
                                                     const char         *display_name,
                                                     MooCommandFactory  *factory,
                                                     char              **data_keys);
MooCommandFactory  *moo_command_factory_lookup      (const char         *name);
/* returns list of MooCommandFactory instances, list should be freed */
GSList             *moo_command_list_factories      (void);


MooCommandData     *moo_command_data_new            (guint               len);

MooCommandData     *moo_command_data_ref            (MooCommandData     *data);
void                moo_command_data_unref          (MooCommandData     *data);
void                moo_command_data_set            (MooCommandData     *data,
                                                     guint               index_,
                                                     const char         *value);
void                moo_command_data_set_code       (MooCommandData     *data,
                                                     const char         *code);
const char         *moo_command_data_get            (MooCommandData     *data,
                                                     guint               index_);
const char         *moo_command_data_get_code       (MooCommandData     *data);


MooCommandContext  *moo_command_context_new         (gpointer            doc,
                                                     gpointer            window);

void                moo_command_context_set_doc     (MooCommandContext  *ctx,
                                                     gpointer            doc);
void                moo_command_context_set_window  (MooCommandContext  *ctx,
                                                     gpointer            window);
gpointer            moo_command_context_get_doc     (MooCommandContext  *ctx);
gpointer            moo_command_context_get_window  (MooCommandContext  *ctx);

void                moo_command_context_set         (MooCommandContext  *ctx,
                                                     const char         *name,
                                                     const GValue       *value);
gboolean            moo_command_context_get         (MooCommandContext  *ctx,
                                                     const char         *name,
                                                     GValue             *value);
void                moo_command_context_unset       (MooCommandContext  *ctx,
                                                     const char         *name);
void                moo_command_context_set_string  (MooCommandContext *ctx,
                                                     const char         *name,
                                                     const char         *value);

typedef void (*MooCommandContextForeachFunc)        (const char         *name,
                                                     const GValue       *value,
                                                     gpointer            data);
void                moo_command_context_foreach     (MooCommandContext  *ctx,
                                                     MooCommandContextForeachFunc func,
                                                     gpointer            data);


typedef MooOutputFilter *(*MooCommandFilterFactory) (const char         *id,
                                                     gpointer            data);

void                moo_command_filter_register     (const char         *id,
                                                     const char         *name,
                                                     MooCommandFilterFactory factory_func,
                                                     gpointer            data,
                                                     GDestroyNotify      data_notify);
void                moo_command_filter_unregister   (const char         *id);
/* returns name */
const char         *moo_command_filter_lookup       (const char         *id);
/* list of ids, must be freed together with content */
GSList             *moo_command_filter_list         (void);
MooOutputFilter    *moo_command_filter_create       (const char         *id);


G_END_DECLS

#endif /* MOO_COMMAND_H */
