#include "mooedit/mooeditprogress.h"
#include "mooedit/mooeditprogress-gxml.h"
#include "mooutils/mooutils-script.h"

#define MOO_TYPE_EDIT_PROGRESS                       (moo_edit_progress_get_type ())
#define MOO_EDIT_PROGRESS(object)                    (G_TYPE_CHECK_INSTANCE_CAST ((object), MOO_TYPE_EDIT_PROGRESS, MooEditProgress))
#define MOO_EDIT_PROGRESS_CLASS(klass)               (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_EDIT_PROGRESS, MooEditProgressClass))
#define MOO_IS_EDIT_PROGRESS(object)                 (G_TYPE_CHECK_INSTANCE_TYPE ((object), MOO_TYPE_EDIT_PROGRESS))
#define MOO_IS_EDIT_PROGRESS_CLASS(klass)            (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_EDIT_PROGRESS))
#define MOO_EDIT_PROGRESS_GET_CLASS(obj)             (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_EDIT_PROGRESS, MooEditProgressClass))

#define PROGRESS_TIMEOUT 100
#define INITIAL_TIMEOUT 500

typedef struct MooEditProgressClass MooEditProgressClass;

struct MooEditProgress
{
    GtkAlignment base;

    ProgressWidgetXml *xml;

    guint timeout;
    moo::gstr text;
    GDestroyNotify cancel_op;
    gpointer cancel_data;

    void update();
};

struct MooEditProgressClass
{
    GtkAlignmentClass base_class;
};

static void     cancel_clicked      (MooEditProgress *pr);

MOO_DEFINE_TYPE_STATIC (MooEditProgress, moo_edit_progress, GTK_TYPE_ALIGNMENT)

static void
moo_edit_progress_init (MooEditProgress *pr)
{
    new(pr)(MooEditProgress);
    pr->xml = progress_widget_xml_new_with_root (GTK_WIDGET (pr));
    g_signal_connect_swapped (pr->xml->cancel, "clicked", G_CALLBACK (cancel_clicked), pr);
}

static void
moo_edit_progress_dispose (GObject *object)
{
    MooEditProgress *pr = MOO_EDIT_PROGRESS (object);

    if (pr->timeout)
    {
        g_source_remove (pr->timeout);
        pr->timeout = 0;
    }

    G_OBJECT_CLASS (moo_edit_progress_parent_class)->dispose (object);
}

static void
moo_edit_progress_finalize(GObject* object)
{
    MooEditProgress* pr = MOO_EDIT_PROGRESS(object);
    pr->~MooEditProgress();
    G_OBJECT_CLASS(moo_edit_progress_parent_class)->finalize(object);
}

static void
moo_edit_progress_class_init (MooEditProgressClass *klass)
{
    G_OBJECT_CLASS(klass)->finalize = moo_edit_progress_finalize;
    G_OBJECT_CLASS(klass)->dispose = moo_edit_progress_dispose;
}

EditProgressPtr
_moo_edit_progress_new (void)
{
    return moo::wrap_new(MOO_EDIT_PROGRESS(g_object_new(MOO_TYPE_EDIT_PROGRESS, NULL)));
}

void MooEditProgress::update()
{
    g_return_if_fail (!text.empty());
    gtk_progress_bar_set_text(xml->progressbar, text);
}

void
_moo_edit_progress_set_text (MooEditProgress& progress,
                             const char*      text)
{
    progress.text.set(text);
    progress.update();
}

static gboolean
pulse_progress (MooEditProgress *progress)
{
    g_return_val_if_fail (MOO_IS_EDIT_PROGRESS (progress), FALSE);
    g_return_val_if_fail (GTK_IS_WIDGET (progress->xml->progressbar), FALSE);
    gtk_progress_bar_pulse (GTK_PROGRESS_BAR (progress->xml->progressbar));
    progress->update();
    return TRUE;
}

static void
cancel_clicked (MooEditProgress *progress)
{
    g_return_if_fail (MOO_IS_EDIT_PROGRESS (progress));
    if (progress->cancel_op)
        progress->cancel_op (progress->cancel_data);
}

static gboolean
show_progress (MooEditProgress *progress)
{
    g_source_remove (progress->timeout);

    progress->timeout =
        g_timeout_add (PROGRESS_TIMEOUT,
                       (GSourceFunc) pulse_progress,
                       progress);
    progress->update();

    gtk_widget_show (GTK_WIDGET (progress));

    return FALSE;
}

void
_moo_edit_progress_set_cancel_func (MooEditProgress& progress,
                                    GDestroyNotify  cancel_func,
                                    gpointer        cancel_func_data)
{
    progress.cancel_op = cancel_func;
    progress.cancel_data = cancel_func_data;
    gtk_widget_set_sensitive (GTK_WIDGET (progress.xml->cancel),
                              cancel_func != NULL);
}

void
_moo_edit_progress_start (MooEditProgress& progress,
                          const char*      text,
                          GDestroyNotify   cancel_func,
                          gpointer         cancel_func_data)
{
    g_return_if_fail(progress.timeout == 0);

    progress.text.set_new(g_strdup(text));

    _moo_edit_progress_set_cancel_func(progress, cancel_func, cancel_func_data);

    progress.timeout =
        g_timeout_add (INITIAL_TIMEOUT,
                       (GSourceFunc) show_progress,
                       &progress);
}
