/*
 *   mooprefspage.h
 *
 *   Copyright (C) 2004-2008 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License version 2.1 as published by the Free Software Foundation.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOO_PREFS_PAGE_H
#define MOO_PREFS_PAGE_H

#include <gtk/gtk.h>
#include <mooutils/mooglade.h>

G_BEGIN_DECLS


#define MOO_TYPE_PREFS_PAGE              (moo_prefs_page_get_type ())
#define MOO_PREFS_PAGE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_PREFS_PAGE, MooPrefsPage))
#define MOO_PREFS_PAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_PREFS_PAGE, MooPrefsPageClass))
#define MOO_IS_PREFS_PAGE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_PREFS_PAGE))
#define MOO_IS_PREFS_PAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_PREFS_PAGE))
#define MOO_PREFS_PAGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_PREFS_PAGE, MooPrefsPageClass))


typedef struct MooPrefsPage        MooPrefsPage;
typedef struct MooPrefsPagePrivate MooPrefsPagePrivate;
typedef struct MooPrefsPageClass   MooPrefsPageClass;

struct MooPrefsPage
{
    GtkVBox base;
    MooPrefsPagePrivate *priv;
    gboolean auto_apply;
    char *label;
};

struct MooPrefsPageClass
{
    GtkVBoxClass base_class;
    void (*init)  (MooPrefsPage *page);
    void (*apply) (MooPrefsPage *page);
};

GType           moo_prefs_page_get_type     (void) G_GNUC_CONST;

GtkWidget      *moo_prefs_page_new          (const char     *label,
                                             const char     *icon_stock_id);

typedef void (*MooPrefsPageInitUi)          (MooPrefsPage   *page);
typedef void (*MooPrefsPageInit)            (MooPrefsPage   *page);
typedef void (*MooPrefsPageApply)           (MooPrefsPage   *page);
void            moo_prefs_page_set_callbacks(MooPrefsPage   *page,
                                             MooPrefsPageInitUi init_ui,
                                             MooPrefsPageInit   init,
                                             MooPrefsPageApply  apply);

void            moo_prefs_page_bind_setting (MooPrefsPage   *page,
                                             GtkWidget      *widget,
                                             const char     *setting);

void            moo_prefs_page_add_page     (MooPrefsPage   *page,
                                             MooPrefsPage   *child_page);


G_END_DECLS

#endif /* MOO_PREFS_PAGE_H */