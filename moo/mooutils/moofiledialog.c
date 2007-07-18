/*
 *   moofiledialog.c
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

#include <gtk/gtk.h>
#include "mooutils/moodialogs.h"
#include "mooutils/mooprefs.h"
#include "mooutils/mooutils-misc.h"
#include "mooutils/moomarshals.h"
#include "mooutils/mooencodings.h"
#include <string.h>


struct _MooFileDialogPrivate {
    gboolean multiple;
    char *title;
    char *dir;
    char *name;
    MooFileDialogType type;
    GtkWidget *parent;
    MooFilterMgr *filter_mgr;
    char *filter_mgr_id;
    gboolean enable_encodings;

    GSList *filenames;
    char *filename;
    char *encoding;
};


G_DEFINE_TYPE (MooFileDialog, moo_file_dialog, G_TYPE_OBJECT)


enum {
    PROP_0,
    PROP_MULTIPLE,
    PROP_TITLE,
    PROP_DIR,
    PROP_NAME,
    PROP_TYPE,
    PROP_PARENT,
    PROP_FILTER_MGR,
    PROP_FILTER_MGR_ID,
    PROP_ENABLE_ENCODINGS
};

enum {
    DIALOG_CREATED,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

static void
moo_file_dialog_set_property (GObject        *object,
                              guint           prop_id,
                              const GValue   *value,
                              GParamSpec     *pspec)
{
    MooFileDialog *dialog = MOO_FILE_DIALOG (object);

    switch (prop_id)
    {
        case PROP_MULTIPLE:
            dialog->priv->multiple = g_value_get_boolean (value) != 0;
            g_object_notify (object, "multiple");
            break;

        case PROP_TITLE:
            g_free (dialog->priv->title);
            dialog->priv->title = g_value_dup_string (value);
            g_object_notify (object, "title");
            break;

        case PROP_DIR:
            g_free (dialog->priv->dir);
            dialog->priv->dir = g_value_dup_string (value);
            g_object_notify (object, "dir");
            break;

        case PROP_NAME:
            g_free (dialog->priv->name);
            dialog->priv->name = g_value_dup_string (value);
            g_object_notify (object, "name");
            break;

        case PROP_TYPE:
            dialog->priv->type = g_value_get_enum (value);
            g_object_notify (object, "type");
            break;

        case PROP_PARENT:
            dialog->priv->parent = g_value_get_object (value);
            g_object_notify (object, "parent");
            break;

        case PROP_FILTER_MGR:
            if (dialog->priv->filter_mgr)
                g_object_unref (dialog->priv->filter_mgr);
            dialog->priv->filter_mgr = g_value_get_object (value);
            if (dialog->priv->filter_mgr)
                g_object_ref (dialog->priv->filter_mgr);
            g_object_notify (object, "filter-mgr");
            break;

        case PROP_FILTER_MGR_ID:
            g_free (dialog->priv->filter_mgr_id);
            dialog->priv->filter_mgr_id = g_value_dup_string (value);
            g_object_notify (object, "filter-mgr-id");
            break;

        case PROP_ENABLE_ENCODINGS:
            dialog->priv->enable_encodings = g_value_get_boolean (value);
            g_object_notify (object, "enable-encodings");
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}


static void
moo_file_dialog_get_property (GObject        *object,
                              guint           prop_id,
                              GValue         *value,
                              GParamSpec     *pspec)
{
    MooFileDialog *dialog = MOO_FILE_DIALOG (object);

    switch (prop_id)
    {
        case PROP_MULTIPLE:
            g_value_set_boolean (value, dialog->priv->multiple != 0);
            break;

        case PROP_TITLE:
            g_value_set_string (value, dialog->priv->title);
            break;

        case PROP_DIR:
            g_value_set_string (value, dialog->priv->dir);
            break;

        case PROP_NAME:
            g_value_set_string (value, dialog->priv->name);
            break;

        case PROP_TYPE:
            g_value_set_enum (value, dialog->priv->type);
            break;

        case PROP_PARENT:
            g_value_set_object (value, dialog->priv->parent);
            break;

        case PROP_FILTER_MGR:
            g_value_set_object (value, dialog->priv->filter_mgr);
            break;

        case PROP_FILTER_MGR_ID:
            g_value_set_string (value, dialog->priv->filter_mgr_id);
            break;

        case PROP_ENABLE_ENCODINGS:
            g_value_set_boolean (value, dialog->priv->enable_encodings);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}


static void
string_slist_free (GSList *list)
{
    g_slist_foreach (list, (GFunc) g_free, NULL);
    g_slist_free (list);
}


static void
moo_file_dialog_finalize (GObject *object)
{
    MooFileDialog *dialog = MOO_FILE_DIALOG (object);

    g_free (dialog->priv->dir);
    g_free (dialog->priv->title);
    g_free (dialog->priv->name);
    g_free (dialog->priv->filter_mgr_id);
    g_free (dialog->priv->filename);
    g_free (dialog->priv->encoding);

    string_slist_free (dialog->priv->filenames);

    if (dialog->priv->filter_mgr)
        g_object_unref (dialog->priv->filter_mgr);

    g_free (dialog->priv);

    G_OBJECT_CLASS(moo_file_dialog_parent_class)->finalize (object);
}


static void
moo_file_dialog_init (MooFileDialog *dialog)
{
    dialog->priv = g_new0 (MooFileDialogPrivate, 1);
    dialog->priv->type = MOO_FILE_DIALOG_OPEN;
}


static void
moo_file_dialog_class_init (MooFileDialogClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->finalize = moo_file_dialog_finalize;
    gobject_class->set_property = moo_file_dialog_set_property;
    gobject_class->get_property = moo_file_dialog_get_property;

    g_object_class_install_property (gobject_class,
                                     PROP_MULTIPLE,
                                     g_param_spec_boolean ("multiple",
                                             "multiple",
                                             "multiple",
                                             FALSE,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_ENABLE_ENCODINGS,
                                     g_param_spec_boolean ("enable-encodings",
                                             "enable-encodings",
                                             "enable-encodings",
                                             FALSE,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_TITLE,
                                     g_param_spec_string ("title",
                                             "title",
                                             "title",
                                             NULL,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_DIR,
                                     g_param_spec_string ("dir",
                                             "dir",
                                             "dir",
                                             NULL,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_NAME,
                                     g_param_spec_string ("name",
                                             "name",
                                             "name",
                                             NULL,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_PARENT,
                                     g_param_spec_object ("parent",
                                             "parent",
                                             "parent",
                                             GTK_TYPE_WIDGET,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_TYPE,
                                     g_param_spec_enum ("type",
                                             "type",
                                             "type",
                                             MOO_TYPE_FILE_DIALOG_TYPE,
                                             MOO_FILE_DIALOG_OPEN,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_FILTER_MGR,
                                     g_param_spec_object ("filter-mgr",
                                             "filter-mgr",
                                             "filter-mgr",
                                             MOO_TYPE_FILTER_MGR,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_FILTER_MGR_ID,
                                     g_param_spec_string ("filter-mgr-id",
                                             "filter-mgr-id",
                                             "filter-mgr-id",
                                             NULL,
                                             G_PARAM_READWRITE));

    signals[DIALOG_CREATED] =
        g_signal_new ("dialog-created",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (MooFileDialogClass, dialog_created),
                      NULL, NULL,
                      _moo_marshal_VOID__OBJECT,
                      G_TYPE_NONE, 1,
                      GTK_TYPE_DIALOG);
}


inline static
GtkWidget *file_chooser_dialog_new (const char *title,
                                    GtkFileChooserAction action,
                                    const char *okbtn,
                                    const char *start_dir)
{
    GtkWidget *dialog =
            gtk_file_chooser_dialog_new (title, NULL, action,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         okbtn, GTK_RESPONSE_OK,
                                         NULL);

    gtk_dialog_set_alternative_button_order (GTK_DIALOG (dialog),
                                             GTK_RESPONSE_OK,
                                             GTK_RESPONSE_CANCEL,
                                             -1);

    if (start_dir)
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(dialog),
                                             start_dir);
    return dialog;
}

#define file_chooser_set_select_multiple(dialog,multiple) \
    gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (dialog), multiple)
#define file_chooser_get_filename(dialog) \
    (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog)))
#define file_chooser_get_filenames(dialog)  \
    (gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (dialog)))
#define file_chooser_set_name(dialog, name) \
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), name)

const char *
moo_file_dialog (GtkWidget  *parent,
                 MooFileDialogType type,
                 const char *start_name,
                 const char *title,
                 const char *start_dir)
{
    static char *filename;
    MooFileDialog *dialog;

    dialog = moo_file_dialog_new (type, parent, FALSE, title, start_dir, start_name);
    g_return_val_if_fail (dialog != NULL, NULL);

    moo_file_dialog_run (dialog);

    g_free (filename);
    filename = g_strdup (moo_file_dialog_get_filename (dialog));

    g_object_unref (dialog);
    return filename;
}


static GtkWidget *
moo_file_dialog_create_widget (MooFileDialog *dialog)
{
    GtkFileChooserAction chooser_action;
    GtkWidget *widget = NULL;
    GtkWidget *extra_box = NULL;

    switch (dialog->priv->type)
    {
        case MOO_FILE_DIALOG_OPEN:
        case MOO_FILE_DIALOG_OPEN_ANY:
        case MOO_FILE_DIALOG_OPEN_DIR:
            if (dialog->priv->type == MOO_FILE_DIALOG_OPEN_DIR)
                chooser_action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
            else
                chooser_action = GTK_FILE_CHOOSER_ACTION_OPEN;

            widget = file_chooser_dialog_new (dialog->priv->title,
                                              chooser_action,
                                              GTK_STOCK_OPEN,
                                              dialog->priv->dir);
            file_chooser_set_select_multiple (widget, dialog->priv->multiple);
            break;

        case MOO_FILE_DIALOG_SAVE:
            chooser_action = GTK_FILE_CHOOSER_ACTION_SAVE;

            widget = file_chooser_dialog_new (dialog->priv->title,
                                              chooser_action,
                                              GTK_STOCK_SAVE,
                                              dialog->priv->dir);

            if (dialog->priv->name)
                file_chooser_set_name (widget, dialog->priv->name);
            break;

        default:
            g_critical ("%s: incorrect dialog type specified", G_STRLOC);
            return NULL;
    }

    gtk_dialog_set_default_response (GTK_DIALOG (widget), GTK_RESPONSE_OK);

    if (dialog->priv->filter_mgr || dialog->priv->enable_encodings)
    {
        extra_box = gtk_hbox_new (FALSE, 0);
        gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER (widget), extra_box);
        gtk_widget_show (extra_box);
    }

    if (dialog->priv->filter_mgr)
        moo_filter_mgr_attach (dialog->priv->filter_mgr,
                               GTK_FILE_CHOOSER (widget), extra_box,
                               dialog->priv->filter_mgr_id);

    if (dialog->priv->enable_encodings)
        _moo_encodings_attach_combo (widget, extra_box,
                                     dialog->priv->type == MOO_FILE_DIALOG_SAVE,
                                     dialog->priv->encoding);

    if (dialog->priv->parent)
        moo_window_set_parent (widget, dialog->priv->parent);

    g_signal_emit (dialog, signals[DIALOG_CREATED], 0, widget);

    return widget;
}


static GSList *
string_slist_copy (GSList *list)
{
    GSList *copy = NULL;
    GSList *l;

    for (l = list; l != NULL; l = l->next)
        copy = g_slist_prepend (copy, g_strdup (l->data));

    return g_slist_reverse (copy);
}


static void
set_filename (MooFileDialog *dialog,
              char          *filename)
{
    g_free (dialog->priv->filename);
    dialog->priv->filename = filename;
}


static void
set_filenames (MooFileDialog *dialog,
               GSList        *filenames)
{
    string_slist_free (dialog->priv->filenames);
    dialog->priv->filenames = filenames;
}


static void
set_encoding (MooFileDialog *dialog,
              const char    *encoding)
{
    char *tmp = dialog->priv->encoding;
    dialog->priv->encoding = g_strdup (encoding);
    g_free (tmp);
}

void
moo_file_dialog_set_encoding (MooFileDialog *dialog,
                              const char    *encoding)
{
    g_return_if_fail (MOO_IS_FILE_DIALOG (dialog));
    set_encoding (dialog, encoding);
}


static gboolean
filename_is_valid (G_GNUC_UNUSED const char *filename,
                   G_GNUC_UNUSED char      **msg)
{
#ifndef __WIN32__
    return TRUE;
#else
    struct name {
        const char *name;
        guint len;
    } names[] = {
        { "con", 3 }, { "aux", 3 }, { "prn", 3 }, { "nul", 3 },
        { "com1", 4 }, { "com2", 4 }, { "com3", 4 }, { "com4", 4 },
        { "lpt1", 4 }, { "lpt2", 4 }, { "lpt3", 4 }
    };
    guint i;
    const char *basename;
    gboolean invalid = FALSE;

    basename = strrchr (filename, '\\');

    if (!basename)
        basename = strrchr (filename, '/');

    if (!basename)
        basename = filename;
    else
        basename += 1;

    for (i = 0; i < G_N_ELEMENTS (names); ++i)
    {
        if (!g_ascii_strncasecmp (basename, names[i].name, names[i].len))
        {
            if (!basename[names[i].len] || basename[names[i].len] == '.')
                invalid = TRUE;
            break;
        }
    }

    if (invalid)
    {
        char *base = g_path_get_basename (filename);
        *msg = g_strdup_printf ("Filename '%s' is a reserved device name.\n"
                                "Please choose another name", base);
        g_free (base);
        return FALSE;
    }

    return TRUE;
#endif
}


gboolean
moo_file_dialog_run (MooFileDialog *dialog)
{
    char *filename;
    GtkWidget *filechooser;
    gboolean result = FALSE;

    g_return_val_if_fail (MOO_IS_FILE_DIALOG (dialog), FALSE);

    set_filename (dialog, NULL);
    set_filenames (dialog, NULL);

    filechooser = moo_file_dialog_create_widget (dialog);

    switch (dialog->priv->type)
    {
        case MOO_FILE_DIALOG_OPEN:
        case MOO_FILE_DIALOG_OPEN_ANY:
        case MOO_FILE_DIALOG_OPEN_DIR:
            if (gtk_dialog_run (GTK_DIALOG (filechooser)) == GTK_RESPONSE_OK)
            {
                set_filename (dialog, file_chooser_get_filename (filechooser));
                if (dialog->priv->multiple)
                    set_filenames (dialog, file_chooser_get_filenames (filechooser));
                result = TRUE;
            }

            goto out;

        case MOO_FILE_DIALOG_SAVE:
            while (TRUE)
            {
                if (GTK_RESPONSE_OK == gtk_dialog_run (GTK_DIALOG (filechooser)))
                {
                    char *msg;

                    filename = file_chooser_get_filename (filechooser);

                    if (!filename_is_valid (filename, &msg))
                    {
                        moo_error_dialog (filechooser, msg, NULL);
                        g_free (filename);
                        g_free (msg);
                    }
                    else if (g_file_test (filename, G_FILE_TEST_EXISTS) &&
                             ! g_file_test (filename, G_FILE_TEST_IS_REGULAR))
                    {
                        moo_error_dialog (filechooser,
                                          "Choosen file is not a regular file",
                                          NULL);
                        g_free (filename);
                    }
                    else if (g_file_test (filename, G_FILE_TEST_EXISTS) &&
                             g_file_test (filename, G_FILE_TEST_IS_REGULAR))
                    {
                        char *basename = g_path_get_basename (filename);
                        char *dirname = g_path_get_dirname (filename);
                        char *display_name = g_filename_display_name (basename);
                        char *display_dirname = g_filename_display_name (dirname);
                        gboolean overwrite;

                        overwrite = moo_overwrite_file_dialog (filechooser, display_name, display_dirname);

                        g_free (basename);
                        g_free (dirname);
                        g_free (display_name);
                        g_free (display_dirname);

                        if (overwrite)
                        {
                            set_filename (dialog, filename);
                            result = TRUE;
                            goto out;
                        }
                    }
                    else
                    {
                        set_filename (dialog, filename);
                        result = TRUE;
                        goto out;
                    }
                }
                else
                {
                    goto out;
                }
            }

        default:
            g_critical ("%s: incorrect dialog type specified", G_STRLOC);
    }

out:
    if (result && dialog->priv->enable_encodings)
        set_encoding (dialog, _moo_encodings_combo_get (filechooser,
                                    dialog->priv->type == MOO_FILE_DIALOG_SAVE));

    gtk_widget_destroy (filechooser);
    return result;
}


const char *
moo_file_dialog_get_filename (MooFileDialog *dialog)
{
    g_return_val_if_fail (MOO_IS_FILE_DIALOG (dialog), NULL);
    return dialog->priv->filename;
}


GSList *
moo_file_dialog_get_filenames (MooFileDialog *dialog)
{
    g_return_val_if_fail (MOO_IS_FILE_DIALOG (dialog), NULL);
    return string_slist_copy (dialog->priv->filenames);
}


const char *
moo_file_dialog_get_encoding (MooFileDialog *dialog)
{
    g_return_val_if_fail (MOO_IS_FILE_DIALOG (dialog), NULL);
    return dialog->priv->encoding ? dialog->priv->encoding : MOO_ENCODING_UTF8;
}


const char *
moo_file_dialogp (GtkWidget          *parent,
                  MooFileDialogType   type,
                  const char         *start_name,
                  const char         *title,
                  const char         *prefs_key,
                  const char         *alternate_prefs_key)
{
    const char *start = NULL;
    const char *filename = NULL;

    if (!title)
        title = "Choose File";

    if (prefs_key)
    {
        moo_prefs_create_key (prefs_key, MOO_PREFS_STATE, G_TYPE_STRING, NULL);
        start = moo_prefs_get_string (prefs_key);
    }

    if (!start && alternate_prefs_key)
    {
        moo_prefs_create_key (alternate_prefs_key, MOO_PREFS_STATE, G_TYPE_STRING, NULL);
        start = moo_prefs_get_string (alternate_prefs_key);
    }

    filename = moo_file_dialog (parent, type, start_name, title, start);

    if (filename && prefs_key)
    {
        char *new_start = g_path_get_dirname (filename);
        moo_prefs_create_key (prefs_key, MOO_PREFS_STATE, G_TYPE_STRING, NULL);
        moo_prefs_set_filename (prefs_key, new_start);
        g_free (new_start);
    }

    return filename;
}


GType moo_file_dialog_type_get_type (void)
{
    static GType type = 0;

    if (G_UNLIKELY (!type))
    {
        static const GEnumValue values[] = {
            { MOO_FILE_DIALOG_OPEN, (char*) "MOO_FILE_DIALOG_OPEN", (char*) "open" },
            { MOO_FILE_DIALOG_OPEN_ANY, (char*) "MOO_FILE_DIALOG_OPEN_ANY", (char*) "open-any" },
            { MOO_FILE_DIALOG_SAVE, (char*) "MOO_FILE_DIALOG_SAVE", (char*) "save" },
            { MOO_FILE_DIALOG_OPEN_DIR, (char*) "MOO_FILE_DIALOG_OPEN_DIR", (char*) "open-dir" },
            { 0, NULL, NULL }
        };
        type = g_enum_register_static ("MooFileDialogType", values);
    }

    return type;
}


MooFileDialog *
moo_file_dialog_new (MooFileDialogType type,
                     GtkWidget      *parent,
                     gboolean        multiple,
                     const char     *title,
                     const char     *start_dir,
                     const char     *start_name)
{
    return g_object_new (MOO_TYPE_FILE_DIALOG,
                         "parent", parent,
                         "type", type,
                         "multiple", multiple,
                         "title", title,
                         "dir", start_dir,
                         "name", start_name,
                         NULL);
}


void
moo_file_dialog_set_filter_mgr (MooFileDialog  *dialog,
                                MooFilterMgr   *mgr,
                                const char     *id)
{
    g_return_if_fail (MOO_IS_FILE_DIALOG (dialog));
    g_return_if_fail (!mgr || MOO_IS_FILTER_MGR (mgr));
    g_object_set (dialog, "filter-mgr", mgr, "filter-mgr-id", id, NULL);
}
